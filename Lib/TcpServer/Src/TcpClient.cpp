
#include <iostream>

#include "Socket.h"
#include "Buffer.h"
#include "Xml.h"

#include "Codes.h"
#include "Request.h"
#include "Response.h"
#include "ErrorResponse.h"
#include "Router.h"
#include "TcpServer.h"
#include "TcpClient.h"

TCP::Client::Client (std::shared_ptr <Router> inRouter, std::unique_ptr <Core::Socket> inSocket) :
    Thread ("Client[" + std::to_string (inSocket->GetId ()) + "]"),
    mRouter (inRouter),
    mSocket (std::move (inSocket))
{
}

TCP::Client::~Client () {
}

void TCP::Client::Execute () {

    Core::Buffer recvBuffer (MAX_BUFFER_SIZE);
    Core::Buffer sendBuffer (MAX_BUFFER_SIZE);

    while (mSocket->IsConnected ()) {
        if (mSocket->Receive (recvBuffer)) { // blocking call

            std::string error;
            std::string rawData;
            recvBuffer.ToString (rawData);

            std::cout << rawData << std::endl;

            XmlNode requestNode;
            XmlNode responseNode;
            Response response;

            if (Xml::Parse (rawData, requestNode, error)) {
                Request request;
                if (request.FromXml (requestNode, error)) {
                    mRouter->Dispatch (request, response);
                }
                else {
                    response = ErrorResponse (Codes::kBadRequest, error);
                }
            }
            else {
                response = ErrorResponse (Codes::kBadRequest, error);
            }

            response.ToXml (responseNode);

            Xml::Print (responseNode, outData, error, false);
            sendBuffer.SetData (outData);
            Send (sendBuffer);
        }
    }
}

void TCP::Client::Send (const Core::Buffer& inBuffer) {
    mSocket->Send (inBuffer);
}

void TCP::Client::ForceClose () {
    mSocket->Close ();
}

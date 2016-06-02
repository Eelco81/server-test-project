
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

Api::Client::Client (std::shared_ptr <Router> inRouter, std::unique_ptr <Core::Socket> inSocket) :
    Thread ("Client[" + std::to_string (inSocket->GetId ()) + "]"),
    mRouter (inRouter),
    mSocket (std::move (inSocket))
{
}

Api::Client::~Client () {
}

void Api::Client::Execute () {

    Core::Buffer recvBuffer (MAX_BUFFER_SIZE);
    Core::Buffer sendBuffer (MAX_BUFFER_SIZE);

    while (mSocket->IsConnected ()) {
        if (mSocket->Receive (recvBuffer)) { // blocking call

            std::string error;
            std::string rawData;
            recvBuffer.ToString (rawData);

            std::cout << rawData << std::endl;

            Core::XmlNode requestNode;
            Core::XmlNode responseNode;
            Response response;

            if (Core::Xml::Parse (rawData, requestNode, error)) {
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

            std::string outData ("HTTP/1.1 200 OK\nConnection: close\n\n");
            //Core::Xml::Print (responseNode, outData, error, false);
            sendBuffer.SetData (outData);
            Send (sendBuffer);
        }
    }
}

void Api::Client::Send (const Core::Buffer& inBuffer) {
    mSocket->Send (inBuffer);
}

void Api::Client::ForceClose () {
    mSocket->Close ();
}


#define MAX_BUFFER_SIZE 2048

#include <string>

#include "Network.h"
#include "Socket.h"
#include "Buffer.h"

#include "TcpServer.h"
#include "TcpClient.h"

TCP::Client::Client (std::unique_ptr <OS::Socket> inSocket) :
    Thread ("Client[" + std::to_string (inSocket->GetId ()) + "]"),
    mSocket (std::move (inSocket))
{
}

TCP::Client::~Client () {
}

unsigned TCP::Client::GetId () const {
    return mSocket->GetId ();
}

void TCP::Client::Execute () {

    OS::Buffer recvBuffer (MAX_BUFFER_SIZE);
    OS::Buffer sendBuffer (MAX_BUFFER_SIZE);

    while (mSocket->IsConnected ()) {
        if (mSocket->Receive (recvBuffer)) { // blocking call
            OnReceived( recvBuffer );
        }
    }
    
    mSocket->Close ();
}

void TCP::Client::Send (const OS::Buffer& inBuffer) {
    mSocket->Send (inBuffer);
}

void TCP::Client::Kill () {
    mSocket->Close();
}

void TCP::Client::OnReceived (OS::Buffer& inBuffer) {
}

            /*
            std::string error;
            std::string rawData;
            recvBuffer.ToString (rawData);

            XmlNode requestNode;
            XmlNode responseNode;
            API::Response response;

            if (Xml::Parse (rawData, requestNode, error)) {
                API::Request request;
                if (request.FromXml (requestNode, error)) {
                    mRouter->Dispatch (request, response);
                }
                else {
                    response = API::ErrorResponse (API::Codes::kBadRequest, error);
                }
            }
            else {
                response = API::ErrorResponse (API::Codes::kBadRequest, error);
            }

            response.ToXml (responseNode);

            std::string outData;
            Xml::Print (responseNode, outData, error, false);
            sendBuffer.SetData (outData);
            Send (sendBuffer);
            */
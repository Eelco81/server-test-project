
#include <iostream>
#include "Socket.h"
#include <Timing.h>
#include "HttpClient.h"
#include "HttpResponse.h"

HTTP::Client::Client (std::string inAddress, std::string inPort) :
    TCP::Client (inAddress, inPort)
{
}
HTTP::Client::Client (std::unique_ptr <OS::Socket> inSocket) :
    TCP::Client (std::move (inSocket))
{
}

void HTTP::Client::OnReceived (const std::vector<uint8_t>& inBuffer) {
    const std::string input (reinterpret_cast<const char*>(inBuffer.data ()), inBuffer.size());
    Write (input);
}

void HTTP::Client::HandleRequest (const Request& inRequest) {
    
    
    
    Response response;
    response.mCode = Code::NOT_IMPLEMENTED;
    response.mVersion = Version::V11;
    response.mHeaders["Server"] = "ServerTest/1.0";
    response.mHeaders["Last-Modified"] = OS::Timing::ToStdString (OS::Timing::Now ());
    response.mHeaders["Host"] = "127.0.0.1:1234";
    response.mHeaders["Content-Length"] = "0";
    
    const auto str (response.ToString ());
    
    std::cout << "-----" << std::endl;
    std::cout << inRequest.ToString () << std::endl;
    std::cout << "-----" << std::endl;
    std::cout << str << std::endl;
    std::cout << "-----" << std::endl;
    
    TCP::Client::Send (reinterpret_cast<const uint8_t*> (str.c_str()), str.size());
    
}

std::unique_ptr<TCP::Client> HTTP::ClientFactory::Create (std::unique_ptr<OS::Socket> inSocket) const {
    return std::make_unique<HTTP::Client> (std::move (inSocket));
}
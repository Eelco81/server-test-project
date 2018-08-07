
#include <iostream>
#include "Socket.h"
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
    
    std::cout << input << std::endl;
    
    Response response;
    response.mCode = Code::NOT_FOUND;
    response.mVersion = Version::V11;
    
    const auto str (response.ToString ());
    
    TCP::Client::Send (reinterpret_cast<const uint8_t*> (str.c_str()), str.size());
}

std::unique_ptr<TCP::Client> HTTP::ClientFactory::Create (std::unique_ptr<OS::Socket> inSocket) const {
    return std::make_unique<HTTP::Client> (std::move (inSocket));
}
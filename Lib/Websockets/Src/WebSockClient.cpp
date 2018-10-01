
#include <iostream>
#include "Socket.h"
#include "Log.h"
#include "HttpCodes.h"
#include "HttpResponse.h"
#include "WebSockClient.h"
#include "WebSockFrame.h"
#include "sha1.hpp"

RFC6455::Client::Client (std::unique_ptr <OS::Socket> inSocket) :
    TCP::Client (std::move (inSocket)),
    HTTP::RequestDecoder (),
    RFC6455::FrameDecoder (),
    mIsUpgraded (false)
{
}

RFC6455::Client::~Client () = default;

void RFC6455::Client::HandlePacket (const std::vector<uint8_t>& inBuffer) {
    if (!mIsUpgraded) {
        const std::string input (reinterpret_cast<const char*>(inBuffer.data ()), inBuffer.size ());
        HTTP::RequestDecoder::Write (input);
    }
    else {
        RFC6455::FrameDecoder::Write (inBuffer);
    }
}

void RFC6455::Client::HandleRequest (const HTTP::Request& inRequest) {
    HTTP::Response response (HTTP::Code::BAD_REQUEST, inRequest.mVersion);
    
    if (inRequest.mHeaders.at ("Connection") == "Upgrade" && inRequest.mHeaders.at ("Upgrade") == "websocket") {
        
        const auto key (inRequest.mHeaders.at ("Sec-WebSocket-Key"));
        const auto keyWithMagicString (key + "258EAFA5-E914-47DA-95CA-C5AB0DC85B11");
        
        char base64[SHA1_BASE64_SIZE];
        sha1 (keyWithMagicString.c_str ()).finalize().print_base64 (base64);
        
        response.mCode = HTTP::Code::SWITCHING_PROTOCOLS;
        response.mHeaders["Connection"] = "Upgrade";
        response.mHeaders["Upgrade"] = "websocket";
        response.mHeaders["Sec-WebSocket-Accept"] = std::string (base64);
        
        mIsUpgraded = true;
    }
    
    SendResponse (inRequest, response);
    
    if (!mIsUpgraded) {
        Stop ();
    }
}

void RFC6455::Client::HandleFrame (const RFC6455::Frame& inFrame) {
    LOGMESSAGE (OS::Log::kTrace, std::string ("Received ") + inFrame.ToMessage ());
    
    if (inFrame.mOpCode == Frame::OpCode::PING) {
        Frame frame;
        frame.mFin = true;
        frame.mOpCode = Frame::OpCode::PONG;
        SendFrame (frame);
    }
    else if (inFrame.mOpCode == Frame::OpCode::CLOSE) {
        Frame frame;
        frame.mFin = true;
        frame.mOpCode = Frame::OpCode::CLOSE;
        SendFrame (frame);
        Quit ();
    }
    else {
        SendFrame (inFrame);
    }
}

void RFC6455::Client::SendResponse (const HTTP::Request& inRequest, const HTTP::Response& inResponse) {
    using namespace HTTP;
    LOGMESSAGE (OS::Log::kInfo, std::string ("HTTP/") + VersionToString (inResponse.mVersion) + std::string (" ") + MethodToString (inRequest.mMethod) + std::string (" ") + inRequest.mPath + std::string (" - ") + std::to_string (inResponse.mCode) + std::string (" ") + CodeToString (inResponse.mCode));
    const auto resStr (inResponse.ToString ());
    TCP::Client::Send (reinterpret_cast<const uint8_t*> (resStr.c_str ()), resStr.size ());
}

void RFC6455::Client::SendFrame (const RFC6455::Frame& inFrame) {
    LOGMESSAGE (OS::Log::kTrace, std::string ("Send ") + inFrame.ToMessage ());
    TCP::Client::Send (inFrame.ToBuffer ());
}

std::unique_ptr<TCP::Client> RFC6455::ClientFactory::Create (std::unique_ptr<OS::Socket> inSocket) const {
    return std::make_unique<RFC6455::Client> (std::move (inSocket));
}

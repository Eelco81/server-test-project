
#include <iostream>
#include "Socket.h"
#include "Log.h"
#include "HttpCodes.h"
#include "HttpResponse.h"
#include "WebSockClient.h"
#include "WebSockFrame.h"
#include "sha1.hpp"

RFC6455::Client::Client (std::unique_ptr <OS::Socket> inSocket) :
    TCP::Client (std::move (inSocket))
{
    GetReadStream ().Pipe (mToStringConverter).Pipe (mRequestDecoder).Pipe (this, &Client::HandleHandshake);
    mResponseEncoder.Pipe (mToPacketConverter).Pipe (GetWriteStream ());
}

RFC6455::Client::~Client () = default;

void RFC6455::Client::HandleHandshake (const HTTP::Request& inRequest) {
    
    HTTP::Response response (HTTP::Code::BAD_REQUEST, inRequest.mVersion);
    
    bool isUpgraded (false);

    try {
        if (inRequest.GetHeaderValue ("Connection") == "Upgrade" && 
            inRequest.GetHeaderValue ("Upgrade") == "websocket" &&  
            inRequest.GetHeaderValue ("Sec-WebSocket-Key") != "" && 
            inRequest.mMethod == HTTP::Method::GET && 
            inRequest.mVersion == HTTP::Version::V11 
        ) {
            
            const auto key (inRequest.GetHeaderValue ("Sec-WebSocket-Key"));
            const auto keyWithMagicString (key + "258EAFA5-E914-47DA-95CA-C5AB0DC85B11");
            
            char base64[SHA1_BASE64_SIZE];
            sha1 (keyWithMagicString.c_str ()).finalize().print_base64 (base64);
            
            response.SetHeader (HTTP::Header ("Connection", "Upgrade"));
            response.SetHeader (HTTP::Header ("Upgrade", "websocket"));
            response.SetHeader (HTTP::Header ("Sec-WebSocket-Accept", std::string (base64)));
            
            response.mCode = HTTP::Code::SWITCHING_PROTOCOLS;
            isUpgraded = true;
        }
    } 
    catch (...) {}
    
    mResponseEncoder.Write (response);
    
    {
        using namespace HTTP;
        LOGINFO << "HTTP/" << VersionToString (response.mVersion) << " " << MethodToString (inRequest.mMethod) 
                << " " << inRequest.mPath << " - " << response.mCode << " " << CodeToString (response.mCode) << " - RFC6455";
    }
 
    if (!isUpgraded) {
        Quit ();
    }
    else {
        // Clear the stream, route the pipe through the frame decoder.
        GetReadStream ().Clear ().Pipe (mFrameDecoder).Pipe (this, &Client::HandleReceivedFrame);
        mResponseEncoder.Clear ();
        
        mPayloadStringEncoder.Pipe (mFrameEncoder).Pipe (GetWriteStream ());
        mPayloadBinaryEncoder.Pipe (mFrameEncoder);
    }
}

void RFC6455::Client::HandleReceivedFrame (const RFC6455::Frame& inFrame) {
    
    if (inFrame.mOpCode == Frame::OpCode::PING) {
        Frame frame (inFrame);
        frame.mOpCode = Frame::OpCode::PONG;
        mFrameEncoder.Write (frame);
    }
    else if (inFrame.mOpCode == Frame::OpCode::CLOSE) {
        Frame frame;
        frame.mFin = true;
        frame.mOpCode = Frame::OpCode::CLOSE;
        mFrameEncoder.Write (frame);
        GetReadStream ().Clear (); // do not process further messages
        Quit ();
    }
    else {
        mPayloadStringDecoder.Write (inFrame);
        mPayloadBinaryDecoder.Write (inFrame);
    }
}

void RFC6455::Client::SendData (const std::vector<uint8_t>& inData) {
    mPayloadBinaryEncoder.Write (inData);
}

void RFC6455::Client::SendData (const std::string& inData) {
    mPayloadStringEncoder.Write (inData);
}

std::unique_ptr<TCP::Client> RFC6455::ClientFactory::Create (std::unique_ptr<OS::Socket> inSocket) const {
    return std::make_unique<RFC6455::Client> (std::move (inSocket));
}

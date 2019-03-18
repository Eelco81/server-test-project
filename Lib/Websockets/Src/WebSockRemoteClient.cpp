
#include <iostream>
#include "Log.h"
#include "HttpCodes.h"
#include "HttpResponse.h"
#include "HttpRequest.h"
#include "WebSockRemoteClient.h"

RFC6455::RemoteClient::RemoteClient (std::string inAddress, std::string inPort) :
    TCP::Client (inAddress, inPort)
{
    GetReadStream ().Pipe (mToStringConverter).Pipe (mResponseDecoder).Pipe (this, &RemoteClient::HandleHandshake);
    mRequestEncoder.Pipe (mToPacketConverter).Pipe (GetWriteStream ());
    
    // Start the listener thread
    TCP::Client::Start ();
    
    HTTP::Request request (HTTP::Method::GET, "/");
    request.SetHeader (HTTP::Header ("Connection", "Upgrade"));
    request.SetHeader (HTTP::Header ("Upgrade", "websocket"));
    request.SetHeader (HTTP::Header ("Sec-WebSocket-Key", "dGhlIHNhbXBsZSBub25jZQ=="));
    mRequestEncoder.Write (request);
}

RFC6455::RemoteClient::~RemoteClient () = default;

void RFC6455::RemoteClient::HandleHandshake (const HTTP::Response& inResponse) {
    
    bool isUpgraded (false);
    try {
        if (inResponse.GetHeaderValue ("Connection") == "Upgrade" && 
            inResponse.GetHeaderValue ("Upgrade") == "websocket" && 
            inResponse.mCode == HTTP::Code::SWITCHING_PROTOCOLS
        ) {
            
            // \todo: implement response.mHeaders.at ("Sec-WebSocket-Accept")
            isUpgraded = true;
        }
    }
    catch (...) {}
            
    if (!isUpgraded) {
        Quit ();
    }
    else {
        // Clear the stream, route the pipe through the frame decoder.
        GetReadStream ().Clear ().Pipe (mFrameDecoder).Pipe (this, &RemoteClient::HandleReceivedFrame);
        mRequestEncoder.Clear ();
        
        mPayloadStringEncoder.Pipe (mFrameEncoder).Pipe (GetWriteStream ());
        mPayloadBinaryEncoder.Pipe (mFrameEncoder);
    }
}

void RFC6455::RemoteClient::HandleReceivedFrame (const RFC6455::Frame& inFrame) {
    
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

void RFC6455::RemoteClient::SendData (const std::vector<uint8_t>& inData) {
    mPayloadBinaryEncoder.Write (inData);
}

void RFC6455::RemoteClient::SendData (const std::string& inData) {
    mPayloadStringEncoder.Write (inData);
}

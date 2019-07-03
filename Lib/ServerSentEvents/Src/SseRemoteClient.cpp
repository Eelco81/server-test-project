
#include <iostream>
#include "Log.h"
#include "HttpCodes.h"
#include "HttpResponse.h"
#include "HttpRequest.h"
#include "SseRemoteClient.h"

SSE::RemoteClient::RemoteClient (std::string inAddress, std::string inPort) :
    TCP::Client (inAddress, inPort)
{
    Pipe (sDataAvailable, mPacket2String)
        .Pipe (mResponseDecoder)
        .Pipe (this, &RemoteClient::HandleHandshake);
    
    mRequestEncoder.Pipe (mString2Packet)
        .Pipe<TCP::Client> (this, &TCP::Client::Send);
    
    // Start the listener thread
    TCP::Client::Start ();
    
    HTTP::Request request (HTTP::Method::GET, "/");
    request.SetHeader (HTTP::Header ("Accept", "text/event-stream"));
    mRequestEncoder.Write (request);
}

SSE::RemoteClient::~RemoteClient () = default;

void SSE::RemoteClient::HandleHandshake (const HTTP::Response& inResponse) {
    
    bool isUpgraded (false);
    try {
        if (inResponse.GetHeaderValue ("Content-Type") == "text/event-stream" && 
            inResponse.GetHeaderValue ("Connection") == "keep-alive" && 
            inResponse.mCode == HTTP::Code::OK
        ) {
            isUpgraded = true;
        }
    }
    catch (...) {}
    
    if (!isUpgraded) {
        Quit ();
    }
    else {
        // Clear the open stream
        mPacket2String.Clear ();
        mResponseDecoder.Clear ();
        mRequestEncoder.Clear ();
        mString2Packet.Clear ();
        
        // Route the pipe through the event decoder.
        Pipe (sDataAvailable, mPacket2String)
            .Pipe (mEventDecoder)
            .Pipe (sEventAvailable);
    }
}

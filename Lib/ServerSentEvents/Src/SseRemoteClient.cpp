

#include <iostream>
#include "Log.h"
#include "HttpCodes.h"
#include "HttpResponse.h"
#include "HttpRequest.h"
#include "SseRemoteClient.h"

SSE::RemoteClient::RemoteClient (std::string inAddress, std::string inPort) :
    TCP::Client (inAddress, inPort)
{
    GetReadStream ().Pipe (mToStringConverter).Pipe (mResponseDecoder).Pipe (this, &RemoteClient::HandleHandshake);
    mRequestEncoder.Pipe (mToPacketConverter).Pipe (GetWriteStream ());
    
    // Start the listener thread
    TCP::Client::Start ();
    
    HTTP::Request request (HTTP::Method::GET, "/");
    request.mHeaders["Accept"] = "text/event-stream";
    mRequestEncoder.Write (request);
}

SSE::RemoteClient::~RemoteClient () = default;

void SSE::RemoteClient::HandleHandshake (const HTTP::Response& inResponse) {
    
    bool isUpgraded (false);
    try {
        if (inResponse.mHeaders.at ("Content-Type") == "text/event-stream" && inResponse.mHeaders.at ("Connection") == "keep-alive" && inResponse.mCode == HTTP::Code::OK) {
            isUpgraded = true;
        }
    }
    catch (...) {}
    
    if (!isUpgraded) {
        Quit ();
    }
    else {
        // Clear the open stream
        mToStringConverter.Clear();
        mResponseDecoder.Clear ();
        mRequestEncoder.Clear ();
        mToPacketConverter.Clear();
        
        // Route the pipe through the event decoder.
        GetReadStream ().Clear ().Pipe (mToStringConverter).Pipe (mEventDecoder);
    }
}

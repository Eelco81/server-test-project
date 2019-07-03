
#include <iostream>
#include "Socket.h"
#include "Log.h"
#include "HttpCodes.h"
#include "HttpResponse.h"
#include "SseClient.h"

SSE::Client::Client (std::unique_ptr <OS::Socket> inSocket) :
    TCP::Client (std::move (inSocket))
{
    Pipe (sDataAvailable, mPacket2String)
        .Pipe (mRequestDecoder)
        .Pipe (this, &Client::HandleHandshake);
    
    mResponseEncoder.Pipe (mString2Packet)
        .Pipe<TCP::Client> (this, &TCP::Client::Send);
}

SSE::Client::~Client () = default;

void SSE::Client::HandleHandshake (const HTTP::Request& inRequest) {
    
    HTTP::Response response (HTTP::Code::BAD_REQUEST, inRequest.mVersion);
    
    bool isUpgraded (false);

    try {
        if (inRequest.GetHeaderValue ("Accept") == "text/event-stream" && 
            inRequest.mMethod == HTTP::Method::GET && 
            inRequest.mVersion == HTTP::Version::V11
        ) {
            
            // Standard SSE headers
            response.SetHeader (HTTP::Header ("Connection", "keep-alive"));
            response.SetHeader (HTTP::Header ("Content-Type", "text/event-stream"));
            
            // Prevent caching
            response.SetHeader (HTTP::Header ("Cache-Control", "no-cache"));
            
            // This SSE server most likely does not host the origin of the request
            // ==> Enable CORS.
            response.SetHeader (HTTP::Header ("Access-Control-Allow-Origin", "*"));
            response.SetHeader (HTTP::Header ("Access-Control-Allow-Credentials", "true"));
            
            // The response constructor sets the length to zero, this makes connections
            // assume there is no more data coming. Note: there is a stream in response, so the body 
            // has variable length! Remove this header.
            response.RemoveHeaders ("Content-Length"); 
            
            // Optionally check for last event ID.
            try {
                const auto list = inRequest.GetHeaders ("Last-Event-ID");
                if (!list.empty ()) {
                    uint8_t lastId = std::stoi (list[0].GetValue ());
                    mEventEncoder.SetLastId (lastId);
                }
            }
            catch (...) {}
            
            // Handshake completed.
            response.mCode = HTTP::Code::OK;
            isUpgraded = true;
        }
    } 
    catch (...) {}
    
    // Send the handshake response
    mResponseEncoder.Write (response);
    
    {
        using namespace HTTP;
        LOGINFO << "HTTP/" << VersionToString (response.mVersion) << " " << MethodToString (inRequest.mMethod) 
                << " " << inRequest.mPath << " - " << response.mCode << " " << CodeToString (response.mCode) << " - SSE";
    }
 
    if (!isUpgraded) {
        Quit ();
    }
    else {
        // Clear the read stream (SSE only contains outgoing events)
        sDataAvailable.DisconnectAll ();
        mResponseEncoder.Clear ();
        mPacket2String.Clear ();
        mString2Packet.Clear ();
        mRequestDecoder.Clear ();
        mResponseEncoder.Clear ();

        mEventEncoder.Pipe (mString2Packet)
            .Pipe<TCP::Client> (this, &TCP::Client::Send);
    }
}

void SSE::Client::SendPayload (const std::string& inData) {
    mEventEncoder.Write (inData);
}

std::unique_ptr<TCP::Client> SSE::ClientFactory::Create (std::unique_ptr<OS::Socket> inSocket) const {
    return std::make_unique<SSE::Client> (std::move (inSocket));
}

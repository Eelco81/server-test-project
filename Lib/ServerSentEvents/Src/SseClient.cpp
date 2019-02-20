
#include <iostream>
#include "Socket.h"
#include "Log.h"
#include "HttpCodes.h"
#include "HttpResponse.h"
#include "SseClient.h"

SSE::Client::Client (std::unique_ptr <OS::Socket> inSocket) :
    TCP::Client (std::move (inSocket))
{
    GetReadStream ().Pipe (mToStringConverter).Pipe (mRequestDecoder).Pipe (this, &Client::HandleHandshake);
    mResponseEncoder.Pipe (mToPacketConverter).Pipe (GetWriteStream ());
}

SSE::Client::~Client () = default;

void SSE::Client::HandleHandshake (const HTTP::Request& inRequest) {
    
    HTTP::Response response (HTTP::Code::BAD_REQUEST, inRequest.mVersion);
    
    bool isUpgraded (false);

    try {
        if (inRequest.mHeaders.at ("Accept") == "text/event-stream" && inRequest.mMethod == HTTP::Method::GET && inRequest.mVersion == HTTP::Version::V11) {
            
            // Standard SSE headers
            response.mHeaders["Connection"] = "keep-alive";
            response.mHeaders["Content-Type"] = "text/event-stream";
            
            // Prevent caching
            response.mHeaders["Cache-Control"] = "no-cache";
            
            // This SSE server most likely does not host the origin of the request
            // ==> Enable CORS.
            response.mHeaders["Access-Control-Allow-Origin"] = "*";
            response.mHeaders["Access-Control-Allow-Credentials"] = "true";
            
            // The response constructor sets the length to zero, this makes connections
            // assume there is no more data coming. Note: there is a stream in response, so the body 
            // has variable length! Remove this header.
            response.mHeaders.erase("Content-Length"); 
            
            // Optionally check for last event ID.
            try {
                uint8_t lastId = std::stoi (inRequest.mHeaders.at ("Last-Event-ID"));
                mEventEncoder.SetLastId (lastId);
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
        LOGINFO << "HTTP/" << VersionToString (response.mVersion) << "(SSE) " << MethodToString (inRequest.mMethod) 
                << " " << inRequest.mPath << " - " << response.mCode << " " << CodeToString (response.mCode);
    }
 
    if (!isUpgraded) {
        Quit ();
    }
    else {
        // Clear the read stream (SSE only contains outgoing events)
        GetReadStream ().Clear ();;
        mResponseEncoder.Clear ();
        
        mEventEncoder.Pipe (mToPacketConverter).Pipe (GetWriteStream ());
    }
}

void SSE::Client::SendData (const std::string& inData) {
    mEventEncoder.Write (inData);
}

std::unique_ptr<TCP::Client> SSE::ClientFactory::Create (std::unique_ptr<OS::Socket> inSocket) const {
    return std::make_unique<SSE::Client> (std::move (inSocket));
}

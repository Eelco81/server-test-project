
#include <iostream>

#include "HttpRemoteClient.h"
#include "HttpRequest.h"
#include "HttpResponse.h"

HTTP::RemoteClient::RemoteClient (std::string inAddress, std::string inPort) :
    TCP::Client (inAddress, inPort),
    mResponse (nullptr)
{
    // Forward raw TCP messages to the decoder
    GetReadStream ().Pipe (mToStringConverter).Pipe (mDecoder).Pipe (this, &HTTP::RemoteClient::HandleResponse);
    
    // Forward HTTP Requests as raw TCP messages through the encoder
    mEncoder.Pipe (mToPacketConverter).Pipe (GetWriteStream ());
    
    // Start the listener thread
    TCP::Client::Start ();
}

HTTP::RemoteClient::~RemoteClient () = default;

const HTTP::Response& HTTP::RemoteClient::Send (const HTTP::Request& inRequest){
    
    // Lock the mutex
    std::unique_lock<std::mutex> lock (mMutex);
    mResponse.reset (nullptr);
    
    // Send the message.
    mEncoder.Write (inRequest);
    
    // Wait for the response to arrive
    mConditionVar.wait (lock, [this]() { return this->mResponse != nullptr; });
    
    // Return teh response
    return *mResponse;
}

void HTTP::RemoteClient::HandleResponse (const Response& inResponse) {
    
    // Lock the mutex, set the response
    std::unique_lock<std::mutex> lock (this->mMutex);
    mResponse = std::make_unique<Response> (inResponse);
    
    // Notify the consumer that the message can be processed
    mConditionVar.notify_one ();
}

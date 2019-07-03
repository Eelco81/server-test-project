
#ifndef _HTTP_REMOTECLIENT_H_
#define _HTTP_REMOTECLIENT_H_

#include <string>
#include <memory>
#include <mutex>
#include <condition_variable>

#include "TcpClient.h"
#include "TcpPacketEncoders.h"
#include "HttpRequestEncoder.h"
#include "HttpResponseDecoder.h"

namespace HTTP {

struct Request;
struct Response;

/**
 * The HTTP::RemoteClient class forwards HTTP Requests to an open TCP socket to 
 * and waits for a resulting HTTP Responses to arrive on the same socket.
 */
class RemoteClient : public TCP::Client {

public:
    /**
     * Create a HTTP remote client, which connects to inAddress:inPort
     */
    RemoteClient (std::string inAddress, std::string inPort);
    
    /**
     * Virtual destructor
     */
    virtual ~RemoteClient ();
    
    /**
     * Send a request, this function waits for a response to arrive.
     */
    const Response& Send (const Request& inRequest);
    
protected:
    /**
     * Private response handler. This will force the RemoteClient::Send
     * method to yield.
     */
    void HandleResponse (const Response& inResponse);
    
    /**
     * MessageStreams which link ReadStream to the WriteStream
     */
    TCP::Packet2StringEncoder mPacket2String;
    TCP::String2PacketEncoder mString2Packet;
    RequestEncoder mEncoder;
    ResponseDecoder mDecoder;
    
    /**
     * Lock which enables waiting 
     */
    std::mutex mMutex;
    std::condition_variable mConditionVar;
    std::unique_ptr<Response> mResponse;
};

} // end namespace HTTP

#endif // _HTTP_REMOTECLIENT_H_

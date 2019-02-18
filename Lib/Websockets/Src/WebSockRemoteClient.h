
#ifndef _RFC6455_REMOTECLIENT_H_
#define _RFC6455_REMOTECLIENT_H_

#include <string>
#include <memory>
#include <mutex>
#include <condition_variable>

#include "TcpClient.h"
#include "HttpRequestEncoder.h"
#include "HttpResponseDecoder.h"
#include "StringConverter.h"
#include "WebSockFrameDecoder.h"
#include "WebSockFrameEncoder.h"
#include "WebSockPayloadDecoder.h"
#include "WebSockPayloadEncoder.h"

namespace HTTP {
    struct Request;
    struct Response;
}

namespace RFC6455 {

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
     * Send data over the RFC6455 connection
     */
    virtual void SendData (const std::vector<uint8_t>& inData) override;

    /**
     * Send data over the RFC6455 connection
     */
    virtual void SendData (const std::string& inData) override;

    /**
     * Get decoders for piping purposes
     */
    inline PayloadStringDecoder& GetStringDecoder () { return mPayloadStringDecoder; }
    inline PayloadBinaryDecoder& GetBinaryDecoder () { return mPayloadBinaryDecoder; }
    
private:
    /**
     * Process an incoming handshake HTTP Response (from HTTP::ResponseDecoder)
     */
    void HandleHandshake (const HTTP::Response& inResponse);
    
    /**
     * Process an incoming RFC6455 Frame (from RFC6455::FrameDecoder)
     */
    virtual void HandleReceivedFrame (const Frame& inFrame);
    
private:
    /**
     * MessageStreams which link ReadStream to the WriteStream
     */
    OS::PacketToStringConverter mToStringConverter;
    OS::StringToPacketConverter mToPacketConverter;
    HTTP::RequestEncoder mRequestEncoder;
    HTTP::ResponseDecoder mResponseDecoder;
    FrameDecoder mFrameDecoder;
    FrameEncoder mFrameEncoder;
    PayloadStringDecoder mPayloadStringDecoder;
    PayloadBinaryDecoder mPayloadBinaryDecoder;
    PayloadStringEncoder mPayloadStringEncoder;
    PayloadBinaryEncoder mPayloadBinaryEncoder;
    
};

} // end namespace HTTP

#endif // _HTTP_REMOTECLIENT_H_

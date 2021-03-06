
#ifndef _RFC6455_REMOTECLIENT_H_
#define _RFC6455_REMOTECLIENT_H_

#include <string>
#include <memory>
#include <mutex>
#include <condition_variable>

#include "TcpClient.h"
#include "TcpPacketEncoders.h"
#include "HttpRequestEncoder.h"
#include "HttpResponseDecoder.h"
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
 * The RFC6455::RemoteClient class forwards establishes a WebSocket
 * connection to a remote server.
 */
class RemoteClient : public TCP::Client {

public:
    /**
     * Create a RFC6455 remote client, which connects to inAddress:inPort
     */
    RemoteClient (std::string inAddress, std::string inPort);
    
    /**
     * Virtual destructor
     */
    virtual ~RemoteClient ();
    
    /**
     * Send data over the RFC6455 connection
     */
    virtual void SendPayload (const std::vector<uint8_t>& inData);

    /**
     * Send data over the RFC6455 connection
     */
    virtual void SendPayload (const std::string& inData);

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
    TCP::Packet2StringEncoder mPacket2String;
    TCP::String2PacketEncoder mString2Packet;
    TCP::RawPacket2PacketEncoder mRaw2Packet; 
    HTTP::RequestEncoder mRequestEncoder;
    HTTP::ResponseDecoder mResponseDecoder;
    FrameDecoder mFrameDecoder;
    FrameEncoder mFrameEncoder;
    PayloadStringDecoder mPayloadStringDecoder;
    PayloadBinaryDecoder mPayloadBinaryDecoder;
    PayloadStringEncoder mPayloadStringEncoder;
    PayloadBinaryEncoder mPayloadBinaryEncoder;
    
};

} // end namespace RFC6455

#endif // _RFC6455_REMOTECLIENT_H_

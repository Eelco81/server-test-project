
#ifndef _ECHOENDPOINT_H_
#define _ECHOENDPOINT_H_

#include "HttpEndpoint.h"

namespace API {

class EchoEndPoint : public HTTP::EndPoint {

public:
    /**
     * Constructor
     */
    EchoEndPoint (const std::string& inPath);
    
    /**
     * Virtual destructor
     */
    virtual ~EchoEndPoint ();

    /**
     * Echo the requests body to the response. 
     */
    virtual void Put (const HTTP::Request& inRequest, HTTP::Response& outResponse) override;
};

}

#endif // _ECHOENDPOINT_H_

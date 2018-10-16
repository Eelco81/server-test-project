
#ifndef _HTTP_ENDPOINT_H_
#define _HTTP_ENDPOINT_H_

#include <string>

#include "Macros.h"
#include "HttpCodes.h"
#include "HttpRequest.h"
#include "HttpResponse.h"

namespace HTTP {

class EndPoint {

public:
    /**
     * Constructor
     */
    EndPoint (const std::string& inPath, Method inMethod);
    
    /**
     * Virtual destructor
     */
    virtual ~EndPoint ();
    
    /**
     * Function to implement. 
     */
    virtual void Execute (const Request& inRequest, Response& outReponse) = 0;
    
    /**
     * Retrieve the path of this endpoint
     */
    const std::string& GetPath () const { return mPath; }
    
    /**
     * Retrieve the method of this endpoint.
     */
    Method GetMethod () const { return mMethod; }

private:
    std::string mPath;
    Method mMethod;
};

}

#endif // _HTTP_ENDPOINT_H_


#ifndef _HTTP_ENDPOINT_H_
#define _HTTP_ENDPOINT_H_

#include <string>
#include "Macros.h"
#include "HttpCodes.h"

namespace HTTP {

class Request;
class Response;

class EndPoint {

    NO_COPY_CONSTRUCTORS (EndPoint);
    
public:
    EndPoint () = delete;
    EndPoint (const std::string& inPath, Method inMethod);
    virtual ~EndPoint ();

public:
    virtual void Execute (const Request& inRequest, Response& outReponse) = 0;

    const std::string& GetPath () const { return mPath; }
    Method GetMethod () const { return mMethod; }

private:
    std::string mPath;
    Method mMethod;
};

}

#endif // _HTTP_ENDPOINT_H_

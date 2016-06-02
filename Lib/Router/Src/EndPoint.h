
#ifndef _ENDPOINT_H_
#define _ENDPOINT_H_

#include <string>
#include "Codes.h"

namespace Api {

class Request;
class Response;

class EndPoint {

public:
    EndPoint () = delete;
    EndPoint (const EndPoint& inEndPoint) = delete;
    EndPoint (const std::string& inPath, Codes::Methods inMethod);
    virtual ~EndPoint ();

public:
    virtual void Execute (const Request& inRequest, Response& outReponse) = 0;

    const std::string& GetPath () const { return mPath; }
    Codes::Methods GetMethod () const { return mMethod; }

private:
    std::string mPath;
    Codes::Methods mMethod;
};

}

#endif

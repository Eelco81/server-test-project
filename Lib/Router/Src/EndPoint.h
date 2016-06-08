
#ifndef _ENDPOINT_H_
#define _ENDPOINT_H_

#include <string>
#include "Macros.h"
#include "Codes.h"

namespace API {

class Request;
class Response;

class EndPoint {

    NO_COPY_CONSTRUCTORS (EndPoint);

public:
    EndPoint () = delete;
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

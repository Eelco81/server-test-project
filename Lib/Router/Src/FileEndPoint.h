
#ifndef _FILEENDPOINT_H_
#define _FILEENDPOINT_H_

#include <string>

#include "HttpRequest.h"
#include "HttpResponse.h"
#include "HttpEndpoint.h"

namespace API {

class FileEndPoint : public HTTP::EndPoint {
    
public:
    FileEndPoint (const std::string& inPath, const std::string& inFileName);
    virtual ~FileEndPoint ();

    virtual void Execute (const HTTP::Request& inRequest, HTTP::Response& outResponse) override;
    
private:
    std::string mFileName;
};

}

#endif // _FILEENDPOINT_H_


#ifndef _FILEENDPOINT_H_
#define _FILEENDPOINT_H_

#include <string>

#include "HttpRequest.h"
#include "HttpResponse.h"
#include "HttpEndpoint.h"

namespace API {

class FileEndPoint : public HTTP::EndPoint {
    
public:
    /**
     * Constructor
     */
    FileEndPoint (const std::string& inPath, const std::string& inFileName);
    
    /**
     * Virtual destructor
     */
    virtual ~FileEndPoint ();

    /**
     * Retrieve contents of a file, header set accordingly
     */
    virtual void Get (const HTTP::Request& inRequest, HTTP::Response& outResponse) override;
    
    /**
     * Set contents of a file
     */
    virtual void Put (const HTTP::Request& inRequest, HTTP::Response& outResponse) override;
    
private:
    std::string mFileName;
};

}

#endif // _FILEENDPOINT_H_

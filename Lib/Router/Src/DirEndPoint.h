
#ifndef _DIRENDPOINT_H_
#define _DIRENDPOINT_H_

#include "HttpRequest.h"
#include "HttpResponse.h"
#include "HttpEndpoint.h"

namespace API {

class DirEndPoint : public HTTP::RegexEndPoint {
    
public:
    /**
     * Constructor
     */
    DirEndPoint (const std::string& inPath, const std::string& inDirName);
    
    /**
     * Virtual destructor
     */
    virtual ~DirEndPoint ();

    /**
     * Retrieve contents of a file, header set accordingly
     */
    virtual void Get (const HTTP::Request& inRequest, HTTP::Response& outResponse) override;
    
    /**
     * Set contents of a file
     */
    virtual void Put (const HTTP::Request& inRequest, HTTP::Response& outResponse) override;
    
private:
    std::string mDirName;
};

}




#endif // _DIRENDPOINT_H_

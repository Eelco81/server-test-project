
#ifndef _API_LISTDIRENDPOINT_H_
#define _API_LISTDIRENDPOINT_H_

#include "HttpEndPoint.h"

namespace API {

class ListDirEndPoint : public HTTP::EndPoint {
    
public:
    /**
     * Constructor
     */
    ListDirEndPoint (const std::string& inPath, const std::string& inDirName, bool inIncludeExtensions = true);
    
    /**
     * Virtual destructor
     */
    virtual ~ListDirEndPoint ();

    /**
     * Retrieve contents of a directory
     */
    virtual void Get (const HTTP::Request& inRequest, HTTP::Response& outResponse) override;
    
private:
    std::string mDirName;
    bool mIncludeExtensions;
};



}

#endif // _API_LISTDIRENDPOINT_H_

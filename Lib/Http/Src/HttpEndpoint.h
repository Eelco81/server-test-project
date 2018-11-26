
#ifndef _HTTP_ENDPOINT_H_
#define _HTTP_ENDPOINT_H_

#include <vector>
#include <string>
#include <regex>

#include "Macros.h"
#include "HttpCodes.h"
#include "HttpRequest.h"
#include "HttpResponse.h"

namespace HTTP {

class EndPoint {

public:
    
    using ParameterList = std::vector<std::string>;
    
    /**
     * Constructor using complete path
     */
    EndPoint (const std::string& inPath, Method inMethod);
    
    /**
     * Constructor using regex
     */
    EndPoint (const std::regex& inPath, Method inMethod);
    
    /**
     * Virtual destructor
     */
    virtual ~EndPoint ();
    
    /**
     * Function to implement for each endpoint.
     */
    virtual void Execute (const Request& inRequest, Response& outResponse) = 0;
    
    /** 
     * Called by the router.
     */
    bool Route (const Request& inRequest, Response& outResponse);
    
protected:
    /**
     * Retrieve the regex parameter list
     */
    const ParameterList& GetParameterList() const { return mParameterList; }
    
private:
    ParameterList mParameterList;
    std::string mPath;
    std::regex mRegex;
    Method mMethod;
};

}

#endif // _HTTP_ENDPOINT_H_

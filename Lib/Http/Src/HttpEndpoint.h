
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

class AbstractEndPoint {

public:
    /**
     * Virtual destructor
     */
    virtual ~AbstractEndPoint () = default;
    
    /**
     * Optional GET Function to implement for each endpoint.
     */
    virtual void Get (const Request& inRequest, Response& outResponse) {}
    
    /**
     * Optional PUT Function to implement for each endpoint.
     */
    virtual void Put (const Request& inRequest, Response& outResponse) {}
    
    /**
     * Optional POST Function to implement for each endpoint.
     */
    virtual void Post (const Request& inRequest, Response& outResponse) {}
    
    /**
     * Optional DELETE Function to implement for each endpoint.
     */
    virtual void Delete (const Request& inRequest, Response& outResponse) {}
    
    /** 
     * Called by the router.
     */
    virtual bool Route (const Request& inRequest, Response& outResponse);
    
};

class EndPoint : public AbstractEndPoint {
    
public:
    /**
     * Constructor using complete path
     */
    EndPoint (const std::string& inPath);
    
    /**
     * Destructor
     */
    virtual ~EndPoint () = default;
    
    /** 
     * Called by the router.
     */
    bool Route (const Request& inRequest, Response& outResponse) override;
    
private:
    std::string mPath;
};

class RegexEndPoint : public AbstractEndPoint {
    
public:
    using ParameterList = std::vector<std::string>;
    
    /**
     * Constructor using complete path
     */
    RegexEndPoint (const std::regex& inPath);
    
    /**
     * Destructor
     */
    virtual ~RegexEndPoint () = default;
    
    /** 
     * Called by the router.
     */
    bool Route (const Request& inRequest, Response& outResponse) override;
    
protected:
    /**
     * Retrieve the regex parameter list
     */
    const ParameterList& GetParameterList() const { return mParameterList; }
    
private:
    ParameterList mParameterList;
    std::regex mRegex;
};

}

#endif // _HTTP_ENDPOINT_H_

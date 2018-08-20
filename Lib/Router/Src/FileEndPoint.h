
#ifndef _FILEENDPOINT_H_
#define _FILEENDPOINT_H_

#include "HttpRequest.h"
#include "HttpResponse.h"
#include "HttpEndPoint.h"
#include "Files.h"

namespace API {

class FileEndPoint : public HTTP::EndPoint {
    
public:
    FileEndPoint (const std::string& inPath, const std::string& inFileName) : 
        HTTP::EndPoint (inPath, HTTP::Method::GET),
        mFileName (inFileName)
    {
    }
    
    virtual void Execute (const HTTP::Request& inRequest, HTTP::Response& outResponse) override {
        
        std::vector<uint8_t> data;
        if (OS::Files::Read (mFileName, data)) {
            outResponse.mCode = HTTP::Code::OK;
            outResponse.mBody.assign (reinterpret_cast<char*> (data.data ()), data.size ());
            outResponse.mHeaders[HTTP::Header::CONTENT_LENGTH] = std::to_string (outResponse.mBody.size ());
            
            const std::string extension (OS::Files::GetExtension (mFileName));
            if (extension == "html") {
                outResponse.mHeaders[HTTP::Header::CONTENT_TYPE] = "text/html";
            }
            else if (extension == "csv") {
                outResponse.mHeaders[HTTP::Header::CONTENT_TYPE] = "text/csv";
            }
            else if (extension == "css") {
                outResponse.mHeaders[HTTP::Header::CONTENT_TYPE] = "text/css";
            }
            else if (extension == "xml") {
                outResponse.mHeaders[HTTP::Header::CONTENT_TYPE] = "text/xml";
            }
            else if (extension == "json") {
                outResponse.mHeaders[HTTP::Header::CONTENT_TYPE] = "application/json";
            }
            else if (extension == "js") {
                outResponse.mHeaders[HTTP::Header::CONTENT_TYPE] = "application/javascript";
            }
            else {
                 outResponse.mHeaders[HTTP::Header::CONTENT_TYPE] = "text/plain";
            }
        }
        else {
            outResponse.mCode = HTTP::Code::INTERNAL_SERVER_ERROR;
        }
    }
    
private:
    std::string mFileName;
};

}

#endif // _FILEENDPOINT_H_

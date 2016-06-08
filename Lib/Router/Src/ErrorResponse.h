
#ifndef _ERRORRESPONSE_H_
#define _ERRORRESPONSE_H_

#include <string>
#include "Response.h"
#include "Codes.h"

namespace API {

class ErrorResponse : public Response {

public:
    ErrorResponse () = delete;
    ErrorResponse (Codes::Responses inCode, std::string& inErrorMessage);
    ~ErrorResponse () = default;

public:
    const std::string& GetMessage () const { return mMessage; }

public:
    virtual void ToXml (XmlNode& outNode) const override;

private:
    std::string mMessage;
};

}


#endif

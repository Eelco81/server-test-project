
#ifndef _ERRORRESPONSE_H_
#define _ERRORRESPONSE_H_

#include <string>
#include "Response.h"
#include "Codes.h"

namespace Api {

class ErrorResponse : public Response {

public:
    ErrorResponse (Codes::Responses inCode, std::string& inErrorMessage);
    ~ErrorResponse ();

public:
    const std::string& GetMessage () const { return mMessage; }

public:
    virtual void ToXml (Core::XmlNode& outNode) const override;

private:
    std::string mMessage;
};

}


#endif

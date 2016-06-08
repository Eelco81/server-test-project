
#ifndef _RESPONSE_H_
#define _RESPONSE_H_

#include <string>

#include "Message.h"
#include "Codes.h"

namespace API {

class Response : public Message {

public:
    Response ();
    Response (const Message& inMessage);
    ~Response () = default;

public:
    virtual void ToXml (XmlNode& outNode) const override;

public:
    Codes::Responses GetCode () const { return mCode; }
    void SetCode (Codes::Responses inCode) { mCode = inCode; }

private:
    Codes::Responses mCode;

};

}

#endif

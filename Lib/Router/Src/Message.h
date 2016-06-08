
#ifndef _MESSAGE_H_
#define _MESSAGE_H_

#include <string>
#include "Header.h"

namespace OS {
    class XmlNode;
}

namespace API {

class Message {

public:
    Message () = delete;
    Message (const std::string& inRootTag);
    Message (const std::string& inRootTag, const Message& inMessage);
    virtual ~Message () = default;

public:
    const Header& GetHeader () const { return mHeader; }
    void SetHeader (const Header& inHeader) { mHeader = inHeader; }

public:
    virtual bool FromXml (const XmlNode& inNode, std::string& outError);
    virtual void ToXml (XmlNode& outNode) const;

private:
    Header mHeader;
    std::string mRootTag;
};

};


#endif

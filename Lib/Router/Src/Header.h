
#ifndef _HEADER_H_
#define _HEADER_H_

#include <string>

#include "Codes.h"

class XmlNode;

namespace API {

struct Header {

    Header ();
    Header (const std::string& inPath, const std::string& inVersion, Codes::Methods inMethod);
    ~Header ();

    std::string mPath;
    std::string mVersion;
    Codes::Methods mMethod;
    unsigned mClient;

    bool FromXml (const XmlNode& inNode, std::string& outError);
    void ToXml (XmlNode& inNode) const;
};


}

#endif

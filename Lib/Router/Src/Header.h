
#ifndef _HEADER_H_
#define _HEADER_H_

#include <string>

#include "Codes.h"

namespace Core {
    class XmlNode;
}

namespace Api {

struct Header {

    Header ();
    Header (const std::string& inPath, const std::string& inVersion, Codes::Methods inMethod);
    ~Header ();

    std::string mPath;
    std::string mVersion;
    Codes::Methods mMethod;
    unsigned mClient;

    bool FromXml (const Core::XmlNode& inNode, std::string& outError);
    void ToXml (Core::XmlNode& inNode) const;
};


}

#endif

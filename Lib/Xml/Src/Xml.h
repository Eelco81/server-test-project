
#ifndef _XML_H_
#define _XML_H_

#include <string>

class TiXmlElement;

#include "XmlNode.h"

class Xml {

public:
    static bool Parse (const std::string& inXmlString, XmlNode& outNode, std::string& outError);
    static bool Print (const XmlNode& inNode, std::string& outXmlString, std::string& outError, bool inPrettyPrint = true);

private:
    static bool ConvertFromTinyXml (TiXmlElement* inElement, XmlNode& ioNode);
    static bool ConvertToTinyXml (const XmlNode& ioNode, TiXmlElement* inElement);
};



#endif


#ifndef _XMLNODE_H_
#define _XMLNODE_H_

#include <string>
#include <map>
#include <vector>

class TiXmlElement;

class XmlNode {

public:
    XmlNode ();
    XmlNode (const std::string& inName);
    ~XmlNode ();

public:
    const std::string& GetName () const;
    void SetName (const std::string& inName);

    bool GetAttribute (const std::string& inKey, std::string& outValue) const;
    const std::map<std::string, std::string>& GetAttributes () const;
    void SetAttribute (const std::string& inKey, const std::string& inValue);

    void AddChild (XmlNode inNode);
    std::size_t GetChildCount () const;
    bool GetChild (std::size_t inIndex, XmlNode& outChild) const;
    bool FindChild (const std::string& inName, XmlNode& outChild) const;

private:
    std::string mName;
    std::map <std::string, std::string> mAttributes;
    std::vector <XmlNode> mChildren;

};


#endif

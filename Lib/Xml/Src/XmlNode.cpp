#include "XmlNode.h"

Core::XmlNode::XmlNode () {
}

Core::XmlNode::XmlNode (const std::string& inName) :
    mName (inName)
{
}

Core::XmlNode::~XmlNode () {
}

const std::string& Core::XmlNode::GetName () const {
    return mName;
}

void Core::XmlNode::SetName (const std::string& inName) {
    mName = inName;
}

bool Core::XmlNode::GetAttribute (const std::string& inKey, std::string& outValue) const {
    const std::map <std::string, std::string>::const_iterator it = mAttributes.find (inKey);
    if (it != mAttributes.end ()) {
        outValue = it->second;
        return true;
    }
    return false;
}

const std::map<std::string, std::string>& Core::XmlNode::GetAttributes () const {
    return mAttributes;
}

void Core::XmlNode::SetAttribute (const std::string& inKey, const std::string& inValue) {
    mAttributes [inKey] = inValue;
}

void Core::XmlNode::AddChild (XmlNode inNode) {
    mChildren.push_back (inNode);
}

std::size_t Core::XmlNode::GetChildCount () const {
    return mChildren.size ();
}

bool Core::XmlNode::GetChild (std::size_t inIndex, XmlNode& outChild) const {
    if (inIndex < GetChildCount ()) {
        outChild = mChildren [inIndex];
        return true;
    }
    return false;
}

bool Core::XmlNode::FindChild (const std::string& inName, XmlNode& outChild) const {
    for (std::size_t i = 0; i < GetChildCount (); i++) {
        if (mChildren [i].GetName () == inName) {
            outChild = mChildren [i];
            return true;
        }
    }
    return false;
}
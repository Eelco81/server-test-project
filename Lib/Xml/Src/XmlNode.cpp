#include "XmlNode.h"

XmlNode::XmlNode () {
}

XmlNode::XmlNode (const std::string& inName) :
    mName (inName)
{
}

XmlNode::~XmlNode () {
}

const std::string& XmlNode::GetName () const {
    return mName;
}

void XmlNode::SetName (const std::string& inName) {
    mName = inName;
}

bool XmlNode::GetAttribute (const std::string& inKey, std::string& outValue) const {
    const std::map <std::string, std::string>::const_iterator it = mAttributes.find (inKey);
    if (it != mAttributes.end ()) {
        outValue = it->second;
        return true;
    }
    return false;
}

const std::map<std::string, std::string>& XmlNode::GetAttributes () const {
    return mAttributes;
}

void XmlNode::SetAttribute (const std::string& inKey, const std::string& inValue) {
    mAttributes [inKey] = inValue;
}

void XmlNode::AddChild (XmlNode inNode) {
    mChildren.push_back (inNode);
}

std::size_t XmlNode::GetChildCount () const {
    return mChildren.size ();
}

bool XmlNode::GetChild (std::size_t inIndex, XmlNode& outChild) const {
    if (inIndex < GetChildCount ()) {
        outChild = mChildren [inIndex];
        return true;
    }
    return false;
}

bool XmlNode::FindChild (const std::string& inName, XmlNode& outChild) const {
    for (std::size_t i = 0; i < GetChildCount (); i++) {
        if (mChildren [i].GetName () == inName) {
            outChild = mChildren [i];
            return true;
        }
    }
    return false;
}
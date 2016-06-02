
#include "tinyxml.h"
#include "Xml.h"

bool Core::Xml::Parse (const std::string& inXmlString, Core::XmlNode& outNode, std::string& outError) {

    TiXmlDocument doc;
    doc.Parse (inXmlString.c_str ());
    if (doc.Error ()) {
        outError = doc.ErrorDesc ();
        return false;
    }
    return ConvertFromTinyXml (doc.FirstChildElement (), outNode);
}

bool Core::Xml::Print (const Core::XmlNode& inNode, std::string& outXmlString, std::string& outError, bool inPrettyPrint) {
    
    TiXmlDocument doc;
    TiXmlElement* root = new TiXmlElement ("");
    doc.LinkEndChild (root);

    ConvertToTinyXml (inNode, doc.FirstChildElement ());

    TiXmlPrinter printer;
    if (!inPrettyPrint) {
        printer.SetStreamPrinting ();
    }

    doc.Accept (&printer);
    outXmlString = printer.CStr ();
    return true;
}

bool Core::Xml::ConvertFromTinyXml (TiXmlElement* inElement, XmlNode& ioNode) {

    // Set attribute name
    ioNode.SetName (inElement->Value ());

    // obtain attributes
    TiXmlAttribute* attribute = inElement->FirstAttribute ();
    while (attribute) {
        ioNode.SetAttribute (attribute->Name (), attribute->Value ());
        attribute = attribute->Next ();
    }

    // obtain children
    TiXmlElement* element = inElement->FirstChildElement ();
    while (element) {
        XmlNode node;
        ConvertFromTinyXml (element, node);
        ioNode.AddChild (node);
        element = element->NextSiblingElement ();
    }
    return true;
}

bool Core::Xml::ConvertToTinyXml (const XmlNode& inNode, TiXmlElement* outElement) {
    
    outElement->SetValue (inNode.GetName ().c_str ());
    
    for (auto it = inNode.GetAttributes ().begin (); it != inNode.GetAttributes ().end (); it++) {
        outElement->SetAttribute (it->first.c_str (), it->second.c_str ());
    }

    for (std::size_t i = 0; i < inNode.GetChildCount (); i++) {
        TiXmlElement* element = new TiXmlElement ("");
        XmlNode node;
        inNode.GetChild (i, node);
        ConvertToTinyXml (node, element);
        outElement->LinkEndChild (element);
    }

    return true;
}


#include "gmock\gmock.h"

#include "Xml.h"

TEST (XmlTester, ParseString) {

    std::string testStr ("<root attr='attribute'><child/><child attr='attribute1'/></root>");

    XmlNode node;
    std::string error;
    ASSERT_TRUE (Xml::Parse (testStr, node, error));
    ASSERT_EQ (node.GetName (), std::string ("root"));
    
    std::string tmp;
    ASSERT_TRUE (node.GetAttribute ("attr", tmp));
    ASSERT_EQ (tmp, std::string ("attribute"));

    XmlNode child;
    ASSERT_TRUE (node.GetChild (0, child));
    ASSERT_EQ (child.GetName (), std::string ("child"));
   
    ASSERT_TRUE (node.GetChild (1, child));
    ASSERT_EQ (child.GetName (), std::string ("child"));

    ASSERT_TRUE (child.GetAttribute ("attr", tmp));
    ASSERT_EQ (tmp, std::string ("attribute1"));
}

TEST (XmlTester, ParseStringErrors) {

    std::string testStr ("<root ");

    std::string error;
    XmlNode node;
    ASSERT_FALSE (Xml::Parse (testStr, node, error));
    ASSERT_EQ (error, std::string ("Error reading Attributes."));

    std::string tmp;
    ASSERT_FALSE (node.GetAttribute ("attr", tmp));
    
    XmlNode child;
    ASSERT_FALSE (node.GetChild (0, child));
}

TEST (XmlTester, PrintString) {
    
    XmlNode root ("root");
    root.SetAttribute ("attr", "attribute");

    XmlNode child1 ("child");
    root.AddChild (child1);

    XmlNode child2 ("child");
    child2.SetAttribute ("attr", "attribute1");
    root.AddChild (child2);

    std::string output;
    std::string error;
    ASSERT_TRUE (Xml::Print (root, output, error));
    ASSERT_EQ (output, std::string ("<root attr=\"attribute\">\n    <child />\n    <child attr=\"attribute1\" />\n</root>\n"));
}



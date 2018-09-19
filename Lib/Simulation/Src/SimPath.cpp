
#include <regex>

#include "SimPath.h"
#include "SimException.h"

SIM::Path::Path (const std::string& inTarget) {
    
    std::regex rePath ("^([A-Za-z0-9]+).([A-Za-z0-9]+).([A-Za-z0-9]+)$");
    
    std::smatch matchPath;
    if (!std::regex_search (inTarget, matchPath, rePath)) {
        throw Exception (std::string ("Illegal path <") + inTarget + std::string (">"));
    }
    
    mBlockID = matchPath[1].str ();
    mPortID = matchPath[3].str ();

    const auto typeName (matchPath[2].str ());
    
    if (typeName == "in") {
        mType = INPUT;
    }
    else if (typeName == "out") {
        mType = OUTPUT;
    }
    else if (typeName == "par") {
        mType = PARAMETER;
    }
    else {
        throw Exception (std::string ("Illegal identifier in path <") + inTarget + std::string (">, only in/out/par allowed"));
    }
}

SIM::Path::Path (const std::string& inBlockID, const std::string& inPortID, Type inType) :
    mBlockID (inBlockID),
    mPortID (inPortID),
    mType (inType)
{
}

std::string SIM::Path::ToString () const {
    
    std::string type ("??");
    if (mType == INPUT) {
        type = "in";
    }
    else if (mType == OUTPUT) {
        type = "out";
    }
    else if (mType == PARAMETER) {
        type = "par";
    }
    
    return mBlockID + std::string (".") + type + std::string (".") + mPortID;
}

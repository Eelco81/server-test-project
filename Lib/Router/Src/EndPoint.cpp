
#include "EndPoint.h"

API::EndPoint::EndPoint (const std::string& inPath, Codes::Methods inMethod) :
    mPath (inPath),
    mMethod (inMethod)
{
}

API::EndPoint::~EndPoint () {
}


#include "EndPoint.h"

Api::EndPoint::EndPoint (const std::string& inPath, Codes::Methods inMethod) :
    mPath (inPath),
    mMethod (inMethod)
{
}

Api::EndPoint::~EndPoint () {
}

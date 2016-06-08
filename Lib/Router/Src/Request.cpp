
#include "Request.h"
#include "Buffer.h"
#include "Xml.h"

namespace {
    const std::string kRootTag ("Request");
    const std::string kHeaderTag ("Header");
    const std::string kBodyTag ("Body");
}

API::Request::Request ():
    Message (kRootTag)
{
}

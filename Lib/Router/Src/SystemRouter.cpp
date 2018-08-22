
#include "SystemRouter.h"
#include "EchoEndPoint.h"
#include "FileEndPoint.h"
#include "VersionEndPoint.h"


API::SystemRouter::SystemRouter () :
    HTTP::Router ()
{
    AddEndPoint (std::make_unique<VersionEndPoint> ());
    AddEndPoint (std::make_unique<EchoEndPoint> ());
    AddEndPoint (std::make_unique<FileEndPoint> ("/home", "./Public/index.html"));
}

API::SystemRouter::~SystemRouter () = default;

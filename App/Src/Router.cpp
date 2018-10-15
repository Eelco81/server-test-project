
#include "Router.h"
#include "EchoEndPoint.h"
#include "FileEndPoint.h"
#include "VersionEndPoint.h"
#include "TimeEndPoint.h"

#include "SimulationEndPoints.h"
#include "SimService.h"

SystemRouter::SystemRouter (std::shared_ptr<SIM::Service> inService) :
    HTTP::Router ()
{
    AddEndPoint (std::make_shared<API::TimeEndPoint> ("/api/time"));
    AddEndPoint (std::make_shared<API::VersionEndPoint> ("/api/version"));
    AddEndPoint (std::make_shared<API::EchoEndPoint> ("/api/echo"));

    AddEndPoint (std::make_shared<API::FileEndPoint> ("/home", "./Public/index.html"));

    AddEndPoint (std::make_shared<API::SimLoadEndPoint> (inService));
    AddEndPoint (std::make_shared<API::SimStartEndPoint> (inService));
    AddEndPoint (std::make_shared<API::SimStopEndPoint> (inService));
}

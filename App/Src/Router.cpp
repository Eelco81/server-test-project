
#include "Router.h"
#include "EchoEndPoint.h"
#include "FileEndPoint.h"
#include "VersionEndPoint.h"
#include "TimeEndPoint.h"

#include "SimulationEndpoints.h"
#include "SimService.h"

SystemRouter::SystemRouter (std::shared_ptr<SIM::Service> inService) :
    HTTP::Router ()
{
    AddEndPoint (std::make_shared<API::TimeEndPoint> ("/api/time"));
    AddEndPoint (std::make_shared<API::VersionEndPoint> ("/api/version"));
    AddEndPoint (std::make_shared<API::EchoEndPoint> ("/api/echo"));

    AddEndPoint (std::make_shared<API::FileEndPoint> ("/home", "./Public/index.html"));

    AddEndPoint (std::make_shared<API::SimLoadEndPoint> ("/api/simulation/config", inService));
    AddEndPoint (std::make_shared<API::SimStartEndPoint> ("/api/simulation/runner", inService));
    AddEndPoint (std::make_shared<API::SimStopEndPoint> ("/api/simulation/runner", inService));
    AddEndPoint (std::make_shared<API::SimGetValuesEndPoint> ("/api/simulation/ports", inService));
    AddEndPoint (std::make_shared<API::SimGetValueEndPoint> ("/api/simulation/ports/id", inService));
    AddEndPoint (std::make_shared<API::SimSetValueEndPoint> ("/api/simulation/ports/id", inService));
}

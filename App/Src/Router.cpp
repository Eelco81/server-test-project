
#include "Router.h"
#include "EchoEndPoint.h"
#include "FileEndPoint.h"
#include "VersionEndPoint.h"
#include "TimeEndPoint.h"
#include "SimExecEndPoint.h"
#include "SimConfigEndPoint.h"
#include "SimPortsEndPoint.h"
#include "SimPortEndPoint.h"

SystemRouter::SystemRouter (std::shared_ptr<SIM::Service> inService) :
    HTTP::Router ()
{
    AddEndPoint (std::make_shared<API::TimeEndPoint> ("/api/time"));
    AddEndPoint (std::make_shared<API::VersionEndPoint> ("/api/version"));
    AddEndPoint (std::make_shared<API::EchoEndPoint> ("/api/echo"));

    AddEndPoint (std::make_shared<API::FileEndPoint> ("/home", "./Public/index.html"));

    AddEndPoint (std::make_shared<API::SIM::ExecEndPoint> ("/api/simulation", inService, "./Config/"));
    AddEndPoint (std::make_shared<API::SIM::ConfigEndPoint> ("/api/simulation/config/([A-Za-z0-9]+)", inService, "./Config/"));
    AddEndPoint (std::make_shared<API::SIM::PortsEndPoint> ("/api/simulation/ports", inService));
    AddEndPoint (std::make_shared<API::SIM::PortEndPoint> ("/api/simulation/ports", inService));
}

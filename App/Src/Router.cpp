
#include "Router.h"
#include "EchoEndPoint.h"
#include "FileEndPoint.h"
#include "DirEndPoint.h"
#include "ListDirEndPoint.h"
#include "VersionEndPoint.h"
#include "TimeEndPoint.h"
#include "SimExecEndPoint.h"
#include "SimPortsEndPoint.h"
#include "SimPortEndPoint.h"
#include "SimSamplersEndPoint.h"

SystemRouter::SystemRouter (std::shared_ptr<SIM::Service> inService) :
    HTTP::Router ()
{
    AddEndPoint (std::make_shared<API::TimeEndPoint> ("/api/time"));
    AddEndPoint (std::make_shared<API::VersionEndPoint> ("/api/version"));
    AddEndPoint (std::make_shared<API::EchoEndPoint> ("/api/echo"));

    AddEndPoint (std::make_shared<API::FileEndPoint> ("/home", "./Public/index.html"));
    AddEndPoint (std::make_shared<API::DirEndPoint> ("/home", "./Public"));

    AddEndPoint (std::make_shared<API::SIM::ExecEndPoint> ("/api/simulation", inService, "./Config/"));
    AddEndPoint (std::make_shared<API::DirEndPoint> ("/api/simulation/config", "./Config/", ".json"));
    AddEndPoint (std::make_shared<API::ListDirEndPoint> ("/api/simulation/config", "./Config/", false));
    AddEndPoint (std::make_shared<API::SIM::PortsEndPoint> ("/api/simulation/ports", inService));
    AddEndPoint (std::make_shared<API::SIM::PortEndPoint> ("/api/simulation/ports", inService));
    AddEndPoint (std::make_shared<API::SIM::SamplersEndPoint> ("/api/simulation/samplers", inService));
    
}

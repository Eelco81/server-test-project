
#include "gmock/gmock.h"
#include "SimulationEndpoints.h"
#include "SimService.h"
#include "SimFactory.h"

#include <json.hpp>
using json = nlohmann::json;

/*
TEST (SimulationEndpointsTester, Load) {
    
    auto service = std::make_shared<MockService> ();
    
    EXPECT_CALL (*service, IsRunning ()).WillOnce (::testing::Return (false));
    EXPECT_CALL (*service, Load (::testing::_)).WillOnce (::testing::Return ());
    
    API::SimRunEndPoint endPoint ("/hello", service);
    
    HTTP::Request request;
    HTTP::Response response;
    
    request.mBody = "{}";
    endPoint.Execute (request, response);
    
    ASSERT_EQ (HTTP::Code::ACCEPTED, response.mCode);
}

TEST (SimulationEndpointsTester, LoadRunningSim) {
    
    auto service = std::make_shared<MockService> ();
    
    EXPECT_CALL (*service, IsRunning ()).WillOnce (::testing::Return (true));
    
    API::SimRunEndPoint endPoint ("/hello", service);
    
    HTTP::Request request;
    HTTP::Response response;
    
    request.mBody = "{}";
    endPoint.Execute (request, response);
    
    ASSERT_EQ (HTTP::Code::FORBIDDEN, response.mCode);
}

TEST (SimulationEndpointsTester, LoadBadConfig) {
    
    auto service = std::make_shared<MockService> ();
    
    EXPECT_CALL (*service, IsRunning ()).WillOnce (::testing::Return (false));
    
    API::SimLoadEndPoint endPoint ("/hello", service);
    
    HTTP::Request request;
    HTTP::Response response;
    
    request.mBody = "I AM NOT JSON";
    endPoint.Execute (request, response);
    
    ASSERT_EQ (HTTP::Code::BAD_REQUEST, response.mCode);
}
*/
/*


TEST (SimulationEndpointsTester, GetValues) {
    
    const std::size_t size (10u);
    const std::vector<SIM::Value> values(size);
    
    auto service = std::make_shared<MockService> ();
    
    EXPECT_CALL (*service, IsLoaded ()).WillOnce (::testing::Return (true));
    EXPECT_CALL (*service, GetValues ()).WillOnce (::testing::Return (values));
    
    API::SimPortsEndPoint endPoint ("/hello", service);
    
    HTTP::Request request;
    HTTP::Response response;
    
    endPoint.Get (request, response);
    
    ASSERT_EQ (HTTP::Code::OK, response.mCode);
    
    ASSERT_THAT (response.GetBody (), ::testing::HasSubstr ("{\"ports\":["));
}
*/
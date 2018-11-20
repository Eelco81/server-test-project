
#include "gmock/gmock.h"
#include "SimulationEndpoints.h"
#include "SimService.h"
#include "SimFactory.h"

#include <json.hpp>
using json = nlohmann::json;

namespace {

class MockService : public SIM::IService {
public:
    MOCK_METHOD0 (Start, void ());
    MOCK_METHOD0 (Stop, void ());
    MOCK_METHOD1 (Load, void (const json&));
    MOCK_METHOD0 (UnLoad, void ());
    MOCK_CONST_METHOD0 (IsRunning, bool ());
    MOCK_CONST_METHOD0 (IsLoaded, bool ());
    MOCK_METHOD0 (GetPaths, std::vector<SIM::Path> ());
    MOCK_METHOD0 (GetValues, std::vector<SIM::Value> ());
    MOCK_METHOD1 (GetValue, SIM::Value (const SIM::Path&));
    MOCK_METHOD1 (SetValue, void (const SIM::Value&));
};

}

TEST (SimulationEndpointsTester, Load) {
    
    auto service = std::make_shared<MockService> ();
    
    EXPECT_CALL (*service, IsRunning ()).WillOnce (::testing::Return (false));
    EXPECT_CALL (*service, Load (::testing::_)).WillOnce (::testing::Return ());
    
    API::SimLoadEndPoint endPoint ("/hello", service);
    
    HTTP::Request request;
    HTTP::Response response;
    
    request.mBody = "{}";
    endPoint.Execute (request, response);
    
    ASSERT_EQ (HTTP::Code::ACCEPTED, response.mCode);
}

TEST (SimulationEndpointsTester, LoadRunningSim) {
    
    auto service = std::make_shared<MockService> ();
    
    EXPECT_CALL (*service, IsRunning ()).WillOnce (::testing::Return (true));
    
    API::SimLoadEndPoint endPoint ("/hello", service);
    
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

TEST (SimulationEndpointsTester, Start) {
    
    auto service = std::make_shared<MockService> ();
    
    EXPECT_CALL (*service, IsLoaded ()).WillOnce (::testing::Return (true));
    EXPECT_CALL (*service, IsRunning ()).WillOnce (::testing::Return (false));
    EXPECT_CALL (*service, Start ()).WillOnce (::testing::Return ());
    
    API::SimStartEndPoint endPoint ("/hello", service);
    
    HTTP::Request request;
    HTTP::Response response;
    
    endPoint.Execute (request, response);
    
    ASSERT_EQ (HTTP::Code::ACCEPTED, response.mCode);
}

TEST (SimulationEndpointsTester, StartRunningSim) {
    
    auto service = std::make_shared<MockService> ();
    
    EXPECT_CALL (*service, IsLoaded ()).WillOnce (::testing::Return (true));
    EXPECT_CALL (*service, IsRunning ()).WillOnce (::testing::Return (true));
    
    API::SimStartEndPoint endPoint ("/hello", service);
    
    HTTP::Request request;
    HTTP::Response response;
    
    endPoint.Execute (request, response);
    
    ASSERT_EQ (HTTP::Code::NOT_MODIFIED, response.mCode);
}

TEST (SimulationEndpointsTester, StartUnloadedSim) {
    
    auto service = std::make_shared<MockService> ();
    
    EXPECT_CALL (*service, IsLoaded ()).WillOnce (::testing::Return (false));
    
    API::SimStartEndPoint endPoint ("/hello", service);
    
    HTTP::Request request;
    HTTP::Response response;
    
    endPoint.Execute (request, response);
    
    ASSERT_EQ (HTTP::Code::FORBIDDEN, response.mCode);
}


TEST (SimulationEndpointsTester, Stop) {
    
    auto service = std::make_shared<MockService> ();
    
    EXPECT_CALL (*service, IsRunning ()).WillOnce (::testing::Return (true));
    EXPECT_CALL (*service, Stop ()).WillOnce (::testing::Return ());
    
    API::SimStopEndPoint endPoint ("/hello", service);
    
    HTTP::Request request;
    HTTP::Response response;
    
    endPoint.Execute (request, response);
    
    ASSERT_EQ (HTTP::Code::ACCEPTED, response.mCode);
}

TEST (SimulationEndpointsTester, StopNotRunningSim) {
    
    auto service = std::make_shared<MockService> ();
    
    EXPECT_CALL (*service, IsRunning ()).WillOnce (::testing::Return (false));
    
    API::SimStopEndPoint endPoint ("/hello", service);
    
    HTTP::Request request;
    HTTP::Response response;
    
    endPoint.Execute (request, response);
    
    ASSERT_EQ (HTTP::Code::NOT_MODIFIED, response.mCode);
}

TEST (SimulationEndpointsTester, GetValues) {
    
    const std::size_t size (10u);
    const std::vector<SIM::Value> values(size);
    
    auto service = std::make_shared<MockService> ();
    
    EXPECT_CALL (*service, IsLoaded ()).WillOnce (::testing::Return (true));
    EXPECT_CALL (*service, GetValues ()).WillOnce (::testing::Return (values));
    
    API::SimGetValuesEndPoint endPoint ("/hello", service);
    
    HTTP::Request request;
    HTTP::Response response;
    
    endPoint.Execute (request, response);
    
    ASSERT_EQ (HTTP::Code::OK, response.mCode);
    
    ASSERT_THAT (response.GetBody (), ::testing::HasSubstr ("{\"ports\":["));
}

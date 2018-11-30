
#include "MockSimService.h"
#include "SimExecEndPoint.h"

TEST (SimExecEndPointTester, Start) {
    
    auto service = std::make_shared<MOCK::Service> ();
    
    EXPECT_CALL (*service, IsLoaded ()).WillOnce (::testing::Return (true));
    EXPECT_CALL (*service, IsRunning ()).WillOnce (::testing::Return (false));
    EXPECT_CALL (*service, Start ()).WillOnce (::testing::Return ());
    
    API::SIM::ExecEndPoint endPoint ("/hello", service);
    
    HTTP::Request request;
    HTTP::Response response;
    
    endPoint.Put (request, response);
    
    ASSERT_EQ (HTTP::Code::ACCEPTED, response.mCode);
}

TEST (SimExecEndPointTester, StartRunningSim) {
    
    auto service = std::make_shared<MOCK::Service> ();
    
    EXPECT_CALL (*service, IsLoaded ()).WillOnce (::testing::Return (true));
    EXPECT_CALL (*service, IsRunning ()).WillOnce (::testing::Return (true));
    
    API::SIM::ExecEndPoint endPoint ("/hello", service);
    
    HTTP::Request request;
    HTTP::Response response;
    
    endPoint.Put (request, response);
    
    ASSERT_EQ (HTTP::Code::NOT_MODIFIED, response.mCode);
}

TEST (SimExecEndPointTester, StartUnloadedSim) {
    
    auto service = std::make_shared<MOCK::Service> ();
    
    EXPECT_CALL (*service, IsLoaded ()).WillOnce (::testing::Return (false));
    
    API::SIM::ExecEndPoint endPoint ("/hello", service);
    
    HTTP::Request request;
    HTTP::Response response;
    
    endPoint.Put (request, response);
    
    ASSERT_EQ (HTTP::Code::FORBIDDEN, response.mCode);
}

TEST (SimExecEndPointTester, Stop) {
    
    auto service = std::make_shared<MOCK::Service> ();
    
    EXPECT_CALL (*service, IsRunning ()).WillOnce (::testing::Return (true));
    EXPECT_CALL (*service, Stop ()).WillOnce (::testing::Return ());
    
    API::SIM::ExecEndPoint endPoint ("/hello", service);
    
    HTTP::Request request;
    HTTP::Response response;
    
    endPoint.Delete (request, response);
    
    ASSERT_EQ (HTTP::Code::ACCEPTED, response.mCode);
}

TEST (SimExecEndPointTester, StopNotRunningSim) {
    
    auto service = std::make_shared<MOCK::Service> ();
    
    EXPECT_CALL (*service, IsRunning ()).WillOnce (::testing::Return (false));
    
    API::SIM::ExecEndPoint endPoint ("/hello", service);
    
    HTTP::Request request;
    HTTP::Response response;
    
    endPoint.Delete (request, response);
    
    ASSERT_EQ (HTTP::Code::NOT_MODIFIED, response.mCode);
}

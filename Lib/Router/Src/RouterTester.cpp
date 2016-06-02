
#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include <string>

#include "Codes.h"
#include "Request.h"
#include "Response.h"
#include "Router.h"
#include "EndPoint.h"

namespace {

    class TestEndPoint : public Api::EndPoint {
    public:
        TestEndPoint () :EndPoint ("/test", Api::Codes::kGet) {}
        virtual ~TestEndPoint () {}
        MOCK_METHOD2 (Execute, void (const Api::Request&, Api::Response&));
    };

    class OtherTestEndPoint : public Api::EndPoint {
    public:
        OtherTestEndPoint () : EndPoint ("/test/other", Api::Codes::kPost) {}
        virtual ~OtherTestEndPoint () {}
        MOCK_METHOD2 (Execute, void (const Api::Request&, Api::Response&));
    };

}

TEST (RouterTester, BasicRouting) {

    Api::Request request;
    request.SetHeader (Api::Header ("/test", "0.0.1", Api::Codes::kGet));

    Api::Router router;

    TestEndPoint* testEndPoint1 = new TestEndPoint ();
    OtherTestEndPoint* testEndPoint2 = new OtherTestEndPoint ();

    router.AddEndPoint (std::unique_ptr <Api::EndPoint> (testEndPoint1));
    router.AddEndPoint (std::unique_ptr <Api::EndPoint> (testEndPoint2));

    EXPECT_CALL (*testEndPoint1, Execute (::testing::_, ::testing::_)).Times (1);
    EXPECT_CALL (*testEndPoint2, Execute (::testing::_, ::testing::_)).Times (0);

    Api::Response response;
    router.Dispatch (request, response);

    ASSERT_EQ (request.GetHeader ().mPath, response.GetHeader ().mPath);
    ASSERT_EQ (request.GetHeader ().mVersion, response.GetHeader ().mVersion);
    ASSERT_EQ (request.GetHeader ().mMethod, response.GetHeader ().mMethod);
}

TEST (RouterTester, 404PathNotFound) {

    Api::Request request;
    request.SetHeader (Api::Header ("/not-existing-path", "0.0.1", Api::Codes::kGet));

    Api::Router router;
    router.AddEndPoint (std::unique_ptr <Api::EndPoint> (new TestEndPoint ()));

    Api::Response response;
    router.Dispatch (request, response);

    ASSERT_EQ (request.GetHeader ().mPath, response.GetHeader ().mPath);
    ASSERT_EQ (request.GetHeader ().mVersion, response.GetHeader ().mVersion);
    ASSERT_EQ (request.GetHeader ().mMethod, response.GetHeader ().mMethod);

    ASSERT_EQ (Api::Codes::kNotFound, response.GetCode ());
}

TEST (RouterTester, 404MethodNotFound) {

    Api::Request request;
    request.SetHeader (Api::Header ("/test", "0.0.1", Api::Codes::kDelete));

    Api::Router router;
    router.AddEndPoint (std::unique_ptr <Api::EndPoint> (new TestEndPoint ()));

    Api::Response response;
    router.Dispatch (request, response);

    ASSERT_EQ (request.GetHeader ().mPath, response.GetHeader ().mPath);
    ASSERT_EQ (request.GetHeader ().mVersion, response.GetHeader ().mVersion);
    ASSERT_EQ (request.GetHeader ().mMethod, response.GetHeader ().mMethod);

    ASSERT_EQ (Api::Codes::kNotFound, response.GetCode ());
}

#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include <string>

#include "Codes.h"
#include "Request.h"
#include "Response.h"
#include "Router.h"
#include "EndPoint.h"

namespace {

    class TestEndPoint : public API::EndPoint {
    public:
        TestEndPoint () :EndPoint ("/test", API::Codes::kGet) {}
        virtual ~TestEndPoint () {}
        MOCK_METHOD2 (Execute, void (const API::Request&, API::Response&));
    };

    class OtherTestEndPoint : public API::EndPoint {
    public:
        OtherTestEndPoint () : EndPoint ("/test/other", API::Codes::kPost) {}
        virtual ~OtherTestEndPoint () {}
        MOCK_METHOD2 (Execute, void (const API::Request&, API::Response&));
    };

}

TEST (RouterTester, BasicRouting) {

    API::Request request;
    request.SetHeader (API::Header ("/test", "0.0.1", API::Codes::kGet));

    API::Router router;

    TestEndPoint* testEndPoint1 = new TestEndPoint ();
    OtherTestEndPoint* testEndPoint2 = new OtherTestEndPoint ();

    router.AddEndPoint (std::unique_ptr <API::EndPoint> (testEndPoint1));
    router.AddEndPoint (std::unique_ptr <API::EndPoint> (testEndPoint2));

    EXPECT_CALL (*testEndPoint1, Execute (::testing::_, ::testing::_)).Times (1);
    EXPECT_CALL (*testEndPoint2, Execute (::testing::_, ::testing::_)).Times (0);

    API::Response response;
    router.Dispatch (request, response);

    ASSERT_EQ (request.GetHeader ().mPath, response.GetHeader ().mPath);
    ASSERT_EQ (request.GetHeader ().mVersion, response.GetHeader ().mVersion);
    ASSERT_EQ (request.GetHeader ().mMethod, response.GetHeader ().mMethod);
}

TEST (RouterTester, 404PathNotFound) {

    API::Request request;
    request.SetHeader (API::Header ("/not-existing-path", "0.0.1", API::Codes::kGet));

    API::Router router;
    router.AddEndPoint (std::unique_ptr <API::EndPoint> (new TestEndPoint ()));

    API::Response response;
    router.Dispatch (request, response);

    ASSERT_EQ (request.GetHeader ().mPath, response.GetHeader ().mPath);
    ASSERT_EQ (request.GetHeader ().mVersion, response.GetHeader ().mVersion);
    ASSERT_EQ (request.GetHeader ().mMethod, response.GetHeader ().mMethod);

    ASSERT_EQ (API::Codes::kNotFound, response.GetCode ());
}

TEST (RouterTester, 404MethodNotFound) {

    API::Request request;
    request.SetHeader (API::Header ("/test", "0.0.1", API::Codes::kDelete));

    API::Router router;
    router.AddEndPoint (std::unique_ptr <API::EndPoint> (new TestEndPoint ()));

    API::Response response;
    router.Dispatch (request, response);

    ASSERT_EQ (request.GetHeader ().mPath, response.GetHeader ().mPath);
    ASSERT_EQ (request.GetHeader ().mVersion, response.GetHeader ().mVersion);
    ASSERT_EQ (request.GetHeader ().mMethod, response.GetHeader ().mMethod);

    ASSERT_EQ (API::Codes::kNotFound, response.GetCode ());
}


#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include <string>

#include "HttpCodes.h"
#include "HttpRequest.h"
#include "HttpResponse.h"
#include "HttpRouter.h"
#include "HttpEndpoint.h"

namespace {

    class TestEndPoint : public HTTP::EndPoint {
    public:
        TestEndPoint () :EndPoint ("/test", HTTP::Method::GET) {}
        virtual ~TestEndPoint () {}
        MOCK_METHOD2 (Execute, void (const HTTP::Request&, HTTP::Response&));
    };

    class OtherTestEndPoint : public HTTP::EndPoint {
    public:
        OtherTestEndPoint () : EndPoint ("/test/other", HTTP::Method::POST) {}
        virtual ~OtherTestEndPoint () {}
        MOCK_METHOD2 (Execute, void (const HTTP::Request&, HTTP::Response&));
    };

}

TEST (RouterTester, BasicRouting) {

    HTTP::Router router;

    TestEndPoint* testEndPoint1 = new TestEndPoint ();
    OtherTestEndPoint* testEndPoint2 = new OtherTestEndPoint ();

    router.AddEndPoint (std::unique_ptr <HTTP::EndPoint> (testEndPoint1));
    router.AddEndPoint (std::unique_ptr <HTTP::EndPoint> (testEndPoint2));

    EXPECT_CALL (*testEndPoint1, Execute (::testing::_, ::testing::_)).Times (1);
    EXPECT_CALL (*testEndPoint2, Execute (::testing::_, ::testing::_)).Times (0);

    const HTTP::Request request (HTTP::Method::GET, std::string ("/test"));
    HTTP::Response response;
    router.Dispatch (request, response);

    ASSERT_EQ (request.mVersion, response.mVersion);
    // endpoint should set status code
}

TEST (RouterTester, 404PathNotFound) {

    HTTP::Router router;
    router.AddEndPoint (std::unique_ptr <HTTP::EndPoint> (new TestEndPoint ()));

    const HTTP::Request request (HTTP::Method::GET, std::string ("/not-existing-path"));
    HTTP::Response response;
    router.Dispatch (request, response);

    ASSERT_EQ (request.mVersion, response.mVersion);
    ASSERT_EQ (HTTP::Code::NOT_FOUND, response.mCode);
}

TEST (RouterTester, 404MethodNotFound) {

    HTTP::Router router;
    router.AddEndPoint (std::unique_ptr <HTTP::EndPoint> (new TestEndPoint ()));

    const HTTP::Request request (HTTP::Method::DELETE, std::string ("/test"));
    HTTP::Response response;
    router.Dispatch (request, response);

    ASSERT_EQ (request.mVersion, response.mVersion);
    ASSERT_EQ (HTTP::Code::NOT_FOUND, response.mCode);
}

TEST (RouterTester, 404BadRequest) {

    HTTP::Router router;
    router.AddEndPoint (std::unique_ptr <HTTP::EndPoint> (new TestEndPoint ()));

    HTTP::Request request (HTTP::Method::GET, std::string ("/test"));
    request.mIsValid = false;
    
    HTTP::Response response;
    router.Dispatch (request, response);

    ASSERT_EQ (request.mVersion, response.mVersion);
    ASSERT_EQ (HTTP::Code::BAD_REQUEST, response.mCode);
}



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

class RegexTestEndPoint : public HTTP::EndPoint {
public:
    RegexTestEndPoint () : EndPoint (std::regex("\\/test\\/([A-Z]+)\\/other\\/([0-9]+)"), HTTP::Method::DELETE) {}
    virtual ~RegexTestEndPoint () {}
    MOCK_METHOD2 (Execute, void (const HTTP::Request&, HTTP::Response&));
    const ParameterList& GetList () const { return GetParameterList (); }
};

}

TEST (RouterTester, BasicRouting) {
    
    HTTP::Router router;
    HTTP::Response response;
    router.Pipe ([&](const auto& data) { response = data; });
    
    TestEndPoint* testEndPoint1 = new TestEndPoint ();
    OtherTestEndPoint* testEndPoint2 = new OtherTestEndPoint ();
    
    router.AddEndPoint (std::shared_ptr <HTTP::EndPoint> (testEndPoint1));
    router.AddEndPoint (std::shared_ptr <HTTP::EndPoint> (testEndPoint2));
    
    EXPECT_CALL (*testEndPoint1, Execute (::testing::_, ::testing::_)).Times (1);
    EXPECT_CALL (*testEndPoint2, Execute (::testing::_, ::testing::_)).Times (0);
    
    const HTTP::Request request (HTTP::Method::GET, std::string ("/test"));
    router.Write (request);
    
    ASSERT_EQ (request.mVersion, response.mVersion);
    // endpoint should set status code
}

TEST (RouterTester, RegularExpressions) {
    
    HTTP::Router router;
    HTTP::Response response;
    router.Pipe ([&](const auto& data) { response = data; });
    
    RegexTestEndPoint* regexEndPoint2 = new RegexTestEndPoint ();
    
    router.AddEndPoint (std::shared_ptr <HTTP::EndPoint> (regexEndPoint2));
    
    EXPECT_CALL (*regexEndPoint2, Execute (::testing::_, ::testing::_)).Times (1);
    
    const HTTP::Request request (HTTP::Method::DELETE, std::string ("/test/ABCDEF/other/012345678"));
    router.Write (request);
    
    ASSERT_EQ (request.mVersion, response.mVersion);
    ASSERT_EQ (std::string ("/test/ABCDEF/other/012345678"), regexEndPoint2->GetList ()[0]);
    ASSERT_EQ (std::string ("ABCDEF"), regexEndPoint2->GetList ()[1]);
    ASSERT_EQ (std::string ("012345678"), regexEndPoint2->GetList ()[2]);

    // endpoint should set status code
}

TEST (RouterTester, 404PathNotFound) {

    HTTP::Router router;
    HTTP::Response response;
    router.Pipe ([&](const auto& data) { response = data; });
    
    router.AddEndPoint (std::shared_ptr <HTTP::EndPoint> (new TestEndPoint ()));
    
    const HTTP::Request request (HTTP::Method::GET, std::string ("/not-existing-path"));
    router.Write (request);
    
    ASSERT_EQ (request.mVersion, response.mVersion);
    ASSERT_EQ (HTTP::Code::NOT_FOUND, response.mCode);
}

TEST (RouterTester, 404MethodNotFound) {
    
    HTTP::Router router;
    HTTP::Response response;
    router.Pipe ([&](const auto& data) { response = data; });
    
    router.AddEndPoint (std::shared_ptr <HTTP::EndPoint> (new TestEndPoint ()));
    
    const HTTP::Request request (HTTP::Method::DELETE, std::string ("/test"));
    router.Write (request);
    
    ASSERT_EQ (request.mVersion, response.mVersion);
    ASSERT_EQ (HTTP::Code::NOT_FOUND, response.mCode);
}

TEST (RouterTester, 404BadRequest) {
    
    HTTP::Router router;
    HTTP::Response response;
    router.Pipe ([&](const auto& data) { response = data; });
    
    router.AddEndPoint (std::shared_ptr <HTTP::EndPoint> (new TestEndPoint ()));
    
    HTTP::Request request (HTTP::Method::GET, std::string ("/test"));
    request.mIsValid = false;
    router.Write (request);
    
    ASSERT_EQ (request.mVersion, response.mVersion);
    ASSERT_EQ (HTTP::Code::BAD_REQUEST, response.mCode);
}


#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include <string>

#include "HttpCodes.h"
#include "HttpRequest.h"
#include "HttpResponse.h"
#include "HttpRouter.h"
#include "HttpEndpoint.h"

namespace {

class MockEndPoint : public HTTP::EndPoint {
public:
    MockEndPoint (const std::string& inPath) : EndPoint (inPath) {}
    virtual ~MockEndPoint () {}
    MOCK_METHOD2 (Get, void (const HTTP::Request&, HTTP::Response&));
    MOCK_METHOD2 (Put, void (const HTTP::Request&, HTTP::Response&));
    MOCK_METHOD2 (Post, void (const HTTP::Request&, HTTP::Response&));
    MOCK_METHOD2 (Delete, void (const HTTP::Request&, HTTP::Response&));
};

class MockRegexEndPoint : public HTTP::RegexEndPoint {
public:
    MockRegexEndPoint (const std::regex& inRegex) : RegexEndPoint (inRegex) {}
    virtual ~MockRegexEndPoint () {}
    MOCK_METHOD2 (Get, void (const HTTP::Request&, HTTP::Response&));
    MOCK_METHOD2 (Put, void (const HTTP::Request&, HTTP::Response&));
    MOCK_METHOD2 (Post, void (const HTTP::Request&, HTTP::Response&));
    MOCK_METHOD2 (Delete, void (const HTTP::Request&, HTTP::Response&));
    const ParameterList& GetList () const { return GetParameterList (); }
};

}

TEST (RouterTester, BasicRouting) {
    
    HTTP::Router router;
    HTTP::Response response;
    router.Pipe ([&](const auto& data) { response = data; });
    
    MockEndPoint* endPoint1 = new MockEndPoint ("/test");
    MockEndPoint* endPoint2 = new MockEndPoint ("/not_test");
    
    router.AddEndPoint (std::shared_ptr <HTTP::AbstractEndPoint> (endPoint1));
    router.AddEndPoint (std::shared_ptr <HTTP::AbstractEndPoint> (endPoint2));
    
    EXPECT_CALL (*endPoint1, Get (::testing::_, ::testing::_)).Times (1);
    EXPECT_CALL (*endPoint1, Put (::testing::_, ::testing::_)).Times (0);
    EXPECT_CALL (*endPoint1, Post (::testing::_, ::testing::_)).Times (0);
    EXPECT_CALL (*endPoint1, Delete (::testing::_, ::testing::_)).Times (0);
 
    EXPECT_CALL (*endPoint2, Get (::testing::_, ::testing::_)).Times (0);
    EXPECT_CALL (*endPoint2, Put (::testing::_, ::testing::_)).Times (0);
    EXPECT_CALL (*endPoint2, Post (::testing::_, ::testing::_)).Times (0);
    EXPECT_CALL (*endPoint2, Delete (::testing::_, ::testing::_)).Times (0);
 
    const HTTP::Request request (HTTP::Method::GET, std::string ("/test"));
    router.Write (request);
    
    ASSERT_EQ (request.mVersion, response.mVersion);
    // endpoint should set status code
}

TEST (RouterTester, RegularExpressions) {
    
    HTTP::Router router;
    HTTP::Response response;
    router.Pipe ([&](const auto& data) { response = data; });
    
    MockRegexEndPoint* endPoint = new MockRegexEndPoint (std::regex("\\/test\\/([A-Z]+)\\/other\\/([0-9]+)"));
    
    router.AddEndPoint (std::shared_ptr <HTTP::AbstractEndPoint> (endPoint));
    
    EXPECT_CALL (*endPoint, Get (::testing::_, ::testing::_)).Times (0);
    EXPECT_CALL (*endPoint, Put (::testing::_, ::testing::_)).Times (0);
    EXPECT_CALL (*endPoint, Post (::testing::_, ::testing::_)).Times (0);
    EXPECT_CALL (*endPoint, Delete (::testing::_, ::testing::_)).Times (1);
    
    const HTTP::Request request (HTTP::Method::DELETE, std::string ("/test/ABCDEF/other/012345678"));
    router.Write (request);
    
    ASSERT_EQ (request.mVersion, response.mVersion);
    ASSERT_EQ (std::string ("/test/ABCDEF/other/012345678"), endPoint->GetList ()[0]);
    ASSERT_EQ (std::string ("ABCDEF"), endPoint->GetList ()[1]);
    ASSERT_EQ (std::string ("012345678"), endPoint->GetList ()[2]);

    // endpoint should set status code
}

TEST (RouterTester, 404PathNotFound) {

    HTTP::Router router;
    HTTP::Response response;
    router.Pipe ([&](const auto& data) { response = data; });
    
    MockEndPoint* endPoint1 = new MockEndPoint ("/test");
    router.AddEndPoint (std::shared_ptr <HTTP::AbstractEndPoint> (endPoint1));
    EXPECT_CALL (*endPoint1, Get (::testing::_, ::testing::_)).Times (0);
    EXPECT_CALL (*endPoint1, Put (::testing::_, ::testing::_)).Times (0);
    EXPECT_CALL (*endPoint1, Post (::testing::_, ::testing::_)).Times (0);
    EXPECT_CALL (*endPoint1, Delete (::testing::_, ::testing::_)).Times (0);
 
    const HTTP::Request request (HTTP::Method::GET, std::string ("/not-existing-path"));
    router.Write (request);
    
    ASSERT_EQ (request.mVersion, response.mVersion);
    ASSERT_EQ (HTTP::Code::NOT_FOUND, response.mCode);
}

TEST (RouterTester, 404MethodNotFound) {
    
    HTTP::Router router;
    HTTP::Response response;
    router.Pipe ([&](const auto& data) { response = data; });
    
    MockEndPoint* endPoint1 = new MockEndPoint ("/test");
    router.AddEndPoint (std::shared_ptr <HTTP::AbstractEndPoint> (endPoint1));
    EXPECT_CALL (*endPoint1, Get (::testing::_, ::testing::_)).Times (0);
    EXPECT_CALL (*endPoint1, Put (::testing::_, ::testing::_)).Times (0);
    EXPECT_CALL (*endPoint1, Post (::testing::_, ::testing::_)).Times (0);
    EXPECT_CALL (*endPoint1, Delete (::testing::_, ::testing::_)).Times (1); // it is called, but set to 404
 
    const HTTP::Request request (HTTP::Method::DELETE, std::string ("/test"));
    router.Write (request);
    
    ASSERT_EQ (request.mVersion, response.mVersion);
    ASSERT_EQ (HTTP::Code::NOT_FOUND, response.mCode);
}

TEST (RouterTester, 404BadRequest) {
    
    HTTP::Router router;
    HTTP::Response response;
    router.Pipe ([&](const auto& data) { response = data; });
    
    MockEndPoint* endPoint1 = new MockEndPoint ("/test");
    router.AddEndPoint (std::shared_ptr <HTTP::AbstractEndPoint> (endPoint1));
    EXPECT_CALL (*endPoint1, Get (::testing::_, ::testing::_)).Times (0);
    EXPECT_CALL (*endPoint1, Put (::testing::_, ::testing::_)).Times (0);
    EXPECT_CALL (*endPoint1, Post (::testing::_, ::testing::_)).Times (0);
    EXPECT_CALL (*endPoint1, Delete (::testing::_, ::testing::_)).Times (0);
 
    HTTP::Request request (HTTP::Method::GET, std::string ("/test"));
    request.mIsValid = false;
    router.Write (request);
    
    ASSERT_EQ (request.mVersion, response.mVersion);
    ASSERT_EQ (HTTP::Code::BAD_REQUEST, response.mCode);
}

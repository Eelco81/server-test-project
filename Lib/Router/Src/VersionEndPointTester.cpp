
#include "gmock/gmock.h"
#include "VersionEndPoint.h"

TEST (VersionEndPointTester, Get) {
    
    HTTP::Request request;
    HTTP::Response response;
    API::VersionEndPoint endPoint ("/hello");
    
    endPoint.Get (request, response);
    
    EXPECT_EQ ("application/json", response.mHeaders[HTTP::Header::CONTENT_TYPE]);
    EXPECT_EQ ("{\"application\":\"Unknown\",\"version\":\"0.0.1\"}", response.GetBody ());
    EXPECT_EQ ("43", response.mHeaders[HTTP::Header::CONTENT_LENGTH]);
    
}

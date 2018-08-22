
#include "gmock/gmock.h"
#include "VersionEndPoint.h"

TEST (VersionEndPointTester, Get) {
    
    HTTP::Request request;
    HTTP::Response response;
    API::VersionEndPoint endPoint;
    
    endPoint.Execute (request, response);
    
    EXPECT_EQ ("application/json", response.mHeaders[HTTP::Header::CONTENT_TYPE]);
    EXPECT_EQ ("{\"application\":\"HttpServer\",\"version\":\"0.0.1\"}", response.mBody);
    EXPECT_EQ ("46", response.mHeaders[HTTP::Header::CONTENT_LENGTH]);

}



#include "gmock/gmock.h"
#include "TimeEndPoint.h"

TEST (TimeEndPointTester, Get) {
    
    HTTP::Request request;
    HTTP::Response response;
    API::TimeEndPoint endPoint ("/hello");
    
    endPoint.Get (request, response);
    
    EXPECT_EQ ("application/json", response.mHeaders[HTTP::Header::CONTENT_TYPE]);
    EXPECT_THAT (response.GetBody (), ::testing::HasSubstr ("\"unit\":\"ms\""));
    EXPECT_EQ ("35", response.mHeaders[HTTP::Header::CONTENT_LENGTH]);

}

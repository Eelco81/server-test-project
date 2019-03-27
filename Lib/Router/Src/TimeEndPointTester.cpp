

#include "gmock/gmock.h"
#include "TimeEndPoint.h"

TEST (TimeEndPointTester, Get) {
    
    HTTP::Request request;
    HTTP::Response response;
    API::TimeEndPoint endPoint ("/hello");
    
    endPoint.Get (request, response);
    
    EXPECT_EQ ("application/json", response.GetHeaderValue (HTTP::Header::CONTENT_TYPE));
    EXPECT_THAT (response.GetBody (), ::testing::HasSubstr ("\"unit\":\"ms\""));
    EXPECT_EQ ("38", response.GetHeaderValue (HTTP::Header::CONTENT_LENGTH));

}

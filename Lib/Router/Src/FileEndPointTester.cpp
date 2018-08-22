
#include <tuple>

#include "gtest/gtest.h"
#include "FileEndPoint.h"

using TestParam = std::tuple<std::string, std::string>;
class FileEndPointTester : public ::testing::TestWithParam<TestParam> {};

TEST_F (FileEndPointTester, NotFound) {
    
    HTTP::Request request;
    HTTP::Response response;
    
    API::FileEndPoint endPoint ("/some/api/path", "I/do/not/exist");
    endPoint.Execute (request, response);
    
    EXPECT_EQ (response.mCode, HTTP::Code::INTERNAL_SERVER_ERROR);
}

INSTANTIATE_TEST_CASE_P (FileEndPointTester, FileEndPointTester,
    ::testing::Values(
        std::make_tuple (std::string ("file.txt"), std::string ("text/plain")),
        std::make_tuple (std::string ("file.html"), std::string ("text/html")),
        std::make_tuple (std::string ("file.css"), std::string ("text/css")),
        std::make_tuple (std::string ("file.csv"), std::string ("text/csv")),
        std::make_tuple (std::string ("file.json"), std::string ("application/json")),
        std::make_tuple (std::string ("file.js"), std::string ("application/javascript"))
    )
);

TEST_P (FileEndPointTester, ResponseCodeAndHeader) {
    
    HTTP::Request request;
    HTTP::Response response;
    
    std::string filePath (std::string ("../Test/FileEndPointTester/") + std::get<0> (GetParam ()));
    API::FileEndPoint endPoint ("/some/api/path", filePath);
    
    endPoint.Execute (request, response);
    
    EXPECT_EQ (response.mCode, HTTP::Code::OK);
    EXPECT_EQ (std::get<1> (GetParam ()), response.mHeaders[HTTP::Header::CONTENT_TYPE]);
    EXPECT_EQ ("content of file", response.mBody);
    EXPECT_EQ ("15", response.mHeaders[HTTP::Header::CONTENT_LENGTH]);
}
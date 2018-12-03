
#include <tuple>

#include "gtest/gtest.h"
#include "FileSystem.h"
#include "DirEndPoint.h"

using TestParam = std::tuple<std::string, std::string>;
class DirEndPointTester : public ::testing::TestWithParam<TestParam> {};

TEST_F (DirEndPointTester, NotFound) {
    
    HTTP::Request request (HTTP::Method::GET, "/some/api/path/NonExistingFile.txt");;
    HTTP::Response response;
    
    API::DirEndPoint endPoint ("/some/api/path", "I/do/not/exist");
    endPoint.Route (request, response);
    
    EXPECT_EQ (response.mCode, HTTP::Code::INTERNAL_SERVER_ERROR);
}

INSTANTIATE_TEST_CASE_P (DirEndPointTester, DirEndPointTester,
    ::testing::Values(
        std::make_tuple (std::string ("file.txt"), std::string ("text/plain")),
        std::make_tuple (std::string ("file.html"), std::string ("text/html")),
        std::make_tuple (std::string ("file.css"), std::string ("text/css")),
        std::make_tuple (std::string ("file.csv"), std::string ("text/csv")),
        std::make_tuple (std::string ("file.json"), std::string ("application/json")),
        std::make_tuple (std::string ("file.js"), std::string ("application/javascript"))
    )
);

TEST_P (DirEndPointTester, ResponseCodeAndHeader) {
    
    HTTP::Request request (HTTP::Method::GET, "/some/api/path/" + std::get<0> (GetParam ()));
    HTTP::Response response;
    
    std::string path (std::string ("../Test/FileEndPointTester"));
    API::DirEndPoint endPoint ("/some/api/path", path);
    
    endPoint.Route (request, response);
    
    EXPECT_EQ (response.mCode, HTTP::Code::OK);
    EXPECT_EQ (std::get<1> (GetParam ()), response.mHeaders[HTTP::Header::CONTENT_TYPE]);
    EXPECT_EQ ("content of file", response.GetBody ());
    EXPECT_EQ ("15", response.mHeaders[HTTP::Header::CONTENT_LENGTH]);
}


TEST_P (DirEndPointTester, ResponseCodeAndHeaderWithExtension) {
    
    HTTP::Request request (HTTP::Method::GET, "/some/api/path/" + OS::FileSystem::GetBasename (std::get<0> (GetParam ())));
    HTTP::Response response;
    
    std::string path (std::string ("../Test/FileEndPointTester"));
    API::DirEndPoint endPoint ("/some/api/path", path, OS::FileSystem::GetExtension (std::get<0> (GetParam ())));
    
    endPoint.Route (request, response);
    
    EXPECT_EQ (response.mCode, HTTP::Code::OK);
    EXPECT_EQ (std::get<1> (GetParam ()), response.mHeaders[HTTP::Header::CONTENT_TYPE]);
    EXPECT_EQ ("content of file", response.GetBody ());
    EXPECT_EQ ("15", response.mHeaders[HTTP::Header::CONTENT_LENGTH]);
}
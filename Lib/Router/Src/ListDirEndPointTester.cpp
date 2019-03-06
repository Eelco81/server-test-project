
#include <json.hpp>
using json = nlohmann::json;

#include "gmock/gmock.h"
#include "FileSystem.h"
#include "ListDirEndPoint.h"

TEST (ListDirEndPointTester, NotFound) {
    
    HTTP::Request request (HTTP::Method::GET, "/some/api/path");;
    HTTP::Response response;
    
    API::ListDirEndPoint endPoint ("/some/api/path", "I/do/not/exist");
    endPoint.Route (request, response);
    
    EXPECT_EQ (response.mCode, HTTP::Code::INTERNAL_SERVER_ERROR);
}

TEST (ListDirEndPointTester, ListFiles) {
    
    HTTP::Request request (HTTP::Method::GET, "/some/api/path");
    HTTP::Response response;
    
    std::string path (std::string ("../Test/FileEndPointTester"));
    API::ListDirEndPoint endPoint ("/some/api/path", path);
    
    endPoint.Route (request, response);
    
    EXPECT_EQ (response.mCode, HTTP::Code::OK);

    json body;
    try {
        body = json::parse (response.GetBody ());
    }
    catch (...) {
        ASSERT_TRUE (false);
    }

    ASSERT_NE (std::find (std::begin (body), std::end (body), std::string ("file.css")), std::end (body));
    ASSERT_NE (std::find (std::begin (body), std::end (body), std::string ("file.csv")), std::end (body));
    ASSERT_NE (std::find (std::begin (body), std::end (body), std::string ("file.html")), std::end (body));
    ASSERT_NE (std::find (std::begin (body), std::end (body), std::string ("file.txt")), std::end (body));
    ASSERT_NE (std::find (std::begin (body), std::end (body), std::string ("file.css")), std::end (body));

}

TEST (ListDirEndPointTester, ListFilesWithoutExtensions) {
    
    HTTP::Request request (HTTP::Method::GET, "/some/api/path");
    HTTP::Response response;
    
    std::string path (std::string ("../Test/FileEndPointTester"));
    API::ListDirEndPoint endPoint ("/some/api/path", path, false);
    
    endPoint.Route (request, response);
    
    EXPECT_EQ (response.mCode, HTTP::Code::OK);
    EXPECT_EQ (std::string ("[\"file\",\"file\",\"file\",\"file\",\"file\",\"file\"]"), response.GetBody ());
}

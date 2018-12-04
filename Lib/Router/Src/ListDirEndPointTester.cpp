
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
    EXPECT_EQ (std::string ("[\"file.css\",\"file.csv\",\"file.html\",\"file.js\",\"file.json\",\"file.txt\"]"), response.GetBody ());
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
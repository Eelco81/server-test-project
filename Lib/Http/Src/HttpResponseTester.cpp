
#include "gtest/gtest.h"
#include "HttpResponse.h"

using TestParam = std::tuple<std::string, HTTP::Code, HTTP::Version>;
class HttpResponseTester : public ::testing::TestWithParam<TestParam> {};

TEST_F (HttpResponseTester, Constructor) {
    HTTP::Response response;
    ASSERT_EQ (response.mVersion, HTTP::Version::UNKNOWN_VERSION);
    ASSERT_EQ (response.mCode, HTTP::Code::UNKNOWN_CODE);
}

INSTANTIATE_TEST_CASE_P (HttpResponseTester, HttpResponseTester,
    ::testing::Values(
        std::make_tuple (std::string ("HTTP/1.0 200 OK\n\n"), HTTP::Code::OK, HTTP::Version::V10 ),
        std::make_tuple (std::string ("HTTP/1.1 200 OK\n\n"), HTTP::Code::OK, HTTP::Version::V11 ),
        std::make_tuple (std::string ("HTTP/1.0 201 Created\n\n"), HTTP::CREATED, HTTP::Version::V10 ),
        std::make_tuple (std::string ("HTTP/1.1 201 Created\n\n"), HTTP::CREATED, HTTP::Version::V11 ),
        std::make_tuple (std::string ("HTTP/1.0 202 Accepted\n\n"), HTTP::ACCEPTED, HTTP::Version::V10 ),
        std::make_tuple (std::string ("HTTP/1.1 202 Accepted\n\n"), HTTP::Code::ACCEPTED, HTTP::Version::V11 ),
        std::make_tuple (std::string ("HTTP/1.0 400 Bad Request\n\n"), HTTP::Code::BAD_REQUEST, HTTP::Version::V10 ),
        std::make_tuple (std::string ("HTTP/1.1 400 Bad Request\n\n"), HTTP::Code::BAD_REQUEST, HTTP::Version::V11 ),
        std::make_tuple (std::string ("HTTP/1.0 403 Forbidden\n\n"), HTTP::Code::FORBIDDEN, HTTP::Version::V10 ),
        std::make_tuple (std::string ("HTTP/1.1 403 Forbidden\n\n"), HTTP::Code::FORBIDDEN, HTTP::Version::V11 ),
        std::make_tuple (std::string ("HTTP/1.0 404 Not Found\n\n"), HTTP::Code::NOT_FOUND, HTTP::Version::V10 ),
        std::make_tuple (std::string ("HTTP/1.1 404 Not Found\n\n"), HTTP::Code::NOT_FOUND, HTTP::Version::V11 ),
        std::make_tuple (std::string ("HTTP/1.0 500 Internal Server Error\n\n"), HTTP::Code::INTERNAL_SERVER_ERROR, HTTP::Version::V10 ),
        std::make_tuple (std::string ("HTTP/1.1 500 Internal Server Error\n\n"), HTTP::Code::INTERNAL_SERVER_ERROR, HTTP::Version::V11 ),
        std::make_tuple (std::string ("HTTP/x.x 0 Unknown\n\n"), HTTP::Code::UNKNOWN_CODE, HTTP::Version::UNKNOWN_VERSION )
    )
);

TEST_P (HttpResponseTester, BuildInitialLine) {
    HTTP::Response response;
    response.mCode = std::get<1> (GetParam ());
    response.mVersion = std::get<2> (GetParam ());
    ASSERT_EQ (std::get<0> (GetParam ()), response.ToString ());
}

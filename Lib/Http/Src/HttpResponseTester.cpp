
#include "gmock/gmock.h"
#include "HttpResponse.h"

TEST (HttpResponseTester, Constructor) {
    HTTP::Response response;
    ASSERT_EQ (response.mVersion, HTTP::Version::UNKNOWN_VERSION);
    ASSERT_EQ (response.mCode, HTTP::Code::UNKNOWN_CODE);
}

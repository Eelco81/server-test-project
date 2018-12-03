
#ifndef _API_UTILS_H_
#define _API_UTILS_H_

#include <string>
#include "HttpResponse.h"

namespace API { namespace Utils {

/**
 * Set error code and message in a HTTP response.
 */
void SetErrorMessage (HTTP::Response& ioResponse, HTTP::Code inCode, const std::string& inMessage);

/**
 * Read a file and set it to a response.
 */
void SetFileMessage (HTTP::Response& ioResponse, const std::string& inPath);

} } // end namespace API::Utils

#endif // _API_UTILS_H_

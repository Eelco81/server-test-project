
#ifndef _CODES_H_
#define _CODES_H_

#include <string>

namespace API {

struct Codes {

    enum Methods {
        kGet,
        kPost,
        kPut,
        kDelete,
        kUnknown
    };

    enum Responses {
        kOk = 200,
        kCreated,
        kAccepted,
        kNoContent,
        kMultipleChoices = 300,
        kMoved,
        kFound,
        kSeeOther,
        kNotModified,
        kBadRequest = 400,
        kUnauthorized,
        kPaymentRequired,
        kForbidden,
        kNotFound,
        kMethodNotAllowed,
        kNotAcceptible,
        kInternalServerError = 500,
        kNotImplemented,
        kBadGateway,
        kServiceUnavailable
    };

    Codes () = default;
    ~Codes () = default;

    static std::string MethodToString (Methods inMethod) {
        switch (inMethod) {
            case kGet:
                return "GET";
            case kPost:
                return "POST";
            case kPut:
                return "PUT";
            case kDelete:
                return "DELETE";
            default:
                return "UNKNOWN";
        }
    }

    static Methods MethodFromString (const std::string& inString) {
        if (inString == "GET") {
            return kGet;
        }
        if (inString == "POST") {
            return kPost;
        }
        if (inString == "PUT") {
            return kPut;
        }
        if (inString == "DELETE") {
            return kDelete;
        }
        return kUnknown;
    }

};

}

#endif

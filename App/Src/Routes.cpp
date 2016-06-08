
#include "Routes.h"

#include "Codes.h"
#include "Router.h"
#include "EndPoint.h"
#include "Request.h"
#include "Response.h"

class EchoEndPoint : public API::EndPoint {
public:
    EchoEndPoint () : 
        EndPoint ("/echo", API::Codes::kGet) 
    {
    }
    virtual ~EchoEndPoint () = default;

    virtual void Execute (const API::Request& inRequest, API::Response& outResponse) {
        outResponse.SetCode (API::Codes::kNoContent);
    }
};

void Routes::SetupEndpoints (std::shared_ptr <API::Router> inRouter) {

    inRouter->AddEndPoint (std::make_unique <EchoEndPoint> ());

}


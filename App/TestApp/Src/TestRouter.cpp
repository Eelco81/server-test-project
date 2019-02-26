
#include <json.hpp>
using json = nlohmann::json;

#include "TestRouter.h"
#include "EchoEndPoint.h"

namespace {

/**
 * The stream endpoint allows the TEST::Service stream to be started.
 */
class StreamEndPoint : public HTTP::EndPoint {

public:
    /**
     * Constructor
     */
    StreamEndPoint (const std::string& inPath, std::shared_ptr<TEST::Service> inService) :
        HTTP::EndPoint (inPath),
        mService (inService)
    {
    }
    
    /**
     * Virtual Destructor
     */
    virtual ~StreamEndPoint () = default;

    /**
     * Start simulation
     */
    virtual void Put (const HTTP::Request& inRequest, HTTP::Response& outResponse) override {
        
        outResponse.mCode = HTTP::Code::OK;
        
        try {
            json body = json::parse (inRequest.GetBody ());
            auto step = body["step"].get<std::size_t> ();
            auto size = body["size"].get<std::size_t> ();
            auto count = body["count"].get<std::size_t> ();
            auto type = body["type"].get<std::string> ();
            
            if (type == "string") {
                mService->StartStringStream (step, size, count);
            }
            else {
                mService->StartBinaryStream (step, size, count);
            } 
        }
        catch (...) {
            outResponse.mCode = HTTP::Code::BAD_REQUEST; 
        }
    }
    
protected:
    std::shared_ptr<TEST::Service> mService;
};

} // end anonymous namespace

TEST::Router::Router (std::shared_ptr<Service> inService) :
    HTTP::Router (),
    mService (inService)
{
    AddEndPoint (std::make_shared<API::EchoEndPoint> ("/echo"));
    AddEndPoint (std::make_shared<StreamEndPoint> ("/stream", mService));
}

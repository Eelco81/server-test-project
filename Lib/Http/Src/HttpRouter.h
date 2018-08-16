
#ifndef _HTTP_ROUTER_H_
#define _HTTP_ROUTER_H_

#include <memory>
#include <vector>
#include <mutex>

#include "Macros.h"
#include "Mutex.h"

namespace HTTP {

struct Request;
struct Response;
class EndPoint;

class Router {

    NO_COPY_CONSTRUCTORS (Router);
    
public:
    Router ();
    virtual ~Router ();

public:
    void Dispatch (const Request& inRequest, Response& outResponse);
    void AddEndPoint (std::unique_ptr <EndPoint> inEndPoint);

private:
    std::vector <std::unique_ptr <EndPoint>> mEndPoints;
    std::mutex mMutex;
};

}

#endif // _HTTP_ROUTER_H_

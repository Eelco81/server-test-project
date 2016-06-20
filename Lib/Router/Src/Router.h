
#ifndef _ROUTER_H_
#define _ROUTER_H_

#include <memory>
#include <mutex>
#include <vector>

namespace API {

class Request;
class Response;
class EndPoint;

class Router {

public:
    Router ();
    ~Router ();

public:
    void Dispatch (const Request& inRequest, Response& outResponse);
    void AddEndPoint (std::unique_ptr <EndPoint> inEndPoint);

private:
    std::vector <std::unique_ptr <EndPoint>> mEndPoints;
    std::mutex mMutex;
};

}

#endif

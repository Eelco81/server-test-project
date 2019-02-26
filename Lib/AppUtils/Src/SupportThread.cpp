
#include "SupportThread.h"
#include "Task.h"
#include "Log.h"

namespace {

class LogTask : public APP::Task {
public:
    LogTask () :
        Task ("FlushLog") 
    {
    }
    virtual ~LogTask () = default;
    virtual bool Step () override {
        OS::Log::Instance ().Flush ();
        return true;
    }
};

}

APP::SupportThread::SupportThread () :
    PeriodicThread ("Support", 1000000u)
{
    AddTask (std::make_unique <LogTask> ());
}

APP::SupportThread::~SupportThread () = default;


#include "SupportThread.h"
#include "Task.h"
#include "Log.h"

namespace {

class LogTask : public Core::Task {
public:
    LogTask () :
        Task ("FlushLog") 
    {
    }
    ~LogTask () {
    }
    virtual bool Step () override {
        Core::Log::Instance ().Flush ();
        return true;
    }
};

}

App::SupportThread::SupportThread () :
    PeriodicThread ("Support", 1000u)
{
    AddTask (std::make_unique <LogTask> ());
}

App::SupportThread::~SupportThread () {
}

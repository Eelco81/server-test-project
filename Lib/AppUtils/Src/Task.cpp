
#include "Task.h"

App::Task::Task (const std::string& inName) :
    mName (inName)
{
}

App::Task::~Task () {
}

const std::string& App::Task::GetName () const {
    return mName;
}

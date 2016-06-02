
#include "Task.h"

Core::Task::Task (const std::string& inName) :
    mName (inName)
{
}

Core::Task::~Task () {
}

const std::string& Core::Task::GetName () const {
    return mName;
}

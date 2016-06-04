
#include "Task.h"

APP::Task::Task (const std::string& inName) :
    mName (inName)
{
}

APP::Task::~Task () {
}

const std::string& APP::Task::GetName () const {
    return mName;
}

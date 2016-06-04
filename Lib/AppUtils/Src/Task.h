
#ifndef _TASK_H_
#define _TASK_H_

#include <string>

namespace APP {

class Task {

public:
    Task () = delete;
    Task (const std::string& inName);
    Task (const Task& inTask) = delete;
    virtual ~Task ();

public:
    const std::string& GetName () const;
    virtual bool Step () = 0;

private:
    std::string mName;
};

}

#endif

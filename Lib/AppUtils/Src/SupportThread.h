
#ifndef _SUPPORTTHREAD_H_
#define _SUPPORTTHREAD_H_

#include "PeriodicThread.h"

namespace App {

class SupportThread : public Core::PeriodicThread {

public:
    SupportThread ();
    ~SupportThread ();

};

}

#endif


#ifndef _SUPPORTTHREAD_H_
#define _SUPPORTTHREAD_H_

#include "Macros.h"
#include "PeriodicThread.h"

namespace APP {

class SupportThread : public PeriodicThread {

    NO_COPY_CONSTRUCTORS (SupportThread);

public:
    SupportThread ();
    virtual ~SupportThread ();

};

}

#endif

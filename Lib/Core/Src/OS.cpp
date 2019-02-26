
#include "OS.h"
#include "Timing.h"
#include "Network.h"

void OS::Initialize () {
    Network::Initialize ();
    Timing::Initialize ();
}

void OS::Finalize () {
    Network::Done ();
}

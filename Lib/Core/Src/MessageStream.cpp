
#include "MessageStream.h"

void OS::MessageStream::AddCallback (OS::MessageStream::Callback inCallback) {
    mCallbacks.push_back (inCallback);
}

void OS::MessageStream::Done (const OS::MessageStream::Packet& inMessage) {
    for (auto callback : mCallbacks) {
        callback (inMessage);
    }
}


#include "SimEventStream.h"

SIM::EventStream::EventStream () = default;
SIM::EventStream::~EventStream () = default;

void SIM::EventStream::Write (const SIM::Event& inEvent) {
    Done (inEvent.ToString ());
}

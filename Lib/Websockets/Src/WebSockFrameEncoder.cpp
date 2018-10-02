

#include "WebSockFrame.h"
#include "WebSockFrameEncoder.h"

void RFC6455::FrameEncoder::Write (const RFC6455::Frame& inFrame) {
    Done (inFrame.ToBuffer ());
}

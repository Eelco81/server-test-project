
#include <sstream>

#include "Log.h"
#include "SseEventEncoder.h"

namespace {
    const std::string newLine ("\n");
}

void SSE::EventEncoder::Write (const std::string& inPayload) {
    
    std::string buffer;
    buffer += std::string ("id: " + std::to_string (++mLastId)) + newLine;
    
    std::istringstream iss (inPayload);
    for (std::string line; std::getline (iss, line); ) {
        buffer += std::string ("data: ") + line + newLine;
    }
    
    buffer += newLine;
    
    LOGINFO << "SSE: send";
    Done (buffer);
}

void SSE::EventEncoder::SetLastId (uint64_t inLastId) {
    mLastId = inLastId;
}

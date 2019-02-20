
#include <iostream>
#include <sstream>
#include <regex>

#include "Log.h"
#include "SseEventDecoder.h"

SSE::EventDecoder::~EventDecoder () = default;

void SSE::EventDecoder::Write (const std::string& inBuffer) {
    
    std::istringstream iss (inBuffer);
    for (std::string line; std::getline (iss, line); ) {
        
        if (line == "" && !mBuffer.empty ()) {
            Done (mBuffer);
            mBuffer.clear ();
        }
        
        std::regex reDataLine ("^data: (.+)");
        std::smatch matchLine;
        if (std::regex_search (line, matchLine, reDataLine)) {
            mBuffer += matchLine[1].str () + "\n";
        }
        
    }
}

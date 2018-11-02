
#include "SimSampleStream.h"

#include <json.hpp>
using json = nlohmann::json;

SIM::SampleStream::SampleStream () = default;
SIM::SampleStream::~SampleStream () = default;

void SIM::SampleStream::Write (const std::vector<std::string>& inList) {
    
    json j;
    j["values"] = inList;
    
    Done (j.dump ());
    
}

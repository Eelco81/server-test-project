
#include <iostream>
#include <algorithm>

#include <json.hpp>
using json = nlohmann::json;

#include "SimPort.h"
#include "SimSampler.h"
#include "SimException.h"

SIM::Sampler::Sampler (std::size_t inId, SIM::Sampler::Type inType) :
    mId (std::string ("sim-sampler-") + std::to_string (inId)),
    mType (inType)
{
}

SIM::Sampler::~Sampler () = default;

void SIM::Sampler::AddPort (std::weak_ptr<SIM::Port> inPort) {
    mPorts.emplace_back (inPort);
    mOutputs.resize (mPorts.size () + 1u);
}

void SIM::Sampler::Write (const uint64_t& inTimeStamp) {
    
    auto sampleFunction = [](auto& port) {
        const auto portPtr (port.lock ());
        if (!portPtr) {
            throw Exception ("Sampler port went out of scope");
        }
        return portPtr->GetNumericValue ();
    };
    
    if (!mPorts.empty ()) {
        mOutputs[0] =  static_cast<double> (inTimeStamp);
        std::transform (mPorts.begin (), mPorts.end (), mOutputs.begin () + 1u, sampleFunction);
        Emit (Event (mId, mOutputs));
    }
}

SIM::Sampler::Info SIM::Sampler::GetInfo () const {
    
    Info info;
    for (auto& port : mPorts) {
        const auto portPtr (port.lock ());
        if (!portPtr) {
            throw Exception ("Sampler port went out of scope");
        }
        info.mPortInfos.push_back (portPtr->GetInfo ());
    }
    info.mId = mId;
    info.mType = mType;
    
    return info;
}


#include "SimBlock.h"

#include <algorithm>

SIM::Block::Block (const std::string& inName) :
    mName (inName) 
{
}

SIM::Block::~Block () = default;

std::weak_ptr<SIM::Port> SIM::Block::GetInPort (const std::string& inName) {
    auto it = std::find_if (mInputs.begin (), mInputs.end (), [inName](const auto& it) {
        return it->GetName () == inName;
    });
    if (it == mInputs.end ()) {
        return std::shared_ptr<Port> (nullptr);
    }
    return *it;
}
    
std::weak_ptr<SIM::Port> SIM::Block::GetOutPort (const std::string& inName) {
    auto it = std::find_if (mOutputs.begin (), mOutputs.end (), [inName](const auto& it) {
        return it->GetName () == inName;
    });
    if (it == mOutputs.end ()) {
        return std::shared_ptr<Port> (nullptr);
    }
    return *it;
}
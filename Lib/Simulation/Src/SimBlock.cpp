
#include "SimBlock.h"
#include "SimException.h"
#include <algorithm>

SIM::Block::Block (const std::string& inName) :
    mName (inName) 
{
}

SIM::Block::~Block () = default;

const std::string& SIM::Block::GetName () const {
    return mName;
}

std::weak_ptr<SIM::Port> SIM::Block::GetPort (const Path& inPath) {
    
    if (inPath.mBlockID != GetName ()) {
        throw Exception (std::string ("Getting illegal path <") + inPath.ToString () + std::string (". from block <") + GetName () + ">"); 
    }
    
    auto nameIterator = [inPath] (const auto& it) { return it->GetName () == inPath.mPortID; };
    
    if (inPath.mType == Path::INPUT) {
        auto it = std::find_if (mInputs.begin (), mInputs.end (), nameIterator);
        if (it != mInputs.end ()) {
            return *it;
        }
    }
    else if (inPath.mType == Path::OUTPUT) {
        auto it = std::find_if (mOutputs.begin (), mOutputs.end (), nameIterator);
        if (it != mOutputs.end ()) {
            return *it;
        }
    }
    else if (inPath.mType == Path::PARAMETER) {
        auto it = std::find_if (mParameters.begin (), mParameters.end (), nameIterator);
        if (it != mParameters.end ()) {
            return *it;
        }
    }
    
    return std::weak_ptr<Port> ();
}

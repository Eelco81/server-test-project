
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
    
    auto nameIterator = [inPath] (const auto& it) { return it->GetPath () == inPath; };
    
    auto it = std::find_if (mPorts.begin (), mPorts.end (), nameIterator);
    if (it != mPorts.end ()) {
        return *it;
    }
    
    return std::weak_ptr<Port> ();
}

std::vector<SIM::Path> SIM::Block::GetAllPorts () const {
    std::vector<SIM::Path> result;
    for (const auto& port : mPorts) {
        result.push_back (port->GetPath ());
    }
    return result;
}

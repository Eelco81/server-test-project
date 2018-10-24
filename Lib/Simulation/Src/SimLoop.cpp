
#include "SimPort.h"
#include "SimLoop.h"
#include "SimBlock.h"
#include "SimConnector.h"
#include "SimException.h"
#include "SimPath.h"

#include <algorithm>
#include <regex>

SIM::Loop::Loop (uint64_t inTimeStep) : 
    mTimer (inTimeStep)
{
}

SIM::Loop::~Loop () {
    Terminate ();
}

std::weak_ptr<SIM::Port> SIM::Loop::FindPort (const SIM::Path& inPath) const {
    
    auto nameFinder = [&](const auto& block) { return (block->GetName () == inPath.mBlockID); };
    auto blockIt = std::find_if (mBlocks.cbegin (), mBlocks.cend (), nameFinder);
    
    if (blockIt == mBlocks.cend ()) {
        throw Exception (std::string ("Non-existing block <") + inPath.mBlockID + std::string (">"));
    }
    
    return (*blockIt)->GetPort (inPath);
}

void SIM::Loop::AddBlock (std::unique_ptr<Block> inBlock) {
    
    std::regex reInitialLine ("([A-Za-z0-9]+)");
    if (!std::regex_match (inBlock->GetName (), reInitialLine)) {
        throw Exception (std::string ("Cannot add block <") + inBlock->GetName () + std::string (">, the name does not meet requirements"));
    }
    
    if (!std::all_of (mBlocks.cbegin (), mBlocks.cend (), [&](const auto& block) { return (block->GetName () != inBlock->GetName ()); })) {
        throw Exception (std::string ("Cannot add block <") + inBlock->GetName () + std::string (">, the name is not unique"));
    }
    
    mBlocks.emplace_back (std::move (inBlock));
}

void SIM::Loop::Connect (const std::string& inSource, const std::string& inTarget) {
    
    try {
        Path sourcePath (inSource);
        Path targetPath (inTarget);
        
        // todo: only allow connections from outputs to inputs
        // todo: do not accept multiple connectors to the same input
        
        auto sourcePort (FindPort (sourcePath));
        auto targetPort (FindPort (targetPath));
        auto connector = std::make_unique<Connector> (sourcePort, targetPort);
        mConnectors.emplace_back (std::move (connector));
    }
    catch (Exception& e) {
        throw Exception (std:: string ("Cannot connect <") + inSource + std::string ("> to <") + inTarget + std::string (">: ") + e.what ());
    }
}

void SIM::Loop::AddSample (const std::string& inSource) {
    
    try {
        Path sourcePath (inSource);
        mSampler.AddPort (FindPort (sourcePath));
    }
    catch (Exception& e) {
        throw Exception (std:: string ("Cannot sample <") + inSource + std::string (">: ") + e.what ());
    }
}

void SIM::Loop::Initialize () {
    
    mTimer.Initialize ();

    // \todo: this causes massive delays, blocks and connectors need to be 
    // executed chronologically.
    
    for (auto& block : mBlocks) {
        block->Initialize (mTimer.GetTime ());
    }
    for (auto& connector : mConnectors) {
        connector->Transfer ();
    }
    mSampler.Write (mTimer.GetTimeStamp ());
}

void SIM::Loop::Update () {
    
    mTimer.Tick ();
    
    // \todo: this causes massive delays, blocks and connectors need to be 
    // executed chronologically.
    
    for (auto& block : mBlocks) {
        block->Step (mTimer.GetTime ());
    }
    for (auto& connector : mConnectors) {
        connector->Transfer ();
    }
    mSampler.Write (mTimer.GetTimeStamp ());
}

void SIM::Loop::Terminate () {
    
    for (auto& block : mBlocks) {
        block->Terminate (mTimer.GetTime ());
    }
    mSampler.Write (mTimer.GetTimeStamp ());
}

std::string SIM::Loop::GetValue (const std::string& inPath) const {
    
    Path path (inPath);
    auto port (FindPort (path).lock ());
    if (port) {
        return port->GetStringValue ();
    }
    throw Exception (std::string ("Failed to find path <") + inPath + std::string (">"));
}

std::vector<SIM::Path> SIM::Loop::GetPaths () const {
    std::vector<SIM::Path> result;
    for (const auto& block : mBlocks) {
        const auto paths (block->GetAllPorts ());
        std::copy (paths.begin (), paths.end (), std::back_inserter (result));
    }
    return result;
}

uint64_t SIM::Loop::GetTimeStamp () const {
    return mTimer.GetTimeStamp ();
}

uint64_t SIM::Loop::GetTimeStep () const {
    return mTimer.GetTimeStep ();
}


#include "SimPort.h"
#include "SimLoop.h"
#include "SimBlock.h"
#include "SimConnector.h"
#include "SimException.h"

#include <algorithm>
#include <regex>

SIM::Loop::Loop () = default;
SIM::Loop::~Loop () = default;

std::weak_ptr<SIM::Port> SIM::Loop::FindPort (const std::string& inName) {
    
    std::regex reInitialLine ("^([A-Za-z0-9]+).([A-Za-z0-9]+).([A-Za-z0-9]+)$");
    
    std::smatch matchLine;
    if (!std::regex_search (inName, matchLine, reInitialLine)) {
        throw Exception (std::string ("Illegal path <") + inName + std::string (">"));
    }
    
    const auto blockName (matchLine[1].str ());
    const auto typeName (matchLine[2].str ());
    const auto portName (matchLine[3].str ());
    
    auto blockIt = std::find_if (mBlocks.cbegin (), mBlocks.cend (), [blockName](const auto& block) { return (block->GetName () == blockName); }); 
    
    if (blockIt == mBlocks.cend ()) {
        throw Exception (std::string ("Non-existing block <") + blockName + std::string (">"));
    }
    if (typeName == "in") {
        return (*blockIt)->GetInPort (portName);
    }
    else if (typeName == "out") {
        return (*blockIt)->GetOutPort (portName);
    }
    throw Exception (std::string ("Illegal identifier <") + typeName + std::string (">, only in/out allowed"));
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
        auto sourcePort (FindPort (inSource));
        auto targetPort (FindPort (inTarget));
        auto connector = std::make_unique<Connector> (sourcePort, targetPort);
        mConnectors.emplace_back (std::move (connector));
    }
    catch (Exception& e) {
        throw Exception (std:: string ("Cannot connect <") + inSource + std::string ("> to <") + inTarget + std::string (">: ") + e.what ());
    }
}

void SIM::Loop::Initialize () {
    
    for (auto& block : mBlocks) {
        block->Initialize ();
    }
    for (auto& connector : mConnectors) {
        connector->Transfer ();
    }
}

void SIM::Loop::Update () {
    
    // todo: this causes massive delays, blocks and connectors need to be 
    // executed chronoloigically.
    
    for (auto& block : mBlocks) {
        block->Step ();
    }
    for (auto& connector : mConnectors) {
        connector->Transfer ();
    }
}
    
void SIM::Loop::Terminate () {
    
    for (auto& block : mBlocks) {
        block->Terminate ();
    }
}

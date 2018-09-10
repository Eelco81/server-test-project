
#include "SimBlock.h"
#include "SimLoop.h"
#include "SimFactory.h"
#include "SimException.h"

namespace {
    const std::string blocksTag ("blocks");
    const std::string connectorsTag ("connectors");
    const std::string settingsTag ("settings");
    const std::string nameTag ("name");
    const std::string typeTag ("type");
}

std::unique_ptr<SIM::Loop> SIM::Factory::Create (const json& inConfig) {
    
    auto loop = std::make_unique<Loop> ();
    
    if (inConfig.find (blocksTag) == inConfig.end () || inConfig[blocksTag].empty ()) {
        throw Exception ("No blocks defined in config");
    }
    const auto& blockConfig (inConfig[blocksTag]);
    
    for (auto& config : blockConfig) {
        
        if (config.find (nameTag) == config.end () || !config[nameTag].is_string ()) {
            throw Exception ("Block without \"name\" element in config");
        }
        const std::string name (config[nameTag].get<std::string> ());
        
        if (config.find (typeTag) == config.end () || !config[typeTag].is_string ()) {
            throw Exception ("Block without \"type\" element in config");
        }
        const std::string type (config[typeTag].get<std::string> ());
        
        auto block = std::make_unique<Block> (name);
        
        block->Configure (config["settings"]);
        loop->AddBlock (std::move (block));
    }
    
    return loop;
}


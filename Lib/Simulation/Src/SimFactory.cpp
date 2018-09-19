
#include "SimBlock.h"
#include "SimLoop.h"
#include "SimFactory.h"
#include "SimException.h"

namespace {
    const std::string stepTag ("step");
    const std::string blocksTag ("blocks");
    const std::string connectorsTag ("connectors");
    const std::string settingsTag ("settings");
    const std::string nameTag ("name");
    const std::string typeTag ("type");
    const std::string sourceTag ("source");
    const std::string targetTag ("target");
}

std::unique_ptr<SIM::Loop> SIM::Factory::Create (const json& inConfig) {
    
    if (inConfig.find (stepTag) == inConfig.end () || !inConfig[stepTag].is_number ()) {
        throw Exception ("No valid step defined in config");
    }
    
    auto loop = std::make_unique<Loop> (inConfig[stepTag].get<uint64_t> ());
    
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
        
        json settings ({});
        if (config.find (settingsTag) != config.end ()) {
            settings = config [settingsTag];
        }
        
        auto block = CreateBlock (name, type);
        block->Configure (settings);
        
        loop->AddBlock (std::move (block));
    }
    
    if (inConfig.find (connectorsTag) != inConfig.end () && !inConfig[connectorsTag].empty ()) {
        
        for (auto& config : inConfig[connectorsTag]) {
            
            if (config.find (sourceTag) == config.end () || !config[sourceTag].is_string ()) {
                throw Exception ("Connector without \"source\" element in config");
            }
            const std::string source (config[sourceTag].get<std::string> ());
            
            if (config.find (targetTag) == config.end () || !config[targetTag].is_string ()) {
                throw Exception ("Connector without \"target\" element in config");
            }
            const std::string target (config[targetTag].get<std::string> ());
            
            loop->Connect (source, target);
        }
    }
    
    return loop;
}

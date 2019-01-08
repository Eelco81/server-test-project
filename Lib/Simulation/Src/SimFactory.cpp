
#include "SimBlock.h"
#include "SimLoop.h"
#include "SimFactory.h"
#include "SimException.h"

namespace {
    const std::string stepTag ("step");
    const std::string blocksTag ("blocks");
    const std::string connectorsTag ("connectors");
    const std::string initializersTag ("initializers");
    const std::string sampleTag ("samplers"); 
    const std::string settingsTag ("settings");
    const std::string nameTag ("name");
    const std::string typeTag ("type");
    const std::string portTag ("port");
    const std::string portsTag ("ports");
    const std::string valueTag ("value");
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
            const auto source (config[sourceTag].get<std::string> ());
            
            if (config.find (targetTag) == config.end () || !config[targetTag].is_string ()) {
                throw Exception ("Connector without \"target\" element in config");
            }
            const auto target (config[targetTag].get<std::string> ());
            
            loop->Connect (source, target);
        }
    }
    
    if (inConfig.find (initializersTag) != inConfig.end () && !inConfig[initializersTag].empty ()) {
        
        for (auto& config : inConfig[initializersTag]) {
            
            if (config.find (portTag) == config.end () || !config[portTag].is_string ()) {
                throw Exception ("Initializer without \"port\" element in config");
            }
            const auto port (config[portTag].get<std::string> ());
            
            if (config.find (valueTag) == config.end () || !config[valueTag].is_number ()) {
                throw Exception ("Initializer without \"value\" element in config");
            }
            const auto value (config[valueTag].get<double> ());
            
            loop->SetInitializer (port, value);
        }
    }
    
    if (inConfig.find (sampleTag) != inConfig.end () && !inConfig[sampleTag].empty ()) {
        
        const auto samplerConfigs = inConfig[sampleTag];
        
        if (!samplerConfigs.is_array ()) {
            throw Exception ("Sampler list is not an array");
        }
        
        for (const auto& samplerConfig : samplerConfigs) {
            
            Sampler::Type type (SIM::Sampler::HIST);
            if (samplerConfig.find (typeTag) != samplerConfig.end () && samplerConfig[typeTag].is_string ()) {
                type = (samplerConfig[typeTag].get<std::string> () == "hist") ? Sampler::HIST : Sampler::ROW;
            }
        
            if (samplerConfig.find (portsTag) == samplerConfig.end () || !samplerConfig[portsTag].is_array ()) {
                throw Exception ("Sampler ports config is not an array");
            }
            
            std::vector <std::string> paths;
            for (const auto& path : samplerConfig[portsTag]) {
                if (!path.is_string ()) {
                    throw Exception ("Sampler element is not a string");
                }
                paths.push_back (path.get<std::string> ());
            }
            
            loop->AddSampler (paths, type);
        }
    }
    
    return loop;
}

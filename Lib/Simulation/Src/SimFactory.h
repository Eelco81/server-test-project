
#ifndef _SIM_FACTORY_H_
#define _SIM_FACTORY_H_

#include <memory>
#include <string>

#include <json.hpp>
using json = nlohmann::json;

namespace SIM {

class Block;
class Loop;

class Factory {

public:
    std::unique_ptr<Loop> Create (const json& inConfig);

protected:
   virtual std::unique_ptr<Block> CreateBlock (const std::string& inName, const std::string& inType) = 0;

};

}

#endif // _SIM_FACTORY_H_


#ifndef _SIM_LOOP_H_
#define _SIM_LOOP_H_

#include <memory>
#include <vector>
#include <string>

#include "SimPath.h"

namespace SIM {

class Port;
class Block;
class Connector;

class Loop {

public:
    Loop ();
    virtual ~Loop ();

    void AddBlock (std::unique_ptr<Block> inBlock);
    void Connect (const std::string& inSource, const std::string& inTarget);
    
    void Initialize ();
    void Update ();
    void Terminate ();
    
    std::string GetValue (const std::string& inPath) const;

private:
    
    std::weak_ptr<Port> FindPort (const Path& inPath) const;
    
    std::vector<std::unique_ptr<Block>> mBlocks;
    std::vector<std::unique_ptr<Connector>> mConnectors;

};

}

#endif // _SIM_LOOP_H_

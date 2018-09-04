
#ifndef _SIM_BLOCK_H_
#define _SIM_BLOCK_H_

#include "SimTypedPort.h"
#include <vector>
#include <string>
#include <memory>

namespace SIM {

class Block {

public:
    Block (const std::string& inName);
    virtual ~Block ();
    
    virtual void Configure () {}
    virtual void Initialize () {}
    virtual void Step () {}
    virtual void Terminate () {}

public:
    std::weak_ptr<Port> GetInPort (const std::string& inName);
    std::weak_ptr<Port> GetOutPort (const std::string& inName);
    
protected:
    
    template<typename T>
    void AddInPort (T* inValuePtr, const std::string& inName) {
        mInputs.emplace_back (std::make_shared<TypedPort<T>> (inName, inValuePtr));
    }
    
    template<typename T>
    void AddOutPort (T* inValuePtr, const std::string& inName) {
        mOutputs.emplace_back (std::make_shared<TypedPort<T>> (inName, inValuePtr));
    }
    
protected:
    
    std::vector<std::shared_ptr<Port>> mInputs;
    std::vector<std::shared_ptr<Port>> mOutputs;
    std::string mName;

};

}

#endif // _SIM_BLOCK_H_

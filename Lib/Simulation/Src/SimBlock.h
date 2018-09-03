
#ifndef _SIM_BLOCK_H_
#define _SIM_BLOCK_H_

#include "SimTypedPort.h"
#include <vector>
#include <string>
#include <memory>
#include <algorithm>

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
    std::weak_ptr<Port> GetInPort (const std::string& inName) {
        auto it = std::find_if (mInputs.begin (), mInputs.end (), [inName](const auto& it) {
            return it->GetName () == inName;
        });
        return *it;
    }
    
    std::weak_ptr<Port> GetOutPort (const std::string& inName) {
        auto it = std::find_if (mOutputs.begin (), mOutputs.end (), [inName](const auto& it) {
            return it->GetName () == inName;
        });
        return *it;
    }
    
    /*
    template<typename T>
    std::weak_ptr<TypedPort<T>> GetInPort (const std::string& inName) {
        auto it = std::find_if (mInputs.begin (), mInputs.end (), [inName](const auto& it) {
            return it->GetName () == inName;
        });
        return std::dynamic_pointer_cast<TypedPort<T>> (*it);
    }
    
    template<typename T>
    std::weak_ptr<TypedPort<T>> GetOutPort (const std::string& inName) {
        auto it = std::find_if (mOutputs.begin (), mOutputs.end (), [inName](const auto& it) {
            return it->GetName () == inName;
        });
        return std::dynamic_pointer_cast<TypedPort<T>> (*it);
    }
    */
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

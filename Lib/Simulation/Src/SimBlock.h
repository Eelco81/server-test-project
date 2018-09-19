
#ifndef _SIM_BLOCK_H_
#define _SIM_BLOCK_H_

#include <vector>
#include <string>
#include <memory>

#include <json.hpp>
using json = nlohmann::json;

#include "SimTypedPort.h"
#include "SimPath.h"

namespace SIM {

class Block {

public:

    Block (const std::string& inName);
    virtual ~Block ();
    
    virtual void Configure (const json& inConfig) {}
    virtual void Initialize (double inTime) {}
    virtual void Step (double inTime) {}
    virtual void Terminate (double inTime) {}

public:

    const std::string& GetName () const;
    std::weak_ptr<Port> GetPort (const Path& inPath);

protected:
    
    template<typename T>
    void AddInPort (T* inValuePtr, const std::string& inName) {
        mInputs.emplace_back (std::make_shared<TypedPort<T>> (inName, inValuePtr));
    }
    
    template<typename T>
    void AddInPort (const std::string& inName) {
        mInputs.emplace_back (std::make_shared<TypedPort<T>> (inName));
    }
    
    template<typename T>
    void AddOutPort (T* inValuePtr, const std::string& inName) {
        mOutputs.emplace_back (std::make_shared<TypedPort<T>> (inName, inValuePtr));
    }
    
    template<typename T>
    void AddOutPort (const std::string& inName) {
        mOutputs.emplace_back (std::make_shared<TypedPort<T>> (inName));
    }
    
    template<typename T>
    void AddParPort (T* inValuePtr, const std::string& inName) {
        mParameters.emplace_back (std::make_shared<TypedPort<T>> (inName, inValuePtr));
    }
    
    template<typename T>
    void AddParPort (const std::string& inName) {
        mParameters.emplace_back (std::make_shared<TypedPort<T>> (inName));
    }
    
protected:
    std::vector<std::shared_ptr<Port>> mInputs;
    std::vector<std::shared_ptr<Port>> mOutputs;
    std::vector<std::shared_ptr<Port>> mParameters;
    
    //todo: rather use ID instead of name
    std::string mName;

};

}

#endif // _SIM_BLOCK_H_

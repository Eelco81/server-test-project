
#ifndef _SIM_BLOCK_H_
#define _SIM_BLOCK_H_

#include <vector>
#include <string>
#include <memory>

#include <json.hpp>
using json = nlohmann::json;

#include "MathMatrix.h"
#include "MathVector.h"

#include "SimTypedPort.h"
#include "SimPath.h"

namespace SIM {

class Block {

public:
    /**
     * Constructor
     */
    Block (const std::string& inName);
    
    /**
     * Virtual destructor
     */
    virtual ~Block ();
    
    /**
     * Empty configure function, to be overloaded
     */
    virtual void Configure (const json& inConfig) {}
    
    /**
     * Empty initialize function, to be overloaded
     */
    virtual void Initialize (double inTime, double inTimeStep) {}
    
    /**
     * Empty step function, to be overloaded
     */
    virtual void Step (double inTime, double inTimeStep) {}
    
    /**
     * Empty terminate function, to be overloaded
     */
    virtual void Terminate (double inTime, double inTimeStep) {}

    /**
     * Get block name
     */
    const std::string& GetName () const;
    
    /**
     * Derive weak pointer to a specific path
     */
    std::weak_ptr<Port> GetPort (const Path& inPath);
    
    /**
     * Get all paths to existing ports
     */
    std::vector<Path> GetAllPorts () const;
    
protected:
    
    template<std::size_t Nrows, std::size_t Ncols, typename T>
    void AddPort (MATH::Matrix<Nrows,Ncols,T>& inMatrix, Path::Type inType, const std::string& inName) {
        for (std::size_t i (0u); i < Nrows; i++) {
            for (std::size_t j (0u); j < Ncols; j++) {
                const std::string name (inName + "[" + std::to_string(i) + "][" + std::to_string (j) + "]");
                const Path path (mName, name, inType);
                mPorts.emplace_back (std::make_shared<TypedPort<T>> (path, inMatrix.GetValuePtr (i,j)));
            }
        }
    }
    
    template<typename T>
    void AddPort (T* inValuePtr, Path::Type inType, const std::string& inName) {
        const Path path (mName, inName, inType);
        mPorts.emplace_back (std::make_shared<TypedPort<T>> (path, inValuePtr));
    }
    
    template<typename T>
    void AddPort (Path::Type inType, const std::string& inName) {
        const Path path (mName, inName, inType);
        mPorts.emplace_back (std::make_shared<TypedPort<T>> (path));
    }
    
protected:
    std::vector<std::shared_ptr<Port>> mPorts;
    
    //todo: rather use ID instead of name
    std::string mName;
};

}

#endif // _SIM_BLOCK_H_

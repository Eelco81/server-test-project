
#include "SimConnector.h"
#include "SimException.h"

// -------------------------------------------------------

class SIM::Connector::Implementation {
public:
    virtual ~Implementation () = default;
    virtual void Transfer () = 0;
};

// -------------------------------------------------------

namespace {

template <typename T>
class TypedImplementation : public SIM::Connector::Implementation {
    
public:
    static std::unique_ptr<TypedImplementation> Create (std::shared_ptr<SIM::Port> inSource, std::shared_ptr<SIM::Port> inTarget) {
        return std::make_unique<TypedImplementation<T>> (
            std::dynamic_pointer_cast<SIM::TypedPort<T>> (inSource), 
            std::dynamic_pointer_cast<SIM::TypedPort<T>> (inTarget)
        );
    }

    virtual ~TypedImplementation () = default;
    
    virtual void Transfer () override {
        auto source (mSource.lock ());
        auto target (mTarget.lock ());
        if (source && target) {
            *target = *source;
        }
    }
    
public:
    TypedImplementation (std::weak_ptr<SIM::TypedPort<T>> inSource, std::weak_ptr<SIM::TypedPort<T>> inTarget)  :
        mSource (inSource),
        mTarget (inTarget)
    {
    }
    
protected:
    std::weak_ptr<SIM::TypedPort<T>> mSource;
    std::weak_ptr<SIM::TypedPort<T>> mTarget;

};

} // end anonymous namespace

// -------------------------------------------------------

SIM::Connector::Connector (std::weak_ptr<SIM::Port> inSource, std::weak_ptr<SIM::Port> inTarget) :
    mImpl (nullptr)
{
    
    auto source (inSource.lock ());
    if (!source) {
        throw Exception ("Cannot connect ports, non existing source port");
    }
    
    auto target (inTarget.lock ());
    if (!target) {
        throw Exception ("Cannot connect ports, non existing target port");
    }
    
    const auto sourceType (source->GetType ());
    const auto targetType (target->GetType ());
        
    if (sourceType == targetType) {
        
        switch (sourceType) {
            case Port::BOOL   : mImpl = std::move (TypedImplementation<bool>::Create (source, target)); break;
            case Port::UINT8  : mImpl = std::move (TypedImplementation<uint8_t>::Create (source, target)); break;
            case Port::UINT16 : mImpl = std::move (TypedImplementation<uint16_t>::Create (source, target)); break;
            case Port::UINT32 : mImpl = std::move (TypedImplementation<uint32_t>::Create (source, target)); break;
            case Port::UINT64 : mImpl = std::move (TypedImplementation<uint64_t>::Create (source, target)); break;
            case Port::INT8   : mImpl = std::move (TypedImplementation<int8_t>::Create (source, target)); break;
            case Port::INT16  : mImpl = std::move (TypedImplementation<int16_t>::Create (source, target)); break;
            case Port::INT32  : mImpl = std::move (TypedImplementation<int32_t>::Create (source, target)); break;
            case Port::INT64  : mImpl = std::move (TypedImplementation<int64_t>::Create (source, target)); break;
            case Port::FLOAT  : mImpl = std::move (TypedImplementation<float>::Create (source, target)); break;
            case Port::DOUBLE : mImpl = std::move (TypedImplementation<double>::Create (source, target)); break;
            default : throw Exception (std::string ("Cannot connect ") + source->GetName () + std::string(" to ") + target->GetName () + std::string (", unknown port type"));
        }
    }
    else {
        throw Exception (std::string ("Cannot connect ") + source->GetName () + std::string(" to ") + target->GetName () + std::string (", the types do not match"));
    }

}

SIM::Connector::~Connector () = default;

void SIM::Connector::Transfer () {
    if (mImpl) {
        mImpl->Transfer ();
    }
}


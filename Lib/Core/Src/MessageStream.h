
#ifndef _OS_MESSAGESTRAEM_H_
#define _OS_MESSAGESTRAEM_H_

#include <cstdint>
#include <vector>
#include <functional>

namespace OS {

template <typename Input_t, typename Output_t>
class MessageStream {

public:
    /**
     * Utility definition
     */
    using Callback = std::function<void(const Output_t&)>;
    
    /**
     * Virtual destructor
     */
    virtual ~MessageStream () = default;
    
    /**
     * Pipe the stream to a callback.
     */
    void Pipe (Callback inCallback);
    
    /**
     * Pipe the stream to a member function of an object
     */
    template <typename T>
    void Pipe (T* inObject, void(T::*inMethod)(const Output_t&)) {
        Pipe ([inObject, inMethod] (const Output_t& data) { (inObject->*inMethod) (data); });
    }
    
    /**
     * Pipe the stream to another stream.
     */
    template<typename T>
    MessageStream<Output_t,T>& Pipe (MessageStream<Output_t,T>& inStream) {
        Pipe ([&] (const Output_t& data) { inStream.Write (data); });
        return inStream;
    }
    
    /**
     * Clear all the pipes
     */
    MessageStream& Clear ();
    
    /**
     * Write to the stream
     */
    virtual void Write (const Input_t& inMessage) = 0;
    
protected:
    /**
     * In function Write (), call Done () when a packet is done for 
     * further processing.
     */
    void Done (const Output_t& inMessage);
    
private:
    std::vector<Callback> mCallbacks;

};

} // end namespace OS

#include "MessageStream.tcc"

#endif // _OS_MESSAGESTRAEM_H_

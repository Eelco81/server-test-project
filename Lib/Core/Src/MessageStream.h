
#ifndef _OS_MESSAGESTRAEM_H_
#define _OS_MESSAGESTRAEM_H_

#include <cstdint>
#include <vector>
#include <functional>
#include "Signals.h"

namespace OS {

/**
 * The MessageStream allows event based data streaming.
 */
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
    void Pipe (T* inObject, void(T::*inMethod)(const Output_t&));
    
    /**
     * Pipe the stream to another stream.
     */
    template<typename T>
    MessageStream<Output_t,T>& Pipe (MessageStream<Output_t,T>& inStream);
    
    /**
     * Pipe the stream to a signal
     */
    void Pipe (OS::Signal<const Output_t&>& inSignal);

    /**
     * Clear all the pipes
     */
    MessageStream& Clear ();
    
    /**
     * Write to the stream
     */
    virtual void Write (const Input_t& inMessage) = 0;
    
    /**
     * In function Write (), emit sigNext when a packet is done for 
     * further processing.
     */
    Signal<const Output_t&> sNext;
};

/**
 * Pipe a signal to a MessageStream. All args are passed as a tuple.
 */
template<typename T, typename ...Args>
OS::MessageStream<std::tuple<Args...>, T>& Pipe (OS::Signal<Args...>& inSignal, OS::MessageStream<std::tuple<Args...>, T>& inStream) {
    inSignal.Connect([&](Args...args) { inStream.Write (std::tuple<Args...>(args...)); });
    return inStream;
}

/**
 * Pipe a signal to a MessageStream. The signal is assumed to adhere to the message stream
 * signature.
 */
template <typename Input_t, typename Output_t>
OS::MessageStream<Input_t, Output_t>& Pipe(OS::Signal<const Input_t&>& inSignal, OS::MessageStream<Input_t, Output_t>& inStream) {
    inSignal.Connect ([&](const Input_t& inData) { inStream.Write (inData); });
    return inStream;
}

} // end namespace OS

#include "MessageStream.tcc"

#endif // _OS_MESSAGESTRAEM_H_

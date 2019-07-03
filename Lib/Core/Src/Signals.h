
#ifndef _OS_SIGNAL_H_
#define _OS_SIGNAL_H_

#include <cstdint>
#include <vector>
#include <functional>
#include <memory>
#include <algorithm>

namespace OS { 

/**
 * The AbstractSignal interface is used for disconnecting
 * a connected signal through the OS::Connection class
 */
class AbstractSignal : public std::enable_shared_from_this<AbstractSignal> {

public:
    /**
     * This method is need for a OS::Connection to disconnect a 
     * connected signal.
     */
    virtual void Disconnect (uint64_t) = 0;
};

/**
 * The Connection class can be used to disconnect a connected signal.
 */
class Connection {

public:
    /**
     * Default constructor, can be used to instantiate a Connection object
     * without it actually containing a connection. 
     */
    Connection ();

    /**
     * Copy constructor, implicitely used when transferring a Connection created
     * by the signal object to a connection created by the Default Constructor
     */
    Connection (const Connection& inConnection);

    /**
     * The constructor used by OS::Signal::Connect.
     */
    Connection (const std::shared_ptr<AbstractSignal>& inPtr, uint64_t inIndex);
    
    /**
     * Close the Connection. This is the one function the user is most interested in.
     */
    void Close ();

private:
    std::weak_ptr<AbstractSignal> mPtr;
    uint64_t mIndex;
};

/**
 * The OS::Signal class provides a way for a Callback to be registered and
 * called when the Signal's Emit function is called. 
 */
template <class ...Args>
class Signal {

public:
    /**
     * The Callback type definition
     */
    using Callback = std::function<void(Args...)>;

    /**
     * The default constructor
     */
    Signal ();

    /**
     * The copy constructor
     */
    Signal (const Signal& inSignal);

    /**
     * Register a Member function of an object as Callback
     */
    template <typename T>
    Connection Connect (T* inObject, void(T::*inMethod)(Args...));

    /**
     * Register a Callback to the Signal.
     */
    Connection Connect (Callback inCallback);

    /**
     * Register a forward signal.
     */
    Connection Forward (Signal& inSignal);

    /**
     * Emit to all registered callbacks.
     */
    void Emit (Args... args);

    /**
     * Clear all connections.
     */
    void DisconnectAll ();

private:
    class Implementation;
    std::shared_ptr<Implementation> mImpl;
};

} // end namespace OS

#include "Signals.tcc"

#endif // OS_SIGNAL_H_

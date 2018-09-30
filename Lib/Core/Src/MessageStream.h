
#ifndef _OS_MESSAGESTRAEM_H_
#define _OS_MESSAGESTRAEM_H_

#include <cstdint>
#include <vector>
#include <functional>

namespace OS {

class MessageStream {

public:
    using Packet = std::vector<uint8_t>;
    using Callback = std::function<void(const Packet&)>;

public:
    virtual void Write (const Packet& inMessage) = 0;
    
public:
    void AddCallback (Callback inCallback);
    
protected:
    void Done (const Packet& inMessage);

private:
    std::vector<Callback> mCallbacks;

};

} // end namespace OS

#endif // _OS_MESSAGESTRAEM_H_

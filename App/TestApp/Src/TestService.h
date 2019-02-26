
#ifndef _TESTSERVICE_H_
#define _TESTSERVICE_H_

#include <cstdint>
#include <memory>

#include "PeriodicThread.h"
#include "MessageStream.h"

namespace TEST {

/**
 * The TEST::Service enables high throughput streams 
 * of watermarked data. 
 */
class Service {

public:
    /**
     * Start a data stream. 
     */
    void StartStringStream (uint64_t inTimeStep, std::size_t inDataSize, std::size_t inDataCount);
    void StartBinaryStream (uint64_t inTimeStep, std::size_t inDataSize, std::size_t inDataCount);
    
    /**
     * Retrieve streams for Piping purposes.
     */
    OS::ForwardStream<std::string>& GetStringStream () { return mStringStream; } 
    OS::ForwardStream<std::vector<uint8_t>>& GetBinaryStream () { return mBinaryStream; } 
    
    /**
     * 
     */
    bool GenerateStringMessage ();
    bool GenerateBinaryMessage ();
    
private:
    OS::ForwardStream<std::string> mStringStream; 
    OS::ForwardStream<std::vector<uint8_t>> mBinaryStream;
    std::unique_ptr<APP::PeriodicThread> mRunner;
    std::size_t mDataCount;
    std::size_t mDataSize;
    std::size_t mCounter;
};

} // end namespace TEST

#endif // end _TESTSERVICE_H_

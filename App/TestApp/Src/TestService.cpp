
#include <json.hpp>
using json = nlohmann::json;

#include "TriggerThread.h"
#include "TestService.h"
#include "Timing.h"

void TEST::Service::StartStringStream (uint64_t inTimeStep, std::size_t inDataSize, std::size_t inDataCount) {
    
    mDataSize = inDataSize;
    mDataCount = inDataCount;
    mCounter = 0u;
    
    mRunner = std::make_unique<APP::TriggerThread<Service>> ("test::service::string", inTimeStep, this, &Service::GenerateStringMessage);
    mRunner->Spawn ();
}

void TEST::Service::StartBinaryStream (uint64_t inTimeStep, std::size_t inDataSize, std::size_t inDataCount) {
    
    mDataSize = inDataSize;
    mDataCount = inDataCount;
    mCounter = 0u;
    
    mRunner = std::make_unique<APP::TriggerThread<Service>> ("test::service::binary", inTimeStep, this, &Service::GenerateBinaryMessage);
    mRunner->Spawn ();
}

bool TEST::Service::GenerateStringMessage () {
    
    // Stop the stream if we have reached the data count
    if (mCounter++ >= mDataCount) {
        return false;
    }
    
    json header;
    header["time"] = OS::Timing::Now ();
    header["tick"] = mCounter;
    header["count"] = mDataCount;
    
    std::string data (mDataSize, ' ');
    data.insert (0u, header.dump ());
    data.resize (mDataSize);
    
    mStringStream.Write (data);
    
    return true;
}

bool TEST::Service::GenerateBinaryMessage () {
    
    // Stop the stream if we have reached the data count
    if (mCounter++ > mDataCount) {
        return false;
    }
    
    std::vector<uint8_t> data;
    mBinaryStream.Write (data);
    
    return true;
}

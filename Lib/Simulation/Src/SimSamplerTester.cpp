
#include "gtest/gtest.h"
#include "SimSampler.h"
#include "SimTypedPort.h"

TEST (SimSamplerTester, Sampling) {
    
    std::string samples;
    auto port = std::make_shared<SIM::TypedPort<uint8_t>> ("TestPort");
    port->Set (0xFF);
    
    SIM::Sampler sampler;
    sampler.AddPort (port);
    
    sampler.Pipe ([&](const auto& newsamples) {samples = newsamples.ToString();});
    
    sampler.Write (0xFE);
    
    ASSERT_EQ ("{\"event-data\":[254.0,255.0],\"event-id\":\"sampler\"}", samples);
}








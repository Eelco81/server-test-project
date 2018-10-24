
#include "gtest/gtest.h"
#include "SimSampler.h"
#include "SimTypedPort.h"

TEST (SimSamplerTester, Sampling) {
    
    std::vector<std::string> samples;
    auto port = std::make_shared<SIM::TypedPort<uint8_t>> ("TestPort");
    port->Set (0xFF);
    
    SIM::Sampler sampler;
    sampler.AddPort (port);
    
    sampler.Pipe ([&](const auto& newsamples) {samples = newsamples;});
    
    sampler.Write (0xFE);
    
    ASSERT_EQ ("254", samples[0]);
    ASSERT_EQ ("255", samples[1]);
}








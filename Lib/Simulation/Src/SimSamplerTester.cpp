
#include "gtest/gtest.h"
#include "SimSampler.h"
#include "SimTypedPort.h"

TEST (SimSamplerTester, Sampling) {
    
    std::vector<double> samples;
    auto port = std::make_shared<SIM::TypedPort<uint8_t>> ("TestPort");
    port->Set (0xFF);
    
    SIM::Sampler sampler;
    sampler.AddPort (port);
    
    sampler.Pipe ([&](const auto& newsamples) {samples = newsamples;});
    
    sampler.Write (0xFE);
    
    ASSERT_EQ (254.0, samples[0]);
    ASSERT_EQ (255.0, samples[1]);
}








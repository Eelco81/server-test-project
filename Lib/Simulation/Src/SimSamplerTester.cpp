
#include "gtest/gtest.h"
#include "SimSampler.h"
#include "SimTypedPort.h"

TEST (SimSamplerTester, Sampling) {
    
    std::string samples;
    auto port = std::make_shared<SIM::TypedPort<uint8_t>> (SIM::Path ("test.in.port"));
    port->Set (0xFF);
    
    SIM::Sampler sampler (1u);
    sampler.AddPort (port);
    
    sampler.Pipe ([&](const auto& newsamples) {samples = newsamples.ToString();});
    
    sampler.Write (0xFE);
    
    ASSERT_EQ ("{\"event-data\":[254.0,255.0],\"event-id\":\"sim-sampler-1\"}", samples);
}

TEST (SimSamplerTester, Info) {
    
    SIM::Sampler sampler (1u);
    
    auto port1 = std::make_shared<SIM::TypedPort<uint8_t>> (SIM::Path ("test.in.port1"));
    auto port2 = std::make_shared<SIM::TypedPort<int64_t>> (SIM::Path ("test.in.port2"));
    auto port3 = std::make_shared<SIM::TypedPort<float>> (SIM::Path ("test.in.port3"));
    auto port4 = std::make_shared<SIM::TypedPort<double>> (SIM::Path ("test.in.port4"));
    
    sampler.AddPort (port1);
    sampler.AddPort (port2);
    sampler.AddPort (port3);
    sampler.AddPort (port4);
    
    const auto info (sampler.GetInfo ());
    
    ASSERT_EQ (std::string ("test.in.port1"), info.mPortInfos[0].mPath.ToString ());
    ASSERT_EQ (std::string ("test.in.port2"), info.mPortInfos[1].mPath.ToString ());
    ASSERT_EQ (std::string ("test.in.port3"), info.mPortInfos[2].mPath.ToString ());
    ASSERT_EQ (std::string ("test.in.port4"), info.mPortInfos[3].mPath.ToString ());
    
    ASSERT_EQ (SIM::Port::UINT8, info.mPortInfos[0].mType);
    ASSERT_EQ (SIM::Port::INT64, info.mPortInfos[1].mType);
    ASSERT_EQ (SIM::Port::FLOAT, info.mPortInfos[2].mType);
    ASSERT_EQ (SIM::Port::DOUBLE, info.mPortInfos[3].mType);
    
    ASSERT_EQ (std::string ("sim-sampler-1"), info.mId);
}


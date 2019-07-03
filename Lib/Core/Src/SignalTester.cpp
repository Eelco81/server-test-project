
#include "gmock/gmock.h"
#include "Signals.h"

TEST (SignalTester, ConnectAndDisconnect) {

    std::size_t value (0u);

    OS::Signal <std::size_t, std::size_t> signal;

    auto connection = signal.Connect ([&value](std::size_t newValue1, std::size_t newValue2) {
        value = newValue1 + newValue2; 
    });

    // After emmision, the data needs to be changed
    ASSERT_EQ (0u, value);
    signal.Emit (1u, 2u);
    ASSERT_EQ (3u, value);

    connection.Close ();
    signal.Emit (10u, 20u);
    ASSERT_EQ (3u, value);
}

TEST (SignalTester, NoCrashesDuringScopeLoss) {

    OS::Connection connection;
    {
        OS::Signal<> sig;
        connection = sig.Connect ([]() { });
    }
    ASSERT_NO_THROW (connection.Close ());
}

TEST (SignalTester, CopyMeansAFreshStart) {

    OS::Signal<> sig1;
    OS::Signal<> sig2 (sig1);

    bool callCount (0u);
    sig1.Connect ([&callCount]() { callCount++; });

    sig1.Emit ();
    ASSERT_EQ (1u, callCount);
}

TEST (SignalTester, MemberFunctionAsCallback) {

    struct TestObject {
        void Func (int inValue) {mValue += inValue;}
        int mValue = 0;
    };
    TestObject object;

    OS::Signal<int> signal;
    signal.Connect (&object, &TestObject::Func);

    ASSERT_EQ (0, object.mValue);
    signal.Emit (1);
    ASSERT_EQ (1, object.mValue);
}

TEST(SignalTester, Forwarding) {
    
    OS::Signal<bool> sig1, sig2;
    bool output = false;
    sig1.Forward (sig2);
    sig2.Connect ([&output](bool value) { output = value; });

    sig1.Emit (true);
    ASSERT_TRUE (output);
}
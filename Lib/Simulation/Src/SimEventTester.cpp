
#include "gtest/gtest.h"
#include "SimEvent.h"

TEST (SimEventTester, StringData) {
    SIM::Event event ("the id", "DATA");
    ASSERT_EQ (std::string ( "{\"event-data\":\"DATA\",\"event-id\":\"the id\"}"), event.ToString ());
}

TEST (SimEventTester, VectorData) {
    SIM::Event event ("the id", std::vector<int> (2,1));
    ASSERT_EQ (std::string ( "{\"event-data\":[1,1],\"event-id\":\"the id\"}"), event.ToString ());
}

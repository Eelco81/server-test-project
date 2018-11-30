
#ifndef _MOCK_SIMSERVICE_H_
#define _MOCK_SIMSERVICE_H_

#include "gmock/gmock.h"
#include "SimService.h"

namespace MOCK {

class Service : public SIM::IService {
public:
    MOCK_METHOD0 (Start, void ());
    MOCK_METHOD0 (Stop, void ());
    MOCK_METHOD1 (Load, void (const json&));
    MOCK_METHOD0 (UnLoad, void ());
    MOCK_CONST_METHOD0 (IsRunning, bool ());
    MOCK_CONST_METHOD0 (IsLoaded, bool ());
    MOCK_METHOD0 (GetPaths, std::vector<SIM::Path> ());
    MOCK_METHOD0 (GetValues, std::vector<SIM::Value> ());
    MOCK_METHOD1 (GetValue, SIM::Value (const SIM::Path&));
    MOCK_METHOD1 (SetValue, void (const SIM::Value&));
};

} // end namespace MOCK

#endif // _MOCK_SIMSERVICE_H_

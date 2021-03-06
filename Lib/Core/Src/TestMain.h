
#include "gtest/gtest.h"
#include "OS.h"
#include "Log.h"

namespace TEST {

    int RunAll (int argc, char ** argv) {
        
        OS::Initialize ();
        OS::Log::Instance ().Initialize (OS::Log::kTrace);
        
        ::testing::InitGoogleTest (&argc, argv);
        int returnValue = RUN_ALL_TESTS ();
        
        OS::Log::Instance().Flush();
        OS::Finalize ();
        
        return returnValue;
        
    }
}

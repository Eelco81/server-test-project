
#ifndef _NETWORK_H_
#define _NETWORK_H_

namespace OS {

class Network {

public: 
    Network () = default;
    ~Network () = default;

    static void Initialize ();
    static void Done ();

private:
    static bool mInitialized;
};

}

#endif

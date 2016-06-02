
#ifndef _NETWORK_H_
#define _NETWORK_H_

namespace Core {

class Network {

public: 
    Network ();
    ~Network ();

    static void Initialize ();
    static void Done ();

private:
    static bool mInitialized;
};

}

#endif

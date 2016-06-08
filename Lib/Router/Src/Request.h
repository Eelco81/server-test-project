
#ifndef _REQUEST_H_
#define _REQUEST_H_

#include <string>

#include "Message.h"

namespace API {

class Request : public Message {
public:
    Request ();
    virtual ~Request () = default;
};

}

#endif

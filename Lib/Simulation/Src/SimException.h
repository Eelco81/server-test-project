
#ifndef _SIM_EXCEPTION_H_
#define _SIM_EXCEPTION_H_

#include <exception>
#include <string>

namespace SIM {

class Exception : public std::exception {

public:
    Exception (const std::string& inMessage);
    virtual ~Exception ();

    virtual const char* what () const throw ();

protected:
    std::string mMessage;

};

}

#endif //  _SIM_EXCEPTION_H_

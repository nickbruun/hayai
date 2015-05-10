#include <sstream>
#include <ostream>

#include "gtest/gtest.h"
#include "hayai.hpp"

using namespace hayai;


#define ADD_FAILURE_STREAM(__msg)                            \
    {                                                        \
        std::stringstream _msg;                              \
        _msg << __msg;                                       \
        ADD_FAILURE() << _msg.str();                         \
    }


std::ostream& operator <<(std::ostream& s,
                          const TestParameterDescriptor& desc)
{
    return s << "::hayai::TestParameterDescriptor(Declaration="
             << desc.Declaration << ", Value=" << desc.Value << ")";
}

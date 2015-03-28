#ifndef __HAYAI_CONSOLEOUTPUTTER
#define __HAYAI_CONSOLEOUTPUTTER
#include "hayai_stream_outputter.hpp"

namespace hayai
{
    /// Console outputter.

    /// Prints the result to standard output.
    class ConsoleOutputter
        :   public StreamOutputter
    {
    public:
        ConsoleOutputter() : StreamOutputter(std::cout)
        {
        }
    };
}
#endif

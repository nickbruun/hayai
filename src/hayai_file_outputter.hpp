#ifndef __HAYAI_FILEOUTPUTTER
#define __HAYAI_FILEOUTPUTTER
#include <fstream>
#include "hayai_stream_outputter.hpp"

namespace hayai
{
    /// File outputter.

    /// Prints the result to the output file. If the file can't be opened
    /// redirects result to standard output.
    class FileOutputter
            : public StreamOutputter
    {
    public:
        /// @param fileName Name of the output file.
        /// @param openMode Mode with which the file will be opened.
        FileOutputter(const char* fileName,
                      const std::ios_base::openmode& openMode = std::ios::out)
        {
            _file.open(fileName, openMode);

            // If the file can't be opened redirect stream to std output
            if(_file)
                _stream = &_file;
            else {
                _stream = &std::cout;
                std::cout << "COULD NOT OPEN \""
                << fileName << "\" FILE." << std::endl;
            }

        }

    private:
        std::ofstream _file;    ///< Output file handler
    };
}
#endif

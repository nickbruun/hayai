#ifndef __HAYAI_FILEOUTPUTTER
#define __HAYAI_FILEOUTPUTTER
#include "hayai_stream_outputter.hpp"

namespace hayai
{
    /// File outputter.

    /// Prints the result to the output file.
    class FileOutputter
            : public StreamOutputter
    {
    public:
        /// @param fileName Name of the output file.
        /// @param openMode Mode with which the file will be opened.
        FileOutputter(const char* fileName,
                      std::ios_base::openmode openMode = std::ios::out)
                : StreamOutputter(_file), _file(fileName, openMode)
        {
            if(!_file) {
                std::cout << "COULD NOT OPEN \""
                << fileName << "\" FILE. PROCESSING IN BACKGROUND..."
                << std::endl;
            }
        }

    private:
        std::ofstream _file;    ///< Output file handler
    };
}
#endif

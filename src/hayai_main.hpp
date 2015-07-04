#ifndef __HAYAI_MAIN
#define __HAYAI_MAIN
#include <algorithm>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <errno.h>
#include <fstream>
#include <set>
#include <vector>

#include "hayai.hpp"


namespace hayai
{
    /// Execution mode.
    enum MainExecutionMode
    {
        /// Run benchmarks.
        MainRunBenchmarks,


        /// List benchmarks but do not execute them.
        MainListBenchmarks
    };


    /// File outputter.
    class FileOutputter
    {
    public:
        /// File outputter.

        /// @param path Output path. Expected to be available during the life
        /// time of the outputter.
        FileOutputter(const char* path)
            :   _path(path),
                _outputter(NULL)
        {

        }


        virtual ~FileOutputter()
        {
            if (_outputter)
                delete _outputter;

            _stream.close();
        }


        /// Set up.

        /// Opens the output file for writing and initializes the outputter.
        virtual void SetUp()
        {
            _stream.open(_path,
                         std::ios_base::out |
                         std::ios_base::trunc |
                         std::ios_base::binary);
            if (_stream.bad())
            {
                std::stringstream error;
                error << "failed to open " << _path << " for writing: "
                      << strerror(errno);
                throw std::runtime_error(error.str());
            }

            _outputter = CreateOutputter(_stream);
        }


        /// Outputter.
        virtual ::hayai::Outputter& Outputter()
        {
            if (!_outputter)
                throw std::runtime_error("outputter has not been set up");

            return *_outputter;
        }
    protected:
        /// Create outputter from output stream.

        /// @param stream Output stream for the outputter.
        /// @returns the outputter for the given format.
        virtual ::hayai::Outputter* CreateOutputter(std::ostream& stream) = 0;
    private:
        const char* _path;
        std::ofstream _stream;
        ::hayai::Outputter* _outputter;
    };


    class JsonFileOutputter
        :   public FileOutputter
    {
    public:
        /// JSON file outputter.

        /// @param path Output path. Expected to be available during the life
        /// time of the outputter.
        JsonFileOutputter(const char* path)
            :   FileOutputter(path)
        {

        }
    protected:
        virtual ::hayai::Outputter* CreateOutputter(std::ostream& stream)
        {
            return new ::hayai::JsonOutputter(stream);
        }
    };


    class ConsoleFileOutputter
        :   public FileOutputter
    {
    public:
        /// Console file outputter.

        /// @param path Output path. Expected to be available during the life
        /// time of the outputter.
        ConsoleFileOutputter(const char* path)
            :   FileOutputter(path)
        {

        }
    protected:
        virtual ::hayai::Outputter* CreateOutputter(std::ostream& stream)
        {
            return new ::hayai::ConsoleOutputter(stream);
        }
    };
}
#endif

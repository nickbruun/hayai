#ifndef __HAYAI_TESTDESCRIPTOR
#define __HAYAI_TESTDESCRIPTOR
#include <cstring>
#include <sstream>
#include <string>
#include <vector>

#include "hayai_test.hpp"
#include "hayai_test_factory.hpp"


namespace hayai
{
    /// Parameter declaration.

    /// Describes parameter type and name.
    class TestParameterDescriptor
    {
    public:
        TestParameterDescriptor(std::string type,
                            std::string name,
                            std::string value)
            :   Type(type),
                Name(name),
                Value(value)
        {

        }


        /// Type.
        std::string Type;


        /// Name.
        std::string Name;


        /// Value.
        std::string Value;
    };


    /// Test parameters descriptor.
    class TestParametersDescriptor
    {
    private:
        /// Quoting state.
        enum QuotingState
        {
            /// Unquoted.
            Unquoted,


            /// Single quoted.
            SingleQuoted,


            /// Double quoted.
            DoubleQuoted
        };


        /// Trimmed string.

        /// @param start Start character.
        /// @param end Character one position beyond end.
        inline static std::string TrimmedString(const char* start,
                                                const char* end)
        {
            while (start < end)
            {
                if ((*start == ' ') ||
                    (*start == '\r') ||
                    (*start == '\n') ||
                    (*start == '\t'))
                    ++start;
                else
                    break;
            }

            while (end > start)
            {
                const char c = *(end - 1);

                if ((c != ' ') &&
                    (c != '\r') &&
                    (c != '\n') &&
                    (c != '\t'))
                    break;

                --end;
            }

            return std::string(start, end - start);
        }


        /// Parse comma separated parentherized value.

        /// @param separated Separated values as "(..[, ..])".
        /// @returns the individual values with white space trimmed.
        static std::vector<std::string>
            ParseCommaSeparated(const char* separated)
        {
            std::vector<std::string> result;

            if (*separated)
                ++separated;

            while ((*separated) && (*separated != ')'))
            {
                std::size_t escapeCounter = 0;
                const char* start = separated;
                QuotingState state = Unquoted;
                bool escaped;

                while (*separated)
                {
                    const char c = *separated++;

                    if (state == Unquoted)
                    {
                        if ((c == '"') || (c == '\''))
                        {
                            state = (c == '"' ? DoubleQuoted : SingleQuoted);
                            escaped = false;
                        }
                        else if ((c == '<') ||
                                 (c == '(') ||
                                 (c == '[') ||
                                 (c == '{'))
                            ++escapeCounter;
                        else if ((escapeCounter) &&
                                 ((c == '>') ||
                                  (c == ')') ||
                                  (c == ']') ||
                                  (c == '}')))
                            --escapeCounter;
                        else if ((!escapeCounter) &&
                                 ((c == ',') || (c == ')')))
                        {
                            result.push_back(TrimmedString(start,
                                                           separated - 1));
                            break;
                        }
                    }
                    else
                    {
                        if (escaped)
                            escaped = false;
                        else if (c == '\\')
                            escaped = true;
                        else if (c == (state == DoubleQuoted ? '"' : '\''))
                            state = Unquoted;
                    }
                }
            }

            return result;
        }


        /// Parse parameter type.

        /// @param position Position.
        std::string ParseType(const char*& position)
        {
            const char* start = position;

            while (*position)
            {
                std::size_t escapeCounter = 0;
                QuotingState state = Unquoted;
                bool escaped;

                while (*position)
                {
                    const char c = *position++;

                    if (state == Unquoted)
                    {
                        if ((c == '"') || (c == '\''))
                        {
                            state = (c == '"' ? DoubleQuoted : SingleQuoted);
                            escaped = false;
                        }
                        else if ((c == '<') ||
                                 (c == '(') ||
                                 (c == '[') ||
                                 (c == '{'))
                            ++escapeCounter;
                        else if ((escapeCounter) &&
                                 ((c == '>') ||
                                  (c == ')') ||
                                  (c == ']') ||
                                  (c == '}')))
                            --escapeCounter;
                        else if ((!escapeCounter) &&
                                 (c == ' '))
                        {
                            return std::string(start, position - start - 1);
                        }
                    }
                    else
                    {
                        if (escaped)
                            escaped = false;
                        else if (c == '\\')
                            escaped = true;
                        else if (c == (state == DoubleQuoted ? '"' : '\''))
                            state = Unquoted;
                    }
                }
            }

            return std::string();
        }


        /// Parse parameter declaration.

        /// @param raw Raw declaration.
        TestParameterDescriptor ParseDescriptor(const std::string& raw)
        {
            const char* position = raw.c_str();

            // Parse the type.
            std::string type = ParseType(position);

            // Parse the name and default value.
            std::string name, defaultValue;

            if (*position)
            {
                const char* equalPosition = strchr(position, '=');
                const char* end = position + strlen(position);

                if (equalPosition)
                {
                    name = std::string(TrimmedString(position,
                                                     equalPosition));
                    defaultValue = std::string(TrimmedString(equalPosition + 1,
                                                             end));
                }
                else
                    name = std::string(TrimmedString(position, end));
            }

            return TestParameterDescriptor(type, name, defaultValue);
        }
    public:
        TestParametersDescriptor()
        {
        
        }


        TestParametersDescriptor(const char* rawDeclarations,
                                 const char* rawValues)
        {
            // Parse the declarations.
            std::stringstream ss;

            std::vector<std::string> declarations =
                ParseCommaSeparated(rawDeclarations);

            for (std::vector<std::string>::const_iterator it =
                     declarations.begin();
                 it != declarations.end();
                 ++it)
                _parameters.push_back(ParseDescriptor(*it));

            // Parse the values.
            std::vector<std::string> values = ParseCommaSeparated(rawValues);

            std::size_t i = 0;
            while ((i < values.size()) && (i < _parameters.size()))
            {
                _parameters[i].Value = values[i];
                ++i;
            }
        }


        inline const std::vector<TestParameterDescriptor>& Parameters() const
        {
            return _parameters;
        }
    private:
        std::vector<TestParameterDescriptor> _parameters;
    };


    /// Test descriptor.
    class TestDescriptor
    {
    public:
        /// Initialize a new test descriptor.

        /// @param fixtureName Name of the fixture.
        /// @param testName Name of the test.
        /// @param runs Number of runs for the test.
        /// @param iterations Number of iterations per run.
        /// @param testFactory Test factory implementation for the test.
        /// @param parameters Parametrized test parameters.
        TestDescriptor(const char* fixtureName,
                       const char* testName,
                       std::size_t runs,
                       std::size_t iterations,
                       TestFactory* testFactory,
                       TestParametersDescriptor parameters,
                       bool isDisabled = false)
            :   FixtureName(fixtureName),
                TestName(testName),
                Runs(runs),
                Iterations(iterations),
                Factory(testFactory),
                Parameters(parameters),
                IsDisabled(isDisabled)
        {

        }


        /// Dispose of a test descriptor.
        ~TestDescriptor()
        {
            delete this->Factory;
        }


        /// Fixture name.
        std::string FixtureName;


        /// Test name.
        std::string TestName;


        /// Test runs.
        std::size_t Runs;


        /// Iterations per test run.
        std::size_t Iterations;


        /// Test factory.
        TestFactory* Factory;


        /// Parameters for parametrized tests
        TestParametersDescriptor Parameters;


        /// Disabled.
        bool IsDisabled;
    };
}
#endif

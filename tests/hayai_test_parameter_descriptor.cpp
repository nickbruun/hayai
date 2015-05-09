#include "base.hpp"


/// Expect parameter descriptor of TestParameterDescriptor.

/// @param declarations Declarations.
/// @param values Values.
/// @param expected Expected parameter descriptors.
void ExpectTestParameterDescriptorParameterDescriptors(
    const char* declarations,
    const char* values,
    const std::vector<TestParameterDescriptor>& expected
)
{
    std::stringstream testDescStream;
    testDescStream << "TestParameterDescriptor(\""
                   << declarations
                   << "\", \""
                   << values
                   << "\")";
    std::string testDesc = testDescStream.str();

    SCOPED_TRACE(testDescStream.str());

    TestParametersDescriptor desc(declarations, values);
    const std::vector<TestParameterDescriptor>& actual = desc.Parameters();

    EXPECT_EQ(expected.size(), actual.size())
        << "Expected " << expected.size() << " parameters, but got "
        << actual.size();

    std::size_t i = 0;
    while ((i < actual.size()) && (i < expected.size()))
    {
        if ((expected[i].Declaration != actual[i].Declaration) ||
            (expected[i].Value != actual[i].Value))
        {
            ADD_FAILURE_STREAM("Expected Parameter[" << i << "] to be " <<
                               expected[i] << " but it is " << actual[i]);
        }
        ++i;
    }

    if (i < actual.size())
    {
        std::stringstream msg;
        msg << "Unexpected parameters: ";
        for (std::size_t j = i; j < actual.size(); ++j)
        {
            if (j > i)
                msg << ", ";
            msg << actual[j];
        }

        ADD_FAILURE() << msg;
    }
    else if (i < expected.size())
    {
        std::stringstream msg;
        msg << "Expected parameters: ";

        for (std::size_t j = i; j < expected.size(); ++j)
        {
            if (j > i)
                msg << ", ";
            msg << expected[j];
        }

        ADD_FAILURE() << msg;
    }
}


TEST(TestParameterDescriptor, Constructor)
{
    // Simple parameters.
    {
        std::vector<TestParameterDescriptor> expected;
        ExpectTestParameterDescriptorParameterDescriptors("", "", expected);
        ExpectTestParameterDescriptorParameterDescriptors("()", "()", expected);
    }

    {
        std::vector<TestParameterDescriptor> expected;
        expected.push_back(TestParameterDescriptor(
            "std::string a",
            "\"x\""
        ));

        ExpectTestParameterDescriptorParameterDescriptors("(std::string a)",
                                                          "(\"x\")",
                                                          expected);
    }

    // References and pointers.
    {
        std::vector<TestParameterDescriptor> expected;
        expected.push_back(TestParameterDescriptor(
            "std::string& a",
            "\"x\""
        ));

        ExpectTestParameterDescriptorParameterDescriptors("(std::string& a)",
                                                          "(\"x\")",
                                                          expected);
    }

    {
        std::vector<TestParameterDescriptor> expected;
        expected.push_back(TestParameterDescriptor(
            "std::string* a",
            "\"x\""
        ));

        ExpectTestParameterDescriptorParameterDescriptors("(std::string* a)",
                                                          "(\"x\")",
                                                          expected);
    }

    {
        std::vector<TestParameterDescriptor> expected;
        expected.push_back(TestParameterDescriptor(
            "std::string &a",
            "\"x\""
        ));

        ExpectTestParameterDescriptorParameterDescriptors("(std::string &a)",
                                                          "(\"x\")",
                                                          expected);
    }

    {
        std::vector<TestParameterDescriptor> expected;
        expected.push_back(TestParameterDescriptor(
            "std::string *a",
            "\"x\""
        ));

        ExpectTestParameterDescriptorParameterDescriptors("(std::string *a)",
                                                          "(\"x\")",
                                                          expected);
    }

    // Const.
    {
        std::vector<TestParameterDescriptor> expected;
        expected.push_back(TestParameterDescriptor(
            "const std::string& a",
            "\"x\""
        ));

        ExpectTestParameterDescriptorParameterDescriptors(
            "(const std::string& a)",
            "(\"x\")",
            expected
        );
    }

    {
        std::vector<TestParameterDescriptor> expected;
        expected.push_back(TestParameterDescriptor(
            "const std::string &a",
            "\"x\""
        ));

        ExpectTestParameterDescriptorParameterDescriptors(
            "(const std::string &a)",
            "(\"x\")",
            expected
        );
    }

    {
        std::vector<TestParameterDescriptor> expected;
        expected.push_back(TestParameterDescriptor(
            "std::string a",
            "\"x\""
        ));
        expected.push_back(TestParameterDescriptor(
            "std::wstring  b",
            "\"y\""
        ));

        ExpectTestParameterDescriptorParameterDescriptors(
            "(std::string a,  std::wstring  b)",
            "(\"x\", \"y\")",
            expected
        );
    }

    // Templates.
    {
        std::vector<TestParameterDescriptor> expected;
        expected.push_back(TestParameterDescriptor(
            "std::string a",
            "\"x\""
        ));
        expected.push_back(TestParameterDescriptor(
            "CrazyTemplate<'\\'', std::vector<std::string> > horse",
            "CrazyTemplate<'\\'', std::vector<std::string> >()"
        ));

        ExpectTestParameterDescriptorParameterDescriptors(
            "(std::string a, CrazyTemplate<'\\'', std::vector<std::string> > horse)",
            "(\"x\", CrazyTemplate<'\\'', std::vector<std::string> >())",
            expected
        );
    }

    // Default values.
    {
        std::vector<TestParameterDescriptor> expected;
        expected.push_back(TestParameterDescriptor(
            "std::string a",
            "\"x\""
        ));
        expected.push_back(TestParameterDescriptor(
            "int b",
            "-1"
        ));

        ExpectTestParameterDescriptorParameterDescriptors(
            "(std::string a, int b = -1)",
            "(\"x\")",
            expected
        );
        ExpectTestParameterDescriptorParameterDescriptors(
            "(std::string a,int b=-1)",
            "(\"x\")",
            expected
        );
    }

    // Variadic methods.
    {
        std::vector<TestParameterDescriptor> expected;
        expected.push_back(TestParameterDescriptor(
            "...",
            "1, 2, 3"
        ));

        ExpectTestParameterDescriptorParameterDescriptors(
            "(...)",
            "(1, 2, 3)",
            expected
        );
    }

    {
        std::vector<TestParameterDescriptor> expected;
        expected.push_back(TestParameterDescriptor(
            "const char* fmt",
            "\"x\""
        ));
        expected.push_back(TestParameterDescriptor(
            "...",
            "1, 2, 3"
        ));

        ExpectTestParameterDescriptorParameterDescriptors(
            "(const char* fmt, ...)",
            "(\"x\", 1, 2, 3)",
            expected
        );
    }
}

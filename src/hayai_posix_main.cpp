#include "hayai.hpp"

int main()
{
#ifdef __HAYAI_USE_XML
    hayai::XmlOutputter outputter;
#else
	hayai::ConsoleOutputter outputter;
#endif

    hayai::Benchmarker::AddOutputter(outputter);
    hayai::Benchmarker::RunAllTests();
    return 0;
}

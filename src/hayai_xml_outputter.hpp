#ifndef __HAYAI_XMLOUTPUTTER
#define __HAYAI_XMLOUTPUTTER
#include "hayai_outputter.hpp"
#include "hayai_console.hpp"

#ifdef __HAYAI_USE_XML 
#include <libxml/tree.h>
#include <libxml/xmlstring.h>
#include <libxml/xmlmemory.h>
#include <sstream>

namespace hayai
{
        
    /// Xml outputter.
    /// Prints the result to standard output in XML format.
    class XmlOutputter
        :   public Outputter
    {

    private:

        template <typename T>
        static xmlChar* ToXmlString(const T& value)
        {
            std::stringstream strs;
            strs << value;
            return xmlCharStrdup(strs.str().c_str());
        }

        template <typename NameType, typename ValueType>
        static void AddProperty(xmlNodePtr node, NameType name, const ValueType& value)
        {         
            xmlChar* xmlRepr = ToXmlString(value);
            xmlNewProp(node, BAD_CAST name, xmlRepr);
            xmlFree(xmlRepr);
        }

        template <typename NameType>
        static xmlNodePtr AddChild(xmlNodePtr node, NameType name)
        {
            return xmlNewChild(node, NULL, BAD_CAST name, NULL);
        }
   
    public:

        void Begin(const std::size_t& benchmarksCount)
        {
            document = xmlNewDoc(BAD_CAST "1.0");
            rootNode = xmlNewNode(NULL, BAD_CAST "hayai::tests");
            xmlDocSetRootElement(document, rootNode);
        }

        void End(const std::size_t& benchmarksCount)
        {
            xmlSaveFormatFileEnc("-", document, "UTF-8", 1);
            xmlFreeDoc(document);
            xmlCleanupParser();
        }

        void BeginTest(const std::string& fixtureName,
                       const std::string& testName,
                       const std::string& parameters,
                       const std::size_t& runsCount,
                       const std::size_t& iterationsCount)
        {
            testNode = AddChild(rootNode, "hayai::test");

            AddProperty(testNode, "fixtureName", fixtureName);
            AddProperty(testNode, "testName", testName);
            AddProperty(testNode, "parameters", parameters);
            AddProperty(testNode, "runsCount", runsCount);
            AddProperty(testNode, "iterationsCount", iterationsCount);
        }


        void EndTest(const std::string& fixtureName,
                     const std::string& testName,
                     const std::string& parameters,
                     const TestResult& result)
        {           
            AddProperty(testNode, "timeTotal", result.TimeTotal() / 1000000.0);
            AddProperty(testNode, "runTimeAverage", result.RunTimeAverage() / 1000.0);

            AddProperty(testNode, "runTimeMinimum", result.RunTimeMinimum() / 1000.0);
            AddProperty(testNode, "runTimeMaximum", result.RunTimeMaximum() / 1000.0);
            AddProperty(testNode, "runTimeAverage", result.RunTimeAverage() / 1000.0);

            AddProperty(testNode, "runsPerSecondMinimum", result.RunsPerSecondMinimum());
            AddProperty(testNode, "runsPerSecondMaximum", result.RunsPerSecondMaximum());
            AddProperty(testNode, "runsPerSecondAverage", result.RunsPerSecondAverage());

            AddProperty(testNode, "iterationTimeMinimum", result.IterationTimeMinimum());
            AddProperty(testNode, "iterationTimeMaximum", result.IterationTimeMaximum());
            AddProperty(testNode, "iterationTimeAverage", result.IterationTimeAverage());

            AddProperty(testNode, "iterationsPerSecondMinimum", result.IterationsPerSecondMinimum());
            AddProperty(testNode, "iterationsPerSecondMaximum", result.IterationsPerSecondMaximum());
            AddProperty(testNode, "iterationsPerSecondAverage", result.IterationsPerSecondAverage());
        }

    private:
        xmlDocPtr document;
        xmlNodePtr rootNode;
        xmlNodePtr testNode;
    };
        
};
#endif
#endif

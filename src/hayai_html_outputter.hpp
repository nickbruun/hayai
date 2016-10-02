#ifndef __HAYAI_HTMLOUTPUTTER
#define __HAYAI_HTMLOUTPUTTER
#include <iomanip>
#include <ostream>

#include "hayai_json_outputter.hpp"

namespace hayai
{
    /// HTML outputter.
    class HtmlOutputter
        :   public JsonOutputter
    {
    public:
        /// Initialize HTML outputter.

        /// @param stream Output stream. Must exist for the entire duration of
        /// the outputter's use.
        HtmlOutputter(std::ostream& stream)
            :   JsonOutputter(stream)
        {

        }


        virtual void Begin(const std::size_t& enabledCount,
                           const std::size_t& disabledCount)
        {
            _stream <<  "<html> \
                          <head> \
                            <title>Hayai Benchmarking Results</title> \
                            <script src=\"https://cdn.plot.ly/plotly-latest.min.js\"></script> \
                            <script> \
                              hayaiOutput = JSON.parse('";
            JsonOutputter::Begin(enabledCount, disabledCount);
        }


        virtual void End(const std::size_t& executedCount,
                         const std::size_t& disabledCount)
        {
            JsonOutputter::End(executedCount, disabledCount);
            _stream <<        "'); \
                              window.addEventListener(\"load\", function() { \
                                traces = []; \
                                var traceTemplate = '{ \"type\": \"box\", \"boxpoints\": \"outliers\" }'; \
                                for(var i=0;i<hayaiOutput.benchmarks.length;i++) { \
                                  traces[i] = JSON.parse(traceTemplate);traces[i].x = []; \
                                  var benchmark = hayaiOutput.benchmarks[i]; \
                                  traces[i].name = benchmark.fixture + \".\" + benchmark.name; \
                                  if(benchmark.parameters !== undefined) { \
                                    traces[i].name += \"(\"; \
                                    for(var j=0;j<benchmark.parameters.length;j++){ \
                                      traces[i].name += benchmark.parameters[j].declaration + \" = \" + benchmark.parameters[j].value + \", \"; \
                                    } \
                                    traces[i].name = traces[i].name.substr(0, traces[i].name.length-2) + \")\"; \
                                  } \
                                  if(benchmark.runs === undefined) continue; \
                                  for(var j=0;j<benchmark.runs.length;j++) { \
                                    traces[i].x[j] = benchmark.runs[j].duration; \
                                  } \
                                } \
                                var layout = { \
                                  title: \"Hayai Benchmarking Results\", \
                                  xaxis: { title: \"Execution time (ms)\" }, \
                                  yaxis: { showticklabels: false }, \
                                  scene: { hovermode: false } \
                                }; \
                                Plotly.newPlot(\"plotContainer\", traces, layout); \
                              }); \
                            </script> \
                          </head> \
                          <body> \
                            <div id=\"plotContainer\"></div> \
                          </body> \
                        </html>";
        }
    };
}

#endif

file(GLOB hayai_headers
  hayai_benchmarker.hpp
  hayai_console.hpp
  hayai_console_outputter.hpp
  hayai_fixture.hpp
  hayai.hpp
  hayai_outputter.hpp
)

add_library(hayai_main
  hayai_posix_main.cpp
)

set_target_properties(hayai_main
  PROPERTIES
  PUBLIC_HEADER "${hayai_headers}"
)

install(TARGETS hayai_main
  # IMPORTANT: Add the library to the "export-set"
  EXPORT hayai-targets
  RUNTIME DESTINATION "${INSTALL_BIN_DIR}" COMPONENT bin
  ARCHIVE DESTINATION "${INSTALL_LIB_DIR}" COMPONENT bin
  LIBRARY DESTINATION "${INSTALL_LIB_DIR}" COMPONENT shlib
  PUBLIC_HEADER DESTINATION "${INSTALL_INCLUDE_DIR}/hayai"
  COMPONENT dev
)

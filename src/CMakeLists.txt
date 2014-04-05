add_library(hayai_main
  hayai_posix_main.cpp
)

if (USE_XML)
    target_link_libraries(hayai_main xml2)
endif()

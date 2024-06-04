include_directories(${HEADPATH})
aux_source_directory(./ SRC)

add_library(testlib ${SRC})
target_link_libraries(testlib mylib)

# add_executable(test ${SRC})
# target_link_libraries(test ${PROTOBUF_LIBRARIES} mylib)

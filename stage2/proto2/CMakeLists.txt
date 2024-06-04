include_directories(${HEADPATH})
aux_source_directory(./ SRC)
add_library(mylib ${SRC})
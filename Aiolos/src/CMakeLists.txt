include_directories(${AIOLOS_HEADER_DIR} /Users/wzp/workspace/wzp_cpp_lib)
aux_source_directory(. SRC_LIST)
add_executable(aiolos ${SRC_LIST})

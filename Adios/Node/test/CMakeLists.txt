
include_directories(${ADIOS_HEADER_DIR} /home/wzp/workspace/wzp_cpp_lib)
aux_source_directory(. SRC_LIST)
#add_executable(server ${SRC_LIST})
add_executable(server main.cpp)
target_link_libraries(server boost_system boost_thread protobuf)

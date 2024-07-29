cmake_minimum_required(VERSION 3.8)
project(grpc_example CXX)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)

#enable address sanitizer (ASAN)
add_compile_options(-fsanitize=address)
add_link_options(-fsanitize=address)

find_package(gRPC CONFIG REQUIRED)

# just sets a variable containing all of our .proto files
set(PROTO_FILES
        proto/example.proto
        # example2.proto ...
)

# myproto is a library we are creating to group our generated files and gRPC+protobuf dependencies
add_library(myproto ${PROTO_FILES})
target_link_libraries(myproto
        PUBLIC
        protobuf::libprotobuf
        gRPC::grpc
        gRPC::grpc++
        gRPC::grpc++_reflection
)

# generated .h files will be placed in the build directory, so we need to add that to the include path
target_include_directories(myproto PUBLIC ${CMAKE_CURRENT_BINARY_DIR})

# actually generate needed files from the protobuf files.  Note that --experimental_editions is currently needed
# to use edition="2023" in the .proto files.
get_target_property(grpc_cpp_plugin_location gRPC::grpc_cpp_plugin LOCATION)
protobuf_generate(TARGET myproto LANGUAGE cpp
        PROTOC_OPTIONS --experimental_editions)
protobuf_generate(TARGET myproto LANGUAGE grpc GENERATE_EXTENSIONS .grpc.pb.h .grpc.pb.cc PLUGIN "protoc-gen-grpc=${grpc_cpp_plugin_location}"
        PROTOC_OPTIONS --experimental_editions)


# Create our executable
add_executable(grpc_example src/main.cpp)

# add "myproto" (generated protobuf files and gRPC libs) to the executable.
target_link_libraries(grpc_example PRIVATE myproto)

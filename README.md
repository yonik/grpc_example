# vcpkg/cmake C++ gRPC example 

This simple example demonstrates how to get started with gRPC using vcpkg to install gRPC and cmake
to build the example.

# Build Instructions

1. Install vcpkg

    ```
    # I installed vcpkg in /opt, but you can do it anywhere.
    cd /opt
    git clone https://github.com/microsoft/vcpkg.git
    cd vcpkg
    ./bootstrap-vcpkg.sh
    ```

2. Install gRPC and dependencies via vcpkg

    ```
    cd /opt/vcpkg
    ./vcpkg install grpc
    ```

3. Download and build this example project
    ```
    cd ~
    git clone https://github.com/yonik/grpc_example.git
    cd ~/grpc_example
    mkdir build
    cd build
    cmake .. -DCMAKE_TOOLCHAIN_FILE=/opt/vcpkg/scripts/buildsystems/vcpkg.cmake
    make
    ```

4. Run the example
    ```
    # should still be in the "build" directory 
    ./grpc_example
    ```

# Notes
If you get AddressSanitizer errors, that may be expected.  I created this project
to replicate the ASAN bugs I was seeing.  You can disable ASAN by commenting out
the ASAN related lines in CMakeLists.txt and then re-running cmake and make.

## License
This project is licensed under the Apache License v2.0
https://www.apache.org/licenses/LICENSE-2.0.txt

cmake_minimum_required(VERSION 3.6)
project(lucene_cpp)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

include_directories(store index util server /usr/local/include)
link_directories(/usr/local/lib)

set(SOURCE_FILES
    server/main.cpp
    store/buffered_index_output.cpp
    store/directory.cpp
    store/fs_lock_factory.cpp
    store/index_input.cpp
    store/index_output.cpp
    store/lock.cpp
    store/lock_factory.cpp
    store/no_lock_factory.cpp
    store/ram_directory.cpp
    store/ram_file.cpp
    store/ram_output_stream.cpp
    store/simple_fs_lock_factory.cpp
    store/single_instance_lock_factory.cpp
    util/file_reader.cpp
    util/file_utils.cpp
    util/info_stream.cpp
    util/lucene_allocator.cpp
    util/lucene_exception.cpp
    util/lucene_object.cpp
    util/lucene_signal.cpp
    util/lucene_sync.cpp
    util/lucene_thread.cpp
    util/misc_utils.cpp
    util/reader.cpp
    util/synchronize.cpp)

ADD_EXECUTABLE(lucene_cpp ${SOURCE_FILES})
TARGET_LINK_LIBRARIES(lucene_cpp boost_system-mt boost_thread-mt boost_filesystem-mt boost_iostreams-mt)

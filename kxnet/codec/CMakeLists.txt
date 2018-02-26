set(CODEC_SRCS 
    ReadStream.cc 
    WriteStream.cc 
    Codec.cc)

add_library(kxnet_codec ${CODEC_SRCS})
target_link_libraries(kxnet_codec)

if(NOT CMAKE_BUILD_NO_EXAMPLES)
    add_subdirectory(tests)
endif()



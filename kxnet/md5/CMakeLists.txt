set(MD5_SRCS MD5.cc)

add_library(kxnet_md5 ${MD5_SRCS})
target_link_libraries(kxnet_md5)

install(TARGETS kxnet_md5 DESTINATION lib)

file(GLOB HEADERS "*.h")
install(FILES ${HEADERS} DESTINATION include/kxnet/md5)

if(NOT CMAKE_BUILD_NO_EXAMPLES)
    add_subdirectory(test)
endif()



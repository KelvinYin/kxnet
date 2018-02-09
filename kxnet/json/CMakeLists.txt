set(JSON_SRCS
    cJSON.c
    JsonUtil.cc
  )

add_library(kxnet_json ${JSON_SRCS})
target_link_libraries(kxnet_json)

install(TARGETS kxnet_json DESTINATION lib)

file(GLOB HEADERS "*.h")
install(FILES ${HEADERS} DESTINATION include/kxnet/json)

if(NOT CMAKE_BUILD_NO_EXAMPLES)
  add_subdirectory(tests)
endif()

set(http_SRCS
  HttpServer.cc
  HttpResponse.cc
  HttpContext.cc
  )

add_library(kxnet_http ${http_SRCS})
target_link_libraries(kxnet_http kxnet_net)

install(TARGETS kxnet_http DESTINATION lib)
set(HEADERS
  HttpRequest.h
  HttpResponse.h
  HttpServer.h
  )
install(FILES ${HEADERS} DESTINATION include/kxnet/net/http)

if(NOT CMAKE_BUILD_NO_EXAMPLES)
add_executable(httpserver_test tests/HttpServer_test.cc)
target_link_libraries(httpserver_test kxnet_http)

if(BOOSTTEST_LIBRARY)
add_executable(httprequest_unittest tests/HttpRequest_unittest.cc)
target_link_libraries(httprequest_unittest kxnet_http boost_unit_test_framework)
endif()

endif()

# add_subdirectory(tests)

add_executable(jsonutil_test JsonUtil_tests.cc)
target_link_libraries(jsonutil_test kxnet_json)
add_test(NAME jsonutil_test COMMAND JsonUtil_test)

add_executable(md5_test md5_test.cc)
target_link_libraries(md5_test kxnet_base kxnet_md5)
add_test(NAME md5_test COMMAND md5_test)

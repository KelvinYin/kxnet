add_executable(codec_client client.cc)
target_link_libraries(codec_client kxnet_net kxnet_codec kxnet_base pthread)

add_executable(codec_server server.cc)
target_link_libraries(codec_server kxnet_net kxnet_codec kxnet_base pthread)

add_test(NAME codec_client COMMAND codec_client)

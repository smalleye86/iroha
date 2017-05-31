############################
#   connection_with_grpc   #
############################
ADD_LIBRARY(connection_with_grpc STATIC
  connection_with_grpc.cpp
)

target_link_libraries(connection_with_grpc
  endpoint_fbs
  flatbuffers
)

ADD_LIBRARY(membership_service STATIC
    peer_service.cpp
)

target_link_libraries(membership_service
    timer
    exception
    logger
    config_manager
    cache_map
    flatbuffers
    connection_with_grpc_flatbuffer
)
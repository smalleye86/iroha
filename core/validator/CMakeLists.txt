ADD_LIBRARY(stateless_validator STATIC
    stateless_validator.cpp
)

target_link_libraries(stateless_validator
    flatbuffers
)
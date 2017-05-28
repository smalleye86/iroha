add_executable(tx_generator_test tx_generator_test.cpp)
target_link_libraries(tx_generator_test
    gtest
    flatbuffers
    crypto
    signature
)
add_test(
    NAME tx_generator_test
    COMMAND $<TARGET_FILE:tx_generator_test>
)

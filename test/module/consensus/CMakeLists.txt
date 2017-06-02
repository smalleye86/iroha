###########################
#    block builder test   #
###########################
add_executable(block_builder_test
    block_builder_test.cpp
)
target_link_libraries(block_builder_test
    gtest
    block_builder
)
add_test(
    NAME block_builder_test
    COMMAND $<TARGET_FILE:block_builder_test>
)

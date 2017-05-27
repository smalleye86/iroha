include(ExternalProject)
set(EP_PREFIX "${PROJECT_SOURCE_DIR}/external")
set_directory_properties(PROPERTIES
        EP_PREFIX ${EP_PREFIX}
        )

# Project dependencies.
find_package(Threads REQUIRED)


###############################
#         flatbuffers         #
###############################
set(flatbuffers_CMAKE_ARGS
  -DCMAKE_C_COMPILER=${CMAKE_C_COMPILER}
  -DCMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER}
  -DFLATBUFFERS_BUILD_TESTS=OFF
  -DFLATBUFFERS_INSTALL=OFF
  -DFLATBUFFERS_BUILD_FLATHASH=OFF
  -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
  -DFLATBUFFERS_BUILD_FLATC=ON
  )
ExternalProject_Add(google_flatbuffers
  GIT_REPOSITORY  "https://github.com/google/flatbuffers.git"
  GIT_TAG         "c7bfe06c54bcd43312ff017e8ca32bc54833d524"
  CMAKE_ARGS      ${flatbuffers_CMAKE_ARGS}
  UPDATE_COMMAND  ""
  TEST_COMMAND    ""
  INSTALL_COMMAND ""
)

ExternalProject_Get_Property(google_flatbuffers source_dir binary_dir)
set(flatbuffers_INCLUDE_DIRS ${source_dir}/include)
set(flatbuffers_LIBRARIES ${binary_dir}/libflatbuffers.a)
set(flatc_EXECUTABLE ${binary_dir}/flatc)
file(MAKE_DIRECTORY ${flatbuffers_INCLUDE_DIRS})

add_custom_target(flatc DEPENDS google_flatbuffers)

add_library(flatbuffers STATIC IMPORTED)
set_target_properties(flatbuffers PROPERTIES
  INTERFACE_INCLUDE_DIRECTORIES ${flatbuffers_INCLUDE_DIRS}
  IMPORTED_LOCATION ${flatbuffers_LIBRARIES}
  )
add_dependencies(flatbuffers google_flatbuffers flatc)


##########################
#         gtest          #
##########################
# testing is an option. Look at the main CMakeLists.txt for details.
if(TESTING)
    ExternalProject_Add(google_test
            GIT_REPOSITORY    "https://github.com/google/googletest.git"
            CMAKE_ARGS        -DCMAKE_C_COMPILER=${CMAKE_C_COMPILER}
            -DCMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER}
            -Dgtest_force_shared_crt=ON
            -Dgtest_disable_pthreads=OFF
            -DBUILD_GTEST=ON
            -DBUILD_GMOCK=OFF
            INSTALL_COMMAND   "" # remove install step
            UPDATE_COMMAND    "" # remove update step
            TEST_COMMAND      "" # remove test step
            )
    ExternalProject_Get_Property(google_test source_dir binary_dir)
    set(gtest_SOURCE_DIR ${source_dir})
    set(gtest_BINARY_DIR ${binary_dir})

    add_library(gtest STATIC IMPORTED)
    file(MAKE_DIRECTORY ${gtest_SOURCE_DIR}/googletest/include)

    set_target_properties(gtest
            PROPERTIES
            INTERFACE_INCLUDE_DIRECTORIES ${gtest_SOURCE_DIR}/googletest/include
            IMPORTED_LINK_INTERFACE_LIBRARIES "pthread;${gtest_BINARY_DIR}/googletest/libgtest_main.a"
            IMPORTED_LOCATION ${gtest_BINARY_DIR}/googletest/libgtest.a
            )
    add_dependencies(gtest google_test)
endif()

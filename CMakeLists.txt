cmake_minimum_required(VERSION 3.25)
project(framework)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-O0 -g --coverage -fprofile-arcs -ftest-coverage")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -march=native -DNDEBUG")

option(ENABLE_TESTS "Enable framework tests" ON)

add_library(framework INTERFACE)
target_include_directories(framework INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include> $<INSTALL_INTERFACE:include>)
target_compile_features(framework INTERFACE cxx_std_23)

include(CMakePackageConfigHelpers)

install(TARGETS framework EXPORT frameworkTargets)

install(EXPORT frameworkTargets
        FILE frameworkTargets.cmake
        NAMESPACE framework::
        DESTINATION lib/cmake/framework
)

write_basic_package_version_file(
        "${CMAKE_CURRENT_BINARY_DIR}/frameworkConfigVersion.cmake"
        VERSION 0.0.1
        COMPATIBILITY SameMajorVersion
)

install(FILES "${CMAKE_CURRENT_SOURCE_DIR}/cmake/frameworkConfig.cmake" "${CMAKE_CURRENT_BINARY_DIR}/frameworkConfigVersion.cmake" DESTINATION lib/cmake/framework)

if (ENABLE_TESTS)
    include(FetchContent)
    FetchContent_Declare(googletest URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip)

    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(googletest)

    enable_testing()
    file(GLOB_RECURSE TEST_FILES CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/tests/*.cc)

    add_executable(framework-tests ${TEST_FILES})

    target_compile_options(framework-tests PRIVATE --coverage -O0 -g -fprofile-arcs -ftest-coverage)
    target_link_options(framework-tests PRIVATE --coverage)

    target_link_libraries(framework-tests GTest::gtest_main framework)

    include(GoogleTest)
    gtest_discover_tests(framework-tests)
endif()

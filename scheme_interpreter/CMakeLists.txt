include_directories("include")

add_library(lib_schemelang
    "src/lang/env.cpp"
    "src/lang/evaluate.cpp"
    "src/lang/lexer.cpp"
    "src/lang/parser.cpp"

    "include/lang/env.hpp"
    "include/lang/evaluate.hpp"	
    "include/lang/lexer.hpp"
    "include/lang/parser.hpp"
)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

set_property(TARGET lib_schemelang PROPERTY LINKER_LANGUAGE CXX)
set_property(TARGET lib_schemelang PROPERTY CXX_STANDARD 17)



add_executable(scheme
    "src/cli.cpp"
)

target_link_libraries(scheme PUBLIC lib_schemelang)

set_property(TARGET scheme PROPERTY LINKER_LANGUAGE CXX)
set_property(TARGET scheme PROPERTY CXX_STANDARD 17)


enable_testing()

add_executable(tests_schemelang
    "tests/main.cpp"
)

target_link_libraries(tests_schemelang PUBLIC lib_schemelang GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(tests_schemelang)

set_property(TARGET tests_schemelang PROPERTY LINKER_LANGUAGE CXX)
set_property(TARGET tests_schemelang PROPERTY CXX_STANDARD 17)
add_executable(test test.cpp)
target_link_libraries(test gtest_main)
add_test(NAME test_test COMMAND test)
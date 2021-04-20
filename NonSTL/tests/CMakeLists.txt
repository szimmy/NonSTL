add_executable(vector_test vector_t.cpp)
target_link_libraries(vector_test gtest_main)
add_test(NAME vec_test COMMAND vector_test)
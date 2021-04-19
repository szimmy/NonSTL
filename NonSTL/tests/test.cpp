#include <gtest/gtest.h>

#include "../vector.h"

TEST(BasicConstruct, Basic) {
	non_stl::vector<int> vec{ };
	ASSERT_EQ(vec.size(), 0);
	ASSERT_TRUE(vec.empty());
	ASSERT_EQ(vec.capacity(), 10);
}

TEST(InitializerConstruct, Basic) {
	non_stl::vector<int> vec = { 0,1,2 };
	
	ASSERT_EQ(vec.size(), 3);
	ASSERT_GT(vec.capacity(), 3);
	
	ASSERT_EQ(vec[0], 0);
	ASSERT_EQ(vec[1], 1);
	ASSERT_EQ(vec[2], 2);
}

TEST(SizeConstruct, Basic) {
	non_stl::vector<int> vec(3);
	
	ASSERT_EQ(vec.size(), 3);
	ASSERT_GT(vec.capacity(), 3);
	
	ASSERT_EQ(vec[0], 0);
	ASSERT_EQ(vec[1], 0);
	ASSERT_EQ(vec[2], 0);
}


TEST(SizeConstructWithValue, Basic) {
	non_stl::vector<int> vec(3, 5);
	
	ASSERT_EQ(vec.size(), 3);
	ASSERT_GT(vec.capacity(), 3);
	
	ASSERT_EQ(vec[0], 5);
	ASSERT_EQ(vec[1], 5);
	ASSERT_EQ(vec[2], 5);
}

TEST(IteratorConstruct, Basic) {
	non_stl::vector<int> vec1{ 0,1,2 };
	non_stl::vector<int> vec2(vec1.begin(), vec1.end());
	
	ASSERT_EQ(vec2.size(), 3);
	ASSERT_GT(vec2.capacity(), 3);
	
	ASSERT_EQ(vec2.size(), vec1.size());
	ASSERT_EQ(vec2.capacity(), vec1.capacity());
	
	ASSERT_EQ(vec2[0], 0);
	ASSERT_EQ(vec2[1], 1);
	ASSERT_EQ(vec2[2], 2);
}

TEST(IteratorConstructReverse, Basic) {
	non_stl::vector<int> vec1{ 0,1,2 };
	non_stl::vector<int> vec2(vec1.rbegin(), vec1.rend());
	
	ASSERT_EQ(vec2.size(), 3);
	ASSERT_GT(vec2.capacity(), 3);
	
	ASSERT_EQ(vec2.size(), vec1.size());
	ASSERT_EQ(vec2.capacity(), vec1.capacity());

	ASSERT_EQ(vec2[0], 2);
	ASSERT_EQ(vec2[1], 1);
	ASSERT_EQ(vec2[2], 0);
}

// TODO

// Copy Constructor

// Move constructor

// Assignment operator (lvalue, rvalue, init_list)

// .at, .front, .back

// resize, shrink_to_fit, reserve

// assign, push_back, emplace, emplace_back, insert, swap, clear

int main(int argc, char** argv) {
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
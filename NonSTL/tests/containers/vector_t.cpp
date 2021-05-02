#include <gtest/gtest.h>

#include "../../containers/vector.h"

// Constructors

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

TEST(CopyConstruct, Basic) {
	non_stl::vector<int> vec1{ 0,1,2 };
	non_stl::vector<int> vec2(vec1);
	ASSERT_EQ(vec2.size(), vec1.size());
	ASSERT_EQ(vec2.capacity(), vec1.capacity());

	ASSERT_EQ(vec2[0], vec1[0]);
	ASSERT_EQ(vec2[1], vec1[1]);
	ASSERT_EQ(vec2[2], vec1[2]);
}

TEST(MoveConstruct, Basic) {
	non_stl::vector<int> vec1{ 0,1,2 };
	non_stl::vector<int> vec2(std::move(vec1));
	ASSERT_EQ(vec2.size(), 3);
	ASSERT_GT(vec2.capacity(), 3);

	ASSERT_EQ(vec2[0], 0);
	ASSERT_EQ(vec2[1], 1);
	ASSERT_EQ(vec2[2], 2);
}

TEST(AtTest, Basic) {
	non_stl::vector<int> vec{ 1,2,3 };
	ASSERT_EQ(vec.at(0), 1);
	ASSERT_EQ(vec.at(1), 2);
	ASSERT_EQ(vec.at(2), 3);
}

TEST(FrontTest, Basic) {
	non_stl::vector<int> vec{ 5,6,7 };
	ASSERT_EQ(vec.front(), 5);
	ASSERT_EQ(vec.front(), *(vec.begin()));
}

TEST(BackTest, Basic) {
	non_stl::vector<int> vec{ 5,6,7 };
	ASSERT_EQ(vec.back(), 7);
}

TEST(ResizeTest, Basic) {
	// Default construct
	non_stl::vector<int> vec1;
	ASSERT_EQ(vec1.size(), 0);

	// Increase size
	vec1.resize(15);
	ASSERT_EQ(vec1.size(), 15);

	// Decrease size
	vec1.resize(3);
	ASSERT_EQ(vec1.size(), 3);

	// Increase size with values
	vec1.resize(15, 7);
	ASSERT_EQ(vec1.size(), 15);
	ASSERT_EQ(vec1[12], 7);

	// Decrease size with values, nothing should change besides size
	vec1.resize(8, 4);
	ASSERT_EQ(vec1.size(), 8);
	ASSERT_EQ(vec1[6], 7);
}

TEST(ShrinkToFitTest, Basic) {
	non_stl::vector<int> vec{ 5,6,7 };
	ASSERT_GT(vec.capacity(), 3);

	// Shrink down to 3
	vec.shrink_to_fit();
	ASSERT_EQ(vec.capacity(), 3);
}

TEST(ReserveTest, Basic) {
	non_stl::vector<int> vec1;
	ASSERT_EQ(vec1.capacity(), 10);

	// reserve more capacity
	vec1.reserve(72);
	ASSERT_GE(vec1.capacity(), 72);

	// asking to reserve less capacity, should ignore this
	vec1.reserve(24);
	ASSERT_GE(vec1.capacity(), 72);


}

TEST(AssignTest, Basic) {
	// Initializer list
	non_stl::vector<int> vec1 = { 0,1,2 };
	ASSERT_EQ(vec1.size(), 3);
	vec1.assign({ 6,7,8,9 });
	ASSERT_EQ(vec1.size(), 4);
	ASSERT_EQ(vec1[0], 6);
	ASSERT_EQ(vec1[1], 7);
	ASSERT_EQ(vec1[2], 8);
	ASSERT_EQ(vec1[3], 9);

	// Iterator
	non_stl::vector<int> vec2 = { 0,1,2 };
	ASSERT_EQ(vec2.size(), 3);
	vec2.assign(vec1.begin(), vec1.end());
	ASSERT_EQ(vec2.size(), 4);
	ASSERT_EQ(vec2[0], 6);
	ASSERT_EQ(vec2[1], 7);
	ASSERT_EQ(vec2[2], 8);
	ASSERT_EQ(vec2[3], 9);

	// Value
	non_stl::vector<int> vec3 = { 0,1,2 };
	ASSERT_EQ(vec3.size(), 3);
	vec3.assign(5, 3);
	ASSERT_EQ(vec3.size(), 5);
	ASSERT_EQ(vec3[0], 3);
	ASSERT_EQ(vec3[1], 3);
	ASSERT_EQ(vec3[2], 3);
	ASSERT_EQ(vec3[3], 3);
	ASSERT_EQ(vec3[4], 3);
}

TEST(PushBackTest, Basic) {
	// From default vector
	non_stl::vector<int> vec{};
	ASSERT_EQ(vec.size(), 0);

	// Push back once
	vec.push_back(2);
	ASSERT_EQ(vec.size(), 1);
	ASSERT_EQ(vec[0], 2);

	// Push back again
	vec.push_back(4);
	ASSERT_EQ(vec.size(), 2);
	ASSERT_EQ(vec[1], 4);

	// On non-default vector
	non_stl::vector<int> vec2{ 2,5,6 };
	ASSERT_EQ(vec2.size(), 3);

	vec2.push_back(9);
	ASSERT_EQ(vec2.size(), 4);
	ASSERT_EQ(vec2[0], 2);
	ASSERT_EQ(vec2[1], 5);
	ASSERT_EQ(vec2[2], 6);
	ASSERT_EQ(vec2[3], 9);

	// lvalue push back
	int x = 13;
	vec2.push_back(x);
	ASSERT_EQ(vec2.size(), 5);

	// Test capacity increase
	vec2.shrink_to_fit();
	vec2.push_back(15);
	ASSERT_EQ(vec2.size(), 6);
}

TEST(EmplaceBackTest, Basic) {
	// From default vector
	non_stl::vector<int> vec{};
	ASSERT_EQ(vec.size(), 0);

	// Push back once
	vec.emplace_back(2);
	ASSERT_EQ(vec.size(), 1);
	ASSERT_EQ(vec[0], 2);

	// Push back again
	vec.emplace_back(4);
	ASSERT_EQ(vec.size(), 2);
	ASSERT_EQ(vec[1], 4);

	// On non-default vector
	non_stl::vector<int> vec2{ 2,5,6 };
	ASSERT_EQ(vec2.size(), 3);

	vec2.emplace_back(9);
	ASSERT_EQ(vec2.size(), 4);
	ASSERT_EQ(vec2[0], 2);
	ASSERT_EQ(vec2[1], 5);
	ASSERT_EQ(vec2[2], 6);
	ASSERT_EQ(vec2[3], 9);

	// Test capacity increase
	vec2.shrink_to_fit();
	vec2.emplace_back(15);
	ASSERT_EQ(vec2.size(), 5);
}

TEST(InsertTest, Basic) {
	non_stl::vector<int> vec1{ 1,2,3 };
	ASSERT_EQ(vec1.size(), 3);
	ASSERT_EQ(vec1[0], 1);

	// RValue insert
	vec1.insert(vec1.begin(), 7);
	ASSERT_EQ(vec1.size(), 4);
	ASSERT_EQ(vec1[0], 7);

	// Test capacity increase
	vec1.shrink_to_fit();
	ASSERT_EQ(vec1.capacity(), 4);
	vec1.insert(vec1.begin(), 12);
	ASSERT_GT(vec1.capacity(), 4);

	// LValue insert
	int x = 19;
	auto it = vec1.begin();
	++it;
	vec1.insert(it, x);
	ASSERT_EQ(vec1.size(), 6);
	ASSERT_EQ(vec1[1], 19);

	// Test capacity increase
	x = 21;
	vec1.shrink_to_fit();
	ASSERT_EQ(vec1.capacity(), 6);
	vec1.insert(vec1.begin(), x);
	ASSERT_GT(vec1.capacity(), 6);

	// Iterator range insert
	non_stl::vector<int> vec2{ 0,1,2 };
	ASSERT_EQ(vec2.size(), 3);
	ASSERT_EQ(vec2[0], 0);

	auto it2 = vec1.begin();
	++it2;
	++it2;
	++it2;
	vec2.insert(vec2.begin(), vec1.begin(), it2);

	ASSERT_EQ(vec2.size(), 6);
	ASSERT_EQ(vec2[0], 21);
	ASSERT_EQ(vec2[1], 12);
	ASSERT_EQ(vec2[2], 19);
	ASSERT_EQ(vec2[3], 0);
	ASSERT_EQ(vec2[4], 1);
	ASSERT_EQ(vec2[5], 2);
}

TEST(SwapTest, Basic) {
	non_stl::vector<int> vec1{ 0,1,2,3,4 };
	non_stl::vector<int> vec2{ 5,6 };

	ASSERT_EQ(vec1.size(), 5);
	ASSERT_EQ(vec2.size(), 2);

	auto cap1 = vec1.capacity();
	auto cap2 = vec2.capacity();

	vec1.swap(vec2);
	ASSERT_EQ(vec1.size(), 2);
	ASSERT_EQ(vec2.size(), 5);
	ASSERT_EQ(vec1.capacity(), cap2);
	ASSERT_EQ(vec2.capacity(), cap1);
}

TEST(ClearTest, Basic) {
	non_stl::vector<int> vec = { 0,1,2 };
	ASSERT_EQ(vec.size(), 3);

	vec.clear();
	ASSERT_EQ(vec.size(), 0);
}

int main(int argc, char** argv) {
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
#include <gtest/gtest.h>

#include "../../containers/circular_buffer.h"
#include "../../containers/vector.h"

// Constructors

TEST(BasicConstruct, Basic) {
	non_stl::circular_buffer<int, 5> buffer;
}

TEST(CapacityTest, Basic) {
	non_stl::circular_buffer<int, 5> buffer;

	ASSERT_EQ(buffer.capacity(), 5);
	ASSERT_EQ(buffer.max_size(), 5);
}

TEST(EmptyTest, Basic) {
	non_stl::circular_buffer<int, 5> buffer;

	ASSERT_TRUE(buffer.empty());

	buffer.push_back(1);

	ASSERT_FALSE(buffer.empty());
}

TEST(SizeTest, Basic) {
	non_stl::circular_buffer<int, 5> buffer;

	ASSERT_EQ(buffer.size(), 0);

	buffer.push_back(1);

	ASSERT_EQ(buffer.size(), 1);

	buffer.push_back(1);
	buffer.push_back(1);
	buffer.push_back(1);
	buffer.push_back(1);
	buffer.push_back(1);
	buffer.push_back(1);
	buffer.push_back(1);
	buffer.push_back(1);

	// Make sure size never goes over capacity
	ASSERT_EQ(buffer.size(), buffer.capacity());
}

TEST(PushBackTest, Basic) {
	non_stl::circular_buffer<int, 5> buffer;

	// For the first element added head and tail should be the same
	buffer.push_back(1);
	ASSERT_EQ(buffer.front(), 1);
	ASSERT_EQ(buffer.back(), 1);

	// Until buffer size is reached tail should keep going up and head stays
	buffer.push_back(2);
	ASSERT_EQ(buffer.front(), 1);
	ASSERT_EQ(buffer.back(), 2);

	buffer.push_back(3);
	ASSERT_EQ(buffer.front(), 1);
	ASSERT_EQ(buffer.back(), 3);

	buffer.push_back(4);
	ASSERT_EQ(buffer.front(), 1);
	ASSERT_EQ(buffer.back(), 4);

	buffer.push_back(5);
	ASSERT_EQ(buffer.front(), 1);
	ASSERT_EQ(buffer.back(), 5);

	// Pushing more in than the buffer can hold, should overwrite the oldest number
	// Tail should still read the most recent value
	buffer.push_back(6);
	ASSERT_EQ(buffer.front(), 2);
	ASSERT_EQ(buffer.back(), 6);

	// Push back until head wraps around
	buffer.push_back(7);
	ASSERT_EQ(buffer.front(), 3);
	ASSERT_EQ(buffer.back(), 7);

	buffer.push_back(8);
	ASSERT_EQ(buffer.front(), 4);
	ASSERT_EQ(buffer.back(), 8);

	buffer.push_back(9);
	ASSERT_EQ(buffer.front(), 5);
	ASSERT_EQ(buffer.back(), 9);

	buffer.push_back(10);
	ASSERT_EQ(buffer.front(), 6);
	ASSERT_EQ(buffer.back(), 10);

	buffer.push_back(11);
	ASSERT_EQ(buffer.front(), 7);
	ASSERT_EQ(buffer.back(), 11);
}

TEST(ConstFrontBackTest, Basic) {
	non_stl::circular_buffer<int, 5> buffer;

	// For the first element added head and tail should be the same
	buffer.push_back(1);

	auto test = [](const non_stl::circular_buffer<int, 5> & buffer) {
		ASSERT_EQ(buffer.front(), 1);
		ASSERT_EQ(buffer.back(), 1);
	};
	
	test(buffer);
}

TEST(NonConstFrontBackTest, Basic) {
	non_stl::circular_buffer<int, 5> buffer;

	buffer.push_back(1);
	buffer.push_back(2);

	ASSERT_EQ(buffer.front(), 1);
	ASSERT_EQ(buffer.back(), 2);

	buffer.front() = 3;
	ASSERT_EQ(buffer.front(), 3);
	ASSERT_EQ(buffer.back(), 2);

	buffer.back() = 5;
	ASSERT_EQ(buffer.front(), 3);
	ASSERT_EQ(buffer.back(), 5);
}

TEST(EmplaceBackTest, Basic) {
	non_stl::circular_buffer<non_stl::vector<int>, 3> buffer;
	buffer.emplace_back(3,5);
	buffer.emplace_back(4,6);
	buffer.emplace_back(1,1);
	buffer.emplace_back(1,1);

	auto x = buffer.front();
	ASSERT_EQ(x[0], 6);
	ASSERT_EQ(x[1], 6);
}

TEST(PopFrontTest, Basic) {
	// Basic two element test
	non_stl::circular_buffer<int, 5> buffer;

	// For the first element added head and tail should be the same
	buffer.push_back(1);
	ASSERT_EQ(buffer.front(), 1);
	ASSERT_EQ(buffer.back(), 1);

	// Tail goes up and head stays the same
	buffer.push_back(2);
	ASSERT_EQ(buffer.front(), 1);
	ASSERT_EQ(buffer.back(), 2);

	// Pop front, head should go up to 2, tail stays
	buffer.pop_front();
	ASSERT_EQ(buffer.front(), 2);
	ASSERT_EQ(buffer.back(), 2);

	// Test pop_front after capacity has rolled over
	non_stl::circular_buffer<int, 3> buffer2;

	// For the first element added head and tail should be the same
	buffer2.push_back(1);
	ASSERT_EQ(buffer2.front(), 1);
	ASSERT_EQ(buffer2.back(), 1);

	// Until buffer size is reached tail should keep going up and head stays
	buffer2.push_back(2);
	ASSERT_EQ(buffer2.front(), 1);
	ASSERT_EQ(buffer2.back(), 2);

	buffer2.push_back(3);
	ASSERT_EQ(buffer2.front(), 1);
	ASSERT_EQ(buffer2.back(), 3);

	// Pushing more in than the buffer can hold, should overwrite the oldest number
	// Tail should still read the most recent value
	buffer2.push_back(6);
	ASSERT_EQ(buffer2.front(), 2);
	ASSERT_EQ(buffer2.back(), 6);

	buffer2.pop_front();
	ASSERT_EQ(buffer2.front(), 3);
	ASSERT_EQ(buffer2.back(), 6);

	buffer2.pop_front();
	ASSERT_EQ(buffer2.front(), 6);
	ASSERT_EQ(buffer2.back(), 6);
}

TEST(CopyCtorTest, Basic) {
	non_stl::circular_buffer<int, 5> buffer;

	buffer.push_back(1);
	buffer.push_back(2);

	ASSERT_EQ(buffer.front(), 1);
	ASSERT_EQ(buffer.back(), 2);

	buffer.front() = 3;
	ASSERT_EQ(buffer.front(), 3);
	ASSERT_EQ(buffer.back(), 2);

	buffer.back() = 5;
	ASSERT_EQ(buffer.front(), 3);
	ASSERT_EQ(buffer.back(), 5);

	non_stl::circular_buffer<int, 5> buffer2{ buffer };
	ASSERT_EQ(buffer2.front(), 3);
	ASSERT_EQ(buffer2.back(), 5);
}

TEST(MoveCtorTest, Basic) {
	non_stl::circular_buffer<int, 5> buffer;

	buffer.push_back(1);
	buffer.push_back(2);

	ASSERT_EQ(buffer.front(), 1);
	ASSERT_EQ(buffer.back(), 2);

	buffer.front() = 3;
	ASSERT_EQ(buffer.front(), 3);
	ASSERT_EQ(buffer.back(), 2);

	buffer.back() = 5;
	ASSERT_EQ(buffer.front(), 3);
	ASSERT_EQ(buffer.back(), 5);

	non_stl::circular_buffer<int, 5> buffer2{ std::move(buffer) };
	ASSERT_EQ(buffer2.front(), 3);
	ASSERT_EQ(buffer2.back(), 5);
}

TEST(OpEqTest, Basic) {
	non_stl::circular_buffer<int, 5> buffer;

	buffer.push_back(1);
	buffer.push_back(2);

	ASSERT_EQ(buffer.front(), 1);
	ASSERT_EQ(buffer.back(), 2);

	buffer.front() = 3;
	ASSERT_EQ(buffer.front(), 3);
	ASSERT_EQ(buffer.back(), 2);

	buffer.back() = 5;
	ASSERT_EQ(buffer.front(), 3);
	ASSERT_EQ(buffer.back(), 5);

	non_stl::circular_buffer<int, 5> buffer2;
	buffer2 = buffer;
	ASSERT_EQ(buffer2.front(), 3);
	ASSERT_EQ(buffer2.back(), 5);
}

TEST(OpEqRvTest, Basic) {
	non_stl::circular_buffer<int, 5> buffer;

	buffer.push_back(1);
	buffer.push_back(2);

	ASSERT_EQ(buffer.front(), 1);
	ASSERT_EQ(buffer.back(), 2);

	buffer.front() = 3;
	ASSERT_EQ(buffer.front(), 3);
	ASSERT_EQ(buffer.back(), 2);

	buffer.back() = 5;
	ASSERT_EQ(buffer.front(), 3);
	ASSERT_EQ(buffer.back(), 5);

	auto rvalueOpEq = [](non_stl::circular_buffer<int, 5>&& rhs) {
		non_stl::circular_buffer<int, 5> buffer;
		buffer = std::forward< non_stl::circular_buffer<int, 5> >(rhs);
		ASSERT_EQ(buffer.front(), 3);
		ASSERT_EQ(buffer.back(), 5);
	};

	rvalueOpEq(std::move(buffer));
}

TEST(AtTest, Basic) {
	non_stl::circular_buffer<int, 5> buffer;

	// For the first element added head and tail should be the same
	buffer.push_back(1);
	ASSERT_EQ(buffer.at(0), 1);

	// Until buffer size is reached tail should keep going up and head stays
	buffer.push_back(2);
	ASSERT_EQ(buffer.at(1), 2);

	buffer.push_back(3);
	ASSERT_EQ(buffer.at(2), 3);

	buffer.push_back(4);
	ASSERT_EQ(buffer.at(3), 4);

	buffer.push_back(5);
	ASSERT_EQ(buffer.at(4), 5);

	// Pushing more in than the buffer can hold, should overwrite the oldest number
	// Tail should still read the most recent value
	buffer.push_back(6);
	ASSERT_EQ(buffer.at(0), 2);
}

TEST(IteratorTest, Basic) {
	non_stl::circular_buffer<int, 5> buffer;
	buffer.push_back(1);
	buffer.push_back(2);
	buffer.push_back(3);

	auto it = buffer.begin();
	ASSERT_EQ(*it, 1);
	++it;
	ASSERT_EQ(*it, 2);
	++it;
	ASSERT_EQ(*it, 3);
	++it;

	// Out of elements, should equal end
	ASSERT_TRUE(it == buffer.end());

	// Make sure iterator doesn't wrap around
	ASSERT_NE(*it, 1);
	// Or stay the same value
	ASSERT_NE(*it, 3);
}

TEST(ReverseIteratorTest, Basic) {
	non_stl::circular_buffer<int, 5> buffer;
	buffer.push_back(1);
	buffer.push_back(2);
	buffer.push_back(3);

	auto it = buffer.rbegin();
	ASSERT_EQ(*it, 3);
	++it;
	ASSERT_EQ(*it, 2);
	++it;
	ASSERT_EQ(*it, 1);
	++it;

	// Out of elements, should equal end
	ASSERT_TRUE(it == buffer.rend());

	// Make sure iterator doesn't wrap around
	ASSERT_NE(*it, 3);
	// Or stay the same value
	ASSERT_NE(*it, 1);
}

int main(int argc, char** argv) {
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
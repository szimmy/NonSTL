#include <gtest/gtest.h>

TEST(EngineString, Basic) {
	const auto x = 1;
	ASSERT_EQ(x, 1);
}

int main(int argc, char** argv) {
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
#include<gtest/gtest.h>
#include<cp_tools/boost_utility.h>

using namespace cp_tools;

TEST(utility_split_string, comma_separated) {
	auto vec = split_str("hello,world,foo",",");
	ASSERT_EQ(vec[0], "hello");
	ASSERT_EQ(vec[1], "world");
	ASSERT_EQ(vec[2], "foo");
}

TEST(utility_split_string, many_separators) {
	auto vec = split_str("hello,world.fooabrrr",",a.");
	ASSERT_EQ(vec[0], "hello");
	ASSERT_EQ(vec[1], "world");
	ASSERT_EQ(vec[2], "foo");
	ASSERT_EQ(vec[3], "brrr");
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

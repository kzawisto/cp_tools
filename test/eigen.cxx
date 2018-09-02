
#include<cp_tools/eigen_utility.h>

#include<gtest/gtest.h>
#include<bits/stdc++.h>

TEST(eigen_utility, should_compute_inplace_cumsum) {
	Eigen::VectorXd vec(3);
	vec<<1.0,1.0,1.0;
	Eigen::VectorXd result_expected(3);
	result_expected<<1.0,2.0,3.0;
	cp_tools::cumsum_inplace(vec);
	ASSERT_EQ(vec, result_expected);
}
TEST(eigen_utility, should_compute_cumsum) {
	Eigen::VectorXd vec(3), result_expected(3), result_actual;
	vec<<1.0,1.0,1.0;
	result_expected<<1.0,2.0,3.0;
	result_actual = cp_tools::cumsum(vec);
	ASSERT_EQ(result_expected, result_actual);
}

TEST(eigen_utility, should_generate_random_normal) {
	Eigen::VectorXd vec = cp_tools::random_normal(10000, 1);
	ASSERT_NEAR(0, vec.mean(), 0.01);//<<vec.st();



}

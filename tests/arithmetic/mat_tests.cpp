#include <mstd/mat.hpp>
#include <gtest/gtest.h>

namespace mstd::test {
	TEST(ARITHMETIC_TESTS_MAT, DIMENSIONS_AND_STATIC_SIZES) {
		using Mat3x2 = mat<3, 2, float>;
		EXPECT_EQ(Mat3x2::columns, 3);
		EXPECT_EQ(Mat3x2::rows, 2);
		EXPECT_EQ(Mat3x2::size, 6);
	}

	TEST(ARITHMETIC_TESTS_MAT, DEFAULT_CONSTRUCTOR_IS_ZERO) {
		mat<2, 2, int> m;
		EXPECT_TRUE(m.is_zero());
		EXPECT_EQ(m[0u][0u], 0);
		EXPECT_EQ(m[1u][1u], 0);
	}

	TEST(ARITHMETIC_TESTS_MAT, INITIALIZER_CONSTRUCTOR) {
		mat<2, 2, int> m(1, 2, 3, 4);
		EXPECT_EQ(m[0u][0u], 1);
		EXPECT_EQ(m[0u][1u], 2);
		EXPECT_EQ(m[1u][0u], 3);
		EXPECT_EQ(m[1u][1u], 4);
	}

	TEST(ARITHMETIC_TESTS_MAT, COLUMN_AND_ELEMENT_ACCESS) {
		mat<3, 3, float> m = mat<3, 3, float>::identity();

		m[2u][0u] = 5.0f;
		EXPECT_EQ(m[2u][0u], 5.0f);

		mat<3, 3, float>::column_type col = { 10.f, 11.f, 12.f };
		m[1u] = col;
		EXPECT_EQ(m[1u][0u], 10.f);
		EXPECT_EQ(m[1u][1u], 11.f);
		EXPECT_EQ(m[1u][2u], 12.f);
	}

	TEST(ARITHMETIC_TESTS_MAT, SQUARE_MAT_SPECIAL) {
		auto identity = mat<4, 4, float>::identity();
		EXPECT_TRUE(identity.is_identity());
		EXPECT_FALSE(identity.is_zero());

		auto one = mat<3, 3, int>::one();
		EXPECT_TRUE(one.is_one());
	}

	TEST(ARITHMETIC_TESTS_MAT, TRANSPOSE) {
		mat<2, 3, int> m(
			1, 2, 3,
			4, 5, 6
		);

		auto mT = m.transposed();

		EXPECT_EQ(mT.columns, 3);
		EXPECT_EQ(mT.rows, 2);
		EXPECT_EQ(mT[0u][0u], 1);
		EXPECT_EQ(mT[0u][1u], 4);
		EXPECT_EQ(mT[2u][1u], 6);
	}

	TEST(ARITHMETIC_TESTS_MAT, SUB_MAT) {
		mat<3, 3, int> m(
			1, 2, 3,
			4, 5, 6,
			7, 8, 9
		);

		auto sub = m.get_sub_matrix(1, 1);

		EXPECT_EQ(sub[0u][0u], 1);
		EXPECT_EQ(sub[0u][1u], 3);
		EXPECT_EQ(sub[1u][0u], 7);
		EXPECT_EQ(sub[1u][1u], 9);
	}

	TEST(ARITHMETIC_TESTS_MAT, TRANSFORMATIONS_4x4) {
		float angle = 3.14159265f / 2.0f;
		auto rotZ = mat<4, 4, float>::rot_z(angle);

		EXPECT_NEAR(rotZ[0u][0u], 0.0f, 1e-6);
		EXPECT_NEAR(rotZ[0u][1u], 1.0f, 1e-6);

		auto trans = mat<4, 4, float>::translation({ 10.f, 20.f, 30.f });
		EXPECT_EQ(trans[3u][0u], 10.f);
		EXPECT_EQ(trans[3u][1u], 20.f);
		EXPECT_EQ(trans[3u][2u], 30.f);
	}

	TEST(ARITHMETIC_TESTS_MAT, CHECKS) {
		mat<2, 2, float> m = mat<2, 2, float>::zero();
		EXPECT_TRUE(m.is_zero());

		m[0u][0u] = 0.0001f;
		EXPECT_FALSE(m.is_zero());

		auto id = mat<2, 2, float>::fill_identity(5.0f);
		EXPECT_TRUE(id.is_identity_filled_with(5.0f));
	}

	TEST(ARITHMETIC_TESTS_MAT, CLAMP) {
		mat<2, 2, int> m(-10, 20, 5, 0);
		m.clamp(0, 10);

		EXPECT_EQ(m[0u][0u], 0);
		EXPECT_EQ(m[0u][1u], 10);
		EXPECT_EQ(m[1u][0u], 5);
		EXPECT_EQ(m[1u][1u], 0);
	}

	TEST(ARITHMETIC_TESTS_MAT, DETERMINANT_2x2) {
		mat<2, 2, float> m(2, 1,
			4, 6);
		EXPECT_FLOAT_EQ(m.determinant(), 8.0f);
	}

	TEST(ARITHMETIC_TESTS_MAT, DETERMINANT_3x3) {
		mat<3, 3, float> m(1, 2, 3,
			0, 1, 4,
			5, 6, 0);
		// det = 1*(0-24) - 2*(0-20) + 3*(0-5) = -24 + 40 - 15 = 1
		EXPECT_FLOAT_EQ(m.determinant(), 1.0f);
	}

	TEST(ARITHMETIC_TESTS_MAT, DETERMINANT_4x4) {
		mat<4, 4, float> m = mat<4, 4, float>::identity();
		m[0u][0u] = 2.0f;
		EXPECT_FLOAT_EQ(m.determinant(), 2.0f);
	}

	TEST(ARITHMETIC_TESTS_MAT, INVERT_2x2) {
		mat<2, 2, float> m(4, 7,
			2, 6);
		auto inv = m.inverted();
		auto res = m * inv;

		EXPECT_TRUE(res.is_identity());
	}

	TEST(ARITHMETIC_TESTS_MAT, INVERT_3x3) {
		mat<3, 3, float> m(1, 0, 5,
			2, 1, 6,
			3, 4, 0);
		auto inv = m.inverted();
		auto res = m * inv;

		for (size_t x = 0; x < 3; ++x) {
			for (size_t y = 0; y < 3; ++y) {
				EXPECT_NEAR(res[x][y], (x == y ? 1.0f : 0.0f), 1e-5);
			}
		}
	}

	TEST(ARITHMETIC_TESTS_MAT, MAT_MULTIPLICATION) {
		// [1 2] * [5 6] = [1*5+2*7 1*6+2*8] = [19 22]
		// [3 4]   [7 8]   [3*5+4*7 3*6+4*8]   [43 50]
		mat<2, 2, float> A(1, 3, 2, 4); // Col-major: [1,3], [2,4]
		mat<2, 2, float> B(5, 7, 6, 8); // Col-major: [5,7], [6,8]

		auto C = A * B;

		EXPECT_EQ(C[0u][0u], 19);
		EXPECT_EQ(C[1u][0u], 22);
		EXPECT_EQ(C[0u][1u], 43);
		EXPECT_EQ(C[1u][1u], 50);
	}

	TEST(ARITHMETIC_TESTS_MAT, SCALAR_MULTIPLICATION) {
		mat<2, 2, float> m(1, 2, 3, 4);
		auto res = m * 2.0f;
		EXPECT_EQ(res[0u][0u], 2.0f);
		EXPECT_EQ(res[1u][1u], 8.0f);
	}

	TEST(ARITHMETIC_TESTS_MAT, MAT_VEC_MULTIPLICATION) {
		mat<2, 2, float> m(1, 2, 3, 4);
		vec<2, float> v(10, 20);
		// [1 3] * [10] = [1*10 + 3*20] = [70]
		// [2 4]   [20]   [2*10 + 4*20]   [100]
		auto res = m * v;
		EXPECT_EQ(res[0u], 70.0f);
		EXPECT_EQ(res[1u], 100.0f);
	}

	TEST(ARITHMETIC_TESTS_MAT, COMPARE_OPERATORS) {
		mat<2, 2, float> m1(1, 2, 3, 4);
		mat<2, 2, float> m2(1, 2, 3, 4);
		mat<2, 2, float> m3(0, 2, 3, 4);

		EXPECT_TRUE(m1 == m2);
		EXPECT_FALSE(m1 == m3);
		EXPECT_TRUE(m1 != m3);
	}
}
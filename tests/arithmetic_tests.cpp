#include <mstd/arithmetic.hpp>
#include <gtest/gtest.h>

namespace mstd::test {
	TEST(ARITHMETIC_TESTS_BIT_OPERATIONS, ALL) {
		uint32_t i = 0x0000000Fu;
		uint32_t j = mstd::swap_endians(i);

		i &= 0x0Fu;
		j &= 0x0F000000u;
		j >>= 56u;

		ASSERT_EQ(i, j);
	}

	TEST(ARITHMETIC_TESTS_MATH_FUNCTIONS, ALL) {
		// signum
		ASSERT_EQ(mstd::signum(0), 0);
		ASSERT_EQ(mstd::signum(1), 1);
		ASSERT_EQ(mstd::signum(-1), -1);

		ASSERT_EQ(mstd::signum(0u), 0);
		ASSERT_EQ(mstd::signum(1u), 1);

		ASSERT_FLOAT_EQ(mstd::signum(0.f), 0.f);
		ASSERT_FLOAT_EQ(mstd::signum(1.f), 1.f);
		ASSERT_FLOAT_EQ(mstd::signum(-1.f), -1.f);

		// step
		ASSERT_EQ(mstd::step(1, 2), 1);
		ASSERT_EQ(mstd::step(1, 0), 0);

		// remap
		ASSERT_FLOAT_EQ(mstd::remap(0.5f, 0.f, 1.f, -1.f, 1.f), 0.f);

		// deg_to_rad
		ASSERT_DOUBLE_EQ(mstd::deg_to_rad(90.0), M_PI_2);

		// rad_to_deg
		ASSERT_DOUBLE_EQ(mstd::rad_to_deg(M_PI_2), 90.0);

		// epsilon_eq
		ASSERT_TRUE(mstd::epsilon_equal(90.f, 90.1f, 0.1f));

		// saturate
		ASSERT_EQ(mstd::saturate(2), 1);

		// fract
		ASSERT_EQ(mstd::fract(1), 0);
		ASSERT_FLOAT_EQ(mstd::fract(1.2f), 0.2f);

		// Q_rstqr
		ASSERT_FLOAT_EQ(mstd::Q_rsqrt(2.f), 0.70693f);
	}

	TEST(ARITHMETIC_TESTS_OVERFLOW_OPERATIONS, ALL) {
		uint8_t a = 2;
		uint8_t b = 3;
		uint8_t c = 0;

		// add
		ASSERT_FALSE(mstd::add_overflow(a, b, c));
		a = 255;
		ASSERT_TRUE(mstd::add_overflow(a, b, c));

		// mul
		b = 1;
		ASSERT_FALSE(mstd::mul_overflow(a, b, c));
		b = 2;
		ASSERT_TRUE(mstd::mul_overflow(a, b, c));

		// sub
		ASSERT_FALSE(mstd::sub_overflow(a, b, c));
		a = 0;
		ASSERT_TRUE(mstd::sub_overflow(a, b, c));

		// div
		int8_t ai = -128;
		int8_t bi = 1;
		int8_t ci = 0;
		ASSERT_FALSE(mstd::div_overflow(ai, bi, ci));
		bi = -1;
		ASSERT_TRUE(mstd::div_overflow(ai, bi, ci));
	}

	TEST(ARITHMETIC_TESTS_QUAT, ALL) {
		// default constructor
		mstd::quat q;
		ASSERT_FLOAT_EQ(q.s, 0.f);
		ASSERT_EQ(q.v, mstd::vec3());

		// scalar vector constructor
		q = mstd::quat(1.f, mstd::vec3(1.f));
		ASSERT_FLOAT_EQ(q.s, 1.f);
		ASSERT_EQ(q.v, mstd::vec3(1.f));

		// scalar x y z constructor
		q = mstd::quat(2.f, 2.f, 2.f, 2.f);
		ASSERT_FLOAT_EQ(q.s, 2.f);
		ASSERT_EQ(q.v, mstd::vec3(2.f, 2.f, 2.f));

		// copy constructor
		q = mstd::quat(mstd::quat());
		ASSERT_FLOAT_EQ(q.s, 0.f);
		ASSERT_EQ(q.v, mstd::vec3());

		// rotation quat
		q = mstd::fquat::rotation(mstd::vec3(1.f, 0.f, 0.f), static_cast<float>(M_PI_2));
		ASSERT_FLOAT_EQ(q.s, 0.70710683f);
		ASSERT_EQ(q.v, mstd::vec3(0.70710683f, 0.f, 0.f));

		// from euler angels
		q = mstd::fquat::from_euler_angels(mstd::vec3(90.f, 0.f, 0.f));
		ASSERT_FLOAT_EQ(q.s, 0.70710683f);
		ASSERT_EQ(q.v, mstd::vec3(0.70710683f, 0.f, 0.f));

		// from radians
		q = mstd::fquat::from_radians(mstd::vec3(M_PI_2, 0.f, 0.f));
		ASSERT_FLOAT_EQ(q.s, 0.70710683f);
		ASSERT_EQ(q.v, mstd::vec3(0.70710683f, 0.f, 0.f));

		// get magintude
		ASSERT_FLOAT_EQ(q.magnitude(), 1.f);

		// normalized
		q = q.normalized();
		ASSERT_FLOAT_EQ(q.s, 0.70710683f);
		ASSERT_EQ(q.v, mstd::vec3(0.70710683f, 0.f, 0.f));

		// conjugated
		q = q.conjugated();
		ASSERT_FLOAT_EQ(q.s, 0.70710683f);
		ASSERT_EQ(q.v, mstd::vec3(-0.70710683f, -0.f, -0.f));

		// inverted
		q = q.inverted();
		ASSERT_FLOAT_EQ(q.s, 0.70710683f);
		ASSERT_EQ(q.v, mstd::vec3(0.70710683f, 0.f, 0.f));

		// get radians
		mstd::vec3 v = q.to_radians();
		ASSERT_FLOAT_EQ(v.x(), static_cast<float>(M_PI_2));
		ASSERT_FLOAT_EQ(v.y(), 4.3711388e-08f);
		ASSERT_FLOAT_EQ(v.z(), 0.f);

		// get euler angels
		v = q.to_euler_angles();
		ASSERT_FLOAT_EQ(v.x(), 90.f);
		ASSERT_FLOAT_EQ(v.y(), 2.5044778e-06f);
		ASSERT_FLOAT_EQ(v.z(), 0.f);

		// get scalar
		ASSERT_FLOAT_EQ(q.scalar(q), 1.f);

		// quat + quat
		q = q + q;
		ASSERT_FLOAT_EQ(q.s, 1.41421366f);
		ASSERT_EQ(q.v, mstd::vec3(1.41421366f, 0.f, 0.f));

		// quat - quat
		q = q - q;
		ASSERT_FLOAT_EQ(q.s, 0.f);
		ASSERT_EQ(q.v, mstd::vec3(0.f, 0.f, 0.f));

		// quat * quat
		q = mstd::fquat::from_radians(mstd::vec3(M_PI_2, 0.f, 0.f));
		q = q * q;
		ASSERT_FLOAT_EQ(q.s, 0.f);
		ASSERT_FLOAT_EQ(q.v.x(), 1.f);
		ASSERT_FLOAT_EQ(q.v.y(), 0.f);
		ASSERT_FLOAT_EQ(q.v.z(), 0.f);

		// quat * vec
		q = q * mstd::vec3(M_PI_2, 0.f, 0.f);
		ASSERT_FLOAT_EQ(q.s, 0.f);
		ASSERT_FLOAT_EQ(q.v.x(), 1.570796f);
		ASSERT_FLOAT_EQ(q.v.y(), 0.f);
		ASSERT_FLOAT_EQ(q.v.z(), 0.f);

		// quat * scalar
		q = q * 2.f;
		ASSERT_FLOAT_EQ(q.s, 0.f);
		ASSERT_EQ(q.v, mstd::vec3(M_PI, 0.f, 0.f));

		// quat / quat
		q = q / q;
		ASSERT_FLOAT_EQ(q.s, 1.f);
		ASSERT_EQ(q.v, mstd::vec3(0.f, 0.f, 0.f));

		// quat / scalar
		q = q / 2.f;
		ASSERT_FLOAT_EQ(q.s, 0.5f);
		ASSERT_EQ(q.v, mstd::vec3(0.f, 0.f, 0.f));

		// -quat
		q = -q;
		ASSERT_FLOAT_EQ(q.s, -0.5f);
		ASSERT_EQ(q.v, mstd::vec3(-0.f, -0.f, -0.f));

		// +quat
		q = +q;
		ASSERT_FLOAT_EQ(q.s, -0.5f);
		ASSERT_EQ(q.v, mstd::vec3(-0.f, -0.f, -0.f));

		// --quat
		q = --q;
		ASSERT_FLOAT_EQ(q.s, -1.5f);
		ASSERT_EQ(q.v, mstd::vec3(-1.f, -1.f, -1.f));

		// ++quat
		q = ++q;
		ASSERT_FLOAT_EQ(q.s, -0.5f);
		ASSERT_EQ(q.v, mstd::vec3(0.f, 0.f, 0.f));

		// quat == quat
		ASSERT_TRUE(q == q);

		// quat != quat
		ASSERT_FALSE(q != q);

	}

	TEST(ARITHMETIC_TESTS_VEC, ALL) {
		// default constructor
		mstd::vec3 v3;
		ASSERT_FLOAT_EQ(v3.x(), 0.f);
		ASSERT_FLOAT_EQ(v3.y(), 0.f);
		ASSERT_FLOAT_EQ(v3.z(), 0.f);

		// values constructor
		v3 = mstd::vec3(1.f, 1.f, 1.f);
		ASSERT_FLOAT_EQ(v3.x(), 1.f);
		ASSERT_FLOAT_EQ(v3.y(), 1.f);
		ASSERT_FLOAT_EQ(v3.z(), 1.f);

		// other vec with values constructor
		v3 = mstd::vec3(mstd::vec2(2.f, 2.f), 2.f);
		ASSERT_FLOAT_EQ(v3.x(), 2.f);
		ASSERT_FLOAT_EQ(v3.y(), 2.f);
		ASSERT_FLOAT_EQ(v3.z(), 2.f);

		// table constructor
		v3 = mstd::vec3({ 3.f, 3.f, 3.f });
		ASSERT_FLOAT_EQ(v3.x(), 3.f);
		ASSERT_FLOAT_EQ(v3.y(), 3.f);
		ASSERT_FLOAT_EQ(v3.z(), 3.f);

		// pointer constructor
		float* values = new float[3] { 4.f, 4.f, 4.f };
		v3 = mstd::vec3(values, 3);
		ASSERT_FLOAT_EQ(v3.x(), 4.f);
		ASSERT_FLOAT_EQ(v3.y(), 4.f);
		ASSERT_FLOAT_EQ(v3.z(), 4.f);
		delete[] values;

		// cross vec constructor
		v3 = mstd::vec3(mstd::vec3(1.f, 0.f, 0.f), mstd::vec3(0.f, 0.f, 1.f));
		ASSERT_FLOAT_EQ(v3.x(), 0.f);
		ASSERT_FLOAT_EQ(v3.y(), -1.f);
		ASSERT_FLOAT_EQ(v3.z(), 0.f);

		// zero
		v3 = mstd::vec3::zero();
		ASSERT_FLOAT_EQ(v3.x(), 0.f);
		ASSERT_FLOAT_EQ(v3.y(), 0.f);
		ASSERT_FLOAT_EQ(v3.z(), 0.f);

		// one
		v3 = mstd::vec3::one();
		ASSERT_FLOAT_EQ(v3.x(), 1.f);
		ASSERT_FLOAT_EQ(v3.y(), 1.f);
		ASSERT_FLOAT_EQ(v3.z(), 1.f);

		// fill
		v3 = mstd::vec3::fill(2.f);
		ASSERT_FLOAT_EQ(v3.x(), 2.f);
		ASSERT_FLOAT_EQ(v3.y(), 2.f);
		ASSERT_FLOAT_EQ(v3.z(), 2.f);

		// is_zero
		ASSERT_FALSE(v3.is_zero());

		// is_one
		ASSERT_FALSE(v3.is_one());

		// is_filled
		ASSERT_TRUE(v3.is_filled_with(2.f));

		// is_normalized
		ASSERT_FALSE(v3.is_normalized());

		// x, r
		mstd::vec4 v4 = mstd::vec4(1.f, 2.f, 3.f, 4.f);
		ASSERT_FLOAT_EQ(v4.x(), 1.f);
		ASSERT_FLOAT_EQ(v4.r(), 1.f);

		// y, g
		ASSERT_FLOAT_EQ(v4.y(), 2.f);
		ASSERT_FLOAT_EQ(v4.g(), 2.f);

		// z, b
		ASSERT_FLOAT_EQ(v4.z(), 3.f);
		ASSERT_FLOAT_EQ(v4.b(), 3.f);

		// w, a
		ASSERT_FLOAT_EQ(v4.w(), 4.f);
		ASSERT_FLOAT_EQ(v4.a(), 4.f);

		// length
		ASSERT_FLOAT_EQ(v3.length(), 3.4641016f);

		// normalized
		v3 = v3.normalized();
		ASSERT_FLOAT_EQ(v3.x(), 0.57735026f);
		ASSERT_FLOAT_EQ(v3.y(), 0.57735026f);
		ASSERT_FLOAT_EQ(v3.z(), 0.57735026f);

		// dot
		ASSERT_FLOAT_EQ(v3.dot(v3), 0.99999994f);

		// angle_between
		ASSERT_FLOAT_EQ(v3.angle_between(mstd::vec3(1.f, 0.f, 0.f)), 0.95531654f);

		// reflected
		v3 = v3.reflected(mstd::vec3(0.f, 1.f, 0.f));
		ASSERT_FLOAT_EQ(v3.x(), 0.57735026f);
		ASSERT_FLOAT_EQ(v3.y(), -0.57735026f);
		ASSERT_FLOAT_EQ(v3.z(), 0.57735026f);

		// refracted
		v3 = v3.refracted(mstd::vec3(0.f, 1.f, 0.f), 20.f);
		ASSERT_FLOAT_EQ(v3.x(), 11.547005f);
		ASSERT_FLOAT_EQ(v3.y(), -16.299284f);
		ASSERT_FLOAT_EQ(v3.z(), 11.547005f);

		// saturated
		v3 = v3.saturated();
		ASSERT_FLOAT_EQ(v3.x(), 1.f);
		ASSERT_FLOAT_EQ(v3.y(), 0.f);
		ASSERT_FLOAT_EQ(v3.z(), 1.f);

		// fracted
		v3 = v3.fracted();
		ASSERT_FLOAT_EQ(v3.x(), 0.f);
		ASSERT_FLOAT_EQ(v3.y(), 0.f);
		ASSERT_FLOAT_EQ(v3.z(), 0.f);

		// modded scalar
		v3 = mstd::vec3(1.f, 2.f, 3.f);
		v3 = v3.modded(2.f);
		ASSERT_FLOAT_EQ(v3.x(), 1.f);
		ASSERT_FLOAT_EQ(v3.y(), 0.f);
		ASSERT_FLOAT_EQ(v3.z(), 1.f);

		// modded vec
		v3 = v3.modded(mstd::vec3(1.f, 2.f, 3.f));
		ASSERT_FLOAT_EQ(v3.x(), 0.f);
		ASSERT_FLOAT_EQ(v3.y(), 0.f);
		ASSERT_FLOAT_EQ(v3.z(), 1.f);

		// powwed scalar
		v3 = mstd::vec3(1.f, 2.f, 3.f);
		v3 = v3.powed(2.f);
		ASSERT_FLOAT_EQ(v3.x(), 1.f);
		ASSERT_FLOAT_EQ(v3.y(), 4.f);
		ASSERT_FLOAT_EQ(v3.z(), 9.f);

		// powwed vec
		v3 = v3.powed(mstd::vec3(1.f, 2.f, 1.f));
		ASSERT_FLOAT_EQ(v3.x(), 1.f);
		ASSERT_FLOAT_EQ(v3.y(), 16.f);
		ASSERT_FLOAT_EQ(v3.z(), 9.f);

		// clamped value
		v3 = v3.clampped(2.f, 10.f);
		ASSERT_FLOAT_EQ(v3.x(), 2.f);
		ASSERT_FLOAT_EQ(v3.y(), 10.f);
		ASSERT_FLOAT_EQ(v3.z(), 9.f);

		// clamped vec
		v3 = v3.clampped(mstd::vec3(3.f, 1.f, 10.f), mstd::vec3(4.f, 11.f, 12.f));
		ASSERT_FLOAT_EQ(v3.x(), 3.f);
		ASSERT_FLOAT_EQ(v3.y(), 10.f);
		ASSERT_FLOAT_EQ(v3.z(), 10.f);

		// stepped value
		v3 = v3.stepped(4.f);
		ASSERT_FLOAT_EQ(v3.x(), 0.f);
		ASSERT_FLOAT_EQ(v3.y(), 1.f);
		ASSERT_FLOAT_EQ(v3.z(), 1.f);

		// stepped vec
		v3 = mstd::vec3(1.f, 2.f, 3.f);
		v3 = v3.stepped(mstd::vec3(0.f, 3.f, 4.f));
		ASSERT_FLOAT_EQ(v3.x(), 1.f);
		ASSERT_FLOAT_EQ(v3.y(), 0.f);
		ASSERT_FLOAT_EQ(v3.z(), 0.f);

		// rotated
		v3 = v3.rotated(mstd::vec3(1.f, 0.f, 0.f), static_cast<float>(M_PI_2));
		ASSERT_FLOAT_EQ(v3.x(), 1.0000001f);
		ASSERT_FLOAT_EQ(v3.y(), 0.f);
		ASSERT_FLOAT_EQ(v3.z(), 0.f);

		// vec + vec
		v3 = v3 + v3;
		ASSERT_FLOAT_EQ(v3.x(), 2.0000002f);
		ASSERT_FLOAT_EQ(v3.y(), 0.f);
		ASSERT_FLOAT_EQ(v3.z(), 0.f);

		// vec + scalar
		v3 = v3 + 2.f;
		ASSERT_FLOAT_EQ(v3.x(), 4.0000002f);
		ASSERT_FLOAT_EQ(v3.y(), 2.f);
		ASSERT_FLOAT_EQ(v3.z(), 2.f);

		// vec - vec
		v3 = v3 - v3;
		ASSERT_FLOAT_EQ(v3.x(), 0.f);
		ASSERT_FLOAT_EQ(v3.y(), 0.f);
		ASSERT_FLOAT_EQ(v3.z(), 0.f);

		// vec - scalar
		v3 = v3 - 2.f;
		ASSERT_FLOAT_EQ(v3.x(), -2.f);
		ASSERT_FLOAT_EQ(v3.y(), -2.f);
		ASSERT_FLOAT_EQ(v3.z(), -2.f);

		// vec * vec
		v3 = v3 * v3;
		ASSERT_FLOAT_EQ(v3.x(), 4.f);
		ASSERT_FLOAT_EQ(v3.y(), 4.f);
		ASSERT_FLOAT_EQ(v3.z(), 4.f);

		// vec * scalar
		v3 = v3 * 2.f;
		ASSERT_FLOAT_EQ(v3.x(), 8.f);
		ASSERT_FLOAT_EQ(v3.y(), 8.f);
		ASSERT_FLOAT_EQ(v3.z(), 8.f);

		// vec / vec
		v3 = v3 / v3;
		ASSERT_FLOAT_EQ(v3.x(), 1.f);
		ASSERT_FLOAT_EQ(v3.y(), 1.f);
		ASSERT_FLOAT_EQ(v3.z(), 1.f);

		// vec / scalar
		v3 = v3 / 2.f;
		ASSERT_FLOAT_EQ(v3.x(), 0.5f);
		ASSERT_FLOAT_EQ(v3.y(), 0.5f);
		ASSERT_FLOAT_EQ(v3.z(), 0.5f);

		// +vec
		v3 = +v3;
		ASSERT_FLOAT_EQ(v3.x(), 0.5f);
		ASSERT_FLOAT_EQ(v3.y(), 0.5f);
		ASSERT_FLOAT_EQ(v3.z(), 0.5f);

		// -vec
		v3 = -v3;
		ASSERT_FLOAT_EQ(v3.x(), -0.5f);
		ASSERT_FLOAT_EQ(v3.y(), -0.5f);
		ASSERT_FLOAT_EQ(v3.z(), -0.5f);

		// ++vec
		v3 = ++v3;
		ASSERT_FLOAT_EQ(v3.x(), 0.5f);
		ASSERT_FLOAT_EQ(v3.y(), 0.5f);
		ASSERT_FLOAT_EQ(v3.z(), 0.5f);

		// --vec
		v3 = --v3;
		ASSERT_FLOAT_EQ(v3.x(), -0.5f);
		ASSERT_FLOAT_EQ(v3.y(), -0.5f);
		ASSERT_FLOAT_EQ(v3.z(), -0.5f);

		// vec == vec
		ASSERT_TRUE(v3 == v3);

		// vec != vec
		ASSERT_FALSE(v3 != v3);

		// resinterpret_cast<T*>(vec)
		//values = reinterpret_cast<float*>(&v3);
		//ASSERT_FLOAT_EQ(values[0], -0.5f);
		//ASSERT_FLOAT_EQ(values[1], -0.5f);
		//ASSERT_FLOAT_EQ(values[2], -0.5f);

		// []
		ASSERT_FLOAT_EQ(v3[0], -0.5f);
		ASSERT_FLOAT_EQ(v3[1], -0.5f);
		ASSERT_FLOAT_EQ(v3[2], -0.5f);
	}

	TEST(ARITHMETIC_TESTS_MAT, DIMENSIONS_AND_STATIC_SIZES) {
		using Mat3x2 = mat<3, 2, float>;
		EXPECT_EQ(Mat3x2::columns, 3);
		EXPECT_EQ(Mat3x2::rows, 2);
		EXPECT_EQ(Mat3x2::size, 6);
	}

	TEST(ARITHMETIC_TESTS_MAT, DEFAULT_CONSTRUCTOR_IS_ZERO) {
		mat<2, 2, int> m;
		EXPECT_TRUE(m.is_zero());
		EXPECT_EQ(m[0][0], 0);
		EXPECT_EQ(m[1][1], 0);
	}

	TEST(ARITHMETIC_TESTS_MAT, INITIALIZER_CONSTRUCTOR) {
		mat<2, 2, int> m(1, 2, 3, 4);
		EXPECT_EQ(m[0][0], 1);
		EXPECT_EQ(m[0][1], 2);
		EXPECT_EQ(m[1][0], 3);
		EXPECT_EQ(m[1][1], 4);
	}

	TEST(ARITHMETIC_TESTS_MAT, COLUMN_AND_ELEMENT_ACCESS) {
		mat<3, 3, float> m = mat<3, 3, float>::identity();

		m[2][0] = 5.0f;
		EXPECT_EQ(m[2][0], 5.0f);

		mat<3, 3, float>::column_type col = { 10.f, 11.f, 12.f };
		m[1] = col;
		EXPECT_EQ(m[1][0], 10.f);
		EXPECT_EQ(m[1][1], 11.f);
		EXPECT_EQ(m[1][2], 12.f);
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
		EXPECT_EQ(mT[0][0], 1);
		EXPECT_EQ(mT[0][1], 4);
		EXPECT_EQ(mT[2][1], 6);
	}

	TEST(ARITHMETIC_TESTS_MAT, SUB_MAT) {
		mat<3, 3, int> m(
			1, 2, 3,
			4, 5, 6,
			7, 8, 9
		);

		auto sub = m.get_sub_matrix(1, 1);

		EXPECT_EQ(sub[0][0], 1);
		EXPECT_EQ(sub[0][1], 3);
		EXPECT_EQ(sub[1][0], 7);
		EXPECT_EQ(sub[1][1], 9);
	}

	TEST(ARITHMETIC_TESTS_MAT, TRANSFORMATIONS_4x4) {
		float angle = 3.14159265f / 2.0f;
		auto rotZ = mat<4, 4, float>::rot_z(angle);

		EXPECT_NEAR(rotZ[0][0], 0.0f, 1e-6);
		EXPECT_NEAR(rotZ[0][1], 1.0f, 1e-6);

		auto trans = mat<4, 4, float>::translation({ 10.f, 20.f, 30.f });
		EXPECT_EQ(trans[3][0], 10.f);
		EXPECT_EQ(trans[3][1], 20.f);
		EXPECT_EQ(trans[3][2], 30.f);
	}

	TEST(ARITHMETIC_TESTS_MAT, CHECKS) {
		mat<2, 2, float> m = mat<2, 2, float>::zero();
		EXPECT_TRUE(m.is_zero());

		m[0][0] = 0.0001f;
		EXPECT_FALSE(m.is_zero());

		auto id = mat<2, 2, float>::fill_identity(5.0f);
		EXPECT_TRUE(id.is_identity_filled_with(5.0f));
	}

	TEST(ARITHMETIC_TESTS_MAT, CLAMP) {
		mat<2, 2, int> m(-10, 20, 5, 0);
		m.clamp(0, 10);

		EXPECT_EQ(m[0][0], 0);
		EXPECT_EQ(m[0][1], 10);
		EXPECT_EQ(m[1][0], 5);
		EXPECT_EQ(m[1][1], 0);
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
		m[0][0] = 2.0f; // Zmiana elementu na przek¹tnej
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

		EXPECT_EQ(C[0][0], 19);
		EXPECT_EQ(C[1][0], 22);
		EXPECT_EQ(C[0][1], 43);
		EXPECT_EQ(C[1][1], 50);
	}

	TEST(ARITHMETIC_TESTS_MAT, SCALAR_MULTIPLICATION) {
		mat<2, 2, float> m(1, 2, 3, 4);
		auto res = m * 2.0f;
		EXPECT_EQ(res[0][0], 2.0f);
		EXPECT_EQ(res[1][1], 8.0f);
	}

	TEST(ARITHMETIC_TESTS_MAT, MAT_VEC_MULTIPLICATION) {
		mat<2, 2, float> m(1, 2, 3, 4);
		vec<2, float> v(10, 20);
		// [1 3] * [10] = [1*10 + 3*20] = [70]
		// [2 4]   [20]   [2*10 + 4*20]   [100]
		auto res = m * v;
		EXPECT_EQ(res[0], 70.0f);
		EXPECT_EQ(res[1], 100.0f);
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
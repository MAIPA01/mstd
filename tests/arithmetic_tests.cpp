#include <mstd/arithmetic.hpp>
#include <gtest/gtest.h>

TEST(ARITHMETIC_TESTS, BIT_OPERATIONS) {
	uint32_t i = 0x0000000Fu;
	uint32_t j = mstd::swap_endians(i);

	ASSERT_TRUE((i & 0x0F) == ((j & 0x0F000000) >> 56));
}

TEST(ARITHMETIC_TESTS, MATH_FUNCTIONS) {
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

TEST(ARITHMETIC_TESTS, OVERFLOW_OPERATIONS) {
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

TEST(ARITHMETIC_TESTS, QUAT) {
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
	q = mstd::fquat::rotation(mstd::vec3(1.f, 0.f, 0.f), M_PI_2);
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
	ASSERT_FLOAT_EQ(v.x(), M_PI_2);
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

TEST(ARITHMETIC_TESTS, VEC) {
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
	v3 = v3.rotated(mstd::vec3(1.f, 0.f, 0.f), M_PI_2);
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

TEST(ARITHMETIC_TESTS, MAT) {

}
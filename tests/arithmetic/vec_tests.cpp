#include <gtest/gtest.h>
#include <mstd/vec.hpp>

namespace mstd::test {
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
		v3			  = mstd::vec3(values, 3);
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
		values = reinterpret_cast<float*>(&v3);
		ASSERT_FLOAT_EQ(values[0], -0.5f);
		ASSERT_FLOAT_EQ(values[1], -0.5f);
		ASSERT_FLOAT_EQ(values[2], -0.5f);

		// []
		ASSERT_FLOAT_EQ(v3[0u], -0.5f);
		ASSERT_FLOAT_EQ(v3[1u], -0.5f);
		ASSERT_FLOAT_EQ(v3[2u], -0.5f);
	}
} // namespace mstd::test
#include <gtest/gtest.h>
#include <mstd/quat.hpp>

namespace mstd::test {
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
} // namespace mstd::test
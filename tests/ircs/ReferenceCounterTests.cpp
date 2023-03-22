#include <gtest/gtest.h>
#include <ircs.hpp>

using namespace IRCS;

TEST(ReferencePointer, ShouldHaveReferenceCountOne)
{
	class Test : public ReferenceCounted { };

	auto test = make_ref<Test>();

	EXPECT_EQ(test->reference_count(), 1) << "Reference count was wrong with value: " << test->reference_count();
};

TEST(ReferencePointer, ShouldHaveReferenceCountTwo)
{
	class Test : public ReferenceCounted { };

	auto test = make_ref<Test>();

	auto k = test;

	EXPECT_EQ(test->reference_count(), 2);
};

TEST(ReferencePointer, ShouldHaveReferenceCountOneAfterOOS)
{
	struct Test : ReferenceCounted {
	};

	auto test = make_ref<Test>();
	{
		auto k = test;

		EXPECT_EQ(test->reference_count(), 2);
	}
	EXPECT_EQ(test->reference_count(), 1);
};

TEST(ReferencePointer, CanActuallyCallMethods)
{
	struct Test : ReferenceCounted {
		bool call() { return true; }
	};

	auto test = make_ref<Test>();
	auto called = test->call();
	EXPECT_TRUE(called);
}

TEST(ReferencePointer, DestructorIsCalled)
{
	static int* a = new int { 0 };
	{
		struct Test : ReferenceCounted {
			~Test() { *a = -1; }
		};

		auto test = make_ref<Test>();
	}

	EXPECT_EQ(*a, -1);
}

TEST(ReferencePointer, DestructorIsNotCalled)
{
	static int* a = new int { 0 };
	{
		struct Test : ReferenceCounted {
			~Test() { *a = -1; }
		};

		auto test = make_ref<Test>();
		EXPECT_EQ(*a, 0);
	}

	EXPECT_EQ(*a, -1);
}
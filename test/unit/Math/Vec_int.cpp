
#include <Crib/Math>

#include <typeinfo>
#include <new>
#include <doctest/doctest.h>

// using namespace Crib;


// test initializers!!!


TEST_CASE("int2-basic")
{
	// printf("%llu %llu\n", (uint64_t)&int2::x, (uint64_t)&int2::y);
	CHECK(sizeof(int2) == 8);
	CHECK(alignof(int2) == 4);
#if !defined(__GNUC__) || defined(__clang__)
	CHECK(typeid(int2::x) == typeid(int));
	CHECK(typeid(int2::y) == typeid(int));
	// CHECK(&int2::x == 0);
#else
	// CHECK_MESSAGE(false, "GCC is not supported");
#endif

	int2 a { 5, 7 };
	CHECK(a.x == 5);
	CHECK(a.y == 7);

	int buffer[2] = { 1, 2 };
	CHECK(buffer[0] == 1);
	CHECK(buffer[1] == 2);

	// make sure we are not zero initializing with default constructor
	// important for performance of big vector allocation
	auto f = new (buffer) int2;
	CHECK(f->x == 1);
	CHECK(f->y == 2);

	// make sure zero-initialize works when using {} initializer
	f = new (buffer) int2 {};
	CHECK(f->x == 0);
	CHECK(f->y == 0);

	// // this is a compiler warning for partial list
	// // should not be used
	// f = new (buffer) int2 { 1 };
	// CHECK(f->x == 1);
	// CHECK(f->y == 1);
}

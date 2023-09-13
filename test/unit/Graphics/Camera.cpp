

#include <Crib/Application.h>




TEST_CASE("Crib::Error")
{
	Crib::Error err(
		std::error_code(0, std::system_category()),
		"description",
		"syscall",
		CurrentSourceLocation);

	CHECK(err.code().value() == 0);
	CHECK(dynamic_cast<std::runtime_error*>(&err));
	CHECK(dynamic_cast<std::exception*>(&err));
}



#if defined(_WIN32)
#	include <Crib/Platform/Windows.h>
#	define NAMESPACE Crib::Platform::Windows
#	define FORMATTED_MESSAGE "Access is denied."
#	define ERROR_TYPE HRESULT
#	define LAST_ERROR GetLastError()
#else
#	include <Crib/Platform/POSIX.h>
#	include <sys/mman.h>
#	include <string.h>
#	define NAMESPACE Crib::Platform::POSIX
#	define FORMATTED_MESSAGE strerror(5)
#	define ERROR_TYPE int
#	define LAST_ERROR errno
#endif

#define Stringify_(x) #x
#define Stringify(x) Stringify_(x)

TEST_CASE(Stringify(NAMESPACE) "::Error")
{

	SUBCASE("1: return thread error code")
	{
		auto code = LAST_ERROR;

		auto ex = NAMESPACE::Error("action", "syscall", CurrentSourceLocation);

		// Make sure our error message extraction didn't produce error
		// (thus changing thread error code).
		CHECK(code == LAST_ERROR);

		CHECK(ex.code().value() == code);

		CHECK(dynamic_cast<Crib::Error*>(&ex));
		CHECK(dynamic_cast<std::runtime_error*>(&ex));
		CHECK(dynamic_cast<std::exception*>(&ex));
	}

	SUBCASE("2: make sure strings are copied correctly")
	{
		auto lasterr = LAST_ERROR;

		try
		{
			struct ClearsStringOnDestruct
			{
				~ClearsStringOnDestruct() { desc = "invalid"; }
				std::string desc = "original";
			} test;

			throw NAMESPACE::Error(
				(ERROR_TYPE)5,
				test.desc.c_str(),
				"syscall",
				CurrentSourceLocation);
		}
		catch (Crib::Error& ex)
		{
			// Make sure our error message extraction didn't produce error
			// (thus changing thread error code).
			CHECK(lasterr == LAST_ERROR);

			// make sure original valid description is used, not a destroyed memory
			// address
			auto expectedMessage = std::string("Error original: ") + FORMATTED_MESSAGE;
			auto actualMessage = std::string(ex.what());
			CHECK(actualMessage.substr(0, expectedMessage.size()) == expectedMessage);

			CHECK(ex.code().value() == (ERROR_TYPE)5);
			// CHECK(ex.code().message() == FORMATTED_MESSAGE);
		}
	}

	SUBCASE("3: use namespace")
	{
		using namespace NAMESPACE;

		// Do something that sets thread error code, so that an actual negative error code
		// is used on Windows instead of 0 "no error" code)
#if defined(_WIN32)
		CreateFileW(nullptr, 0, 0, nullptr, 0, 0, nullptr);
#else
		mmap(0, 0, 0, 0, 0, 0);
#endif

		auto code = LAST_ERROR;

		auto ex = Error("action", "syscall", CurrentSourceLocation);

		// Make sure our error message extraction didn't produce error
		// (thus changing thread error code).
		CHECK(code == LAST_ERROR);

		CHECK(ex.code().value() == code);
	}

#if defined(_WIN32)
	SUBCASE("4: ThrowIfFailed without namespace")
	{
		CHECK_THROWS(ThrowIfFailed("description", "syscall") E_INVALIDARG);
		CHECK_NOTHROW(ThrowIfFailed("description", "syscall") S_OK);
	}

	SUBCASE("5: ThrowIfFailed with namespace")
	{
		using namespace Crib::Platform::Windows;
		CHECK_THROWS(ThrowIfFailed("description", "syscall") E_INVALIDARG);
		CHECK_NOTHROW(ThrowIfFailed("description", "syscall") S_OK);
	}
#endif
}

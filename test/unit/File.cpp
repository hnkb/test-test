
#include <Crib/File.h>
#include <doctest/doctest.h>

using namespace Crib;
using namespace std;


// const std::filesystem::path existingFile = __FILE__;
//
//
// std::filesystem::path executableFilename()
//{
//
//}


TEST_CASE("Crib::Application::executablePath")
{
	CHECK_NOTHROW(Application::executablePath());

	CHECK(!Application::executablePath().empty());

	CHECK(filesystem::exists(Application::executablePath()));

	CHECK(!filesystem::exists(Application::executablePath().replace_extension("_invalid!")));
}


TEST_CASE("Crib::File::Stream")
{
	auto validFile = Application::executablePath();
	//		filesystem::weakly_canonical(filesystem::path(
	//#ifdef _WIN32
	//			__WFILE__
	//#else
	//			__FILE__
	//#endif
	//			));

	auto invalidFile = Application::executablePath().replace_extension("_invalid!");

	CHECK(filesystem::exists(validFile));
	CHECK(!filesystem::exists(invalidFile));

	CHECK_NOTHROW(File::Stream(validFile, "rb"));
	CHECK_THROWS(File::Stream(invalidFile, "rb"));

	{
		auto file = File::Stream(validFile, "rb");

		CHECK(file.size() == filesystem::file_size(validFile));
		CHECK(ftell(file) == 0);  // make sure file.size() has not moved seek position


		std::vector<int> vec(5);
		file >> vec;

		printf("\n\n %x %x %x %x %x \n", vec[0], vec[1], vec[2], vec[3], vec[4]);
	}

	{
		auto file = File::Stream(validFile.parent_path() / "test.txt", "w");
		std::vector<int> vec { 65, 66, 67, 68 };
		file << vec;
	}

	CHECK_NOTHROW(File::readAll<int>(validFile));
	CHECK(File::readAll<int>(validFile).size() == filesystem::file_size(validFile) / 4);

	string s = "The quick brown fox jumps over the lazy dog!\n";

	File::writeAll(vector<char>(s.begin(), s.end()), validFile.parent_path() / "test2.txt");
}

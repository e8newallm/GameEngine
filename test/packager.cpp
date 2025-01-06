#include <cstdlib>
#include <cstdint>
#include <unistd.h>
#include <vector>
#include <filesystem>

#include "tools/packager/packager.h"
#include "geerror.h"

#include "catch2/catch_test_macros.hpp"
#include "catch2/matchers/catch_matchers.hpp"
#include "catch2/matchers/catch_matchers_exception.hpp"

inline void conversionTest(uint64_t value) {
	std::vector<uint8_t> data = {};
	numToByte(data, value);
	REQUIRE(byteToNum(data) == value);
}

inline void fileEntryTest(const FileEntry lhs, const FileEntry rhs)
{
	REQUIRE(lhs.name == rhs.name);
	REQUIRE(lhs.start == rhs.start);
	REQUIRE(lhs.compressedLength == rhs.compressedLength);
	REQUIRE(lhs.length == rhs.length);
};

inline void headerTest(std::vector<FileEntry> data) {
	std::vector<FileEntry> result = headerDecompress(headerCompress(data));
	REQUIRE(result.size() == data.size());
	for(uint64_t i = 0; i < result.size(); i++) fileEntryTest(result[i], data[i]);

}

TEST_CASE("Packager/NumByteTest", "[packager]")
{
	conversionTest(0x1123456789ABCDEF);
	conversionTest(0);
	conversionTest(-1);
	conversionTest(1);

	srand(10);
	for (int i = 0; i < 1000; i++)
		conversionTest(((uint64_t)rand() << 32) + rand());
}

TEST_CASE("Packager/HeaderData", "[packager]")
{
	headerTest( {{"test", (uint64_t)-1, 4, 8}} );
	headerTest( {{"t\\est", (uint64_t)-1, 4, 8}, {"wsenbogw/eniop/fvcwed.gewrs", 17, 255, 97}, {"wsenerthg.gdf", 1700, 2525, 99678677}} );
	headerTest( std::vector<FileEntry>(200, {"", 0, 0, 0}));
	headerTest( std::vector<FileEntry>());
}

TEST_CASE("Packager/FullTest", "[packager]")
{
	std::vector<std::string> dirFiles = getFileList("packagerTest");
	dataCompress("testfiles", "packagerTest.bin");
	PackageManager testPackage = PackageManager("packagerTest.bin");
	std::vector<std::string> packageFiles = testPackage.getFileList();
	REQUIRE(packageFiles.size() == dirFiles.size());
	for(uint64_t i = 0; i < packageFiles.size(); i++)
	{
		REQUIRE(packageFiles[i] == dirFiles[i]);
	}

	for(uint64_t i = 0; i < packageFiles.size(); i++)
	{
		std::vector<uint8_t> fileReturn = testPackage.getFile(dirFiles[i]);
		FILE* originalFile = fopen(("testfiles/"+dirFiles[i]).c_str(), "rb");
		std::vector<uint8_t> originalData(std::filesystem::file_size("testfiles/"+dirFiles[i]));
		fread(originalData.data(), 1, std::filesystem::file_size("testfiles/"+dirFiles[i]), originalFile);
		fclose(originalFile);
		REQUIRE(fileReturn == originalData);
	}
}

TEST_CASE("Packager/Exceptions", "[packager][exceptions]")
{
	dataCompress("testfiles/", "test.file");
		PackageManager pack("test.file");

	SECTION("FILE_NOT_FOUND")
	{
		REQUIRE_THROWS_MATCHES(getFileList("/NOTADIR/"), GameEngineException,
			Catch::Matchers::Message("Could not get file list of /NOTADIR/: No such file or directory"));
		REQUIRE_THROWS_MATCHES(PackageManager("missingFile.bin"), GameEngineException,
			Catch::Matchers::Message("File missingFile.bin not found"));

		REQUIRE_THROWS_MATCHES(pack.getFile("FILEDOESNOTEXIST"), GameEngineException,
			Catch::Matchers::Message("File FILEDOESNOTEXIST not found in package test.file"));
	}

	SECTION("INVALID_FILE_FORMAT")
	{
		REQUIRE_THROWS_MATCHES(PackageManager("testfiles/packager/invalidHeaderSize.bin"), GameEngineException,
			Catch::Matchers::Message("Could not read header size of testfiles/packager/invalidHeaderSize.bin"));

		REQUIRE_THROWS_MATCHES(PackageManager("testfiles/packager/oversizedHeaderSize.bin"), GameEngineException,
			Catch::Matchers::Message("Unable to allocate 7378428322150573407 bytes for testfiles/packager/oversizedHeaderSize.bin"));

		REQUIRE_THROWS_MATCHES(PackageManager("testfiles/packager/invalidHeaderData.bin"), GameEngineException,
			Catch::Matchers::Message("Could not read entire header of testfiles/packager/invalidHeaderData.bin"));
	}
}
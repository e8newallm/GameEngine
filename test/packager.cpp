#include <cstdlib>
#include <cstdint>
#include <vector>
#include <iostream>

#include "tools/packager/packager.h"

#include "Catch2/src/catch2/catch_all.hpp"

inline void fileEntryTest(const FileEntry lhs, const FileEntry rhs)
{
	REQUIRE(lhs.name == rhs.name);
	REQUIRE(lhs.start == rhs.start);
	REQUIRE(lhs.compressedLength == rhs.compressedLength);
	REQUIRE(lhs.length == rhs.length);
};

inline void conversionTest(uint64_t value) {
	std::vector<uint8_t> data = {};
	numToByte(data, value);
	REQUIRE(byteToNum(data) == value);
}

inline void headerTest(std::vector<FileEntry> data) {
	std::vector<FileEntry> result = headerDecompress(headerCompress(data));
	REQUIRE(result.size() == data.size());
	for(int i = 0; i < result.size(); i++) fileEntryTest(result[i], data[i]);

}

TEST_CASE("Packager", "[packager]") {

	SECTION("Number to byte conversion")
	{
		conversionTest(0x1123456789ABCDEF);
		conversionTest(0);
		conversionTest(-1);
		conversionTest(1);

		srand(10);
		for (int i = 0; i < 1000; i++)
			conversionTest(((uint64_t)rand() << 32) + rand());
	}

	SECTION("Header data")
	{
		headerTest( {{"test", (uint64_t)-1, 4, 8}} );
		headerTest( {{"t\\est", (uint64_t)-1, 4, 8}, {"wsenbogw/eniop/fvcwed.gewrs", 17, 255, 97}, {"wsenerthg.gdf", 1700, 2525, 99678677}} );
		headerTest( std::vector<FileEntry>(200, {"", 0, 0, 0}));
		headerTest( std::vector<FileEntry>());
	}
}
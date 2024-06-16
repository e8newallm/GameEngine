#include <iterator>
#include <stdio.h>
#include <assert.h>
#include <cstdint>

#include <algorithm>
#include <string>
#include <filesystem>
#include <vector>
#include <iostream>

#include "zlib.h"
#include "packager.h"

#define CHUNK (256 * 1024)

std::vector<std::string> getFileList(std::string directory)
{
    std::vector<std::string> list;
    for (const auto & entry : std::filesystem::recursive_directory_iterator(directory))
    {
        if(!entry.is_directory())
        {
            std::string filename = entry.path();
            filename.erase(0, directory.length());
            list.push_back(filename);
        }
    }
    return list;
}

std::string fileCompress(std::string file)
{
    int ret, flush;
    unsigned have;
    z_stream strm;
    unsigned char in[CHUNK];
    unsigned char out[CHUNK];
    FILE* source = fopen(file.c_str(), "rb");
    std::string compressedData = "";

    /* allocate deflate state */
    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;
    ret = deflateInit(&strm, 9);
    if (ret != Z_OK)
    {}    //return ret;

    do
    {
        strm.avail_in = fread(in, 1, CHUNK, source);
        if (ferror(source))
        {
            (void)deflateEnd(&strm);
            //return Z_ERRNO;
        }
        flush = feof(source) ? Z_FINISH : Z_NO_FLUSH;
        strm.next_in = in;

        do
        {
            strm.avail_out = CHUNK;
            strm.next_out = out;
            ret = deflate(&strm, flush);    /* no bad return value */
            assert(ret != Z_STREAM_ERROR);  /* state not clobbered */
            have = CHUNK - strm.avail_out;
            compressedData += std::string((char*)&out, have);
        } while (strm.avail_out == 0);
    } while(flush != Z_FINISH);

    (void)deflateEnd(&strm);
    return compressedData;
}

void numToByte(std::vector<uint8_t>& data, uint64_t value)
{
    for(int i = 0; i <= 7; i++)
    {
        data.push_back(value & 0xff);
        value = value >> 8;
    }
}

uint64_t byteToNum(std::vector<uint8_t>& data)
{
    assert(data.size() >= 8);
    uint64_t result = 0;
    for(int i = 7; i >= 0; i--)
    {
        result *= 0x100;
        result += data.back();
        data.pop_back();
    }
    return result;
}

std::vector<uint8_t> headerCompress(std::vector<FileEntry> fileList)
{
    std::vector<uint8_t> headData;
    for(FileEntry data : fileList)
    {
        headData.push_back('\0');
        std::copy(data.name.begin(), data.name.end(), std::back_inserter(headData));
        numToByte(headData, data.start);
        numToByte(headData, data.compressedLength);
        numToByte(headData, data.length);
    }
    return headData;
}

std::vector<FileEntry> headerDecompress(std::vector<uint8_t> data)
{
    std::vector<FileEntry> fileList;
    while(data.size() > 0)
    {
        FileEntry newEntry;
        newEntry.length = byteToNum(data);
        newEntry.compressedLength = byteToNum(data);
        newEntry.start = byteToNum(data);
        for(std::vector<uint8_t>::iterator pos = data.end()-1; pos >= data.begin(); pos--)
        {
            if(*pos == '\0')
            {
                newEntry.name = std::string(pos+1, data.end());
                data.erase(pos, data.end());
                break;
            }
        }
        fileList.push_back(newEntry);
    }
    std::reverse(fileList.begin(), fileList.end());
    return fileList;
}

int dataCompress(std::string directory, std::string file)
{
    FILE* dest = std::tmpfile();

    std::vector<std::string> list = getFileList(directory);
    std::vector<FileEntry> dataHeader;
    long currentPosition = 0;
    for(std::string filename : list)
    {
        std::cout << "compressing " <<  directory + filename << "\r\n";
        std::string compressedData = fileCompress(directory + filename);
        FileEntry newData;
        newData.name = filename;
        newData.length = std::filesystem::file_size(directory + filename);
        newData.start = currentPosition;
        newData.compressedLength = compressedData.length();
        currentPosition += newData.compressedLength;
        fwrite(compressedData.c_str(), 1, compressedData.length(), dest);
        dataHeader.push_back(newData);
    }

    //fopen(file.c_str(), "wb");
    return 0;
}

std::string dataDecompress(std::string dataFile, std::string file)
{
    FILE* src = fopen(dataFile.c_str(), "rb");


    fclose(src);
    return 0;
}
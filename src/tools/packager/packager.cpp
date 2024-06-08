#include <stdio.h>
#include <assert.h>
#include <cstdint>

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

std::string stringCompress(std::string data)
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

    std::string headData = "";
    for(FileEntry data : dataHeader)
    {
        std::cout << data.name << " : length: " << data.length << " file position: "
                    << data.start << " compressed size: " << data.compressedLength << "\r\n";
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
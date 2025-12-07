#ifndef UTILS_FILE_H
#define UTILS_FILE_H

#include <vector>
#include <iostream>
#include <string>
#include <fstream>

#ifdef _WIN32
    #define NOMINMAX
    #include <Windows.h>
    #include <shobjidl.h>
#endif


#include <filesystem>
#include <algorithm>
#include <codecvt>
#ifdef __linux__
    #include <limits.h>
    #include <unistd.h>
#endif

#include "Utils/Text/Encoding/Wstring/Wstring.h"

bool isFileExist(const std::wstring& PathFile);
bool isFileExist(const std::string& PathFile);
bool isFileExist(const std::ifstream& Fstream_File);

bool isFileNotEmpty(std::ifstream& Fstream_File);

bool HaveFolder(const std::wstring& Path);
bool CreateFolder(const std::wstring& Path);


std::vector<uint8_t> ReadFileToByteArray(const std::string& filePath);

std::vector<std::string> ReadTextFromFile(const std::wstring& PathFile);
std::vector<std::string> ReadTextFromFile(const std::string& PathFile);


std::string GetPathApplication();
std::wstring GetPathDirectory();


#endif

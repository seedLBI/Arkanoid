#include "File.h"


bool isFileExist(const std::wstring& PathFile) {
    return std::filesystem::exists(PathFile);
}

bool isFileExist(const std::string& PathFile) {
#ifdef _DEBUG
    std::cout << "bool isFileExist(const std::string& PathFile)\n";
    std::cout << "PathFile = [" << PathFile << "]\n";
    std::cout << "absolute: PathFile = [" << std::filesystem::absolute(PathFile) << "]\n";
#endif
    return std::filesystem::exists(PathFile);
}

bool isFileExist(const std::ifstream& Fstream_File) {
    return Fstream_File.good();
}

bool isFileNotEmpty(std::ifstream& Fstream_File) {
    return !(Fstream_File.peek() == std::ifstream::traits_type::eof());
}

bool HaveFolder(const std::wstring& Path) {
    return std::filesystem::is_directory(Path);
}

bool CreateFolder(const std::wstring& Path){
    std::filesystem::path dir(Path);

    if (std::filesystem::create_directory(dir))
        return true;
    else
        return false;
}

std::vector<uint8_t> ReadFileToByteArray(const std::string& filePath) {
    std::ifstream file(filePath, std::ios::binary);
    if (!file.is_open()) {
        return {};
    }

    file.seekg(0, std::ios::end);
    std::streamsize fileSize = file.tellg();
    file.seekg(0, std::ios::beg);

    std::vector<uint8_t> byteArray(fileSize);

    if (!file.read(reinterpret_cast<char*>(byteArray.data()), fileSize)) {
        throw std::runtime_error(u8"ќшибка при чтении файла");
    }
    file.close();
    return byteArray;
}

std::vector<std::string> ReadTextFromFile(const std::string& PathFile) {
    return ReadTextFromFile(stringUTF8_to_wstring(PathFile));
}

std::vector<std::string> ReadTextFromFile(const std::wstring& PathFile) {
    std::ifstream ifn(wstring_to_stringUTF8(PathFile));

    std::vector<std::string> result;

    std::string line;

    while (std::getline(ifn, line)) {
        result.push_back(line);
    }

    return result;
}


std::string GetPathApplication() {
#ifdef _WIN32
    CHAR path[MAX_PATH];
    GetModuleFileNameA(NULL, path, MAX_PATH);
    return path;
#endif
#if defined(__linux__)
    char path[PATH_MAX];
    ssize_t count = readlink("/proc/self/exe", path, PATH_MAX);
    if (count == -1) {
        return ""; // ошибка
    }
    return std::string(path, count);
#endif
}

std::wstring GetPathDirectory() {
    return std::filesystem::current_path().wstring();
}


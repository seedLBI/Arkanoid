#include "Wstring.h"

#include "ThirdParty/utf8.h"

std::wstring stringUTF8_to_wstring(const std::string& str)
{
    std::wstring result;
    result.reserve(str.size()); // примерная резервация

    const char* src = str.c_str();
    const char* end = src + str.size();

    try {
        utf8::utf8to16(src, end, std::back_inserter(result));
    } catch (utf8::invalid_utf8& e) {
        throw std::runtime_error("Invalid UTF-8 sequence");
    }

    return result;
}

std::string wstring_to_stringUTF8(const std::wstring& str)
{
    std::string result;
    result.reserve(str.size() * 4); // wchar_t может быть до 4 байт в UTF-8

    const wchar_t* src = str.c_str();
    const wchar_t* end = src + str.size();

    try {
        utf8::utf16to8(src, end, std::back_inserter(result));
    } catch (utf8::invalid_utf16& e) {
        throw std::runtime_error("Invalid UTF-16 sequence");
    }

    return result;
}

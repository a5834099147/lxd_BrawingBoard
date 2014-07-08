
#include "Utility.h"
#include <Windows.h>
#include <io.h>
#include <boost/algorithm/string.hpp>
#include <boost/xpressive/xpressive.hpp>

///< UTF-8编码设置, 可以用来显示中文乱码问题, 前提是文档的编码格式为UTF-8
#pragma execution_character_set("utf-8")

String getExecuteDir()
{
    char szPath[256];
    if (!GetModuleFileNameA(NULL, szPath, 256))
    {
        assert(0);
    }

    String path(szPath);
    path = path.substr(0, path.rfind("\\")+1);
    return path;
}

bool isFileExist( const String& filePath )
{
    return (0 == _access_s(filePath.c_str(), 00));
}

void stringTrim( String& str )
{
    boost::trim(str);
}

std::vector<String> stringToken( const String& str, const String& regex )
{
    using namespace boost::xpressive;

    std::vector<String> strs;
    sregex reg = sregex::compile(regex);
    sregex_token_iterator pos(str.begin(), str.end(), reg, -1);
    sregex_token_iterator end;
    while (pos != end)
    {
        strs.push_back((*pos));
        pos++;
    }

    return strs;
}

std::vector<String> stringMatch( const String& str, const String& regex )
{
    using namespace boost::xpressive;

    std::vector<String> strs;
    sregex reg = sregex::compile(regex);
    sregex_iterator it(str.begin(), str.end(), reg);
    sregex_iterator end;
    while(it != end)
    {
        strs.push_back((*it)[0]);
        ++it;
    }

    return strs;
}

String stringReplace( String& str, const String& regex, const String& rep )
{
    using namespace boost::xpressive;

    sregex reg = sregex::compile(regex);
    return regex_replace(str, reg, rep);
}

void analysisStringArray( int *arr, int len, const char* buffer )
{
    size_t count = 0;
    const char* ptr = buffer;

    while (count < len) {
        if (*(ptr += strspn(ptr, "\x20\t\r\n")) == '\0')
            break;
        sscanf(ptr, "%d", arr + count);
        ptr += strcspn(ptr, "\x20\t\r\n");
        count++;
    }
}

void analysisStringArray( int *arr, int len, const String &str )
{
    analysisStringArray(arr, len, str.c_str());
}

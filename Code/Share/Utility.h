

#ifndef __Utility_H__
#define __Utility_H__

#include <string>
#include <vector>

typedef std::string String;

/// 获取当前运行目录
String getExecuteDir();

/// 判断文件是否存在
bool isFileExist(const String& filePath);

// 去除字符串开始或结尾的空格
void stringTrim(String& str);

/// 字符串分词
std::vector<String> stringToken(const String& str, const String& regex);

/// 字符串匹配
std::vector<String> stringMatch(const String& str, const String& regex);

/// 字符串替换
String stringReplace(String& str, const String& regex, const String& rep);

/// 将字符串转换成指定类型的数组
template <typename T>
void extractStringArray(T* arr, int len, const String &str)
{
    StringStream stream;
    stream << str;
    for (int i= 0; i<len; i++)
    {
        stream >> arr[i];
    }
}

template <typename T>
T stringToType(const String& str)
{
    StringStream  stream;
    T val;
    stream << str;
    stream >> val;
    return val;
}

template <typename T>
String typeToString(T val)
{
    StringStream  stream;
    stream << val;
    return stream.str();
}

#endif



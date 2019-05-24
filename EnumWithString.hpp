#pragma once
#include <vector>
#include <string>
#include <sstream>

/*
ã^éóenumÇToString, ToIndexÇ∆àÍèèÇ…íËã`Ç∑ÇÈ
*/
#define ENUM_WITH_STRING(EnumName, ...)          \
class EnumName                                   \
{                                                \
private:                                         \
    int enumIndex = 0;                           \
                                                 \
public:                                          \
    EnumName() = default;                        \
    EnumName(int index) : enumIndex(index){}     \
                                                 \
	enum                                         \
	{                                            \
		__VA_ARGS__                              \
	};                                           \
                                                 \
	static std::string ToString(int enumValue)   \
	{                                            \
        static std::vector<std::string> enumStr; \
        static bool initialize = false;          \
                                                 \
		if (!initialize)                         \
		{                                        \
            std::stringstream ss(#__VA_ARGS__);  \
            std::string buf;                     \
            bool isFirst = true;                 \
            while (std::getline(ss, buf, ','))   \
            {                                    \
                if (isFirst)                     \
                {                                \
	                isFirst = false;             \
                }                                \
                else                             \
                {                                \
	                buf.erase(buf.begin());      \
                }                                \
                enumStr.emplace_back(buf);       \
			}                                    \
			                                     \
			initialize = true;                   \
		}                                        \
                                                 \
        if (0 <= enumValue && enumValue < static_cast<int>(enumStr.size())) \
	        return enumStr[enumValue];           \
        else                                     \
            return "";                           \
	}                                            \
                                                 \
    static int ToIndex(std::string str)              \
    {                                                \
	    for (int i = 0;; ++i)                        \
	    {                                            \
		    std::string tempStr = ToString(i);       \
		    if (tempStr == str)                      \
			    return i;                            \
		    else if (tempStr == "")                  \
			    break;                               \
	    }                                            \
	    return -1;                                   \
    }                                                \
                                                 \
    operator int() const {return enumIndex;}     \
};                                               

/*
enum classÇToStringÇ∆àÍèèÇ…íËã`
*/
#define ENUM_CLASS_WITH_STRING(EnumName, ...)                                   \
enum class EnumName;                                                            \
namespace EnumName ## _Util                                                     \
{                                                                               \
	static std::string ToString(EnumName enumValue)                             \
	{                                                                           \
        static std::vector<std::string> enumStr;                                \
        static bool initialize = false;                                         \
                                                                                \
		if (!initialize)                                                        \
		{                                                                       \
            std::stringstream ss(#__VA_ARGS__);                                 \
            std::string buf;                                                    \
            bool isFirst = true;                                                \
            while (std::getline(ss, buf, ','))                                  \
            {                                                                   \
                if (isFirst)                                                    \
                {                                                               \
	                isFirst = false;                                            \
                }                                                               \
                else                                                            \
                {                                                               \
	                buf.erase(buf.begin());                                     \
                }                                                               \
                enumStr.emplace_back(buf);                                      \
			}                                                                   \
			                                                                    \
			initialize = true;                                                  \
		}                                                                       \
                                                                                \
        int enumIndex = static_cast<int>(enumValue);                            \
        if (0 <= enumIndex && enumIndex < static_cast<int>(enumStr.size()))     \
	        return enumStr[enumIndex];                                          \
        else                                                                    \
            return "";                                                          \
	}                                                                           \
                                                                                \
    static EnumName ToEnum(std::string str)                                     \
    {                                                                           \
	    for (int i = 0;; ++i)                                                   \
	    {                                                                       \
            EnumName tempEnum = static_cast<EnumName>(i);                       \
		    std::string tempStr = ToString(tempEnum);                           \
		    if (tempStr == str)                                                 \
			    return tempEnum;                                                \
		    else if (tempStr == "")                                             \
			    break;                                                          \
	    }                                                                       \
	    return static_cast<EnumName>(0);                                        \
    }                                                                           \
};                                                                              \
enum class EnumName { __VA_ARGS__ };                                                                                                            

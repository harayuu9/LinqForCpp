#pragma once
#include <string>

struct CData
{
    int id = 0;
    std::string name = "";
    float fValue = 0;
    std::size_t sizeValue = 0;

    bool operator==(const CData& other) const
    {
        return id == other.id;
    }
    bool operator!=(const CData& other) const
    {
        return !(*this == other);
    }
    bool operator>(const CData& other) const
    {
        return id > other.id;
    }
    bool operator>=(const CData& other) const
    {
        return id >= other.id;
    }
    bool operator<(const CData& other) const
    {
        return id < other.id;
    }
    bool operator<=(const CData& other) const
    {
        return id <= other.id;
    }
};

namespace std 
{
    template <>
    struct hash<CData> {
        [[nodiscard]] size_t operator()(const CData& val) const noexcept {
            return val.id;
        }
    };
}

extern std::vector<CData> gBigStruct;

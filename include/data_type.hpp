#ifndef DATATYPE_HH
#define DATATYPE_HH

#include <cstdint>
#include <optional>

enum class DataType : uint8_t{
    Int32,
    Float64,
    String,
    Bool,
    Undefined
};

constexpr std::optional<size_t> byte_size(DataType dtype){
    switch (dtype)
    {
    case DataType::Int32:
        return 4;

    case DataType::Float64:
        return 8;
    
    case DataType::Bool:
        return 1;

    case DataType::String:
        return std::nullopt;

    case DataType::Undefined:
        return std::nullopt;

    default:
        return std::nullopt;
    }


}


#endif //DATATYPE_HH
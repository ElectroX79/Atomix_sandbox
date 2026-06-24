#ifndef DATATYPE_HH
#define DATATYPE_HH

#include <cstdint>

enum class DataType : uint8_t{
    Int32,
    Float64,
    String,
    Bool
};

constexpr int byte_size(DataType dtype){
    switch (dtype)
    {
    case DataType::Int32:
        return 4;

    case DataType::Float64:
        return 8;
    
    case DataType::String:
        return -1;

    case DataType::Bool:
        return 1;
    
    default:
        return 0;
    }


}


#endif //DATATYPE_HH
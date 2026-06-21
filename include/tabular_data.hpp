#ifndef TABULARDATA_HPP
#define TABULARDATA_HPP

#include <vector>
#include <cstdint>

#include "data_type.hpp"

class TabularData{
private:

    struct meta_data{
        size_t offset; //Represent start of the element 0 of the column
        DataType data_type; //See data_type.hpp
    };
    
    std::vector<uint8_t> data_;
    std::vector<meta_data> data_info_; //meta data

public:


    





};


#endif TABULARDATA_HPP
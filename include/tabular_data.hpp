#ifndef TABULARDATA_HPP
#define TABULARDATA_HPP

#include <vector>
#include <cstdint>
#include <stdexcept>
#include <string.h>

#include "data_type.hpp"

class TabularData{
private:

    struct meta_data{
        size_t padding;
        size_t offset; //Represent start the column (in bytes) respect data_[0]
        size_t n_elements; 
        size_t element_size; //size of each element (in bytes)
        DataType data_type; //Enumeration of data types, more info see data_type.hpp
    };
    
    std::vector<uint8_t> data_;
    std::vector<meta_data> data_info_; //meta data, indicates n_columns (data_info_.size()), offset of the columns, and the types
    //std::vector<size_t> logic_to_real_; //for optimization reason (avoiding padding), the logical order may differ to physical order, that vector translates the logical index to logical index
    //std::vector<size_t> real_to_logic_;

//primitive methods


    uint8_t* get_point_column(size_t i_column, size_t column_offset = 0){
       return &data_[data_info_[i_column].offset + data_info_[i_column].element_size*column_offset];
    }

public:
    /* method deprecated/frozen
    TabularData extract(size_t begin, size_t end){
        if(begin>end){
            throw std::invalid_argument("The begin cannot be bigger than the end");
        }

        
        if(begin<0){
            throw std::out_of_range("Invalid container acces, begin<0");
        }
        

        if(end>=data_info_.size()){
            throw std::out_of_range("Invalid container acces, end>=size");
        }

        TabularData aux;

        aux.data_info_.assign(data_info_.begin() + begin, data_info_.begin() + end + 1);


        size_t acc = 0;
        for(int i=begin;i<=end;i++){
            acc+=(data_info_[i].n_elements*data_info_[i].element_size) + data_info_[i].padding ;
        }
        aux.data_.reserve(acc);

        size_t aux_offset = 0;
        for(int i=begin;i<=end;i++){
            size_t aux_acc = (data_info_[i].n_elements*data_info_[i].element_size) + data_info_[i].padding;
            aux.data_.resize(aux_offset)
            memcpy(aux.data_.data(), data_.data() + data_info_[i].offset, data_info_[i].offset + aux_acc);
            aux_offset += aux_acc;
        }

        

        

    }
    */

    size_t column_n_elements(size_t i_column){
        return  data_info_[i_column].n_elements;
    }


    





};


#endif 
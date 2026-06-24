#ifndef TABULARDATA_HPP
#define TABULARDATA_HPP

#include <vector>
#include <cstdint>
#include <stdexcept>
#include <string.h>
#include <string>

#include "data_type.hpp"

class TabularData{
private:

    //each column has specialized meta_data 
    struct meta_data{
        std::string name; 
        size_t padding; //Padding goes before the real data (in bytes)
        size_t offset; //Represent start the column (in bytes) respect data_[0]
        size_t n_elements;  // NO in bytes, to deduce size in bytes depends in element size
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

        

        

 */
    
    void append(const TabularData &t_data){

        auto it_aux = data_info_.end();
        size_t size_aux = data_.size();

        data_.insert(data_.end(), t_data.data_.begin(), t_data.data_.end());


        data_info_.insert(data_info_.end(), t_data.data_info_.begin(), t_data.data_info_.end());

        for(auto it = it_aux; it!=data_info_.end(); ++it){
            it->offset += size_aux;
        }
    }

    // its not [a,b) its [a,b] in mathematical notation
    void erase(const size_t begin, const size_t end){
        if(begin>end){
            throw std::invalid_argument("The begin cannot be bigger than the end");
        }

        /*
        if(begin<0){
            throw std::out_of_range("Invalid container acces, begin<0");
        }
        */
        

        if(end>=data_info_.size()){
            throw std::out_of_range("Invalid container acces, end>=size");
        }

    
        auto opt = byte_size(data_info_[end].data_type);
        if(!opt.has_value()){
            throw std::invalid_argument("the selected interval is not suitable to erase, because the byte_size of the element is undefined");
        }


        auto it_begin = data_.begin() + (data_info_[begin].offset - data_info_[begin].padding);
        auto it_end = data_.begin() + (data_info_[end].offset + (opt.value() * data_info_[end].n_elements));

        size_t diff = (it_end - it_begin);

        for(int i = end + 1; i < data_info_.size(); i++){
            data_info_[i].offset -= diff;
        }

        
        data_.erase(it_begin, it_end);

        data_info_.erase(data_info_.begin() + begin, data_info_.begin() + end + 1 );
    }



    size_t column_n_elements(size_t i_column)const{
        return  data_info_[i_column].n_elements;
    }




};


#endif 
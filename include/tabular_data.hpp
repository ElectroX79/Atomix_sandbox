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
    std::vector<std::string> data_string_; //an auxiliar buffer to storage strings, because his variable length nature. 
    //std::vector<size_t> logic_to_real_; //for optimization reason (avoiding padding), the logical order may differ to physical order, that vector translates the logical index to logical index
    //std::vector<size_t> real_to_logic_;

//primitive methods

    /**
     * @brief Returns the pointer of a raw element of a column
     * * @param i_column The column to evaluate
     * * @param column_offset The index of the column
     * * @note This methods works with BYTES 
     * * @warning Proceed carefully, wrong manipulation could cause serius misinterpretation with the data
     */
    uint8_t* get_point_column(size_t i_column, size_t column_offset = 0){
        auto opt = byte_size(data_info_[i_column].data_type);
        if(!opt.has_value()){
            throw std::invalid_argument("the selected interval is not suitable to erase, because the byte_size of the element is undefined");
        }

       return &data_[data_info_[i_column].offset + opt.value()*column_offset];
    }

public:

    /**
     * @brief Extract a partial tabular data defined by a interval
     * * @param begin The first index of the interval of columns to extract
     * * @param end The last index of the interval of columns to extract
     * * @note Unlike c++ standar, the end is not exclusive, in other words we consider [begin, end], not [begin, end)
     */
    TabularData extract(size_t begin, size_t end){
        if(begin>end){
            throw std::invalid_argument("The begin cannot be bigger than the end");
        }

        //Always begin<0, because it's size_t type
        
        
        if(end>=data_info_.size()){
            throw std::out_of_range("Invalid container acces, end>=size");
        }


        TabularData aux;
        

        
    }
        

    /**
    * @brief Appends the content of another TabularData instance to the current one.
    * * Merges the data buffers of 't_data' into the current instance. 
    * * @param t_data The source TabularData object to be copied and concatenated.
    * * @note Always at the end of the tabular data
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

    /**
     * @brief Erase an interval of columns of the current tabular data 
     * * @param begin The first index of the interval of columns to erase
     * * @param end The last index of the interval of columns to erase
     * * @note Unlike c++ standar, the end is not exclusive, in other words we consider [begin, end], not [begin, end)
     */
    void erase(const size_t begin, const size_t end){
        if(begin>end){
            throw std::invalid_argument("The begin cannot be bigger than the end");
        }

        //Always begin<0, because it's size_t type
        
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


    /**
     * @brief Returns the numbers of elements contained in a column
     * * @param i_column The column to evaluate
     */
    size_t column_n_elements(size_t i_column)const{
        return  data_info_[i_column].n_elements;
    }




};


#endif 
#ifndef TABULARDATA_HPP
#define TABULARDATA_HPP

#include <vector>
#include <cstdint>
#include <iostream>
#include <stdexcept>
#include <string>

#include "data_type.hpp"
#include "security_check.hpp"


class TabularData{

public: 
    struct meta_data{
        std::string name; 
        size_t padding; //Padding goes before the real data (in bytes)
        size_t offset; //Represent start the column (in bytes) respect data_[0]
        size_t n_elements;  // NO in bytes, to deduce size in bytes depends on element size
        DataType data_type; //Enumeration of data types, more info see data_type.hpp

        bool operator==(const meta_data& other) const {
            return (name == other.name) && (padding == other.padding) && (offset == other.offset) && (n_elements == other.n_elements) && (data_type == other.data_type);
        }
    };


private:
    //each column has specialized meta_data 
   
    
    std::vector<uint8_t> data_;
    std::vector<meta_data> data_info_; //metadata, indicates n_columns (data_info_.size()), offset of the columns, and the types
    std::vector<std::string> data_string_; //an auxiliar buffer to storage strings, because of his variable length nature.
    //std::vector<size_t> logic_to_real_; //for optimization reason (avoiding padding), the logical order may differ to physical order, that vector translates the logical index to logical index
    //std::vector<size_t> real_to_logic_;

//primitive methods


    /**
     * @brief Returns the pointer of a raw element of a column
     * * @param i_column The column to evaluate
     * * @param column_offset The index of the column
     * * @note This method works with BYTES
     * * @warning Proceed carefully, wrong manipulation could cause serious misinterpretation with the data
     * * @warning Proceed carefully, wrong manipulation could cause serius misinterpretation with the data

     */
    uint8_t* get_point_column(size_t i_column, size_t column_offset = 0);



     /**
     * @brief Updates the offset of each element of a column of type DataType::String
     * * @param i The index of the column to update
     * * @param stride The value to add to the offset of each string in the column
     * * @note This method is only compatible with columns of type DataType::String.
     */
    void update_string_offset(size_t i, const int stride);

    /** @brief Returns the offset of the last element in a column
     * @param i The index of the column
     * @return The offset of the last element in the column
     * @note This method is only compatible with columns with defined byte sizes. 
     */
    size_t last_offset(size_t i)const;



public:

    explicit TabularData(){}
    explicit TabularData(size_t data_size){
        data_.reserve(data_size);
    }


    /**
     * @brief Extract a copy of partial tabular data defined by an interval
     * * @param begin The first index of the interval of columns to extract
     * * @param end The last index of the interval of columns to extract
     * * @note Unlike c++ standard, the end is not exclusive, in other words we consider [begin, end], not [begin, end)
     */
    TabularData extract(const size_t begin, const size_t end);


    /**
    * @brief Appends the content of another TabularData instance to the current one.
    * * Merges the data buffers of 't_data' into the current instance. 
    * * @param t_data The source TabularData object to be copied and concatenated.
    * * @note Always appends at the end of the tabular data
    * * @note Always at the end of the tabular data
    */
    void append(const TabularData &t_data);

    /**
     * @brief Erase an interval of columns of the current tabular data 
     * * @param begin The first index of the interval of columns to erase
     * * @param end The last index of the interval of columns to erase
     * * @note Unlike c++ standar, the end is not exclusive, in other words consider [begin, end], not [begin, end)
     */
    void erase(const size_t begin, const size_t end);


    /**
     * @brief Returns the numbers of elements contained in a column
     * * @param i_column The column to evaluate
     */
    inline size_t column_n_elements(size_t i_column)const{
        return  data_info_[i_column].n_elements;
    }


     /**
     * @brief Returns whether the tabular data is empty
     */
    inline bool empty()const{
        return data_.empty();
    }


    bool operator==(const TabularData& td)const{
        return (td.data_ == data_) && (td.data_info_ == data_info_) && (td.data_string_ == data_string_);
    }



};


#endif 
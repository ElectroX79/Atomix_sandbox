#ifndef TABULARDATA_HPP
#define TABULARDATA_HPP

#include <vector>
#include <cstdint>
#include <stdexcept>
#include <string>

#include "data_type.hpp"

template <typename T>

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
     * * @warning Proceed carefully, wrong manipulation could cause serious misinterpretation with the data
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
     * @brief Updates the offset of each element of a column of type DataType::String
     * * @param i The index of the column to update
     * * @param stride The value to add to the offset of each string in the column
     * * @note This method is only compatible with columns of type DataType::String.
     */
    void update_string_offset(size_t i, const int stride){
        Bound_check::check_index(i, i, data_info_.size());
        if (data_info_[i].data_type != DataType::String){
            throw std::invalid_argument("the operation is only compatible with DataType::String ");
        }
        auto opt = byte_size(DataType::String);
        if(!opt.has_value()) {
            throw std::runtime_error("byte_size do not admit DataType::String, process shut off");
        }
        for (size_t n = data_info_[i].offset; n < data_info_[i].offset + (data_info_[i].n_elements * opt.value()); n += opt.value()) {
            uint64_t* cast_pointer = reinterpret_cast<uint64_t*>(&data_[n]);
            (*cast_pointer) += stride;
            
        }

    }

    /** @brief Returns the offset of the last element in a column
     * @param i The index of the column
     * @return The offset of the last element in the column
     * @note This method is only compatible with columns with defined byte sizes. 
     */
    size_t last_offset(size_t i)const{
        Bound_check::check_index(i, i, data_info_.size());
        auto opt = byte_size(data_info_[i].data_type);
        if(!opt.has_value()){
            throw std::invalid_argument("Cannot calculate last offset for column with undefined byte size");
        }
        return data_info_[i].offset + data_info_[i].n_elements * opt.value();   
    }





public:

    explicit TabularData(){}
    explicit TabularData(size_t size):{
        data_.reserve(size);
    }

    /**
     * @brief Extract a copy of partial tabular data defined by a interval
     * * @param begin The first index of the interval of columns to extract
     * * @param end The last index of the interval of columns to extract
     * * @note Unlike c++ standar, the end is not exclusive, in other words we consider [begin, end], not [begin, end)
     */
    TabularData extract(size_t begin, size_t end){

        Bound_check::check_index(begin, end, data_info_.size());

        TabularData aux;

        aux.data_info_.assign(data_info_.begin() + begin, data_info_.begin() + end + 1);
        aux.data_.insert(aux.data_.end(), data_.begin() + data_info_[begin].offset, data_.begin() + last_offset(end));
        aux.data_info_.insert(aux.data_info_.end(), data_info_.begin() + begin, data_info_.begin() + end + 1);

        size_t stride = 0;
        size_t string_stride = 0;
        size_t string_data_begin = 0;
        size_t string_data_end = 0;
        bool string_data_begin_set = false;
    
        
        auto opt = byte_size(data_info_[begin - 1].data_type);
        if(!opt.has_value()){
            throw std::invalid_argument("the selected interval is not suitable to erase, because the byte_size of the final element is undefined, causing imposibility to calculate the size of the interval in bytes");
        }
        stride = last_offset(begin - 1);

        auto opt2 = byte_size(DataType::String);
        if(!opt2.has_value()) {
            throw std::runtime_error("byte_size do not admit DataType::String, causing impossibility to calculate the size of the interval in bytes");
        }
        for(size_t i = 0; i < begin; ++i){
            if(data_info_[i].data_type == DataType::String){
                string_stride += data_info_[i].n_elements;
            }
        }

        for(size_t i = begin; i <= end; ++i){
            if(data_info_[i].data_type == DataType::String){
                update_string_offset(i, -string_stride * opt2.value());
                if(!string_data_begin_set){
                    string_data_begin = (*reinterpret_cast<uint64_t*>(&data_[data_info_[i].offset]));
                    string_data_begin_set = true;
                }
                string_data_end = (*reinterpret_cast<uint64_t*>(&data_[data_info_[i].offset + (data_info_[i].n_elements * opt2.value())]));

            }
            aux.data_info_[i].offset -= stride;
        }

        if(string_data_begin_set){
            aux.data_string_.insert(aux.data_string_.end(), data_string_.begin() + string_data_begin, data_string_.begin() + string_data_end);
        }

    
    

    }


        


    /**
    * @brief Appends the content of another TabularData instance to the current one.
    * * Merges the data buffers of 't_data' into the current instance. 
    * * @param t_data The source TabularData object to be copied and concatenated.
    * * @note Always appends at the end of the tabular data
    */
    void append(const TabularData &t_data){

        auto opt = byte_size(DataType::String);
        if(!opt.has_value()) {
            throw std::runtime_error("byte_size do not admit DataType::String");
        }

        auto it_aux = data_info_.end();
        size_t size_aux = data_.size();
        size_t str_offset = data_string_.size();

        data_.insert(data_.end(), t_data.data_.begin(), t_data.data_.end());

        data_info_.insert(data_info_.end(), t_data.data_info_.begin(), t_data.data_info_.end());






        data_string_.insert(data_string_.end(), t_data.data_string_.begin(), t_data.data_string_.end());

        for(auto it = it_aux; it!=data_info_.end(); ++it){
            
            if(it->data_type == DataType::String){
               update_string_offset(std::distance(data_info_.begin(), it), str_offset);
            }
            else it->offset += size_aux;
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

    /**
     * @brief Erase an interval of columns of the current tabular data 
     * * @param begin The first index of the interval of columns to erase
     * * @param end The last index of the interval of columns to erase
     * * @note Unlike c++ standar, the end is not exclusive, in other words consider [begin, end], not [begin, end)
     */
    void erase(const size_t begin, const size_t end){


        Bound_check::check_index(begin, end, data_info_.size());
    
        auto opt = byte_size(data_info_[end].data_type);
        if(!opt.has_value()){
            throw std::invalid_argument("The selected interval is not suitable to erase, because the byte_size of the final element is undefined, causing imposibility to calculate the size of the interval in bytes");
        }

        auto opt2 = byte_size(DataType::String);
        if(!opt2.has_value()) {
            throw std::runtime_error("byte_size do not admit DataType::String");
        }



        int acc_stride = 0;
        for(size_t i = begin; i <= end; ++i){
            
            if(data_info_[i].data_type == DataType::String){
                acc_stride += data_info_[i].n_elements;
    
                auto start = reinterpret_cast<uint64_t*>(&data_[data_info_[i].offset]);
                auto end = reinterpret_cast<uint64_t*>(&data_[data_info_[i].offset + (data_info_[i].n_elements * opt2.value())]);
                data_string_.erase(data_string_.begin() + *start, data_string_.begin() + *end);
            }
        
        }
        acc_stride *= opt2.value();
       

        for(size_t i = end + 1; i < data_info_.size(); ++i){
            
            if(data_info_[i].data_type == DataType::String){
                for(int j = data_info_[i].offset; j < data_info_[i].offset + (data_info_[i].n_elements * opt2.value()); j+=opt2.value()){
                    uint64_t* cast_pointer = reinterpret_cast<uint64_t*>(&data_[j]);
                    (*cast_pointer) -= acc_stride;
                }
            }
            data_info_[i].offset -= (data_info_[end].n_elements * opt2.value() + data_info_[end].offset) - data_info_[begin].offset;
        }

        data_.erase(data_.begin() + data_info_[begin].offset, data_.begin() + data_info_[end].offset + (data_info_[end].n_elements * opt.value()));
        data_info_.erase(data_info_.begin() + begin, data_info_.begin() + end + 1);

        int string_stride = 0;
        for(size_t i = begin; i <= end; ++i){
            
            if(data_info_[i].data_type == DataType::String){
                string_stride += data_info_[i].n_elements;

                auto start = reinterpret_cast<uint64_t*>(&data_[data_info_[i].offset]);
                auto end = reinterpret_cast<uint64_t*>(&data_[data_info_[i].offset + (data_info_[i].n_elements * opt2.value())]);
                data_string_.erase(data_string_.begin() + *start, data_string_.begin() + *end);
            }
        
        }
        string_stride *= opt2.value();
    

        for(size_t i = end + 1; i < data_info_.size(); ++i){
            
            if(data_info_[i].data_type == DataType::String){
                update_string_offset(i, -string_stride);
            }
            data_info_[i].offset -= (data_info_[end].n_elements * opt2.value() + data_info_[end].offset) - data_info_[begin].offset;
        }

        data_.erase(data_.begin() + data_info_[begin].offset, data_.begin() + data_info_[end].offset + (data_info_[end].n_elements * opt.value()));
        data_info_.erase(data_info_.begin() + begin, data_info_.begin() + end + 1);

    }



    size_t column_n_elements(size_t i_column)const{
        return  data_info_[i_column].n_elements;
    }




};


#endif 
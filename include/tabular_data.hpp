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



    struct Column {
        size_t padding_bytes;
        DataType type;
        size_t n_elements;
        size_t offset;
    };

    uint8_t* data_;
    std::vector<Column> columns_;
    size_t total_size_;
    size_t str_set_offset_;
    size_t str_array_size_; //in bytes

    explicit TabularData(
        uint8_t* data,
        std::vector<Column>&& columns,
        const size_t total_size,
        const size_t str_set_offset,
        const size_t str_array_size ):

        data_(data),
        columns_(columns),
        total_size_(total_size),
        str_set_offset_(str_set_offset),
        str_array_size_(str_array_size)
    {} //pending for the rest of implementation



    explicit TabularData(TabularData&& other)noexcept:
        data_(other.data_),
        columns_(std::move(other.columns_)),
        total_size_(other.total_size_),
        str_set_offset_(other.str_set_offset_),
        str_array_size_(other.str_array_size_){

        other.data_ = nullptr;
        other.columns_.clear();
        other.total_size_ = 0;
        other.str_set_offset_ = 0;
        other.str_array_size_ = 0;

    }
    TabularData& operator=(TabularData&& other)noexcept {
        if (this != &other) {
            data_ = other.data_;
            columns_ = std::move(other.columns_);
            total_size_ = other.total_size_;
            str_set_offset_ = other.str_set_offset_;
            str_array_size_ = other.str_array_size_;

            other.data_ = nullptr;
            other.columns_.clear();
            other.total_size_ = 0;
            other.str_set_offset_ = 0;
            other.str_array_size_ = 0;
        }
        return *this;
    }


public:

    friend class Engine;

    explicit TabularData(const TabularData&) = delete;
    TabularData& operator=(const TabularData&) = delete;

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






};


#endif 
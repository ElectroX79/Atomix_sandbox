#ifndef TABULARDATA_HPP
#define TABULARDATA_HPP

#include <vector>
#include <stdexcept>
#include <string_view>


#include "data_type.hpp"
#include "buffer.hpp"
#include "security_check.hpp"


class TabularData{


    struct Column {
        size_t padding_bytes;
        DataType type;
        size_t n_elements;
        size_t offset;
    };

    // Main dense buffer containing primitive values or offsets into string_data_.
    Buffer data_;

    //string_data is a buffer which storage the strings in form of array of chars
    //in data_ for the columns which are DataType::String there is an integer that indicates offset respect string_data_
    Buffer string_data_;

    std::vector<Column> columns_;


    std::vector<std::string> columns_name_; // to avoid a non-static size of the struct Column separate the strings (the names of each column)


public:

    TabularData() = default;
    explicit TabularData(const TabularData&) = delete;
    TabularData& operator=(const TabularData&) = delete;

    explicit TabularData(TabularData&& other)noexcept:
    data_(std::move (other.data_)),
    string_data_(std::move(other.string_data_)),
    columns_(std::move(other.columns_)),
    columns_name_(std::move(other.columns_name_))
    {}

    TabularData& operator=(TabularData&& other)noexcept {
        if (this != &other) {
            data_ = std::move(other.data_);
            string_data_ = std::move(other.string_data_);
            columns_ = std::move(other.columns_);
            columns_name_ = std::move(other.columns_name_);
        }
        return *this;
    }



    [[nodiscard]] auto n_columns() const {
        return columns_.size();
    }


    [[nodiscard]] std::string_view column_name(const size_t size_index) {
        BoundCheck::check_index(size_index, columns_name_.size(), columns_name_.size());
        return columns_name_[size_index];
    }

    [[nodiscard]] auto column_datatype(const size_t size_index) const {
        return columns_[size_index].type;
    }

    [[nodiscard]] auto column_size(const size_t size_index) const {
        return columns_[size_index].n_elements;
    }

    /**
     * @brief Extract a copy of partial tabular data defined by an interval
     * * @param begin The first index of the interval of columns to extract
     * * @param end The last index of the interval of columns to extract
     * * @note Consider  [begin, end)
     */
    [[nodiscard]]TabularData extract(size_t begin, size_t end);
    //I removed the const keyword from these arguments because they only have an effect when we are defining the functions.
    //this is simply a declaration - const is redundant

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
     * * @note Consider [begin, end)
     */
    void erase(size_t begin, size_t end);






};


#endif 
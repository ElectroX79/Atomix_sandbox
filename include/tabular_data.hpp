#ifndef TABULARDATA_HPP
#define TABULARDATA_HPP

#include <vector>
#include <stdexcept>


#include "data_type.hpp"
#include "buffer.hpp"


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


public:

    TabularData() = default;
    explicit TabularData(const TabularData&) = delete;
    TabularData& operator=(const TabularData&) = delete;

    explicit TabularData(TabularData&& other)noexcept:
    data_(std::move (other.data_)),
    string_data_(std::move(other.string_data_)),
    columns_(std::move(other.columns_))
    {}

    TabularData& operator=(TabularData&& other)noexcept {
        if (this != &other) {
            data_ = std::move(other.data_);
            string_data_ = std::move(other.string_data_);
            columns_ = std::move(other.columns_);
        }
        return *this;
    }


    /**
     * @brief Extract a copy of partial tabular data defined by an interval
     * * @param begin The first index of the interval of columns to extract
     * * @param end The last index of the interval of columns to extract
     * * @note Unlike c++ standard, the end is not exclusive, in other words we consider [begin, end], not [begin, end)
     */
    [[nodiscard]]TabularData extract(const size_t begin, const size_t end);


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
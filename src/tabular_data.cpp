#include <tabular_data.hpp>
#include <vector>
#include "security_check.hpp"



TabularData TabularData::extract(const size_t begin, const size_t end)const{
    TabularData td;
    bound_check::check_index_interval(begin, end, this->columns_.size());
    td.columns_.reserve(end - begin);
    td.columns_.insert(td.columns_.end(), this->columns_.begin()+begin, this->columns_.begin()+end );
    return td;
}

TabularData TabularData::append(const TabularData &t_data)const{
    TabularData td;
    td.columns_ = this->columns_;
    td.columns_.insert(td.columns_.end(), t_data.columns_.begin(), t_data.columns_.end());
    return td;
}


TabularData TabularData::erase(const size_t begin, const size_t end)const{
    TabularData td;
    bound_check::check_index_interval(begin, end, this->columns_.size());
    td.columns_.reserve(this->columns_.size() - (end - begin));
    if (begin > 0) {
        td.columns_.insert(td.columns_.begin(), this->columns_.begin(), this->columns_.begin()+begin);
    }
    if (end < this->columns_.size()) {
        td.columns_.insert(td.columns_.end(), this->columns_.begin() + end, this->columns_.end());
    }
    return td;
}

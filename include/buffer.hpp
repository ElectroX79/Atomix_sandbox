#ifndef BUFFER_HPP
#define BUFFER_HPP


#include <vector>


template<typename T>
class Buffer{

private:
    std::vector<T> data_;



public:

    explicit Buffer(size_t size = 1024){
        data_.reserve(size);
    }
    
    void insert(const T &info, const size_t offset){
        data_.insert(data_.begin()+offset, info);
    }

    void erase(const size_t offset){
        assert(offset<data_.size() && offset>=0);
        data_.erase(data_.begin()+offset);
    }


    void push_back(const T &info){
        data_.push_back(info);
    }


    int size()const{
        return data_.size();
    }
    

};





#endif
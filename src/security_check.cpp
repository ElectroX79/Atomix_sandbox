#include <stdexcept>



namespace BoundCheck{
    void check_index(const size_t begin, const size_t end, const size_t container_size){
        if(begin>end){
            throw std::invalid_argument("The begin cannot be bigger than the end");
        }

        //Always begin < 0, because it's size_t type

        if(end>=container_size){
            throw std::out_of_range("Invalid container acces, end>=size");
        }

    }
};



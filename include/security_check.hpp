#ifndef SECURITY_CHECK_HPP
#define SECURITY_CHECK_HPP


#include <stdexcept>




struct Bound_check{
    static void check_index(const size_t begin, const size_t end, const size_t cointainer_size){
        if(begin>end){
            throw std::invalid_argument("The begin cannot be bigger than the end");
        }

        //Always begin<0, because it's size_t type
        
        if(end>=cointainer_size){
            throw std::out_of_range("Invalid container acces, end>=size");
        }

    }
};
 

#endif
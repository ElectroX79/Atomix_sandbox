#ifndef SECURITY_CHECK_HPP
#define SECURITY_CHECK_HPP


#include <stdexcept>




namespace BoundCheck{
     void check_index(const size_t begin, const size_t end, const size_t container_size);

};
 

#endif
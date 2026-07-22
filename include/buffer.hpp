#ifndef  BUFFER_HPP
#define BUFFER_HPP

#include <cstdint>
#include <stdexcept>
#include <system_allocator.hpp>

class Buffer {

    private:
    uint8_t* begin_ = nullptr;
    size_t size_ = 0;


    public:

    Buffer(): begin_(nullptr), size_(0) {}
    Buffer(const Buffer&) = delete;
    Buffer& operator=(const Buffer&) = delete;

    Buffer(Buffer&& other) noexcept: begin_(other.begin_), size_(other.size_) {
        // no need to deallocate yet (using contructor)
        other.begin_ = nullptr;
        other.size_ = 0;
    }

    Buffer& operator=(Buffer&& other) noexcept {
        if (this != &other) {
            sys_alloc::deallocate(begin_, size_);
            begin_ = nullptr;
            size_ = 0;

            begin_ = other.begin_;
            size_ = other.size_;

            other.begin_ = nullptr;
            other.size_ = 0;
        }
        return *this;

    }

    explicit Buffer(const size_t size) {
       begin_ = sys_alloc::allocate(size);
        size_ = size;
    }

    ~Buffer(){
        sys_alloc::deallocate(begin_, size_);
        begin_ = nullptr;
        size_ = 0;
    }

    [[nodiscard]] uint8_t* get_begin() const{
        return begin_;
    }

    [[nodiscard]] size_t size() const {
        return size_;
    }

};










#endif


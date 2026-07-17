#ifndef  BUFFER_HPP
#define BUFFER_HPP

#include <cstdint>
#include <stdexcept>
#include <cstdlib>

class Buffer {

    private:
    uint8_t* begin_ = nullptr;
    size_t size_ = 0;


    public:
    Buffer(): begin_(nullptr), size_(0) {}
    Buffer(const Buffer&) = delete;
    Buffer& operator=(const Buffer&) = delete;

    Buffer(Buffer&& other) noexcept: begin_(other.begin_), size_(other.size_) {
        // no need to free() because begin == nullptr
        other.begin_ = nullptr;
        other.size_ = 0;
    }

    Buffer& operator=(Buffer&& other) noexcept {
        if (this != &other) {
            free(begin_);

            begin_ = other.begin_;
            size_ = other.size_;

            other.begin_ = nullptr;
            other.size_ = 0;
        }
        return *this;

    }

    explicit Buffer(const size_t size, const size_t alignment = 64) {

        if (alignment < sizeof(void*)) {
            throw std::invalid_argument("alignment must be greater than sizeof(void*)");
        }

        void* ptr;

        if (posix_memalign(&ptr, alignment, size) != 0) {
            throw std::runtime_error("Failed to allocate memory");
        }

        begin_ = static_cast<uint8_t *>(ptr);
        size_ = size;
    }

    ~Buffer(){
        free(begin_);
    }

    [[nodiscard]] uint8_t* get_begin() const{
        return begin_;
    }

    [[nodiscard]] size_t size() const {
        return size_;
    }

};










#endif


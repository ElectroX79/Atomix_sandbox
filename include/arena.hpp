#ifndef ARENA_HH
#define ARENA_HH

#define _POSIX_C_SOURCE 200809L

#include <cstdlib>
#include <cstdint>
#include <stdexcept>
#include <cstring>

class Arena{
    private:
    void* mem_ = nullptr;
    size_t mem_size_ = 0; //In bytes
    size_t offset_ = 0;


    public:

    Arena(const Arena&) = delete;
    Arena& operator=(const Arena&) = delete;

    Arena(Arena&& other)noexcept: mem_(other.mem_), mem_size_(other.mem_size_), offset_(other.offset_){
        other.mem_ = nullptr;
        other.offset_ = 0;
    }

    Arena& operator=(Arena&& other)noexcept{
        if (this != &other) {
            mem_ = other.mem_;
            mem_size_ = other.mem_size_;
            offset_ = other.offset_;
            other.mem_ = nullptr;
            other.offset_ = 0;
        }
        return *this;

    }



    explicit Arena(size_t mem_size, size_t alignment = 64){
        mem_size_ = mem_size;

        if(alignment < sizeof(void*)) {
            throw std::invalid_argument("Alignment must be at least sizeof(void*)");
        }

        if (posix_memalign(&mem_, alignment, mem_size) != 0) {
            throw std::runtime_error("Failed to allocate memory");
        }
    }

    ~Arena(){
        free(mem_);
    }


    void* alloc(const size_t size, const size_t alignment = 64) {

        if(alignment < sizeof(void*)) {
            return nullptr;
        }

        const uintptr_t mem_ptr = reinterpret_cast<uintptr_t>(mem_);
        const size_t padding = (alignment - ((mem_ptr + offset_) % alignment)) % alignment;

        if (size > mem_size_ - offset_ - padding) {
            return nullptr;
        }

        offset_ += padding + size;
        return reinterpret_cast<void*>(mem_ptr + offset_ - size);
    }

    void expand(const size_t new_size, const size_t alignment = 64){
        if (new_size < offset_) {
            throw std::invalid_argument("New size must be greater than the current occupied memory");
        }

        auto aux = Arena(new_size, alignment);

        memcpy(aux.mem_, mem_, offset_);
        *this = std::move(aux);
    }



    void clear() {
        offset_ = 0;
    }
};







#endif

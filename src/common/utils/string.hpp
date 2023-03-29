#pragma once
#include "memory.hpp"

#ifndef ARRAYSIZE
template <class Type, size_t n> size_t ARRAYSIZE(Type (&)[n]) { return n; }
#endif

namespace utils::string {
template <size_t buffers, size_t min_buffer_size> class va_provider final {
public:
  static_assert(buffers != 0 && min_buffer_size != 0,
                "buffers and min_buffer_size mustn't be 0");

  va_provider() : current_buffer_(0) {}

  char* get(const char* format, const va_list ap) {
    ++this->current_buffer_ %= ARRAYSIZE(this->string_pool_);
    auto entry = &this->string_pool_[this->current_buffer_];

    if (!entry->size_ || !entry->buffer_) {
      throw std::runtime_error("String pool not initialized");
    }

    while (true) {
      const int res =
          vsnprintf_s(entry->buffer_, entry->size_, _TRUNCATE, format, ap);
      if (res > 0)
        break; // Success
      if (res == 0)
        return nullptr; // Error

      entry->double_size();
    }

    return entry->buffer_;
  }

private:
  class entry final {
  public:
    explicit entry(const size_t size = min_buffer_size)
        : size_(size), buffer_(nullptr) {
      if (this->size_ < min_buffer_size)
        this->size_ = min_buffer_size;
      this->allocate();
    }

    ~entry() {
      if (this->buffer_)
        memory::get_allocator()->free(this->buffer_);
      this->size_ = 0;
      this->buffer_ = nullptr;
    }

    void allocate() {
      if (this->buffer_)
        memory::get_allocator()->free(this->buffer_);
      this->buffer_ =
          memory::get_allocator()->allocate_array<char>(this->size_ + 1);
    }

    void double_size() {
      this->size_ *= 2;
      this->allocate();
    }

    size_t size_;
    char* buffer_;
  };

  size_t current_buffer_;
  entry string_pool_[buffers];
};

const char* va(const char* fmt, ...);

std::vector<std::string> split(const std::string& s, char delim);

std::string to_lower(const std::string& text);
std::string to_upper(const std::string& text);

bool starts_with(const std::string& text, const std::string& substring);
bool ends_with(const std::string& text, const std::string& substring);

std::string dump_hex(const std::string& data,
                     const std::string& separator = " ");

std::string get_clipboard_data();

void strip(const char* in, char* out, size_t max);

std::string convert(const std::wstring& wstr);
std::wstring convert(const std::string& str);

std::string replace(std::string str, const std::string& from,
                    const std::string& to);
} // namespace utils::string

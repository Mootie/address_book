#pragma once

#include <memory>

namespace company {

template<class T>
class Vector;

template<class T>
class VectorIterator : public std::iterator<std::random_access_iterator_tag, T> {
public:
  VectorIterator()
    : ptr_(nullptr) {}

  VectorIterator& operator++() {
    ++ptr_;
    return *this;
  }

  VectorIterator& operator--() {
    --ptr_;
    return *this;
  }

  VectorIterator& operator++(int) {
    auto pre = *this;
    ++ptr_;
    return pre;
  }

  VectorIterator operator-(size_t offset) const {
    auto tmp = *this;
    tmp.ptr_ -= offset;
    return tmp;
  }

  VectorIterator operator+(size_t offset) const {
    auto tmp = *this;
    tmp.ptr_ += offset;
    return tmp;
  }

  bool operator<(VectorIterator<T> rhs) const {
    return ptr_ < rhs.ptr_;
  }

  size_t operator-(VectorIterator<T> rhs) const {
    return ptr_ - rhs.ptr_;
  }

  bool operator==(const VectorIterator<T>& rhs) const {
    return ptr_ == rhs.ptr_;
  }
  
  bool operator!=(const VectorIterator<T>& rhs) const {
    return !(*this == rhs);
  }

  T& operator*() {
    return *ptr_;
  }

  const T& operator*() const {
    return *ptr_;
  }

  T* operator->() {
    return ptr_;
  }

  const T* operator->() const {
    return ptr_;
  }

private:
  friend Vector<T>;

  VectorIterator(T& ptr)
    : ptr_(&ptr) {}
  
  T* ptr_;
};


// const overloads omitted for brevity
template<class T>
class Vector {
public:
  using iterator = VectorIterator<T>;

  Vector()
    : capacity_(0), size_(0) {}
  
  Vector(size_t size)
    : size_(size) {
    realloc(size);
  }

  // rule of zero, don't need move constructors or destructor

  Vector(const Vector& rhs)
  : capacity_(0), size_(0) {
    *this = rhs;
  }

  Vector& operator=(const Vector& rhs) {
    // inefficient implementation
    clear();
    realloc(rhs.size_);
    for (auto& val : rhs) {
      push_back(val);
    }
    return *this;
  }

  bool operator==(const Vector<T>& rhs) const {
    if (size_ != rhs.size_) {
      return false;
    }

    for (size_t i = 0; i < size_; ++i) {
      if (rhs[i] != (*this)[i]) {
        return false;
      }
    }
    return true;
  }

  size_t size() const {
    return size_;
  }

  bool empty() const {
    return size_ == 0;
  }

  T* data() {
    return elements_;
  }

  void clear() {
    elements_.reset();
    size_ = 0;
    capacity_ = 0;
  }

  iterator begin() {
    return iterator(elements_[0]);
  }

  const iterator begin() const {
    return iterator(elements_[0]);
  }

  iterator end() {
    return iterator(elements_[size_]);
  }

  const iterator end() const {
    return iterator(elements_[size_]);
  }

  T& operator[](size_t pos) {
    return elements_[pos];
  }

  const T& operator[](size_t pos) const {
    return elements_[pos];
  }

  T& front() {
    return elements_[0];
  }

  T& back() {
    return elements_[size_ - 1];
  }

  template<class Ty>
  void push_back(Ty&& element) {
    if (size_ + 1 >= capacity_) {
      realloc(size_ + 1);
    }

    elements_[size_] = std::forward<Ty>(element);
    ++size_;
  }

  void pop_back() {
    back().~T();
    --size_;
  }

  void erase(iterator pos) {
    // left shift
    auto prev = pos;
    ++pos;
    for (auto it=pos; it != end(); ++it) {
      *prev = std::move(*it);
      prev = it;
    }
    --size_;
  }

  void reserve(int new_capacity) {
    if (new_capacity > capacity_) {
      realloc(new_capacity);
    }
  }

private:
  void realloc(size_t new_size) {
    size_t new_capacity = new_size * 2;
    auto new_elements = std::unique_ptr<T[]>(new T[new_capacity]); // allocate
    for (size_t i=0; i<size_; ++i) {
      new_elements[i] = std::move(elements_[i]); // reconstruct
    }

    elements_ = std::move(new_elements); // replace
    capacity_ = new_capacity;
  }
  
  std::unique_ptr<T[]> elements_;
  size_t size_;
  size_t capacity_;
};

} // namespace company
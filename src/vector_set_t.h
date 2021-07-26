#ifndef vector_set_h
#define vector_set_h


#include <vector>

struct vector_set_t {
  std::vector<u_char> set;
  std::size_t capacity;
  std::size_t count;
  
  vector_set_t() {}
  
  vector_set_t(std::size_t capacity) {
    count = 0;
    this->capacity = capacity;
    for (int i = 0; i < capacity; i++) {
      set.push_back(0);
    }
  }
  
  void insert(std::size_t i) {
    if (set[i]) {
      return;
    }
    set[i] = 1;
    count++;
  }
  
  void erase(std::size_t i) {
    if (!set[i]) {
      return;
    }
    set[i] = 0;
    count--;
  }
  
  void clear() {
    count = 0;
    for (int i = 0; i < capacity; i++) {
      set[i] = 0;
    }
  }
  
  u_char find(std::size_t i) {
    return set[i] == 1;
  }
  
  std::size_t size() {
    return count;
  }
  
};


#endif /* vector_set_h */

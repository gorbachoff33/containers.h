#ifndef SET_H
#define SET_H
#include <initializer_list>
#include <iostream>
#include <limits>
#include <utility>
#include <vector>

#include "Set_node.h"
namespace s21 {

template <typename T>
class Set {
 public:
  class iter;
  friend class iter;
  class const_iter;
  friend class const_iter;
  using key_type = T;
  using Key = key_type;
  using value_type = T;
  using reference = T&;
  using const_reference = const T&;
  using iterator = iter;
  using const_iterator = const_iter;
  using size_type = size_t;

  Set_node<T>* root = nullptr;
  Set_node<T>* nil = nullptr;
  Set_node<T>* tail = nullptr;
  Set_node<T>* end_Set_node = nullptr;
  size_type m_size = 0;
  class iter {
   public:
    Set_node<T>* elem = nullptr;

   protected:
    Set_node<T>* lastvist_ = nullptr;
    Set_node<T>* lasvisit1_ = nullptr;
    value_type nul_ = 0;
    reference nul_ref_ = nul_;

   public:
    iter() noexcept;
    iter(Set_node<T>* o) noexcept;
    iter(const iter& o) noexcept;
    bool operator==(const iter& o) noexcept;

    iter operator=(const iter& o) noexcept;
    reference operator*() noexcept;
    void operator++() noexcept;
    void operator--() noexcept;
    iter operator=(Set_node<T>* o) noexcept;
    bool operator!=(const iter& o) noexcept;
    ~iter() {}
  };
  class const_iter : public iter {
   public:
    const_iter() noexcept;
    const_iter(Set_node<T>* o) noexcept;
    const_iter(const iter& o) noexcept;
    const_iter operator=(const iter& o) noexcept;
    const_reference operator*() noexcept;
  };

 public:
  // Constructors
  Set() noexcept;
  Set(std::initializer_list<value_type> const& items) noexcept;
  Set(Set& s) noexcept;
  Set(Set&& s) noexcept;
  ~Set();
  void operator=(Set&& s) noexcept;

  iterator begin() const noexcept;
  iterator end() noexcept;
  bool empty() noexcept;
  size_type size() noexcept;
  size_type max_size() noexcept;

  template <typename... Args>
  std::vector<std::pair<Set<T>::iterator, bool>> emplace(
      Args&&... args) noexcept;

  void clear() noexcept;
  std::pair<iterator, bool> insert(const value_type& value) noexcept;
  void Erase(typename Set<T>::iterator it) noexcept;
  void swap(Set& other) noexcept;
  void merge(Set& other) noexcept;

  iterator find(const Key& key) noexcept;
  bool contains(const Key& key) noexcept;

 private:
  Set_node<T>* insert_nil() noexcept {
    Set_node<T>* n = new Set_node<T>();
    n->pLeft = nullptr;
    n->pRight = nullptr;
    n->parent = nullptr;
    n->color = BLACK;
    n->Nil = true;
    n->val = -1;
    return n;
  }
  value_type nul_ = 0;
  reference nul_ref_ = nul_;
  void DeleteFixup(Set_node<T>* x) noexcept;
  void InsertBalance(Set_node<T>* x) noexcept;
  void RotateLeft(Set_node<T>* x) noexcept;
  void RotateRight(Set_node<T>* x) noexcept;
  void CopySet(Set& s) noexcept;
  void InsertTail() noexcept;
  void DeleteAll(Set_node<T>* list) noexcept;
  std::pair<Set_node<T>*, bool> insert_node(const value_type& i,
                                            Set_node<T>* r) noexcept;
};
}  // namespace s21
#endif
#include "set.tpp"
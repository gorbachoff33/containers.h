#ifndef Multiset_H
#define Multiset_H
#include <initializer_list>
#include <iostream>
#include <limits>
#include <utility>
#include <vector>

#include "Node.h"
namespace s21 {
template <typename T>
class Multiset {
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

  class iter {
   public:
    Node<T>* elem_;
    iter() noexcept;
    iter(Node<T>* o) noexcept;
    iter(const iter& o) noexcept;
    bool operator==(const iter& o) const noexcept;
    iter operator=(const iter& o) noexcept;
    T& operator*() noexcept;
    void operator++() noexcept;
    void operator--() noexcept;
    iter operator=(Node<T>* o) noexcept;
    bool operator!=(const iter& o) const noexcept;

   protected:
    Node<T>* end_a_;
    Node<T>* lastvist_;
    Node<T>* lasvisit1_;
    Node<T>* last_left_;
    Node<T>* last_right_;
    value_type nul = 0;
    reference nul_ref = nul;
  };
  class const_iter : public iter {
   public:
    const_iter() noexcept;
    const_iter(Node<T>* o) noexcept;
    const_iter(const iter& o) noexcept;
    const_iter operator=(const iter& o) noexcept;
    const_reference operator*() noexcept;
  };

 public:
  iter it;
  // Constructors
  Multiset() noexcept;
  Multiset(std::initializer_list<value_type> const& items);
  Multiset(Multiset& s) noexcept;
  Multiset(Multiset&& s) noexcept;
  ~Multiset() noexcept;
  void operator=(Multiset&& s) noexcept;
  iter begin() const noexcept;
  iter end() noexcept;
  bool empty() const noexcept;
  size_type size() const noexcept;
  size_type max_size() const noexcept;
  void clear();
  template <typename... Args>
  std::vector<std::pair<Multiset<T>::iterator, bool>> emplace(Args&&... args);
  iter insert(const value_type& value) noexcept;
  void Erase(typename Multiset<T>::iter it) noexcept;
  void swap(Multiset& other) noexcept;
  void merge(Multiset& other) noexcept;
  std::pair<iterator, iterator> equal_range(const Key& key) noexcept;
  iterator find(const Key& key) noexcept;
  bool contains(const Key& key) noexcept;
  iterator lower_bound(const Key& key) noexcept;
  iterator upper_bound(const Key& key) noexcept;
  size_type count(const Key& key) noexcept;

 private:
  Node<T>* InsertNil() {
    Node<T>* n = new Node<T>();
    n->pLeft = nullptr;
    n->pRight = nullptr;
    n->parent = nullptr;
    n->color = BLACK_M;
    n->Nil = true;
    n->val = m_size_;
    return n;
  }
  Node<T>* root_;
  Node<T>* nil_;
  Node<T>* tail_;
  Node<T>* end_node_;
  size_type m_size_;

  void DeleteFixup(Node<T>* x) noexcept;
  void InsertBalance(Node<T>* x) noexcept;
  void RotateLeft(Node<T>* x) noexcept;
  void RotateRight(Node<T>* x) noexcept;
  void MoveFnc(Multiset& s) noexcept;

  void CopyMultiset(Multiset& s) noexcept;
  void InsertTail() noexcept;
  void DeleteAll(Node<T>* list) noexcept;
  Node<T>* InsertNode(const value_type& i, Node<T>* r) noexcept;
};
}  // namespace s21
#endif
#include "Multiset.tpp"
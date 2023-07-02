// created by pizpotli
#ifndef CPP2_s21_containers_MAP_1_S21_MAP_H_
#define CPP2_s21_containers_MAP_1_S21_MAP_H_

#include <initializer_list>
#include <iostream>
#include <limits>
#include <utility>
#include <vector>

namespace s21 {
template <typename Key, typename T>
class Map {
 private:
  struct Node;

 public:
  class MapIterator;
  using key_type = Key;
  using mapped_type = T;
  using ref = T &;
  using Iterator = Map<Key, T>::MapIterator;
  using value_type = std::pair<key_type, mapped_type>;
  using reference = value_type &;
  using const_reference = const value_type &;
  using size_type = size_t;
  class ConstMapIterator {
   public:
    ConstMapIterator() noexcept : _node(nullptr) {}
    ConstMapIterator(Node *node) noexcept : _node(node){};
    ConstMapIterator(const ConstMapIterator &node) noexcept
        : _node(node._node){};
    ConstMapIterator(ConstMapIterator &&node) noexcept {
      _node = node._node;
      node._node = nullptr;
    };
    bool operator!=(const ConstMapIterator &other) const noexcept {
      return !operator==(other);
    }
    bool operator==(const ConstMapIterator &other) const noexcept;
    void operator++() noexcept;
    void operator++(int) noexcept;
    void operator--() noexcept;
    void operator--(int) noexcept;
    std::pair<Key, T> *operator->() noexcept;

   public:
    std::pair<Key, T> _own;
    Node *_node;
    mapped_type zero_ = 0;
    ref error_ = zero_;
  };
  using const_Iterator = Map<Key, T>::ConstMapIterator;

  class MapIterator : public ConstMapIterator {
   public:
    MapIterator() noexcept : ConstMapIterator(){};
    MapIterator(Node *node) noexcept : ConstMapIterator(node){};
    MapIterator(const MapIterator &node) noexcept : ConstMapIterator(node){};
    MapIterator(MapIterator &&node) noexcept : ConstMapIterator(node){};
    void operator=(const MapIterator &node) noexcept {
      this->_node = node._node;
    }
    void operator=(const_Iterator node) noexcept { this->_node = node._node; }
    void operator=(Node *node) noexcept { this->_node = node; }
    void operator=(Node &node) noexcept;
  };

  Map() noexcept;
  Map(std::initializer_list<value_type> const &items);
  Map(const Map &m) noexcept;
  Map(Map &&m) noexcept;
  ~Map() noexcept;
  void operator=(Map &m) noexcept;
  void operator=(Map &&m) noexcept;

  mapped_type &At(const key_type &key);
  mapped_type &operator[](const key_type &key) noexcept;
  Iterator Begin() const noexcept;
  Iterator End() const noexcept;
  bool Empty() noexcept;
  size_type Size() noexcept;
  size_type MaxSize() noexcept;
  void Clear() noexcept;
  std::pair<Iterator, bool> Insert(const value_type &value) noexcept;
  std::pair<Iterator, bool> Insert(const key_type &key, const T &obj) noexcept;
  std::pair<Iterator, bool> InsertOrAssign(const key_type &key,
                                           const T &obj) noexcept;
  void Erase(MapIterator pos) noexcept;
  void Swap(Map &other) noexcept;
  void Merge(Map &other) noexcept;
  bool Contains(const key_type &key) noexcept;
  template <class... Args>
  std::vector<std::pair<Iterator, bool>> Emplace(Args &&...args) {
    std::vector<std::pair<Iterator, bool>> result;
    std::vector<typename Map<Key, T>::value_type> argsVector = {args...};
    for (auto &i : argsVector) {
      result.push_back(Insert(i));
    }
    return result;
  }

 private:
  struct Node {
   public:
    Node *parent_, *left_, *right_, *next_, *prev_;
    bool is_red_;
    key_type key_;
    T value_;
    Node() noexcept;
    Node(Key k, T val) noexcept;
    Node(const Node &other) noexcept;
    void operator=(Node &node) noexcept;
  };
  Node *_head, *_tail, *_end, *_root, *_next, *_prev;
  size_t _size;

 private:
  //--------INSERT--------
  bool KeyPozition(Node *node) noexcept;
  void LeftPozition(Node *node) noexcept;
  void RightPozition(Node *node) noexcept;
  void CheckNextPrev(Node *node, Node *tmp) noexcept;
  void InsertCase1(Node *node) noexcept;
  void InsertCase2(Node *node) noexcept;
  void InsertCase3(Node *node) noexcept;
  void InsertCase4(Node *node) noexcept;
  void InsertCase5(Node *node) noexcept;
  void GrandFather(Node *node) noexcept;
  void LeftTurn(Node *node) noexcept;
  void RightTurn(Node *node) noexcept;
  bool CheckParrent(Node *node) noexcept;
  bool CheckUncle(Node *node) noexcept;

  //--------FIND--------
  std::pair<Node *, bool> FindPos(Key key) noexcept;
  Node *Pos(key_type key, Node *node, Node *res) noexcept;

  //--------DELETE--------
  Node *MinRightElement(Node *node) noexcept;
  Node *MaxLeftElement(Node *node) noexcept;
  bool BratherChaild(Node *node) noexcept;
  void PrevNext(Node *node) noexcept;
  void PrevNextDoubleMin(Node *node, Node *tmp) noexcept;
  void PrevNextDoubleMax(Node *node, Node *tmp) noexcept;
  void Delete(Node *node) noexcept;
  void DeleteElement(Node *node) noexcept;
  void DeleteElementTwo(Node *node) noexcept;
  bool MinMax(Node *node) noexcept;
  void DeleteCase1(Node *node) noexcept;
  void DeleteCase2(Node *node) noexcept;
  void DeleteCase3(Node *node) noexcept;
  void DeleteCase4(Node *node) noexcept;
  void DeleteCase41(Node *tmp, Node *brather) noexcept;
  void DeleteCase42(Node *chaild, Node *brather, Node *tmp) noexcept;
  void DeleteCase43(Node *node) noexcept;
  void DeleteCase44(Node *parent, Node *brather) noexcept;
  void DeleteCase5(Node *node) noexcept;
  void DeleteCase51(Node *tmp, Node *brather) noexcept;
  void DeleteCase52(Node *chaild, Node *brather, Node *tmp) noexcept;
  void DeleteCase53(Node *node) noexcept;
  void DeleteCase54(Node *brather, Node *parent) noexcept;
  bool CheckBrather(Node *node) noexcept;
  bool CheckColorChaildRight(Node *node) noexcept;
  bool CheckColorChaildLeft(Node *node) noexcept;
  void LeftTurnNoRec(Node *chaild_left, Node *brather, Node *parent) noexcept;
  void RightTurnNoRec(Node *chaild_left, Node *brather, Node *parent) noexcept;
  void BlackBrather(Node *node) noexcept;

  void MoveMap(Map &other) noexcept;
  void CopyMap(const Map &other) noexcept;
  void EndAddress() noexcept;
};
}  // namespace s21

#include "s21_map.tpp"
#endif  // CPP2_s21_containers_MAP_1_S21_MAP_H_
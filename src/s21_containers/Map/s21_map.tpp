#include "s21_map.h"

namespace s21 {
//*************************------------------**************************
//*************************--ITERATOR-CONST--**************************
//*************************------------------**************************

template <typename Key, typename T>
bool Map<Key, T>::ConstMapIterator::operator==(
    const ConstMapIterator &other) const noexcept {
  if (this == &other) {
    return true;
  }
  return _node == other._node;
};

//--------------------------------------------------------------------

template <typename Key, typename T>
void Map<Key, T>::ConstMapIterator::operator++() noexcept {
  if (_node) {
    _node = _node->next_;
  }
};

//--------------------------------------------------------------------

template <typename Key, typename T>
void Map<Key, T>::ConstMapIterator::operator++(int) noexcept {
  if (_node) {
    _node = _node->next_;
  }
};

//--------------------------------------------------------------------

template <typename Key, typename T>
void Map<Key, T>::ConstMapIterator::operator--() noexcept {
  if (_node) {
    _node = _node->prev_;
  }
};

//--------------------------------------------------------------------

template <typename Key, typename T>
void Map<Key, T>::ConstMapIterator::operator--(int) noexcept {
  if (_node) {
    _node = _node->prev_;
  }
};

//--------------------------------------------------------------------

template <typename Key, typename T>
typename std::pair<Key, T>
    *Map<Key, T>::ConstMapIterator::operator->() noexcept {
  if (this->_node) {
    this->_own.first = this->_node->key_;
    this->_own.second = this->_node->value_;
  }
  return &this->_own;
}

//*************************------------------**************************
//*************************-----ITERATOR-----**************************
//*************************------------------**************************

template <typename Key, typename T>
void Map<Key, T>::MapIterator::operator=(Node &node) noexcept {
  this->_node = &node;
};

//*************************-------MAP--------**************************
//*************************-------NODE-------**************************
//*************************------------------**************************

template <typename Key, typename T>
Map<Key, T>::Node::Node() noexcept
    : parent_(nullptr),
      left_(nullptr),
      right_(nullptr),
      next_(nullptr),
      prev_(nullptr),
      is_red_(true),
      key_(0),
      value_(0){};

//--------------------------------------------------------------------

template <typename Key, typename T>
Map<Key, T>::Node::Node(Key k, T val) noexcept
    : parent_(nullptr),
      left_(nullptr),
      right_(nullptr),
      next_(nullptr),
      prev_(nullptr),
      is_red_(true),
      key_(k),
      value_(val){};

//--------------------------------------------------------------------

template <typename Key, typename T>
Map<Key, T>::Node::Node(const Node &other) noexcept
    : parent_(other.parent_),
      left_(other.left_),
      right_(other.right_),
      next_(other.next_),
      prev_(other.prev_),
      is_red_(other.is_red_),
      key_(other.key_),
      value_(other.value_){};

//--------------------------------------------------------------------

template <typename Key, typename T>
void Map<Key, T>::Node::operator=(Node &node) noexcept {
  parent_ = node.parent_;
  left_ = node.left_;
  right_ = node.right_;
  next_ = node.next_;
  prev_ = node.next_;
  is_red_ = node.is_red_;
  key_ = node.key_;
  value_ = node.value_;
};

//*************************-------MAP--------**************************
//*************************---CONSTRUCTORS---**************************
//*************************------------------**************************

template <typename Key, typename T>
Map<Key, T>::Map() noexcept
    : _head(nullptr),
      _tail(nullptr),
      _end(nullptr),
      _root(nullptr),
      _next(nullptr),
      _prev(nullptr),
      _size(0){};

//--------------------------------------------------------------------

template <typename Key, typename T>
Map<Key, T>::Map(std::initializer_list<value_type> const &items)
    : _head(nullptr),
      _tail(nullptr),
      _end(nullptr),
      _root(nullptr),
      _next(nullptr),
      _prev(nullptr),
      _size(0) {
  for (auto it = items.begin(); it != items.end(); it++) {
    Insert(it->first, it->second);
  }
};

//--------------------------------------------------------------------

template <typename Key, typename T>
Map<Key, T>::Map(const Map &m) noexcept
    : _head(nullptr),
      _tail(nullptr),
      _end(nullptr),
      _root(nullptr),
      _next(nullptr),
      _prev(nullptr),
      _size(0) {
  CopyMap(m);
};

//--------------------------------------------------------------------

template <typename Key, typename T>
Map<Key, T>::Map(Map &&m) noexcept
    : _head(nullptr),
      _tail(nullptr),
      _end(nullptr),
      _root(nullptr),
      _next(nullptr),
      _prev(nullptr),
      _size(0) {
  MoveMap(m);
};

//--------------------------------------------------------------------

template <typename Key, typename T>
Map<Key, T>::~Map() noexcept {
  Clear();
};

//*************************-------MAP--------**************************
//*************************-----FUNCTIONS----**************************
//*************************------------------**************************

template <typename Key, typename T>
void Map<Key, T>::operator=(Map &&m) noexcept {
  MoveMap(m);
};

//--------------------------------------------------------------------

template <typename Key, typename T>
void Map<Key, T>::operator=(Map &m) noexcept {
  CopyMap(m);
};

//--------------------------------------------------------------------

template <typename Key, typename T>
T &Map<Key, T>::At(const key_type &key) {
  std::pair<Map<Key, T>::Node *, bool> tmp;
  tmp = FindPos(key);
  if (!tmp.second) {
    throw std::out_of_range("Map::At: key not faund");
  }
  Node *node = tmp.first;
  return node->value_;
};

//--------------------------------------------------------------------

template <typename Key, typename T>
T &Map<Key, T>::operator[](const key_type &key) noexcept {
  std::pair<Map<Key, T>::Node *, bool> tmp;
  Node *node, nod;
  tmp = FindPos(key);
  if (tmp.second) {
    node = tmp.first;
  }
  if (!tmp.first) {
    node = &nod;
  }
  return node->value_;
};

//--------------------------------------------------------------------

template <typename Key, typename T>
typename Map<Key, T>::Iterator Map<Key, T>::Begin() const noexcept {
  return _head;
};

//--------------------------------------------------------------------

template <typename Key, typename T>
typename Map<Key, T>::Iterator Map<Key, T>::End() const noexcept {
  return _end;
};

//--------------------------------------------------------------------

template <typename Key, typename T>
bool Map<Key, T>::Empty() noexcept {
  if (_head) {
    return false;
  } else {
    return true;
  }
};

//--------------------------------------------------------------------

template <typename Key, typename T>
size_t Map<Key, T>::Size() noexcept {
  return _size;
};

//--------------------------------------------------------------------

template <typename Key, typename T>
size_t Map<Key, T>::MaxSize() noexcept {
  return std::numeric_limits<std::size_t>::max() / (sizeof(Key) + sizeof(T));
};

//--------------------------------------------------------------------

template <typename Key, typename T>
void Map<Key, T>::Clear() noexcept {
  while (_head != _end && _head) {
    Node *tmp = _head->next_;
    delete _head;
    _head = tmp;
  }
  if (_end) delete _end;
  _tail = _root = _end = _prev = _next = _head = nullptr;
  _size = 0;
};

//--------------------------------------------------------------------

template <typename Key, typename T>
std::pair<typename Map<Key, T>::Iterator, bool> Map<Key, T>::Insert(
    const value_type &value) noexcept {
  return Insert(value.first, value.second);
};

//--------------------------------------------------------------------

template <typename Key, typename T>
std::pair<typename Map<Key, T>::Iterator, bool> Map<Key, T>::Insert(
    const key_type &key, const T &obj) noexcept {
  Node *node = new Node(key, obj);
  std::pair<typename Map<Key, T>::Iterator, bool> res;
  res.first = node;
  bool x = false;
  x = KeyPozition(node);
  if (x) {
    if (_end == nullptr) {
      _end = new Node(key, obj);
    }
    _tail->next_ = _end;
    _end->prev_ = _tail;
    InsertCase1(node);
    _size++;
  }
  res.second = x;
  EndAddress();
  return res;
};

//--------------------------------------------------------------------

template <typename Key, typename T>
std::pair<typename Map<Key, T>::Iterator, bool> Map<Key, T>::InsertOrAssign(
    const key_type &key, const T &obj) noexcept {
  std::pair<Map<Key, T>::Node *, bool> tmp;
  std::pair<typename Map<Key, T>::Iterator, bool> tmp2;
  tmp = FindPos(key);
  if (tmp.second) {
    Node *node = tmp.first;
    node->value_ = obj;
    tmp2.first = node;
  } else {
    tmp2 = Insert(key, obj);
  }
  return tmp2;
};

//--------------------------------------------------------------------

template <typename Key, typename T>
void Map<Key, T>::Erase(MapIterator pos) noexcept {
  std::pair<Map<Key, T>::Node *, bool> tmp = FindPos(pos->first);
  Delete(tmp.first);
};

//--------------------------------------------------------------------

template <typename Key, typename T>
void Map<Key, T>::Swap(Map &other) noexcept {
  Map<Key, T> tmp(*this);
  MoveMap(other);
  other.MoveMap(tmp);
};

//--------------------------------------------------------------------

template <typename Key, typename T>
void Map<Key, T>::Merge(Map &other) noexcept {
  for (Map<Key, T>::MapIterator i = other.Begin(); i != other.End(); ++i) {
    std::pair<Iterator, bool> tmp;
    Node node;
    node.key_ = i->first;
    node.value_ = i->second;
    tmp = FindPos(node.key_);
    if (!tmp.second) {
      tmp = Insert(node.key_, node.value_);
      other.Erase(i);
    }
  }
};

//--------------------------------------------------------------------

template <typename Key, typename T>
bool Map<Key, T>::Contains(const key_type &key) noexcept {
  std::pair<Map<Key, T>::Node *, bool> tmp;
  tmp = FindPos(key);
  if (!tmp.second) {
    return false;
  }
  return true;
};

//*************************-------MAP--------**************************
//*************************------INSERT------**************************
//*************************-----PRIVATE------**************************

template <typename Key, typename T>
bool Map<Key, T>::KeyPozition(Node *node) noexcept {
  bool insert = true;
  _next = _prev = _root;
  if (!_root) {
    _head = _tail = _root = node;
  } else if (node->key_ < _root->key_) {
    node->parent_ = _root;
    LeftPozition(node);
  } else if (node->key_ > _root->key_) {
    node->parent_ = _root;
    RightPozition(node);
  } else {
    insert = false;
  }
  return insert;
};

//--------------------------------------------------------------------

template <typename Key, typename T>
void Map<Key, T>::LeftPozition(Node *node) noexcept {
  Node *tmp = node->parent_;
  CheckNextPrev(node, tmp);
  if (tmp->key_ > node->key_) {
    if (!tmp->left_) {
      Node *prev = tmp->prev_;
      if (prev) {
        prev->next_ = node;
      }
      node->next_ = tmp;
      node->prev_ = tmp->prev_;
      tmp->left_ = node;
      tmp->prev_ = node;
      if (_head->key_ > node->key_) {
        _head->key_ = node->key_;
      }
    } else {
      node->parent_ = node->next_ = tmp->left_;
      LeftPozition(node);
    }
  } else if (tmp->key_ < node->key_) {
    RightPozition(node);
  } else {
  }
};

//--------------------------------------------------------------------

template <typename Key, typename T>
void Map<Key, T>::RightPozition(Node *node) noexcept {
  Node *tmp = node->parent_;
  CheckNextPrev(node, tmp);
  if (tmp->key_ < node->key_) {
    if (!tmp->right_) {
      Node *nex = tmp->next_;
      nex->prev_ = node;
      node->prev_ = tmp;
      tmp->right_ = node;
      node->next_ = tmp->next_;
      tmp->next_ = node;
      if (_tail->key_ < node->key_) {
        _tail->key_ = node->key_;
      }
    } else {
      node->parent_ = node->prev_ = tmp->right_;
      RightPozition(node);
    }
  } else if (tmp->key_ > node->key_) {
    LeftPozition(node);
  }
};

//--------------------------------------------------------------------

template <typename Key, typename T>
void Map<Key, T>::CheckNextPrev(Node *node, Node *tmp) noexcept {
  if (_head->key_ > node->key_) {
    _head = node;
  }
  if (_tail->key_ < node->key_) {
    _tail = node;
  }
  if (_next->key_ > node->key_ && _next->key_ > tmp->key_) {
    _next = tmp;
    node->next_ = _next;
  }
  if (_prev->key_ < node->key_ && _prev->key_ < tmp->key_) {
    _prev = tmp;
    node->prev_ = _prev;
  }
};

//--------------------------------------------------------------------

template <typename Key, typename T>
void Map<Key, T>::InsertCase1(Node *node) noexcept {
  if (!node->parent_) {
    _root = node;
    _root->is_red_ = false;
  } else if (!node->left_ && !node->right_ && node->parent_ == _root) {
  } else {
    InsertCase2(node);
  }
};

//--------------------------------------------------------------------

template <typename Key, typename T>
void Map<Key, T>::InsertCase2(Node *node) noexcept {
  Node *grandpa = node->parent_->parent_;
  Node *father = node->parent_;
  if (node == _root) {
    return;
  }
  if (CheckParrent(node)) {
    if (CheckUncle(node)) {
      InsertCase3(node);
    } else {
      if (node == father->left_ && father == grandpa->right_) {
        InsertCase4(node);
      } else if (node == father->right_ && father == grandpa->left_) {
        grandpa->left_ = node;
        father->right_ = node->left_;
        father->parent_ = node;
        node->left_ = father;
        node->parent_ = grandpa;
        LeftTurn(father);
      } else if (node == father->left_ && father == grandpa->left_) {
        LeftTurn(node);
      } else if (node == father->right_ && father == grandpa->right_) {
        RightTurn(node);
      }
    }
  }
};

//--------------------------------------------------------------------

template <typename Key, typename T>
void Map<Key, T>::InsertCase3(Node *node) noexcept {
  GrandFather(node);
};

//--------------------------------------------------------------------

template <typename Key, typename T>
void Map<Key, T>::InsertCase4(Node *node) noexcept {
  Node *father = node->parent_;
  Node *grandpa = father->parent_;
  grandpa->right_ = node;
  father->left_ = node->right_;
  father->parent_ = node;
  node->right_ = father;
  node->parent_ = grandpa;
  RightTurn(father);
};

//--------------------------------------------------------------------

template <typename Key, typename T>
void Map<Key, T>::InsertCase5(Node *node) noexcept {
  Node *father = node->parent_;
  Node *grandpa = father->parent_;
  grandpa->left_ = node;
  father->right_ = node->left_;
  father->parent_ = node;
  node->left_ = father;
  node->parent_ = grandpa;
  RightTurn(father);
};

//--------------------------------------------------------------------

template <typename Key, typename T>
void Map<Key, T>::LeftTurn(Node *node) noexcept {
  Node *father = node->parent_;
  Node *grandpa = father->parent_;
  if (grandpa->parent_) {
    Node *tmp2 = grandpa->parent_;
    if (tmp2->right_ == grandpa) {
      tmp2->right_ = father;
    } else {
      tmp2->left_ = father;
    }
  }
  Node *tmp;
  if (father->right_) {
    tmp = father->right_;
    tmp->parent_ = grandpa;
  }
  grandpa->left_ = father->right_;
  father->right_ = grandpa;
  father->parent_ = grandpa->parent_;
  grandpa->parent_ = father;
  father->is_red_ = false;
  grandpa->is_red_ = true;
  node = father;
  if (_root == grandpa) {
    _root = father;
    return;
  }
  InsertCase2(node);
};

//--------------------------------------------------------------------

template <typename Key, typename T>
void Map<Key, T>::RightTurn(Node *node) noexcept {
  Node *father = node->parent_;
  Node *grandpa = father->parent_;
  if (grandpa->parent_) {
    Node *tmp2 = grandpa->parent_;
    if (tmp2->left_ == grandpa) {
      tmp2->left_ = father;
    } else {
      tmp2->right_ = father;
    }
  }
  Node *tmp;
  if (father->left_) {
    tmp = father->left_;
    tmp->parent_ = grandpa;
  }
  grandpa->right_ = father->left_;
  father->left_ = grandpa;
  father->parent_ = grandpa->parent_;
  grandpa->parent_ = father;
  father->is_red_ = false;
  grandpa->is_red_ = true;
  node = father;
  if (_root == grandpa) {
    _root = father;
    return;
  }

  InsertCase2(node);
};

//--------------------------------------------------------------------

template <typename Key, typename T>
void Map<Key, T>::GrandFather(Node *node) noexcept {
  Node *tmp = node->parent_->parent_;
  tmp->is_red_ = true;
  if (tmp->left_) {
    tmp->left_->is_red_ = false;
  }
  if (tmp->right_) {
    tmp->right_->is_red_ = false;
  }
  node = tmp;
  if (tmp == _root) {
    _root->is_red_ = false;
  }
};

//--------------------------------------------------------------------

template <typename Key, typename T>
bool Map<Key, T>::CheckParrent(Node *node) noexcept {
  if (node->parent_->is_red_) {
    return true;
  } else {
    return false;
  }
  return true;
};

//--------------------------------------------------------------------

template <typename Key, typename T>
bool Map<Key, T>::CheckUncle(Node *node) noexcept {
  Node *tmp = node->parent_;
  if (tmp->parent_->left_ == tmp) {
    if (tmp->parent_->right_) {
      if (tmp->parent_->right_->is_red_) {
        return true;
      } else {
        return false;
      }
    } else {
      return false;
    }
  } else {
    if (tmp->parent_->left_) {
      if (tmp->parent_->left_->is_red_) {
        return true;
      } else {
        return false;
      }
    } else {
      return false;
    }
  }
};

//*************************-------MAP--------**************************
//*************************------ERASE-------**************************
//*************************-----PRIVATE------**************************

template <typename Key, typename T>
void Map<Key, T>::PrevNext(Node *node) noexcept {
  if (_head != node) {
    Node *nextn = node->next_;
    Node *prevn = node->prev_;
    if (node == _head) {
      _head = nextn;
      nextn->prev_ = nullptr;
    } else {
      if (nextn) nextn->prev_ = prevn;
      if (prevn) prevn->next_ = nextn;
    }
  } else {
    _head = node->next_;
    _head->prev_ = nullptr;
  }
};

//------------------------------------------------------------------

template <typename Key, typename T>
void Map<Key, T>::Delete(Node *node) noexcept {
  PrevNext(node);
  if (_root == node && !node->left_ && !node->right_) {
    _head = _tail = _root = nullptr;
    _size--;
  } else {
    DeleteElement(node);
    _size--;
  }
  if (node) delete node;
};

//------------------------------------------------------------------

template <typename Key, typename T>
void Map<Key, T>::DeleteElement(Node *node) noexcept {
  if (node == _head) {
    _head = _head->next_;
  }
  if (node->is_red_ && !node->left_ && !node->right_) {
    DeleteCase1(node);
  } else if (node->left_ && node->right_) {
    DeleteCase2(node);
  } else {
    DeleteElementTwo(node);
  }
};

//------------------------------------------------------------------

template <typename Key, typename T>
void Map<Key, T>::DeleteElementTwo(Node *node) noexcept {
  if (!node->is_red_ &&
      ((node->left_ && !node->right_) || (!node->left_ && node->right_))) {
    DeleteCase3(node);
  } else if (node == node->parent_->left_) {
    DeleteCase4(node);
  } else if (node == node->parent_->right_) {
    DeleteCase5(node);
  }
};

//--------------------------------DELETECASE1--------------------------------

template <typename Key, typename T>
void Map<Key, T>::DeleteCase1(Node *node) noexcept {
  Node *tmp = node->parent_;
  if (tmp->left_ == node) {
    tmp->left_ = nullptr;
  } else {
    tmp->right_ = nullptr;
  }
};

//--------------------------------DELETECASE2--------------------------------

template <typename Key, typename T>
void Map<Key, T>::DeleteCase2(Node *node) noexcept {
  Node *tmp;
  if (MinMax(node)) {
    tmp = MinRightElement(node->right_);
    PrevNextDoubleMin(node, tmp);
  } else {
    tmp = MaxLeftElement(node->left_);
    PrevNextDoubleMax(node, tmp);
  }
  DeleteElement(node);
};

//------------------------------------------------------------------

template <typename Key, typename T>
bool Map<Key, T>::MinMax(Node *node) noexcept {
  int count_min = 0, count_max = 0;
  bool result = true;
  Node *node_min = node->right_;
  Node *node_max = node->left_;
  for (; node_max->right_; node_max = node_max->right_, count_max++) {
  }
  for (; node_min->left_; node_min = node_min->left_, count_min++) {
  }
  if (node_max->left_) {
    count_max++;
  }
  if (node_min->right_) {
    count_min++;
  }
  if (count_min < count_max) {
    result = false;
  }
  return result;
};

//------------------------------------------------------------------

template <typename Key, typename T>
typename Map<Key, T>::Node *Map<Key, T>::MinRightElement(Node *node) noexcept {
  for (; node->left_; node = node->left_) {
  };
  return node;
};

//------------------------------------------------------------------

template <typename Key, typename T>
typename Map<Key, T>::Node *Map<Key, T>::MaxLeftElement(Node *node) noexcept {
  for (; node->right_; node = node->right_) {
  };
  return node;
};

//------------------------------------------------------------------

template <typename Key, typename T>
void Map<Key, T>::PrevNextDoubleMin(Node *node, Node *tmp) noexcept {
  Node *righ = node->right_;
  Node *lef = node->left_;
  Node *parent = tmp->parent_;
  Node *chaild = tmp->right_;
  Node *parent_node = node->parent_;
  if (parent_node) {
    if (parent_node->left_ == node) {
      parent_node->left_ = tmp;
    } else {
      parent_node->right_ = tmp;
    }
  }
  if (_root == node) _root = tmp;
  if (tmp->is_red_) {
    tmp->is_red_ = node->is_red_;
    node->is_red_ = true;
  } else {
    tmp->is_red_ = node->is_red_;
    node->is_red_ = false;
  }
  if (lef) lef->parent_ = tmp;
  if (righ != tmp) righ->parent_ = tmp;
  tmp->left_ = node->left_;
  if (tmp != node->right_) tmp->right_ = node->right_;
  node->left_ = nullptr;
  if (chaild) {
    node->right_ = chaild;
  } else {
    node->right_ = nullptr;
  }
  if (parent != node) parent->left_ = node;
  tmp->parent_ = node->parent_;
  node->parent_ = parent;
};

//------------------------------------------------------------------

template <typename Key, typename T>
void Map<Key, T>::PrevNextDoubleMax(Node *node, Node *tmp) noexcept {
  Node *righ = node->right_;
  Node *lef = node->left_;
  Node *parent = tmp->parent_;
  Node *chaild = tmp->left_;
  Node *parent_node = node->parent_;
  if (parent_node) {
    if (parent_node->left_ == node) {
      parent_node->left_ = tmp;
    } else {
      parent_node->right_ = tmp;
    }
  }
  if (_root == node) _root = tmp;
  if (tmp->is_red_) {
    tmp->is_red_ = node->is_red_;
    node->is_red_ = true;
  } else {
    tmp->is_red_ = node->is_red_;
    node->is_red_ = false;
  }
  if (lef != tmp) lef->parent_ = tmp;
  if (righ) righ->parent_ = tmp;
  tmp->right_ = node->right_;
  if (tmp != node->left_) tmp->left_ = node->left_;
  node->right_ = nullptr;
  if (chaild) {
    node->left_ = chaild;
  } else {
    node->left_ = nullptr;
  }
  parent->right_ = node;
  tmp->parent_ = node->parent_;
  node->parent_ = parent;
};

//--------------------------------DELETECASE3--------------------------------

template <typename Key, typename T>
void Map<Key, T>::DeleteCase3(Node *node) noexcept {
  Node *tmp_chaild;
  Node *parent = node->parent_;
  if (node->left_) {
    tmp_chaild = node->left_;
  } else {
    tmp_chaild = node->right_;
  }
  if (parent) {
    if (parent->right_ == node) {
      parent->right_ = tmp_chaild;
    } else {
      parent->left_ = tmp_chaild;
    }
    tmp_chaild->parent_ = parent;
    tmp_chaild->is_red_ = node->is_red_;
  } else {
    _root = tmp_chaild;
    tmp_chaild->parent_ = nullptr;
  }
};

//--------------------------------DELETECASE4--------------------------------

template <typename Key, typename T>
void Map<Key, T>::DeleteCase4(Node *node) noexcept {
  if (CheckBrather(node)) {
    if (BratherChaild(node)) {
      Node *tmp = node->parent_;
      Node *brather = tmp->right_;
      Node *chaild;
      tmp->left_ = nullptr;
      if (CheckColorChaildRight(node)) {
        DeleteCase41(tmp, brather);
      } else {
        chaild = brather->left_;
        chaild->is_red_ = false;
        brather->is_red_ = true;
        DeleteCase42(chaild, brather, tmp);
      }
    } else {
      Node *tmp = node->parent_;
      tmp->left_ = nullptr;
      DeleteCase43(node);
    }
  } else {
    DeleteCase44(node->parent_->right_, node->parent_);
    node->parent_->left_ = nullptr;
  }
};

//------------------------------------------------------------------

template <typename Key, typename T>
void Map<Key, T>::BlackBrather(Node *node) noexcept {
  if (node == _root) {
    return;
  } else if (node == node->parent_->left_) {
    if (CheckBrather(node)) {
      if (BratherChaild(node)) {
        if (CheckColorChaildRight(node)) {
          DeleteCase41(node->parent_, node->parent_->right_);
        } else {
          node->parent_->right_->left_->is_red_ = false;
          node->parent_->right_->is_red_ = true;
          DeleteCase42(node->parent_->right_->left_, node->parent_->right_,
                       node->parent_);
        }
      } else {
        DeleteCase43(node);
      }
    } else {
      DeleteCase44(node->parent_->right_, node->parent_);
    }
  } else {
    if (CheckBrather(node)) {
      if (BratherChaild(node)) {
        if (CheckColorChaildLeft(node)) {
          DeleteCase51(node->parent_, node->parent_->left_);
        } else {
          node->parent_->left_->right_->is_red_ = false;
          node->parent_->left_->is_red_ = true;
          DeleteCase52(node->parent_->left_->right_, node->parent_->left_,
                       node->parent_);
          node->parent_->left_ = nullptr;
        }
      } else {
        DeleteCase53(node);
      }
    }
  }
};

//------------------------------------------------------------------

template <typename Key, typename T>
bool Map<Key, T>::CheckBrather(Node *node) noexcept {
  Node *parent = node->parent_;
  bool res = true;
  if (node == parent->left_) {
    if (!parent->right_) {
      res = true;
    } else {
      if (parent->right_->is_red_) {
        res = false;
      } else {
        res = true;
      }
    }
  } else {
    if (!parent->left_) {
      res = true;
    } else {
      if (parent->left_->is_red_) {
        res = false;
      } else {
        res = true;
      }
    }
  }
  return res;
};

//------------------------------------------------------------------

template <typename Key, typename T>
bool Map<Key, T>::BratherChaild(Node *node) noexcept {
  bool res = false;
  Node *parent = node->parent_;
  if (node == parent->left_) {
    parent = parent->right_;
  } else {
    parent = parent->left_;
  }
  if (parent) {
    if (parent->left_) {
      if (parent->left_->is_red_) {
        return true;
      }
    }
    if (parent->right_) {
      if (parent->right_->is_red_) {
        return true;
      }
    }
  } else {
    res = false;
  }
  return res;
};

//------------------------------------------------------------------

template <typename Key, typename T>
bool Map<Key, T>::CheckColorChaildRight(Node *node) noexcept {
  bool res = false;
  Node *parent = node->parent_;
  if (parent->right_) {
    Node *brather = parent->right_;
    if (brather->right_) {
      if (brather->right_->is_red_) {
        res = true;
      } else {
        res = false;
      }
    } else {
      res = false;
    }
  } else {
    res = false;
  }
  return res;
};

//--------------------------------DELETECASE4.1--------------------------------

template <typename Key, typename T>
void Map<Key, T>::DeleteCase41(Node *parent, Node *brather) noexcept {
  Node *right_chaild = brather->right_;
  brather->is_red_ = parent->is_red_;
  right_chaild->is_red_ = false;
  parent->is_red_ = false;
  LeftTurnNoRec(brather->left_, brather, parent);
};

//------------------------------------------------------------------

template <typename Key, typename T>
void Map<Key, T>::LeftTurnNoRec(Node *chaild_left, Node *brather,
                                Node *parent) noexcept {
  if (parent->parent_) {
    Node *tmp = parent->parent_;
    brather->parent_ = tmp;
    if (tmp->left_ == parent) {
      tmp->left_ = brather;
    } else {
      tmp->right_ = brather;
    }
  } else {
    brather->parent_ = nullptr;
    _root = brather;
  }
  parent->parent_ = brather;
  parent->right_ = chaild_left;
  brather->left_ = parent;
  if (chaild_left) {
    chaild_left->parent_ = parent;
  }
};

//--------------------------------DELETECASE4.2--------------------------------

template <typename Key, typename T>
void Map<Key, T>::DeleteCase42(Node *chaild, Node *brather,
                               Node *node) noexcept {
  chaild->parent_ = node;
  brather->left_ = chaild->right_;
  chaild->right_ = brather;
  node->right_ = chaild;
  brather->parent_ = chaild;
  DeleteCase41(node, brather->parent_);
};

//--------------------------------DELETECASE4.3--------------------------------

template <typename Key, typename T>
void Map<Key, T>::DeleteCase43(Node *node) noexcept {
  if (node->parent_->right_) node->parent_->right_->is_red_ = true;
  if (!node->parent_->is_red_) {
    BlackBrather(node->parent_);
  } else {
    node->parent_->is_red_ = false;
  }
};

//--------------------------------DELETECASE4.4--------------------------------

template <typename Key, typename T>
void Map<Key, T>::DeleteCase44(Node *brather, Node *parent) noexcept {
  Node *chaild = brather->left_;
  brather->is_red_ = false;
  parent->is_red_ = true;
  if (parent->parent_) {
    Node *grandpa = parent->parent_;
    brather->parent_ = grandpa;
    if (parent == grandpa->left_) {
      grandpa->left_ = brather;
    } else {
      grandpa->right_ = brather;
    }
  } else {
    brather->parent_ = nullptr;
  }
  brather->left_ = parent;
  parent->parent_ = brather;
  parent->right_ = chaild;
  if (chaild) {
    chaild->parent_ = parent;
  }
  if (!brather->parent_) {
    _root = brather;
  }
  BlackBrather(parent->left_);
};

//--------------------------------DELETECASE5--------------------------------

template <typename Key, typename T>
void Map<Key, T>::DeleteCase5(Node *node) noexcept {
  if (node == _root) {
    return;
  } else if (CheckBrather(node)) {
    if (BratherChaild(node)) {
      if (CheckColorChaildLeft(node)) {
        DeleteCase51(node->parent_, node->parent_->left_);
      } else {
        node->parent_->left_->right_->is_red_ = false;
        node->parent_->left_->is_red_ = true;
        DeleteCase52(node->parent_->left_->right_, node->parent_->left_,
                     node->parent_);
        node->parent_->right_ = nullptr;
      }
    } else {
      Node *tmp = node->parent_;
      DeleteCase53(node);
      tmp->right_ = nullptr;
    }
  } else {
    DeleteCase54(node->parent_->left_, node->parent_);
    node->parent_->right_ = nullptr;
  }
};

//------------------------------------------------------------------

template <typename Key, typename T>
bool Map<Key, T>::CheckColorChaildLeft(Node *node) noexcept {
  bool res = false;
  Node *parent = node->parent_;
  if (parent->left_) {
    Node *brather = parent->left_;
    if (brather->left_) {
      if (brather->left_->is_red_) {
        res = true;
      } else {
        res = false;
      }
    } else {
      res = false;
    }
  } else {
    res = false;
  }
  return res;
};

//--------------------------------DELETECASE5.1--------------------------------

template <typename Key, typename T>
void Map<Key, T>::DeleteCase51(Node *parent, Node *brather) noexcept {
  brather->is_red_ = parent->is_red_;
  if (brather->left_) {
    brather->left_->is_red_ = false;
  }
  parent->is_red_ = false;
  RightTurnNoRec(brather->right_, brather, parent);
};

//------------------------------------------------------------------

template <typename Key, typename T>
void Map<Key, T>::RightTurnNoRec(Node *chaild_right, Node *brather,
                                 Node *parent) noexcept {
  if (parent->parent_) {
    Node *tmp = parent->parent_;
    brather->parent_ = tmp;
    if (tmp->left_ == parent) {
      tmp->left_ = brather;
    } else {
      tmp->right_ = brather;
    }
  } else {
    brather->parent_ = nullptr;
    _root = brather;
  }
  parent->parent_ = brather;
  parent->left_ = chaild_right;
  brather->right_ = parent;
  if (chaild_right) {
    chaild_right->parent_ = parent;
  }
};

//--------------------------------DELETECASE5.2--------------------------------

template <typename Key, typename T>
void Map<Key, T>::DeleteCase52(Node *chaild, Node *brather,
                               Node *node) noexcept {
  chaild->parent_ = node;
  chaild->left_ = brather;
  node->left_ = chaild;
  brather->right_ = nullptr;
  brather->parent_ = chaild;
  DeleteCase51(node, brather->parent_);
};

//--------------------------------DELETECASE5.3--------------------------------

template <typename Key, typename T>
void Map<Key, T>::DeleteCase53(Node *node) noexcept {
  if (node->parent_->left_) node->parent_->left_->is_red_ = true;
  if (!node->parent_->is_red_) {
    BlackBrather(node->parent_);
  } else {
    node->parent_->is_red_ = false;
  }
};

//--------------------------------DELETECASE5.4--------------------------------

template <typename Key, typename T>
void Map<Key, T>::DeleteCase54(Node *brather, Node *parent) noexcept {
  Node *chaild = brather->right_;
  brather->is_red_ = false;
  parent->is_red_ = true;
  if (parent->parent_) {
    Node *grandpa = parent->parent_;
    brather->parent_ = grandpa;
    if (parent == grandpa->left_) {
      grandpa->left_ = brather;
    } else {
      grandpa->right_ = brather;
    }
  } else {
    brather->parent_ = nullptr;
  }
  brather->right_ = parent;
  parent->parent_ = brather;
  parent->left_ = chaild;
  if (chaild) {
    chaild->parent_ = parent;
  }
  if (!brather->parent_) {
    _root = brather;
  }
  BlackBrather(parent->right_);
};

//------------------------------------------------------------------

template <typename Key, typename T>
void Map<Key, T>::EndAddress() noexcept {
  if (_end == nullptr) {
    Key key = 0;
    T t = 0;
    _end = new Node(key, t);
  }
  _head->prev_ = _end;
  _tail->next_ = _end;
  _end->prev_ = _tail;
  _end->next_ = _head;
};

//*************************-------MAP--------**************************
//*************************-----PRIVATE------**************************
//*************************------------------**************************

template <typename Key, typename T>
std::pair<typename Map<Key, T>::Node *, bool> Map<Key, T>::FindPos(
    Key key) noexcept {
  Node *node(_root);
  Node *res = nullptr;
  std::pair<Map<Key, T>::Node *, bool> tmp;
  if (!node) {
    tmp.second = false;
  } else {
    tmp.first = Pos(key, node, res);
    if (tmp.first) {
      tmp.second = true;
    } else {
      tmp.second = false;
    }
  }
  return tmp;
};

//------------------------------------------------------------------

template <typename Key, typename T>
typename Map<Key, T>::Node *Map<Key, T>::Pos(Key key, Node *node,
                                             Node *res) noexcept {
  if (!node) {
    return nullptr;
  } else if (key < node->key_) {
    node = node->left_;
    res = Pos(key, node, res);
  } else if (key > node->key_) {
    node = node->right_;
    res = Pos(key, node, res);
  } else if (key == node->key_) {
    res = node;
    return res;
  }
  return res;
};

//------------------------------------------------------------------

template <typename Key, typename T>
void Map<Key, T>::MoveMap(Map &other) noexcept {
  Clear();
  _head = other._head;
  _end = other._end;
  _tail = other._tail;
  _next = other._next;
  _prev = other._prev;
  _root = other._root;
  _size = other._size;
  other._head = nullptr;
  other._end = nullptr;
  other._tail = nullptr;
  other._next = nullptr;
  other._prev = nullptr;
  other._root = nullptr;
  other._size = 0;
};

//------------------------------------------------------------------

template <typename Key, typename T>
void Map<Key, T>::CopyMap(const Map &other) noexcept {
  Clear();
  Node node;
  for (Map<Key, T>::MapIterator i = other.Begin(); i != other.End(); ++i) {
    node.key_ = i->first;
    node.value_ = i->second;
    Insert(node.key_, node.value_);
  }
};
}  // namespace s21

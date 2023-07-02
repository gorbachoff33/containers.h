// created by pizpotli
#include "s21_list.h"

namespace s21 {

//*************************------------------**************************
//*************************--ITERATOR-CONST--**************************
//*************************------------------**************************

using size_type = size_t;

template <class T>
bool s21::List<T>::ConstListIterator::operator==(
    const ConstListIterator other) const noexcept {
  if (this == &other) {
    return true;
  }
  return _node == other._node;
};

//--------------------------------------------------------------------

template <class T>
typename s21::List<T>::const_reference
s21::List<T>::ConstListIterator::operator*() noexcept {
  if (_node) return _node->value_;
  return error_;
};

//--------------------------------------------------------------------

template <class T>
typename s21::List<T>::ConstListIterator
s21::List<T>::ConstListIterator::operator++() noexcept {
  if (_node) {
    _node = _node->next_;
  }
  return _node;
};

//--------------------------------------------------------------------

template <class T>
typename s21::List<T>::ConstListIterator
s21::List<T>::ConstListIterator::operator++(int) noexcept {
  s21::List<T>::ConstListIterator tmp;
  if (_node) {
    _node = _node->next_;
  }
  return _node;
};

//--------------------------------------------------------------------

template <class T>
typename s21::List<T>::ConstListIterator
s21::List<T>::ConstListIterator::operator--() noexcept {
  if (_node) {
    _node = _node->prev_;
  }
  return _node;
};

//--------------------------------------------------------------------

template <class T>
typename s21::List<T>::ConstListIterator
s21::List<T>::ConstListIterator::operator--(int) noexcept {
  if (_node) {
    _node = _node->prev_;
  }
  return _node;
};

//*************************------------------**************************
//*************************-----ITERATOR-----**************************
//*************************------------------**************************

template <class T>
typename s21::List<T>::value_type &
s21::List<T>::ListIterator::operator*() noexcept {
  if (this->_node) return this->_node->value_;
  return this->error_;
};

//*************************------------------**************************
//*************************---CONSTRUCTORS---**************************
//*************************------------------**************************
template <class T>
s21::List<T>::List() noexcept
    : _head(nullptr), _tail(nullptr), _end(nullptr), _size(0){};

//--------------------------------------------------------------------

template <class T>
s21::List<T>::List(size_type n)
    : _head(nullptr), _tail(nullptr), _end(nullptr), _size(0) {
  if (n < 1) {
    throw std::out_of_range(
        "s21_List::constructor:: Limit of the container is exceeded");
  }
  for (size_t i = 0; i < n; i++) {
    PushBack(0);
  }
};

//--------------------------------------------------------------------

template <class T>
s21::List<T>::List(std::initializer_list<value_type> const &items)
    : _head(nullptr), _tail(nullptr), _end(nullptr), _size(0) {
  for (auto it = items.begin(); it != items.end(); ++it) {
    PushBack(*it);
  }
};
//--------------------------------------------------------------------

template <class T>
s21::List<T>::List(const List &l) noexcept
    : _head(nullptr), _tail(nullptr), _end(nullptr), _size(0) {
  CopyList(l);
};

//--------------------------------------------------------------------

template <class T>
s21::List<T>::List(List &&l) noexcept
    : _head(nullptr), _tail(nullptr), _end(nullptr), _size(0) {
  MoveList(l);
};

//--------------------------------------------------------------------

template <class T>
s21::List<T>::~List() noexcept {
  Clear();
};

//--------------------------------------------------------------------

template <class T>
void s21::List<T>::operator=(List &&l) noexcept {
  MoveList(l);
};

//--------------------------------------------------------------------

template <class T>
void s21::List<T>::operator=(List &l) noexcept {
  CopyList(l);
};

//*************************------------------**************************
//*************************--ELEMENT ACCESS--**************************
//*************************------------------**************************

template <class T>
typename s21::List<T>::const_reference s21::List<T>::Front() noexcept {
  if (_head) return _head->value_;
  return zero_;
};

//--------------------------------------------------------------------

template <class T>
typename s21::List<T>::const_reference s21::List<T>::Back() noexcept {
  if (_tail) return _tail->value_;
  return zero_;
};

//*************************------------------**************************
//*****************************ITERATORS*******************************
//*************************------------------**************************

template <typename T>
typename s21::List<T>::Iterator s21::List<T>::Begin() const noexcept {
  return Iterator(_head);
};

//--------------------------------------------------------------------

template <typename T>
typename s21::List<T>::Iterator s21::List<T>::End() const noexcept {
  return Iterator(_end);
};

//*************************------------------**************************
//*************************-----CAPACITY-----**************************
//*************************------------------**************************

template <class T>
bool s21::List<T>::Empty() const noexcept {
  if (_head) {
    return false;
  } else {
    return true;
  }
};

//--------------------------------------------------------------------

template <typename T>
size_type s21::List<T>::Size() const noexcept {
  return _size;
};

//--------------------------------------------------------------------

template <class T>
size_type s21::List<T>::MaxSize() const noexcept {
  return std::numeric_limits<std::size_t>::max() / sizeof(T);
};

//*************************------------------**************************
//*************************-----MODIFIERS----**************************
//*************************------------------**************************

template <class T>
void s21::List<T>::Clear() noexcept {
  while (_head != _end && _head) {
    PopFront();
  }
  delete _end;
  _head = _tail = _end = nullptr;
  _size = 0;
};

//--------------------------------------------------------------------

template <typename T>
typename s21::List<T>::Iterator s21::List<T>::Insert(Iterator pos,
                                                     const_reference value) {
  Node *first = _head;
  for (auto it = Begin(); it != pos; first = first->next_, ++it) {
    if (!first) {
      throw std::out_of_range("s21_List::Insert:: list not found!");
    }
  }
  if (!_head) {
    PushFront(value);
    EndAddress();
    pos = _end;
  } else if (first && first->prev_ != _end) {
    Node *node = new Node(value);
    Node *left = first->prev_;
    node->prev_ = left;
    node->next_ = first;
    left->next_ = node;
    first->prev_ = node;
    _size++;
  } else {
    PushFront(value);
  }
  --pos;
  return pos;
};

//--------------------------------------------------------------------

template <class T>
void s21::List<T>::Erase(Iterator pos) {
  Node *tmp = _head;
  s21::List<T>::Iterator it = Begin();
  while (it != pos) {
    tmp = tmp->next_;
    ++it;
    if (tmp == _end) {
      throw std::out_of_range("s21::List::Erase not found position");
    }
  }
  if (it == _end) {
    throw std::out_of_range("s21::List::Erase position not found");
  }
  if (tmp->next_ == _end) {
    PopBack();
  } else if (tmp == _head) {
    PopFront();
  } else {
    Node *left = tmp->prev_;
    Node *right = tmp->next_;
    if (left) left->next_ = right;
    if (right) right->prev_ = left;
    delete tmp;
    _size--;
  }
};

//--------------------------------------------------------------------

template <class T>
void s21::List<T>::PushBack(const_reference value) {
  if (Size() >= MaxSize())
    throw std::out_of_range(
        "s21_List::PushFront:: Limit of the container is exceeded");
  Node *node = new Node(value);
  node->next_ = _end;
  node->prev_ = _tail;
  if (_tail) {
    _tail->next_ = node;
  }
  if (_size == 0) {
    _end = nullptr;
    _head = _tail = node;
  } else {
    _tail = node;
  }
  _size++;
  EndAddress();
};

//--------------------------------------------------------------------

template <class T>
void s21::List<T>::PopBack() noexcept {
  if (_head == _tail) {
    _head = nullptr;
  }
  if (_tail) {
    Node *newHead = _tail->prev_;
    delete _tail;
    _tail = newHead;
    _end->prev_ = _tail;
    _tail->next_ = _end;
    _size--;
  }
};

//--------------------------------------------------------------------

template <class T>
void s21::List<T>::PushFront(const_reference value) {
  if (Size() >= MaxSize())
    throw std::out_of_range(
        "s21_List::PushFront:: Limit of the container is exceeded");
  Node *node = new Node(value);
  node->next_ = _head;
  if (_head) {
    _head->prev_ = node;
  }
  if (_size == 0) {
    _end = nullptr;
    _head = _tail = node;
  } else {
    _head = node;
    EndAddress();
  }
  _size++;
};

//--------------------------------------------------------------------

template <class T>
void s21::List<T>::PopFront() noexcept {
  if (_head == _tail) {
    _tail = nullptr;
  }
  if (_head != _end) {
    Node *newHead = _head->next_;
    delete _head;
    _head = newHead;
    _end->next_ = _head;
    _head->prev_ = _end;
    _size--;
  }
};

//--------------------------------------------------------------------

template <class T>
void s21::List<T>::Swap(List &other) noexcept {
  List<T> tmp(*this);
  CopyList(other);
  other.CopyList(tmp);
};

//--------------------------------------------------------------------

template <class T>
void s21::List<T>::Merge(List &other) {
  Iterator own = Begin();
  Iterator iter = other.Begin();
  if (!_head) {
    for (; iter != other.End(); ++iter) {
      PushBack(*iter);
      EndAddress();
    }
  } else {
    for (; iter != other.End() && own != End();) {
      if (*iter <= *own) {
        Insert(own, *iter);
        ++iter;
      } else if (*iter > *own && own != End()) {
        ++own;
      }
      if (own == End()) {
        PushBack(*iter);
      }
    }
  }
  other.Clear();
};

//--------------------------------------------------------------------

template <class T>
void s21::List<T>::Splice(const_Iterator pos, List &other) {
  Iterator tmp = Begin();
  for (; tmp != pos; ++tmp) {
  }
  auto it = other.Begin();
  if (it == other.End() || (tmp == End() && _end)) {
    return;
  }
  if (!_head) {
    PushBack(*it);
    EndAddress();
    ++it;
    tmp = Begin();
    --tmp;
  }
  for (; it != other.End(); ++it) {
    Insert(tmp, *it);
  }
};

//--------------------------------------------------------------------

template <class T>
void s21::List<T>::Reverse() noexcept {
  Node *first = _head;
  Node *last = _tail;
  T tmp;
  for (; first != last && first != last->next_;) {
    tmp = first->value_;
    first->value_ = last->value_;
    last->value_ = tmp;
    first = first->next_;
    last = last->prev_;
  }
};

//--------------------------------------------------------------------

template <class T>
void s21::List<T>::Unique() {
  Node *tmp;
  Node *it = _head;
  for (auto i = Begin(); i != End(); ++i, it = it->next_) {
    if (i != Begin()) {
      if (tmp->value_ == *i) {
        Erase(i);
      }
    }
    tmp = it;
  }
};

//--------------------------------------------------------------------

template <class T>
void s21::List<T>::Sort() noexcept {
  T temp;
  Node *first = _head;
  for (; first != _end; first = first->next_) {
    Node *second = first->next_;
    for (; second != _end; second = second->next_) {
      if (second->value_ < first->value_) {
        temp = first->value_;
        first->value_ = second->value_;
        second->value_ = temp;
      }
    }
  }
};

//*************************------------------**************************
//*************************---PRIVATE FUNC---**************************
//*************************------------------**************************

template <class T>
void s21::List<T>::CopyList(const List &l) noexcept {
  Clear();
  for (auto i = l.Begin(); i != l.End(); i++) {
    PushBack(*i);
  }
};

//--------------------------------------------------------------------

template <class T>
void s21::List<T>::MoveList(List &l) noexcept {
  Clear();
  _head = l._head;
  _tail = l._tail;
  _size = l._size;
  _end = l._end;
  l._end = nullptr;
  l._head = nullptr;
  l._tail = nullptr;
  l._size = 0;
};

//--------------------------------------------------------------------

template <class T>
void s21::List<T>::EndAddress() noexcept {
  if (_end == nullptr) {
    _end = new Node(0);
    _end->next_ = nullptr;
  }
  _head->prev_ = _end;
  _tail->next_ = _end;
  _end->prev_ = _tail;
  _end->value_ = _size;
};

//--------------------------------------------------------------------

template <class T>
template <typename... Args>
void s21::List<T>::EmplaceFront(Args &&...args) {
  ([&] { PushFront(args); }(), ...);
};

//--------------------------------------------------------------------

template <class T>
template <typename... Args>
void s21::List<T>::EmplaceBack(Args &&...args) {
  ([&] { PushBack(args); }(), ...);
};

//--------------------------------------------------------------------

template <class T>
template <typename... Args>
typename s21::List<T>::Iterator s21::List<T>::Emplace(Iterator pos,
                                                      Args &&...args) {
  Iterator iter = pos;
  ([&] { Insert(pos, args); }(), ...);
  --iter;
  return iter;
};
}  // namespace s21

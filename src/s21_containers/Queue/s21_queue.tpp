// created by pizpotli
#include "s21_queue.h"

namespace s21 {

//*************************------------------**************************
//*************************---CONSTRUCTORS---**************************
//*************************------------------**************************

template <class T>
Queue<T>::Queue() noexcept : _head(nullptr), _prev(nullptr){};

//--------------------------------------------------------------------

template <class T>
Queue<T>::Queue(const std::initializer_list<T> &items)
    : _head(nullptr), _prev(nullptr) {
  for (auto i = items.begin(); i != items.end(); ++i) {
    NewElement(*i);
  }
};

//--------------------------------------------------------------------

template <class T>
Queue<T>::Queue(const Queue &q) noexcept : _head(nullptr), _prev(nullptr) {
  CopyQueue(q);
};

//--------------------------------------------------------------------

template <class T>
Queue<T>::Queue(Queue &&q) noexcept : _head(nullptr), _prev(nullptr) {
  MoveQueue(q);
};

//--------------------------------------------------------------------

template <class T>
Queue<T>::~Queue() noexcept {
  Clear();
};

//--------------------------------------------------------------------

template <class T>
void Queue<T>::operator=(Queue &&q) noexcept {
  MoveQueue(q);
};

//--------------------------------------------------------------------

template <class T>
void Queue<T>::operator=(Queue &q) noexcept {
  CopyQueue(q);
};

//*************************------------------**************************
//*************************--ELEMENT ACCESS--**************************
//*************************------------------**************************

template <class T>
const T &Queue<T>::Front() const noexcept {
  return _head->value_;
};

//--------------------------------------------------------------------

template <class T>
const T &Queue<T>::Back() const noexcept {
  return _prev->value_;
};

//*************************------------------**************************
//*************************-----CAPACITY-----**************************
//*************************------------------**************************

template <class T>
bool Queue<T>::Empty() const noexcept {
  if (_head)
    return false;
  else
    return true;
};

//--------------------------------------------------------------------

template <class T>
size_t Queue<T>::Size() const noexcept {
  return _size;
};

//*************************------------------**************************
//*************************-----MODIFIERS----**************************
//*************************------------------**************************

template <class T>
void Queue<T>::Push(const T &value) noexcept {
  NewElement(value);
};

//--------------------------------------------------------------------

template <class T>
void Queue<T>::Pop() noexcept {
  DeleteElement();
};

//--------------------------------------------------------------------

template <class T>
void Queue<T>::Swap(Queue &other) noexcept {
  Queue<T> tmp(*this);
  CopyQueue(other);
  other.CopyQueue(tmp);
};

template <class T>
template <typename... Args>
void Queue<T>::EmplaceBack(Args &&...args) {
  ([&] { Push(args); }(), ...);
}

//*************************------------------**************************
//*************************---PRIVATE FUNC---**************************
//*************************------------------**************************

template <class T>
void Queue<T>::CopyQueue(const Queue &other) noexcept {
  Clear();
  for (Node *tmp = other._head; tmp; tmp = tmp->next_) {
    NewElement(tmp->value_);
  }
};

//--------------------------------------------------------------------

template <class T>
void Queue<T>::Clear() noexcept {
  while (!Empty()) {
    DeleteElement();
  }
};

//--------------------------------------------------------------------

template <class T>
void Queue<T>::MoveQueue(Queue &other) noexcept {
  Clear();
  _head = other._head;
  _prev = other._prev;
  _size = other._size;
  other._head = nullptr;
  other._prev = nullptr;
  other._size = 0;
};

//--------------------------------------------------------------------

template <class T>
void Queue<T>::NewElement(const T &value) noexcept {
  Node *node = new Node(value);
  if (_size == 0) {
    _head = _prev = node;
  } else {
    _prev->next_ = node;
    _prev = node;
  }
  _size++;
};

//--------------------------------------------------------------------

template <class T>
void Queue<T>::DeleteElement() noexcept {
  if (_head) {
    Node *newHead = _head->next_;
    delete _head;
    _head = newHead;
    _size--;
  }
};
}  // namespace s21

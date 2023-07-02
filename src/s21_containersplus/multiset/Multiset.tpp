
// CONSTRUCTORS
namespace s21 {
template <typename T>
Multiset<T>::Multiset() noexcept
    : root_(nullptr), tail_(nullptr), end_node_(nullptr), m_size_(0) {
  nil_ = InsertNil();
}

template <typename T>
Multiset<T>::Multiset(
    std::initializer_list<Multiset<T>::value_type> const& items) {
  Node<T>* n;
  root_ = nullptr;
  tail_ = nullptr;
  end_node_ = nullptr;
  nil_ = InsertNil();
  m_size_ = 0;
  for (auto it = items.begin(); it != items.end(); it++) {
    n = InsertNode(*it, nullptr);
    n->pRight = nil_;
    n->pLeft = nil_;
    InsertBalance(n);
  }
}

template <typename T>
template <typename... Args>
std::vector<std::pair<typename Multiset<T>::iterator, bool>>
Multiset<T>::emplace(Args&&... args) {
  std::vector<std::pair<typename Multiset<T>::iterator, bool>> result;
  std::pair<typename Multiset<T>::iterator, bool> p;
  std::vector<T> buff = {args...};
  auto it = buff.begin();
  for (; it != buff.end(); it++) {
    p.first = insert(*it);
    p.second = true;
    result.push_back(p);
  }
  return result;
}

template <typename T>
Multiset<T>::Multiset(Multiset& s) noexcept {
  CopyMultiset(s);
}

template <typename T>
void Multiset<T>::CopyMultiset(Multiset& s) noexcept {
  root_ = nullptr;
  tail_ = nullptr;
  end_node_ = nullptr;
  m_size_ = 0;
  nil_ = InsertNil();
  Node<T>* n;
  for (iter it = s.begin(); it != s.end(); ++it) {
    n = InsertNode(*it, nullptr);
    n->pRight = nil_;
    n->pLeft = nil_;
    InsertBalance(n);
  }
}

template <typename T>
Multiset<T>::Multiset(Multiset&& s) noexcept {
  MoveFnc(s);
}
template <typename T>
void Multiset<T>::MoveFnc(Multiset& s) noexcept {
  nil_ = s.nil_;
  m_size_ = s.m_size_;
  root_ = s.root_;
  s.m_size_ = 0;
  s.nil_ = nullptr;
  s.root_ = nullptr;
}
template <typename T>
Multiset<T>::~Multiset() noexcept {
  DeleteAll(root_);
  if (nil_) {
    delete nil_;
  }
}
template <typename T>
typename Multiset<T>::iter Multiset<T>::find(const Key& key) noexcept {
  Node<T>* n = root_;
  while (true) {
    if (key > n->val) {
      if (n->pRight != nil_) {
        n = n->pRight;
      } else {
        iter i = n->pRight;
        return i;
      }

    } else if (key < n->val) {
      if (n->pLeft != nil_) {
        n = n->pLeft;
      } else {
        iter i = n->pLeft;
        return i;
      }
    } else {
      iter i = n;
      return i;
    }
  }
}

template <typename T>
bool Multiset<T>::contains(const Key& key) noexcept {
  iter it = begin();
  while (key != *it) {
    ++it;
    if (it == end()) {
      return false;
    }
  }
  return true;
}
// ITERATOR
template <typename T>
Multiset<T>::iter::iter() noexcept {
  elem_ = nullptr;
  lastvist_ = nullptr;
  lasvisit1_ = nullptr;
  last_left_ = nullptr;
  last_right_ = nullptr;
}
template <typename T>
Multiset<T>::iter::iter(Node<T>* o) noexcept {
  lastvist_ = nullptr;
  lasvisit1_ = nullptr;
  last_left_ = nullptr;
  last_right_ = nullptr;
  elem_ = o;
}
template <typename T>
Multiset<T>::iter::iter(const iter& o) noexcept {
  lastvist_ = nullptr;
  lasvisit1_ = nullptr;
  last_left_ = nullptr;
  last_right_ = nullptr;
  elem_ = o.elem_;
}
template <typename T>
bool Multiset<T>::iter::operator==(const iter& o) const noexcept {
  return this->elem_ == o.elem_;
}
template <typename T>
typename Multiset<T>::iter Multiset<T>::iter::operator=(
    const iter& o) noexcept {
  lastvist_ = nullptr;
  lasvisit1_ = nullptr;
  last_left_ = nullptr;
  last_right_ = nullptr;
  elem_ = o.elem_;
  return *this;
}

template <typename T>
void Multiset<T>::iter::operator++() noexcept {
  if (elem_->end) {
    elem_ = elem_->pRight;
    return;
  }
  if (elem_->parent) {
    if (elem_->parent->end) {
      elem_ = elem_->parent;
      return;
    }
  }
  if (elem_->pLeft && elem_->pLeft != lastvist_ && !elem_->pLeft->Nil &&
      elem_->pLeft != last_left_) {
    elem_ = elem_->pLeft;
    last_left_ = elem_;
  } else if (elem_->pRight && (!elem_->pRight->Nil) &&
             elem_->pRight != last_right_) {
    elem_ = elem_->pRight;
    last_right_ = elem_;
    while (elem_->pLeft && !elem_->pLeft->Nil) {
      elem_ = elem_->pLeft;
      last_left_ = elem_;
    }
  } else {
    if (elem_ == last_right_) {
      if (elem_->parent) {
        elem_ = elem_->parent;
      }
    }
    T a = elem_->val;
    do {
      lastvist_ = elem_;
      if (elem_->parent) {
        elem_ = elem_->parent;
      }
    } while (a > elem_->val);
  }
}
template <typename T>
void Multiset<T>::iter::operator--() noexcept {
  if (elem_->parent) {
    if (elem_->parent->end) {
      elem_ = elem_->parent;
      return;
    }
  }
  if (elem_->pLeft && elem_->pLeft != lasvisit1_ && !elem_->pLeft->Nil) {
    elem_ = elem_->pLeft;
    last_left_ = elem_;
    while (elem_->pRight && !elem_->pRight->Nil) {
      elem_ = elem_->pRight;
      last_right_ = elem_->pRight;
    }
  } else {
    T a = elem_->val;
    if (elem_ == last_left_) {
      do {
        lasvisit1_ = elem_;
        if (elem_->parent) {
          elem_ = elem_->parent;
        } else {
          return;
        }
      } while (a <= elem_->val);
      return;
    }

    do {
      lasvisit1_ = elem_;
      if (elem_->parent) {
        elem_ = elem_->parent;
      } else {
        return;
      }
    } while (a < elem_->val);
  }
}

template <typename T>
typename Multiset<T>::iter Multiset<T>::iter::operator=(Node<T>* o) noexcept {
  elem_ = o;
  lastvist_ = nullptr;
  lasvisit1_ = nullptr;
  return *this;
}

template <typename T>
bool Multiset<T>::iter::operator!=(const iter& o) const noexcept {
  return this->elem_ != o.elem_;
}

template <typename T>
T& Multiset<T>::iter::operator*() noexcept {
  if (elem_) {
    return elem_->val;
  } else {
    return nul_ref;
  }
}
// Insert and balance
template <typename T>
typename Multiset<T>::iterator Multiset<T>::insert(
    const value_type& value) noexcept

{
  Node<T>* n;
  iter t = root_;
  n = InsertNode(value, nullptr);
  n->pRight = nil_;
  n->pLeft = nil_;
  if (n) {
    t = n;
  }
  InsertBalance(n);
  return t;
}

template <typename T>
Node<T>* Multiset<T>::InsertNode(const value_type& i, Node<T>* r) noexcept {
  m_size_++;
  if (!root_) {
    root_ = new Node<T>(i, r);
    root_->color = BLACK_M;
    return root_;
  }
  Node<T>* n = root_;
  while (true) {
    if (i <= n->val) {
      if (n->pLeft && n->pLeft != nil_) {
        n = n->pLeft;
      } else {
        n->pLeft = new Node<T>(i, n);
        return n->pLeft;
      }
    } else if (i > n->val) {
      if (n->pRight && n->pRight != nil_) {
        n = n->pRight;
      } else {
        n->pRight = new Node<T>(i, n);
        return n->pRight;
      }
    } else if (i == n->val) {
      return root_;
    }
  }
}

template <typename T>
void Multiset<T>::InsertTail() noexcept {
  Node<T>* n = root_;
  if (n) {
    while (n->pRight && n->pRight != nil_) {
      n = n->pRight;
    }
    end_node_ = n;
    end_node_->end = true;
    end_node_->pRight->parent = end_node_;
  }
}

// Oter
template <typename T>
typename Multiset<T>::size_type Multiset<T>::max_size() const noexcept {
  return std::numeric_limits<std::size_t>::max() / sizeof(Node<T>);
}

template <typename T>
void Multiset<T>::DeleteAll(Node<T>* list) noexcept {
  if (list && list != nil_) {
    Node<T>* l = list->pLeft;
    Node<T>* r = list->pRight;
    delete list;
    if (l) {
      DeleteAll(l);
    }
    if (r) {
      DeleteAll(r);
    }
  }
}

template <typename T>
typename Multiset<T>::iter Multiset<T>::begin() const noexcept {
  Node<T>* n;
  if (!root_) {
    n = nullptr;
  } else {
    n = root_;
    while (n->pLeft && !n->pLeft->Nil) {
      n = n->pLeft;
    }
  }
  iter a = n;
  return a;
}
template <typename T>
typename Multiset<T>::iter Multiset<T>::end() noexcept {
  InsertTail();
  iter a;
  if (end_node_) {
    a = end_node_->pRight;
    *a = m_size_;
  } else {
  }
  return a;
}

template <typename T>
void Multiset<T>::swap(Multiset& other) noexcept {
  Multiset<T> buff(std::move(other));
  other.MoveFnc(*this);
  MoveFnc(buff);
}

template <typename T>
void Multiset<T>::operator=(Multiset&& s) noexcept {
  DeleteAll(root_);
  if (nil_) {
    delete nil_;
  }
  MoveFnc(s);
}

template <typename T>
bool Multiset<T>::empty() const noexcept {
  return !(m_size_ > 0);
}

template <typename T>
typename Multiset<T>::size_type Multiset<T>::size() const noexcept {
  return m_size_;
}

template <typename T>
void Multiset<T>::InsertBalance(Node<T>* x) noexcept {
  while (x != root_ && x->parent->color == RED_M) {
    // we have a violation
    if (x->parent == x->parent->parent->pLeft) {
      Node<T>* y = x->parent->parent->pRight;
      if (y->color == RED_M) {
        // uncle is RED_M
        x->parent->color = BLACK_M;
        y->color = BLACK_M;
        x->parent->parent->color = RED_M;
        x = x->parent->parent;
      } else {
        // uncle is BLACK_M
        if (x == x->parent->pRight) {
          // make x a left child
          x = x->parent;
          RotateLeft(x);
        }

        // recolor and rotate
        x->parent->color = BLACK_M;
        x->parent->parent->color = RED_M;
        RotateRight(x->parent->parent);
      }
    } else {
      // mirror image of above code
      Node<T>* y = x->parent->parent->pLeft;
      if (y->color == RED_M) {
        // uncle is RED_M
        x->parent->color = BLACK_M;
        y->color = BLACK_M;
        x->parent->parent->color = RED_M;
        x = x->parent->parent;
      } else {
        // uncle is BLACK_M
        if (x == x->parent->pLeft) {
          x = x->parent;
          RotateRight(x);
        }
        x->parent->color = BLACK_M;
        x->parent->parent->color = RED_M;
        RotateLeft(x->parent->parent);
      }
    }
  }
  root_->color = BLACK_M;
}

template <typename T>
void Multiset<T>::RotateLeft(Node<T>* x) noexcept {
  // rotate node x to left

  Node<T>* y = x->pRight;

  // establish x->right link
  x->pRight = y->pLeft;
  if (y->pLeft != nil_) y->pLeft->parent = x;

  // establish y->parent link
  if (y != nil_) y->parent = x->parent;
  if (x->parent) {
    if (x == x->parent->pLeft)
      x->parent->pLeft = y;
    else
      x->parent->pRight = y;
  } else {
    root_ = y;
  }

  // link x and y
  y->pLeft = x;
  if (x != nil_) x->parent = y;
}

template <typename T>
void Multiset<T>::RotateRight(Node<T>* x) noexcept {
  // rotate node x to right

  Node<T>* y = x->pLeft;

  // establish x->left link
  x->pLeft = y->pRight;
  if (y->pRight != nil_) y->pRight->parent = x;

  // establish y->parent link
  if (y != nil_) y->parent = x->parent;
  if (x->parent) {
    if (x == x->parent->pRight)
      x->parent->pRight = y;
    else
      x->parent->pLeft = y;
  } else {
    root_ = y;
  }

  // link x and y
  y->pRight = x;
  if (x != nil_) x->parent = y;
}
template <typename T>
void Multiset<T>::Erase(typename Multiset<T>::iter it) noexcept {
  Node<T>*x, *y, *z;
  z = it.elem_;
  if (z->pLeft == nil_ || z->pRight == nil_) {
    // y has a SENTINEL node as a child
    y = z;
  } else {
    // find tree successor with a SENTINEL node as a child
    y = z->pRight;
    while (y->pLeft != nil_) y = y->pLeft;
  }

  // x is y's only child
  if (y->pLeft != nil_)
    x = y->pLeft;
  else
    x = y->pRight;

  // remove y from the parent chain
  x->parent = y->parent;
  if (y->parent)
    if (y == y->parent->pLeft)
      y->parent->pLeft = x;
    else
      y->parent->pRight = x;
  else
    root_ = x;

  if (y != z) {
    z->val = y->val;
  }

  if (y->color == BLACK_M) DeleteFixup(x);
  m_size_--;
  free(y);
}

template <typename T>
void Multiset<T>::DeleteFixup(Node<T>* x) noexcept {
  // maintain RED_M-BLACK_M tree balance
  // after deleting node x

  while (x != root_ && x->color == BLACK_M) {
    if (x == x->parent->pLeft) {
      Node<T>* w = x->parent->pRight;
      if (w->color == RED_M) {
        w->color = BLACK_M;
        x->parent->color = RED_M;
        RotateLeft(x->parent);
        w = x->parent->pRight;
      }
      if (w->pLeft->color == BLACK_M && w->pRight->color == BLACK_M) {
        w->color = RED_M;
        x = x->parent;
      } else {
        if (w->pRight->color == BLACK_M) {
          w->pLeft->color = BLACK_M;
          w->color = RED_M;
          RotateRight(w);
          w = x->parent->pRight;
        }
        w->color = x->parent->color;
        x->parent->color = BLACK_M;
        w->pRight->color = BLACK_M;
        RotateLeft(x->parent);
        x = root_;
      }
    } else {
      Node<T>* w = x->parent->pLeft;
      if (w->color == RED_M) {
        w->color = BLACK_M;
        x->parent->color = RED_M;
        RotateRight(x->parent);
        w = x->parent->pLeft;
      }
      if (w->pRight->color == BLACK_M && w->pLeft->color == BLACK_M) {
        w->color = RED_M;
        x = x->parent;
      } else {
        if (w->pLeft->color == BLACK_M) {
          w->pRight->color = BLACK_M;
          w->color = RED_M;
          RotateLeft(w);
          w = x->parent->pLeft;
        }
        w->color = x->parent->color;
        x->parent->color = BLACK_M;
        w->pLeft->color = BLACK_M;
        RotateRight(x->parent);
        x = root_;
      }
    }
  }
  x->color = BLACK_M;
}

template <typename T>
void Multiset<T>::merge(Multiset& other) noexcept {
  for (iterator it = other.begin(); it != other.end(); ++it) {
    insert(*it);
  }
  other.clear();
}

template <typename T>
void Multiset<T>::clear() {
  Multiset<T> buff(std::move(*this));
}

template <typename T>
std::pair<typename Multiset<T>::iterator, typename Multiset<T>::iterator>
Multiset<T>::equal_range(const Key& key) noexcept {
  std::pair<iterator, iterator> m;
  m.first = lower_bound(key);
  m.second = upper_bound(key);
  return m;
}

template <typename T>
void sw(T* first, T* second) {
  T* buff = first;
  first = second;
  second = buff;
}

template <typename T>
Multiset<T>::const_iter::const_iter() noexcept : iter() {}
template <typename T>
Multiset<T>::const_iter::const_iter(Node<T>* o) noexcept : iter(o) {}

template <typename T>
Multiset<T>::const_iter::const_iter(const iter& o) noexcept : iter(o) {}

template <typename T>
typename Multiset<T>::const_iter Multiset<T>::const_iter::operator=(
    const iter& o) noexcept {
  this->elem_ = o->elem_;
  return *this;
}

template <typename T>
typename Multiset<T>::const_reference
Multiset<T>::const_iter::operator*() noexcept {
  return (this->elem_->val);
}
template <typename T>
typename Multiset<T>::iterator Multiset<T>::lower_bound(
    const Key& key) noexcept {
  iterator it = begin();
  while (*it < key) {
    ++it;
  }
  return it;
}

template <typename T>
typename Multiset<T>::iterator Multiset<T>::upper_bound(
    const Key& key) noexcept {
  iterator it = begin();
  while (*it <= key) {
    ++it;
  }
  return it;
}

template <typename T>
typename Multiset<T>::size_type Multiset<T>::count(const Key& key) noexcept {
  size_type i = 0;
  iterator it = begin();
  while (it != end()) {
    if (*it == key) {
      i++;
    } else if (i > 0) {
      return i;
    }
    ++it;
  }
  return i;
}
}  // namespace s21
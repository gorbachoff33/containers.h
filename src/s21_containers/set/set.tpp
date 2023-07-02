
// CONSTRUCTORS
namespace s21 {
template <typename value_type>
Set<value_type>::Set() noexcept
    : root(nullptr), tail(nullptr), end_Set_node(nullptr), m_size(0) {
  nil = insert_nil();
}

template <typename value_type>
Set<value_type>::Set(
    std::initializer_list<Set<value_type>::value_type> const& items) noexcept {
  std::pair<Set_node<value_type>*, bool> n;
  root = nullptr;
  tail = nullptr;
  end_Set_node = nullptr;
  nil = insert_nil();
  m_size = 0;
  for (auto it = items.begin(); it != items.end(); it++) {
    n = insert_node(*it, nullptr);
    n.first->pRight = nil;
    n.first->pLeft = nil;
    InsertBalance(n.first);
  }
}

template <typename value_type>
template <typename... Args>
typename std::vector<std::pair<typename Set<value_type>::iterator, bool>>
Set<value_type>::emplace(Args&&... args) noexcept {
  std::vector<std::pair<typename Set<value_type>::iterator, bool>> result;
  std::vector<value_type> buff = {args...};
  auto it = buff.begin();
  for (; it != buff.end(); it++) {
    result.push_back(insert(*it));
  }
  return result;
}
template <typename value_type>
Set<value_type>::Set(Set& s) noexcept {
  CopySet(s);
}

template <typename value_type>
void Set<value_type>::CopySet(Set& s) noexcept {
  std::pair<Set_node<value_type>*, bool> n;
  root = nullptr;
  tail = nullptr;
  end_Set_node = nullptr;
  nil = insert_nil();
  m_size = 0;
  for (iterator it = s.begin(); it != s.end(); ++it) {
    value_type a = *it;
    n = insert_node(a, nullptr);
    n.first->pRight = nil;
    n.first->pLeft = nil;
    InsertBalance(n.first);
  }
}

template <typename value_type>
Set<value_type>::Set(Set&& s) noexcept {
  nil = s.nil;
  m_size = s.m_size;
  root = s.root;
  s.m_size = 0;
  s.nil = nullptr;
  s.root = nullptr;
}

template <typename value_type>
Set<value_type>::~Set() {
  DeleteAll(root);
  if (nil) {
    delete nil;
  }
}

template <typename value_type>
typename Set<value_type>::iterator Set<value_type>::find(
    const Key& key) noexcept {
  iterator it = begin();
  while (key != *it) {
    ++it;
    if (it == end()) {
      return it;
    }
  }
  return it;
}

template <typename value_type>
bool Set<value_type>::contains(const Key& key) noexcept {
  if (root) {
    iterator it = begin();
    while (key != *it) {
      ++it;
      if (it == end()) {
        return false;
      }
    }
    return true;
  }
  return false;
}
// ITERATORS
template <typename value_type>
Set<value_type>::iter::iter() noexcept {
  elem = nullptr;
  lastvist_ = nullptr;
  lasvisit1_ = nullptr;
}

template <typename value_type>
Set<value_type>::iter::iter(Set_node<value_type>* o) noexcept {
  elem = o;
}

template <typename value_type>
Set<value_type>::iter::iter(const iter& o) noexcept {
  elem = o.elem;
}

template <typename value_type>
bool Set<value_type>::iter::operator==(const iter& o) noexcept {
  return this->elem == o.elem;
}
template <typename value_type>
typename Set<value_type>::iter Set<value_type>::iter::operator=(
    const iter& o) noexcept {
  elem = o.elem;
  return *this;
}

template <typename value_type>
void Set<value_type>::iter::operator++() noexcept {
  if (elem->End) {
    elem = elem->pRight;
    return;
  }

  if (elem->pLeft && elem->pLeft != lastvist_ && !elem->pLeft->Nil) {
    elem = elem->pLeft;
  } else if (elem->pRight && !elem->pRight->Nil) {
    elem = elem->pRight;
    while (elem->pLeft && !elem->pLeft->Nil) {
      elem = elem->pLeft;
    }
  } else {
    value_type a = elem->val;
    while (a >= elem->val) {
      lastvist_ = elem;
      if (elem->parent) {
        elem = elem->parent;
      } else {
        return;
      }
    }
  }
}
template <typename value_type>
void Set<value_type>::iter::operator--() noexcept {
  if (elem->parent) {
    if (elem->parent->End) {
      elem = elem->parent;
      return;
    }
  }
  if (elem->pLeft && elem->pLeft != lasvisit1_ && !elem->pLeft->Nil) {
    elem = elem->pLeft;
    while (elem->pRight && !elem->pRight->Nil) {
      elem = elem->pRight;
    }
  } else {
    value_type a = elem->val;
    while (a <= elem->val) {
      lasvisit1_ = elem;
      if (elem->parent) {
        elem = elem->parent;
      } else {
        return;
      }
    }
  }
}

template <typename value_type>
typename Set<value_type>::iter Set<value_type>::iter::operator=(
    Set_node<value_type>* o) noexcept {
  elem = o;
  lastvist_ = nullptr;
  lasvisit1_ = nullptr;
  return *this;
}
template <typename value_type>
bool Set<value_type>::iterator::operator!=(const iterator& o) noexcept {
  return this->elem != o.elem;
}
template <typename value_type>
typename Set<value_type>::reference
Set<value_type>::iter::operator*() noexcept {
  if (elem) {
    return elem->val;
  } else {
    return nul_ref_;
  }
}

// Insert and balance
template <typename value_type>
std::pair<typename Set<value_type>::iterator, bool> Set<value_type>::insert(
    const value_type& value) noexcept

{
  std::pair<iterator, bool> m;
  m.second = false;
  std::pair<Set_node<value_type>*, bool> n;
  n = insert_node(value, nullptr);
  if (n.second) {
    n.first->pRight = nil;
    n.first->pLeft = nil;
    m.first = n.first;
    m.second = true;
    InsertBalance(n.first);
  }
  return m;
}

template <typename value_type>
std::pair<Set_node<value_type>*, bool> Set<value_type>::insert_node(
    const value_type& i, Set_node<value_type>* r) noexcept {
  std::pair<Set_node<value_type>*, bool> n;
  if (!root) {
    m_size++;
    root = new Set_node<value_type>(i, r);
    end_Set_node = root;
    root->color = BLACK;
    n.first = root;
    n.second = true;
    return n;
  }
  n.first = root;
  while (true) {
    if (i < n.first->val) {
      if (n.first->pLeft && n.first->pLeft != nil) {
        n.first = n.first->pLeft;
      } else {
        n.first->pLeft = new Set_node<value_type>(i, n.first);
        n.second = true;
        n.first = n.first->pLeft;
        m_size++;
        return n;
      }
    } else if (i > n.first->val) {
      if (n.first->pRight && n.first->pRight != nil) {
        n.first = n.first->pRight;
      } else {
        n.first->pRight = new Set_node<value_type>(i, n.first);
        end_Set_node = n.first->pRight;
        n.second = true;
        n.first = n.first->pRight;
        m_size++;
        return n;
      }
    } else if (i == n.first->val) {
      n.second = false;
      return n;
    }
  }
}

template <typename value_type>
void Set<value_type>::InsertTail() noexcept {
  Set_node<value_type>* n = root;
  if (n) {
    while (n->pRight && n->pRight != nil) {
      n = n->pRight;
    }
    end_Set_node = n;
    end_Set_node->End = true;
    end_Set_node->pRight->parent = end_Set_node;
  } else {
    end_Set_node = n;
  }
}

// Oter
template <typename value_type>
typename Set<value_type>::size_type Set<value_type>::max_size() noexcept {
  return std::numeric_limits<std::size_t>::max() / sizeof(value_type);
}

template <typename value_type>
void Set<value_type>::DeleteAll(Set_node<value_type>* list) noexcept {
  if (list && list != nil) {
    Set_node<value_type>* l = list->pLeft;
    Set_node<value_type>* r = list->pRight;
    delete list;
    list = nullptr;
    if (l) {
      DeleteAll(l);
    }
    if (r) {
      DeleteAll(r);
    }
  }
}

template <typename value_type>
typename Set<value_type>::iterator Set<value_type>::begin() const noexcept {
  Set_node<value_type>* n;
  if (!root) {
    n = nullptr;
  } else {
    n = root;
    while (n->pLeft && !n->pLeft->Nil) {
      n = n->pLeft;
    }
  }
  iterator a = n;
  return a;
}
template <typename value_type>
typename Set<value_type>::iterator Set<value_type>::end() noexcept {
  InsertTail();
  iterator a;
  if (end_Set_node) {
    a = end_Set_node->pRight;
    *a = m_size;
  }

  return a;
}

template <typename value_type>
void Set<value_type>::clear() noexcept {
  Set<value_type> a(std::move(*this));
}

template <typename value_type>
void Set<value_type>::swap(Set& other) noexcept {
  Set<value_type> buff(other);
  other.DeleteAll(other.root);
  other.m_size = 0;
  if (other.nil) {
    delete other.nil;
  }
  other.CopySet(*this);
  DeleteAll(root);
  m_size = 0;
  if (nil) {
    delete nil;
  }
  CopySet(buff);
}

template <typename value_type>
void Set<value_type>::operator=(Set&& s) noexcept {
  DeleteAll(root);
  if (nil) {
    delete nil;
  }
  root = s.root;
  tail = s.tail;
  end_Set_node = s.end_Set_node;
  m_size = s.m_size;
  s.root = nullptr;
  s.tail = nullptr;
  s.end_Set_node = nullptr;
  s.m_size = 0;
}

template <typename value_type>
bool Set<value_type>::empty() noexcept {
  return !(m_size > 0);
}

template <typename value_type>
typename Set<value_type>::size_type Set<value_type>::size() noexcept {
  return m_size;
}

template <typename value_type>
void Set<value_type>::InsertBalance(Set_node<value_type>* x) noexcept {
  while (x != root && x->parent->color == RED) {
    if (x->parent == x->parent->parent->pLeft) {
      Set_node<value_type>* y = x->parent->parent->pRight;
      if (y->color == RED) {
        x->parent->color = BLACK;
        y->color = BLACK;
        x->parent->parent->color = RED;
        x = x->parent->parent;
      } else {
        if (x == x->parent->pRight) {
          x = x->parent;
          RotateLeft(x);
        }
        x->parent->color = BLACK;
        x->parent->parent->color = RED;
        RotateRight(x->parent->parent);
      }
    } else {
      Set_node<value_type>* y = x->parent->parent->pLeft;
      if (y->color == RED) {
        x->parent->color = BLACK;
        y->color = BLACK;
        x->parent->parent->color = RED;
        x = x->parent->parent;
      } else {
        if (x == x->parent->pLeft) {
          x = x->parent;
          RotateRight(x);
        }
        x->parent->color = BLACK;
        x->parent->parent->color = RED;
        RotateLeft(x->parent->parent);
      }
    }
  }
  root->color = BLACK;
}

template <typename value_type>
void Set<value_type>::RotateLeft(Set_node<value_type>* x) noexcept {
  Set_node<value_type>* y = x->pRight;
  x->pRight = y->pLeft;
  if (y->pLeft != nil) y->pLeft->parent = x;
  if (y != nil) y->parent = x->parent;
  if (x->parent) {
    if (x == x->parent->pLeft)
      x->parent->pLeft = y;
    else
      x->parent->pRight = y;
  } else {
    root = y;
  }
  y->pLeft = x;
  if (x != nil) x->parent = y;
}

template <typename value_type>
void Set<value_type>::RotateRight(Set_node<value_type>* x) noexcept {
  Set_node<value_type>* y = x->pLeft;
  x->pLeft = y->pRight;
  if (y->pRight != nil) y->pRight->parent = x;
  if (y != nil) y->parent = x->parent;
  if (x->parent) {
    if (x == x->parent->pRight)
      x->parent->pRight = y;
    else
      x->parent->pLeft = y;
  } else {
    root = y;
  }
  y->pRight = x;
  if (x != nil) x->parent = y;
}
template <typename value_type>
void Set<value_type>::Erase(typename Set<value_type>::iterator it) noexcept {
  Set_node<value_type>*x, *y, *z;
  z = it.elem;
  if (z->pLeft == nil || z->pRight == nil) {
    y = z;
  } else {
    y = z->pRight;
    while (y->pLeft != nil) y = y->pLeft;
  }
  if (y->pLeft != nil)
    x = y->pLeft;
  else
    x = y->pRight;
  x->parent = y->parent;
  if (y->parent)
    if (y == y->parent->pLeft)
      y->parent->pLeft = x;
    else
      y->parent->pRight = x;
  else
    root = x;

  if (y != z) {
    z->val = y->val;
  }
  if (y->color == BLACK) DeleteFixup(x);
  m_size--;
  delete (y);
}

template <typename value_type>
void Set<value_type>::DeleteFixup(Set_node<value_type>* x) noexcept {
  while (x != root && x->color == BLACK) {
    if (x == x->parent->pLeft) {
      Set_node<value_type>* w = x->parent->pRight;
      if (w->color == RED) {
        w->color = BLACK;
        x->parent->color = RED;
        RotateLeft(x->parent);
        w = x->parent->pRight;
      }
      if (w->pLeft->color == BLACK && w->pRight->color == BLACK) {
        w->color = RED;
        x = x->parent;
      } else {
        if (w->pRight->color == BLACK) {
          w->pLeft->color = BLACK;
          w->color = RED;
          RotateRight(w);
          w = x->parent->pRight;
        }
        w->color = x->parent->color;
        x->parent->color = BLACK;
        w->pRight->color = BLACK;
        RotateLeft(x->parent);
        x = root;
      }
    } else {
      Set_node<value_type>* w = x->parent->pLeft;
      if (w->color == RED) {
        w->color = BLACK;
        x->parent->color = RED;
        RotateRight(x->parent);
        w = x->parent->pLeft;
      }
      if (w->pRight->color == BLACK && w->pLeft->color == BLACK) {
        w->color = RED;
        x = x->parent;
      } else {
        if (w->pLeft->color == BLACK) {
          w->pRight->color = BLACK;
          w->color = RED;
          RotateLeft(w);
          w = x->parent->pLeft;
        }
        w->color = x->parent->color;
        x->parent->color = BLACK;
        w->pLeft->color = BLACK;
        RotateRight(x->parent);
        x = root;
      }
    }
  }
  x->color = BLACK;
}

template <typename value_type>
void Set<value_type>::merge(Set& other) noexcept {
  std::pair<Set<value_type>::iterator, bool> n;
  iterator it = other.begin();
  while (it != other.end()) {
    it = other.begin();
    for (; it != other.end(); ++it) {
      n = insert(*it);
      if (n.second) {
        other.Erase(it);
        break;
      }
    }
  }
}

template <typename value_type>
Set<value_type>::const_iter::const_iter() noexcept : iter() {}
template <typename value_type>
Set<value_type>::const_iter::const_iter(Set_node<value_type>* o) noexcept
    : iter(o) {}

template <typename value_type>
Set<value_type>::const_iter::const_iter(const iter& o) noexcept : iter(o) {}

template <typename value_type>
typename Set<value_type>::const_iter Set<value_type>::const_iter::operator=(
    const iter& o) noexcept {
  this->elem = o->elem;
  return *this;
}

template <typename value_type>
typename Set<value_type>::const_reference
Set<value_type>::const_iter::operator*() noexcept {
  return (this->elem->val);
}

}  // namespace s21
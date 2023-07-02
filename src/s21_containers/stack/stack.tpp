namespace s21 {
template <typename value_type>
Stack<value_type>::Stack() noexcept {
  size_all_ = 0;
  root_ = nullptr;
}

template <typename value_type>
Stack<value_type>::Stack(std::initializer_list<value_type> const &items) {
  size_all_ = 0;
  root_ = nullptr;
  for (auto it = items.begin(); it != items.end(); it++) {
    push(*it);
  }
}

template <typename value_type>
Stack<value_type>::Stack(const Stack &s) noexcept {
  size_all_ = 0;
  root_ = nullptr;
  node *st = s.root_;

  if (st) {
    size_all_++;
    root_ = new node;
    root_->value_ = st->value_;
    root_->next_ = nullptr;
    st = st->next_;
  }
  node *t_st = root_;
  while (st) {
    size_all_++;
    t_st->next_ = new node;
    t_st->next_->value_ = st->value_;
    t_st = t_st->next_;
    t_st->next_ = nullptr;
    st = st->next_;
  }
}

template <typename value_type>
Stack<value_type>::Stack(Stack &&s) noexcept {
  MoveFnc(s);
}

template <typename value_type>
void Stack<value_type>::MoveFnc(Stack &s) noexcept {
  size_all_ = s.size_all_;
  s.size_all_ = 0;
  root_ = s.root_;
  s.root_ = nullptr;
}

template <typename value_type>
Stack<value_type>::~Stack() noexcept {
  delete_all();
}

template <typename value_type>
void Stack<value_type>::operator=(Stack &&s) noexcept {
  delete_all();
  MoveFnc(s);
}

template <typename value_type>
void Stack<value_type>::push(const_reference value) noexcept {
  size_all_++;
  node *st = new node;
  st->value_ = value;
  if (!root_) {
    root_ = st;
    root_->next_ = nullptr;
  } else {
    st->next_ = root_;
    root_ = st;
  }
}

template <typename value_type>
void Stack<value_type>::pop() noexcept {
  if (root_) {
    node *st = root_->next_;
    delete root_;
    root_ = st;
    size_all_--;
  }
}

template <typename value_type>
void Stack<value_type>::swap(Stack &other) noexcept {
  size_type buff = size_all_;
  size_all_ = other.size_all_;
  other.size_all_ = buff;
  node *s = other.root_;
  other.root_ = root_;
  root_ = s;
}

template <typename value_type>
typename Stack<value_type>::const_reference Stack<value_type>::top() {
  if (root_) {
    return root_->value_;
  } else {
    throw std::out_of_range("No root_");
    return root_->value_;
  }
}
template <typename value_type>
bool Stack<value_type>::empty() noexcept {
  return root_ == nullptr;
}
template <typename value_type>
typename Stack<value_type>::size_type Stack<value_type>::size() const noexcept {
  return size_all_;
}
template <typename value_type>
template <typename... Args>
void Stack<value_type>::emplace_front(Args &&...args) {
  std::vector<value_type> buff{args...};
  auto it = buff.begin();
  for (; it != buff.end(); it++) {
    push(*it);
  }
}
}  // namespace s21
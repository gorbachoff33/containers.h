
/*
IMPLEMENTATION FILE
*/
namespace s21 {
template <typename value_type>
void Vector<value_type>::MoveFnc(Vector<value_type>& v) noexcept {
  arr_ = v.arr_;
  m_size_ = v.m_size_;
  m_capacity_ = v.m_capacity_;
  v.arr_ = nullptr;
  v.m_size_ = 0;
  v.m_capacity_ = 0;
};

template <typename value_type>
void Vector<value_type>::reserve(size_type size) {
  if (size > m_capacity_) {
    value_type* buff = new value_type[size];
    for (size_t i = 0; i < m_size_; ++i) buff[i] = std::move(arr_[i]);
    delete[] arr_;
    arr_ = buff;
    m_capacity_ = size;
  }
}

// initializer list constructor (allows creating lists with initializer lists,
// see main.cpp)
template <typename value_type>
Vector<value_type>::Vector(std::initializer_list<value_type> const& items) {
  m_size_ = 0;
  m_capacity_ = 0;
  arr_ = new value_type[items.size()];
  int i = 0;
  for (auto it = items.begin(); it != items.end(); it++) {
    arr_[i] = *it;
    i++;
    m_size_++;
  }
  m_capacity_ = m_size_;
}

template <typename value_type>
typename Vector<value_type>::reference Vector<value_type>::at(size_type pos) {
  if (pos < m_size_ && arr_) {
    return arr_[pos];
  } else {
    throw std::out_of_range("No values");
  }
}

template <typename value_type>
void Vector<value_type>::push_back(const_reference v) {
  if (m_size_ == m_capacity_) {
    reserve((m_size_ + 1) * 2);
  }
  arr_[m_size_++] = v;
}
template <typename value_type>
typename Vector<value_type>::reference Vector<value_type>::operator[](
    size_type pos) {
  if (pos < m_size_ && arr_) {
    return arr_[pos];
  } else {
    throw std::out_of_range("Out of range");
    return nil_ref_;
  }
}

template <typename value_type>
typename Vector<value_type>::const_reference Vector<value_type>::front() {
  if (arr_) {
    return arr_[0];
  } else {
    throw std::out_of_range("No arr_ay");
    return nil_;
  }
}

template <typename value_type>
void Vector<value_type>::clear() noexcept {
  delete arr_;
  arr_ = nullptr;
  m_size_ = 0;
  m_capacity_ = 0;
}

template <typename value_type>
typename Vector<value_type>::const_reference Vector<value_type>::back() {
  if (arr_) {
    return arr_[m_size_ - 1];
  } else {
    throw std::out_of_range("No arr_ay");
    return nil_;
  }
}
template <typename value_type>
value_type* Vector<value_type>::data() noexcept {
  return arr_;
}
template <typename value_type>
bool Vector<value_type>::empty() const noexcept {
  return !m_size_;
}

template <typename value_type>
typename Vector<value_type>::size_type Vector<value_type>::size()
    const noexcept {
  return m_size_;
}

template <typename value_type>
typename Vector<value_type>::size_type Vector<value_type>::max_size()
    const noexcept {
  return std::numeric_limits<std::size_t>::max() / sizeof(value_type);
}

template <typename value_type>
typename Vector<value_type>::size_type Vector<value_type>::capacity() noexcept {
  return m_capacity_;
}
template <typename value_type>
typename Vector<value_type>::iter Vector<value_type>::insert(
    iter pos, const_reference value) noexcept {
  MoveRight(pos);
  *pos = value;
  return pos;
}
template <typename value_type>
void Vector<value_type>::MoveRight(iterator pos) noexcept {
  reserve(m_size_ * 2);
  iterator it = end();
  while (pos != it) {
    --it;
    *(it.elem + 1) = *it;
  }
  m_size_++;
}
template <typename value_type>
void Vector<value_type>::erase(iter pos) noexcept {
  iter it = begin();
  Vector buff;
  buff.reserve(m_size_ - 1);
  iter it_buff = buff.begin();
  while (it != end()) {
    if (it != pos) {
      buff.m_size_++;
      *it_buff = *it;
      ++it_buff;
    }
    ++it;
  }
  DeleteAll();
  MoveFnc(buff);
}
template <typename value_type>
typename Vector<value_type>::iter Vector<value_type>::begin() noexcept {
  iter k;
  k.elem = arr_;
  return k;
}

template <typename value_type>
typename Vector<value_type>::iter Vector<value_type>::end() noexcept {
  iter k;
  k.elem = &arr_[m_size_];
  return k;
}

template <typename value_type>
void Vector<value_type>::swap(Vector& other) noexcept {
  value_type* buff = other.arr_;
  other.arr_ = arr_;
  arr_ = buff;
  size_type sz = m_size_;
  m_size_ = other.m_size_;
  other.m_size_ = sz;
  size_type cz = m_capacity_;
  m_capacity_ = other.m_capacity_;
  other.m_capacity_ = cz;
}

template <typename value_type>
void Vector<value_type>::pop_back() noexcept {
  if (m_size_ > 0) {
    m_size_--;
  }
}

template <typename value_type>
void Vector<value_type>::shrink_to_fit() noexcept {
  m_capacity_ = m_size_;
  reserve(m_size_);
}

template <typename value_type>
template <typename... Args>
void Vector<value_type>::emplace_back(Args&&... args) {
  ([&] { push_back(args); }(), ...);
}
template <typename value_type>
template <typename... Args>
typename Vector<value_type>::iterator Vector<value_type>::emplace(
    typename Vector<value_type>::iterator pos, Args&&... args) {
  ([&] { insert(pos, args); }(), ...);
}

}  // namespace s21
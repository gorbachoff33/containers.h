#ifndef Set_NODE_H_INCLUDED
#define Set_NODE_H_INCLUDED
enum Set_node_colors { RED, BLACK };

template <typename T>
class Set_node {
 public:
  Set_node<T>* pLeft;
  Set_node<T>* pRight;
  Set_node<T>* parent;
  enum Set_node_colors color;
  bool Nil;
  bool End;
  T val;
  Set_node<T>() noexcept {
    pLeft = nullptr;
    pRight = nullptr;
    parent = nullptr;
    color = RED;
    Nil = false;
    End = false;
  }
  Set_node<T>(T val, Set_node<T>* p) noexcept {
    this->val = val;
    parent = p;
    pLeft = pRight = nullptr;
    color = RED;
    Nil = false;
    End = false;
  }
  Set_node<T>(Set_node<T>& o) noexcept { this = o; }
  Set_node<T>* operator=(Set_node<T>* o) noexcept {
    this->color = o->color;
    this->parent = o->parent;
    this->pLeft = o->pLeft;
    this->pRight = o->pRight;
    this->val = o->val;
    this->Nil = o->Nil;
    this->End = o->End;
  }
  ~Set_node<T>() {}
};

#endif  // Set_NODE_H_INCLUDED
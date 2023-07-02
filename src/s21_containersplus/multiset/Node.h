#ifndef NODE_H_INCLUDED
#define NODE_H_INCLUDED
enum node_colors { RED_M, BLACK_M };

template <typename T>
class Node {
 public:
  Node<T>* pLeft;
  Node<T>* pRight;
  Node<T>* parent;
  enum node_colors color;
  bool Nil;
  bool end;
  T val;
  Node<T>() {
    pLeft = nullptr;
    pRight = nullptr;
    parent = nullptr;
    color = RED_M;
    Nil = false;
    end = false;
  }
  Node<T>(T val, Node<T>* p) {
    this->val = val;
    parent = p;
    //  pLeft = insert_nil(p);
    //  pRight = insert_nil(p);
    pLeft = pRight = nullptr;
    color = RED_M;
    Nil = false;
    end = false;
  }
  Node<T>(Node<T>& o) { this = o; }
  Node<T>* operator=(Node<T>* o) {
    this->color = o->color;
    this->parent = o->parent;
    this->pLeft = o->pLeft;
    this->pRight = o->pRight;
    this->val = o->val;
    this->Nil = o->Nil;
    this->end = o->end;
  }
  ~Node<T>() {}
};

#endif  // NODE_H_INCLUDED
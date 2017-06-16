// http://codeforces.com/contest/809/submission/27251747

template<typename T, typename Compare>
class CartesianTree {
  static int GenRandom() {
    return rand() + (rand() << 16);
  }

  struct Node {
    T key;
    T add;
    int priority;
    Node *left, *right;

    Node(T key_) : key(key_), add(0), priority(GenRandom()), left(NULL), right(NULL) {}

    Node() : key(0), add(0), priority(0), left(NULL), right(NULL) {}
  };

  Node* root;

  Node* NewNode(T key) {
    return new Node(key);
  }

  void AddToSubtree(Node* node, T add) {
    if (node == NULL) {
      return;
    }
    node->add += add;
  }

  void Update(Node* node) {
    AddToSubtree(node->left, node->add);
    AddToSubtree(node->right, node->add);
    node->key += node->add;
    node->add = 0;
  }

  int GetSize(Node* root) const {
    if (root == NULL) {
      return 0;
    }
    return 1 + GetSize(root->left) + GetSize(root->right);
  }

  T FindMin(Node* root) {
    if (root == NULL) {
      return numeric_limits<T>::max();
    }
    Node *p = root;
    while (p->left != NULL) {
      Update(p);
      p = p->left;
    }
    Update(p);
    return p->key;
  }

public:
  CartesianTree() : root(NULL) {}

  CartesianTree(Node* root_) : root(root_) {}

  CartesianTree(const CartesianTree& other) {
    root = other.root;
  }

  CartesianTree& operator=(const CartesianTree& other) {
    root = other.root;
    return *this;
  }

  Node* Merge(Node* left, Node* right) {
    if (left == NULL) {
      return right;
    }
    if (right == NULL) {
      return left;
    }
    Update(left);
    Update(right);
    if (left->priority > right->priority) {
      left->right = Merge(left->right, right);
      Update(left);
      return left;
    }
    else {
      right->left = Merge(left, right->left);
      Update(right);
      return right;
    }
  }

  void MergeWith(const CartesianTree& other) {
    root = Merge(root, other.root);
  }

  pair<Node*, Node*> Split(Node* root, T key) {
    if (root == NULL) {
      return MP((Node*)NULL, (Node*)NULL);
    }
    Update(root);
    if (Compare()(root->key, key)) {
      pair<Node*, Node*> res = Split(root->right, key);
      root->right = res.first;
      Update(root);
      res.first = root;
      Update(res.first);
      return res;
    }
    else {
      pair<Node*, Node*> res = Split(root->left, key);
      root->left = res.second;
      Update(root);
      res.second = root;
      Update(res.second);
      return res;
    }
  }

  pair<CartesianTree, CartesianTree> Split(T key) {
    pair<Node*, Node*> res = Split(root, key);
    return MP(CartesianTree(res.first), CartesianTree(res.second));
  }

  void PrintTree(Node* root) {
    if (!root) {
      return;
    }
    PrintTree(root->left);
    cerr << root->key << ' ' << root->value << ' ' << root->add << endl;
    PrintTree(root->right);
  }

  void PrintTree() {
    PrintTree(root);
  }

  void Insert(T key) {
    pair<Node*, Node*> p = Split(root, key);
    Node* new_node = NewNode(key);
    p.first = Merge(p.first, new_node);
    root = Merge(p.first, p.second);
  }

  void AddAll(T add) {
    if (root == NULL) {
      return;
    }
    root->add += add;
  }

  int GetSize() const {
    return GetSize(root);
  }

  T FindMin() {
    return FindMin(root);
  }
};

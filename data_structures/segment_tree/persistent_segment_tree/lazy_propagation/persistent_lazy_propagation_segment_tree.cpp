struct Node {
  Long sum;
  Long lazy;
  Node* left;
  Node* right;
  Node(Long sum = 0, Node* left = nullptr, Node* right = nullptr) :
    sum(sum), left(left), right(right), lazy(0) {}
  pair<Node*, Node*> Push(int tl, int tr) {
    int tm = (tl + tr) / 2;
    // Aplicar
    Node* new_left = new Node(*left);
    Node* new_right = new Node(*right);
    new_left += lazy * (tm - tl + 1);
    new_right += lazy * (tr - tm);
    // Propagar
    new_left->lazy += lazy;
    new_right->lazy += lazy;
    // No reiniciar ya que obtendremos un nuevo nodo en la nueva version
    return {new_left, new_right};
  }
};
struct SegmentTree {
  int n;
  vector<Node*> roots;
  Long Merge(const Long& x, const Long& y) { return x + y; }
  Node* Merge(Node* left, Node* right) { return new Node(Merge(left->sum, right->sum), left, right); }
  Node* Build(const vector<Long>& a, int tl, int tr) {
    if (tl == tr) return new Node(a[tl]);
    int tm = (tl + tr) / 2;
    return Merge(Build(a, tl, tm), Build(a, tm + 1, tr));
  }
  SegmentTree(void) {}
  SegmentTree(const vector<Long>& a) {
    n = a.size();
    roots = {Build(a, 0, n - 1)};
  }
  Long Query(int l, int r, Node* node, int tl, int tr) {
    if (l <= tl && tr <= r) return node->sum;
    int tm = (tl + tr) / 2;
    auto children = node->Push(tl, tr);
    if (r <= tm) return Query(l, r, children.first, tl, tm);
    if (tm < l) return Query(l, r, children.second, tm + 1, tr);
    return Merge(Query(l, r, children.first, tl, tm), Query(l, r, children.second, tm + 1, tr));
  }
  Long Query(int l, int r, int version = -1) {
    if (version == -1) version = roots.size() - 1;
    return Query(l, r, roots[version], 0, n - 1);
  }
  void Update(int l, int r, Long add, Node* node, int tl, int tr) {
    if (tr < l || tl > r) return;
    if (l <= tl && tr <= r) {
      // Aplicar
      node->sum += add * (tr - tl + 1);
      // Acumular
      node->lazy += add;
    } else {
      int tm = (tl + tr) / 2;
      auto children = node->Push(tl, tr);
      Update(l, r, add, children.first, tl, tm);
      Update(l, r, add, children.second, tm + 1, tr);
      *node = *Merge(children.first, children.second);
    }
  }
  void Update(int l, int r, Long add, int version = -1) {
    if (version == -1) version = roots.size() - 1;
    Node* new_root = new Node(*roots[version]);
    Update(l, r, val, new_root, 0, n - 1);
    roots.push_back(new_root);
  }
};

# Binary tree with traversal iterator

## examle

Construct a bintree, set its traversal mode from {PreOrder, InOrder, PostOrder}. Then .begin() and increment operator++ creates itreator in the designated traversal manner.

```cpp
  BinaryTree<int> tree;

  for (decltype(tree)::value_type i : {4, 2, 1, 3, 6, 5}) {
    tree.add(i);
  }
  
  tree.mode() = decltype(tree)::Mode::PostOrder;
  for (auto it = tree.begin(); it != tree.end(); ++it) {
    cout << it->val() << ": ";
    if (it->left())
      cout << "left: " << it->left()->val() << ", ";
    if (it->right())
      cout << "right: " << it->right()->val() << ", ";
    cout << endl;
  }
```

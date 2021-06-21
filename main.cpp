#include "bintree.hpp"

int test1() {
  using namespace std;

  BinaryTree<int> tree;

  for (auto &&i : {4, 2, 1, 3, 6, 5}) {
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
}

int test2() {
  using namespace std;

  BinaryTree<int> tree;

  for (auto &&i : {1, 2, 3, 4, 5, 6}) {
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
}

int test3() {
  using namespace std;

  BinaryTree<int> tree;

  for (auto &&i : {4, 2, 1, 3, 6, 5, 10, 9, 8, 15}) {
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
}

int main() {
  test1();
  test2();
  test3();
}

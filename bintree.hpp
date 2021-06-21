#include <iostream>
#include <iterator>
#include <memory>
#include <stack>

template <typename T> class Node {
public:
  using Ptr = std::shared_ptr<Node<T>>;
  using ConstPtr = const std::shared_ptr<Node<T>>;

public:
  explicit Node(T val)
      : val_(val), left_(nullptr), right_(nullptr), parent_(nullptr) {}
  Node(T val, Ptr left, Ptr right, Ptr parent)
      : val_(val), left_(left), right_(right), parent_(parent) {}
  const T &val() const { return val_; }
  Ptr &left() { return left_; }
  Ptr &right() { return right_; }
  Ptr &parent() { return parent_; }

private:
  const T val_;
  Ptr left_;
  Ptr right_;
  Ptr parent_;
};

template <typename T> class BinaryTree {
public:
  enum Mode { PreOrder, InOrder, PostOrder };

  explicit BinaryTree() : root_(nullptr), mode_(PreOrder) {}
  BinaryTree(T val) : root_(std::make_shared<Node<T>>(val)), mode_(PreOrder) {}
  BinaryTree(typename Node<T>::Ptr root) : root_(root), mode_(PreOrder) {}
  void add(T val) {
    auto iter = root_;
    decltype(iter) parent = nullptr;

    while (iter != nullptr) {
      if (val < iter->val()) {
        parent = iter;
        iter = iter->left();
      } else if (iter->val() < val) {
        parent = iter;
        iter = iter->right();
      } else
        // do nothing
        return;
    }
    if (parent == nullptr) {
      // root was nullptr(initial state)
      root_ = std::make_shared<Node<T>>(val);
      return;
    }
    if (val < parent->val()) {
      auto node = std::make_shared<Node<T>>(val);
      parent->left() = node;
      node->parent() = parent;
      return;
    } else {
      auto node = std::make_shared<Node<T>>(val);
      parent->right() = node;
      node->parent() = parent;
      return;
    }
  }
  void add(typename Node<T>::Ptr node);
  void print() { print(root_); }
  void print(typename Node<T>::ConstPtr node) {
    switch (mode_) {
    case PreOrder:
      printPre(node);
      break;
    case InOrder:
      printIn(node);
      break;
    case PostOrder:
      printPost(node);
      break;
    default:
      break;
    }
    std::cout << std::endl;
  }
  void printPre(typename Node<T>::Ptr node) {
    if (node == nullptr)
      return;

    std::cout << node->val() << ' ';
    printPre(node->left());
    printPre(node->right());
  }
  void printIn(typename Node<T>::Ptr node) {
    if (node == nullptr)
      return;

    printIn(node->left());
    std::cout << node->val() << ' ';
    printIn(node->right());
  }
  void printPost(typename Node<T>::Ptr node) {
    if (node == nullptr)
      return;

    printPost(node->left());
    printPost(node->right());
    std::cout << node->val() << ' ';
  }
  Mode &mode() { return mode_; }
  Mode mode() const { return mode_; }
  typename Node<T>::ConstPtr root() const { return root_; }

  class BinaryTreeIterator {
  private:
    using Iterator = typename Node<T>::Ptr;

  public:
    using iterator_category = std::forward_iterator_tag;
    using value_type = Node<T>;
    using difference_type = std::ptrdiff_t;
    using pointer = typename Node<T>::Ptr;
    using reference = Node<T> &;

    BinaryTreeIterator(const BinaryTree &tree, Iterator begin, bool done)
        : tree_(tree), mode_(tree.mode()), begin_(begin), cursor_(begin),
          current_(nullptr), done_(done) {
      setup();
    }
    void setup() {
      if (!done_ and mode_ == Mode::PreOrder) {
        stack_.push(begin_);
        cursor_ = next_node();
        return;
      }

      // https://www.geeksforgeeks.org/inorder-tree-traversal-without-recursion/
      if (!done_ and mode_ == Mode::InOrder) {
        current_ = begin_;
        while (current_ != nullptr) {
          stack_.push(current_);
          current_ = current_->left();
        }
        cursor_ = next_node();
      }

      // https://www.geeksforgeeks.org/iterative-postorder-traversal-using-stack/
      if (!done_ and mode_ == Mode::PostOrder) {
        current_ = begin_;
        while (current_ != nullptr) {
          if (current_->right())
            stack_.push(current_->right());
          stack_.push(current_);
          current_ = current_->left();
        }
        cursor_ = next_node();
      }
    }
    Iterator next_node() {
      if (mode_ == Mode::PreOrder) {
        if (stack_.empty())
          return nullptr;
        auto cur = stack_.top();
        stack_.pop();
        if (cur->right() != nullptr)
          stack_.push(cur->right());
        if (cur->left() != nullptr)
          stack_.push(cur->left());
        return cur;
      }

      // https://www.geeksforgeeks.org/inorder-tree-traversal-without-recursion/
      if (mode_ == Mode::InOrder) {
        while (current_ != nullptr) {
          stack_.push(current_);
          current_ = current_->left();
        }
        if (stack_.empty()) {
          return nullptr;
        }
        cursor_ = stack_.top();
        stack_.pop();
        current_ = cursor_->right();
        return cursor_;
      }

      // https://www.geeksforgeeks.org/iterative-postorder-traversal-using-stack/
      if (mode_ == Mode::PostOrder) {
        while (current_ != nullptr) {
          if (current_->right())
            stack_.push(current_->right());
          stack_.push(current_);
          current_ = current_->left();
        }
        if (stack_.empty()) {
          return nullptr;
        }
        current_ = stack_.top();
        stack_.pop();
        if (stack_.empty()) {
          cursor_ = current_;
          current_ = nullptr;
          return cursor_;
        }
        if (current_->right() and current_->right() == stack_.top()) {
          stack_.pop();
          stack_.push(current_);
          current_ = current_->right();
          return next_node();
        } else {
          cursor_ = current_;
          current_ = nullptr;
          return cursor_;
        }
      }
    }
    reference operator*() const { return *cursor_; }
    pointer operator->() const { return cursor_; }
    // Pre-increment
    BinaryTreeIterator &operator++() {
      if (done_)
        return *this;
      cursor_ = next_node();
      if (cursor_ == nullptr)
        done_ = true;
      return *this;
    };
    bool operator==(const BinaryTreeIterator &it) const {
      if (done_ and it.done_)
        return true;
      else if ((not done_) and (not it.done_))
        return cursor_ == it.cursor_;
      else
        return false;
    }
    bool operator!=(const BinaryTreeIterator &it) const {
      return !(*this == it);
    }

  private:
    const BinaryTree &tree_;
    const Mode mode_;
    const Iterator begin_;
    Iterator cursor_;
    Iterator current_; // on the tree
    bool done_;        // end() is done_ == true
    std::stack<typename Node<T>::Ptr> stack_;
  };
  using iterator = BinaryTreeIterator;

  iterator begin() { return BinaryTreeIterator(*this, root_, false); }
  iterator end() { return BinaryTreeIterator(*this, root_, true); }

private:
  typename Node<T>::Ptr root_;
  Mode mode_;
};

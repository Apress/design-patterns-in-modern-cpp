#include <iostream>
#include <vector>
using namespace std;

template <typename T>
struct Node
{
    T value;
    Node *left{nullptr}, *right{nullptr}, *parent{nullptr};

    Node(T value) : value(value) {}

    Node(T value, Node<T> *left, Node<T> *right) : value(value), left(left), right(right) {
      left->parent = right->parent = this;
    }

    void preorder_traversal_impl(Node<T>* current, vector<Node<T>*>& result)
    {
      result.push_back(current);
      if (current->left)
      {
        preorder_traversal_impl(current->left, result);
      }
      if (current->right)
      {
        preorder_traversal_impl(current->right, result);
      }
    }

    // traverse the node and its children preorder
    // and put all the results into `result`
    void preorder_traversal(vector<Node<T>*>& result)
    {
      preorder_traversal_impl(this, result);
    }
};

#include "gtest/gtest.h"

//#include "helpers/iohelper.h"

//#include "exercise.cpp"

namespace {

    class Evaluate : public ::testing::Test {};

    TEST_F(Evaluate, ExampleTest) {
      Node<char> c{'c'};
      Node<char> d{'d'};
      Node<char> e{'e'};
      Node<char> b{'b', &c, &d};
      Node<char> a{'a', &b, &e};

      vector<Node<char>*> result;
      a.preorder_traversal(result);

      ostringstream oss;
      for (auto n : result)
        oss << n->value;
      ASSERT_EQ("abcde", oss.str());
    }

}  // namespace

int main(int ac, char* av[])
{
  //::testing::GTEST_FLAG(catch_exceptions) = false;
  testing::InitGoogleTest(&ac, av);
  return RUN_ALL_TESTS();
}
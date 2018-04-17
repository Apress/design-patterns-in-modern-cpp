#pragma once

#include <experimental/coroutine>

// This class implements delegating (potentially recursive) recursive_generator.
// It supports two kind of yield expressions:
//
//    co_yield V;
//    co_yield G_of_T;
//
// Where V is a value convertible to T and
// G_of_T is a recursive_generator<T>.
//
// Usage example:
//
//   #include <stdio.h>
//   #include "recursive_recursive_generator.h"
//
//   recursive_generator<int> range(int start, int end, int step = 1) {
//     for (; start < end; start += step)
//       co_yield start;
//   }
//
//   recursive_generator<int> f() {
//     co_yield 1;
//     co_yield range(10, 15);
//     co_yield -1;
//     co_yield range(1000, 9999, 1000);
//   }
//
//   int main() {
//     for (auto v : f())
//       printf("%d ", v);
//     puts("");
//   }
//
// Output: 1 10 11 12 13 14 -1 1000 2000 3000 4000 5000 6000 7000 8000 9000


template <typename T> struct recursive_generator {
  struct promise_type;
  using handle = std::experimental::coroutine_handle<promise_type>;

  struct promise_type {
    T const *value;

    promise_type *prev;
    promise_type *top_or_root;

    promise_type *top() { return top_or_root; }
    promise_type *root() {
      if (is_root())
        return this;
      return top_or_root;
    }

    void set_top(promise_type *p) { top_or_root = p; }
    void set_root(promise_type *p) { top_or_root = p; }

    promise_type() : prev(this), top_or_root(this) {}

    bool is_root() { return prev == this; }

    T const &get() { return *value; }

    void resume() { handle::from_promise(*this)(); }
    bool done() { return handle::from_promise(*this).done(); }

    recursive_generator<T> get_return_object() { return { *this }; }

    auto initial_suspend() { return std::experimental::suspend_always{}; }

    auto final_suspend() { return std::experimental::suspend_always{}; }

    auto yield_value(T const &v) {
      value = &v;
      return std::experimental::suspend_always{};
    }

    auto yield_value(recursive_generator<T> &&v) {
      auto &inner = v.impl.promise();
      inner.prev = this;
      inner.top_or_root = root();
      inner.top_or_root->top_or_root = &v.impl.promise();

      inner.resume();

      struct suspend_if {
        bool _Ready;
        explicit suspend_if(bool _Condition) : _Ready(!_Condition) {}
        bool await_ready() { return _Ready; }
        void await_suspend(std::experimental::coroutine_handle<>) {}
        void await_resume() {}
      };

      return suspend_if(!top()->done());
    }

    void pull() {
      if (!top()->done()) {
        top()->resume();
      }
      while (top()->done()) {
        if (top()->is_root())
          return;

        top_or_root = top()->prev;
        top()->resume();
      }
    }
  };

  ~recursive_generator() {
    if (impl) {
      impl.destroy();
    }
  }

  struct iterator {
    handle rh;

    iterator(decltype(nullptr)) {}
    iterator(handle rh) : rh(rh) {}

    iterator &operator++() {
      rh.promise().pull();
      if (rh.done()) {
        rh = nullptr;
      }
      return *this;
    }

    bool operator!=(iterator const &rhs) { return rh != rhs.rh; }

    T const &operator*() { return rh.promise().top()->get(); }
  };

  iterator begin() {
    impl.promise().pull();
    if (impl.done())
      return { nullptr };
    return { impl };
  }

  iterator end() { return { nullptr }; }

  recursive_generator(recursive_generator const &) = delete;
  recursive_generator &operator=(recursive_generator const &) = delete;

  recursive_generator(recursive_generator &&rhs) : impl(rhs.impl) { rhs.impl = nullptr; }
  recursive_generator &operator=(recursive_generator &&rhs) = delete;

private:
  recursive_generator(promise_type &p) : impl(handle::from_promise(p)) {}

  handle impl;
};
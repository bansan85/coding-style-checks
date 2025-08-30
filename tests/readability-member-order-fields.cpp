// RUN: clang-tidy-19 -load=%clang_tidy_plugin --checks='readability-member-order' %s 2>&1 | FileCheck-19 %s --implicit-check-not="warning:"

// Correct order - should pass
class Example {
  static_assert(true);
public:
  // 1. Types and type aliases
  typedef int MyInt;
  using MyDouble = double;
  enum Status { OK, ERROR };
  class NestedClass {};
  friend class Helper;
  friend void print(const Example &e);

  // 3. Static constants
  static const int MAX_SIZE = 100;
  static constexpr double PI = 3.14;

  // 4. Factory functions
  static Example create();
  static Example fromDouble(const double &s);

  // 5. Constructors and assignment operators
  Example();
  Example(int value);
  Example(const Example &other);
  Example &operator=(const Example &other);
  Example &operator=(Example &&other);

  // 6. Destructor
  ~Example();

  // 7. Other functions
  void process();
  int getValue() const;
  static void helper();

  // 8. Other data members
  int value_;
  double name_;
  static int count_;
};

// Wrong order - constructor before factory function
class Bad1 {
public:
  static const int MAX = 10;
  Bad1();
  // CHECK: [[@LINE+1]]:15: warning: members should be ordered: types/aliases, data members (structs), static constants, factory functions, constructors/assignment, destructor, other functions, other data members [readability-member-order]
  static Bad1 create();
};

// Wrong order - function before destructor
class Bad2 {
public:
  Bad2();
  void process();
  // CHECK: [[@LINE+1]]:3: warning: members should be ordered: types/aliases, data members (structs), static constants, factory functions, constructors/assignment, destructor, other functions, other data members [readability-member-order]
  ~Bad2();
};

// Wrong order - data member before function
class Bad3 {
public:
  Bad3();
  ~Bad3();
  int value_;
  // CHECK: [[@LINE+1]]:8: warning: members should be ordered: types/aliases, data members (structs), static constants, factory functions, constructors/assignment, destructor, other functions, other data members [readability-member-order]
  void process();
};

// Wrong order - typedef after static constant
class Bad4 {
public:
  static const int MAX = 10;
  // CHECK: [[@LINE+1]]:15: warning: members should be ordered: types/aliases, data members (structs), static constants, factory functions, constructors/assignment, destructor, other functions, other data members [readability-member-order]
  typedef int MyInt;
};

// Struct with correct order including non-static data members
struct MyStruct {
public:
  // 1. Types
  enum Type { A, B };

  // 2. Non-static data members (allowed for structs)
  int x;
  double y;

  // 3. Static constants
  static const int DEFAULT_SIZE = 5;

  // 4. Factory functions
  static MyStruct create();

  // 5. Constructors
  MyStruct();
  MyStruct(int x, double y);

  // 7. Other functions
  void reset();
};

// Class with wrong struct-style data member placement
class Bad5 {
public:
  enum Type { A, B };
  int x;
  // CHECK: [[@LINE+1]]:20: warning: members should be ordered: types/aliases, data members (structs), static constants, factory functions, constructors/assignment, destructor, other functions, other data members [readability-member-order]
  static const int MAX = 10;
};

// Multiple access levels - each should have correct internal order
class MultiLevel {
public:
  typedef int PublicInt;
  static const int PUBLIC_MAX = 10;
  MultiLevel();
  void publicMethod();

protected:
  using ProtectedDouble = double;
  static const bool PROTECTED_FLAG = true;
  void protectedMethod();
  double protected_data_;

private:
  enum PrivateEnum { PRIV_A };
  static const int PRIVATE_MAX = 5;
  void privateMethod();
  int private_value_;
};

// Wrong order in private section
class BadPrivate {
public:
  void publicMethod();

private:
  void privateMethod();
  // CHECK: [[@LINE+1]]:8: warning: members should be ordered: types/aliases, data members (structs), static constants, factory functions, constructors/assignment, destructor, other functions, other data members [readability-member-order]
  enum PrivateEnum { A };
};

// Factory function detection
class Factory {
public:
  static Factory create();
  static Factory fromData(int data);

  static void helper();
  static int getCount();

  // CHECK: [[@LINE+1]]:3: warning: members should be ordered: types/aliases, data members (structs), static constants, factory functions, constructors/assignment, destructor, other functions, other data members [readability-member-order]
  Factory();
  void process();
};
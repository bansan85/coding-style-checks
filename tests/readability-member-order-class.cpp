// RUN: clang-tidy-19 -load=%clang_tidy_plugin --checks='readability-member-order' %s 2>&1 | FileCheck-19 %s --implicit-check-not="warning:"

class Class1 {
public:
  bool toto1;

protected:
  bool toto2;

private:
  bool toto3;
};

class Class2 {
protected:
  bool toto1;

// CHECK: [[@LINE+1]]:1: warning: member access specifiers should be in order: public, protected, private [readability-member-order]
public:
  bool toto2;
};

class Class3 {
private:
  bool toto1;

// CHECK: [[@LINE+1]]:1: warning: member access specifiers should be in order: public, protected, private [readability-member-order]
public:
  bool toto2;
};

class Class4 {
private:
  bool toto1;

// CHECK: [[@LINE+1]]:1: warning: member access specifiers should be in order: public, protected, private [readability-member-order]
protected:
  bool toto2;
};

class Class5 {
private:
  bool toto1;

// CHECK: [[@LINE+1]]:1: warning: member access specifiers should be in order: public, protected, private [readability-member-order]
protected:
  bool toto2;
};

class Class6 {
  bool toto1;

// CHECK: [[@LINE+1]]:1: warning: member access specifiers should be in order: public, protected, private [readability-member-order]
public:
  bool toto2;
};

class Class7 {
  bool toto1;

// CHECK: [[@LINE+1]]:1: warning: member access specifiers should be in order: public, protected, private [readability-member-order]
protected:
  bool toto2;
};

class Class8 {
  bool toto1;

private:
  bool toto2;
};

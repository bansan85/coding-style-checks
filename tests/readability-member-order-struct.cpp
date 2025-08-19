// RUN: clang-tidy-19 -load=%clang_tidy_plugin --checks='readability-member-order' %s 2>&1 | FileCheck-19 %s --implicit-check-not="warning:"

struct Struct1 {
public:
  bool toto1;

protected:
  bool toto2;

private:
  bool toto3;
};

struct Struct2 {
protected:
  bool toto1;

// CHECK: [[@LINE+1]]:1: warning: member access specifiers should be in order: public, protected, private [readability-member-order]
public:
  bool toto2;
};

struct Struct3 {
private:
  bool toto1;

// CHECK: [[@LINE+1]]:1: warning: member access specifiers should be in order: public, protected, private [readability-member-order]
public:
  bool toto2;
};

struct Struct4 {
private:
  bool toto1;

// CHECK: [[@LINE+1]]:1: warning: member access specifiers should be in order: public, protected, private [readability-member-order]
protected:
  bool toto2;
};

struct Struct5 {
private:
  bool toto1;

// CHECK: [[@LINE+1]]:1: warning: member access specifiers should be in order: public, protected, private [readability-member-order]
protected:
  bool toto2;
};

struct Struct6 {
  bool toto1;

public:
  bool toto2;
};

struct Struct7 {
  bool toto1;

protected:
  bool toto2;
};

struct Struct8 {
  bool toto1;

private:
  bool toto2;
};

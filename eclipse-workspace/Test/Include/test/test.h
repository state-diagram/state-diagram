/*   
   Copyright 2019-2020 XY State Contributors

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/

#ifndef TEST_H_
#define TEST_H_

#include <iostream>
#include <string>

using namespace std;

namespace test
{

using TestFun = void (* const)();

struct TestNode
{
  TestNode(string const & name, TestFun const userTest, string const & fileName, TestNode * const nxt);
  TestNode(TestNode const &) = delete;

  ~TestNode();

  void operator=(TestNode const &) = delete;

  size_t no;
  string const name;
  TestFun const testFun;
  string const fileName;
  bool pass;
  TestNode * const nxt;
};

class Test
{
private:
  static TestNode * s_fst;

public:
  Test(string const & name, TestFun const testFun, string const & fileName);
  ~Test();

  static int runAllTests();
  static int runTest(string const & nameOfTest);

private:
  static int runTests(string const & namePattern = "*");
};

class TestThrow
{
public:
  TestThrow(string const & fileName, int const lineNo);
  virtual ~TestThrow();

  string const fileName;
  int const lineNo;

  void msg() const;

protected:
  virtual void specific() const = 0;
};

class TestError
:
  public TestThrow
{
public:
  TestError(string const & fileName, int const lineNo);

private:
  void specific() const override;
};

class TestPass
:
  public TestThrow
{
public:
  TestPass(string const & fileName, int const lineNo);

private:
  void specific() const override;
};

int runAllTests();
int runTest(string const & nameOfTest);

} // namespace test

#define TEST(test) \
  class test \
  { \
  public: \
    static string const name; \
    static void testFun(); \
  }; \
  \
  string const test::name{#test}; \
  \
  Test test##_asTest{test::name, test::testFun, __FILE__}; \
  \
  void \
  test \
  ::testFun()

#define RUN_ALL_TESTS test::runAllTests()
#define RUN_TEST(name) test::runTest(#name)

#define ASSERT(expr) if (!(expr)) throw ::test::TestError(__FILE__, __LINE__)
#define ASSERT_RESET(var) if (!var) throw ::test::TestError(__FILE__, __LINE__); var = false
#define ASSERT_EQ(lft, rgt) try { ASSERT(lft == rgt); } \
  catch (::test::TestError const & testErr) \
  { \
    cout << endl; \
    cout << "ASSERT_EQ FAIL ON lft == \"" << lft << "\", rgt == \"" << rgt << "\"" << endl; \
    cout << endl; \
    cout.flush(); \
    throw; \
  }
#define PASS throw ::test::TestPass(__FILE__, __LINE__)

#endif




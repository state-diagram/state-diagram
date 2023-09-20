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

#include <test/test.h>

#include <algorithm>
#include <cassert>
#include <functional>
#include <iostream>
#include <vector>

namespace test
{

using namespace std;

TestNode
::TestNode(string const & name, TestFun const testFun, string const & fileName, TestNode * const nxt)
:
  name{name}
, testFun{testFun}
, fileName{fileName}
, pass{true}
, nxt{nxt}
{
  // This space intentionally left empty
}

TestNode
::~TestNode()
{
  if (nxt != nullptr)
  {
    delete nxt;
  }
}

TestNode * Test::s_fst = nullptr;

int
Test
::runAllTests()
{
  return runTests("*");
}

int
Test
::runTest(string const & name)
{
  return runTests(name);
}

int
Test
::runTests(string const & namePattern)
{
  string const dividingBar{"################################################################################################################"};

  size_t nrOfTests{0};

  for (TestNode * cur{s_fst}; cur != nullptr; cur = cur->nxt)
  {
    nrOfTests++;
  }
  vector<TestNode *> test{nrOfTests};

  {
    size_t idx{0};
    for (TestNode * cur{s_fst}; cur != nullptr; cur = cur->nxt, idx++)
    {
      test[idx] = cur;
    }
  }

  sort(test.begin(), test.end(), [](auto const lft, auto const rgt){return lft->name < rgt->name;});

  {
    size_t idx{0};
    for (auto const cur : test)
    {
      idx++;
      cur->no = idx;
    }
  }

  auto const runTest
  {
    [&]
    (TestNode * const test, size_t & runningNo, size_t & nrOfPasses, size_t & nrOfFails)
    {
      if ((namePattern != "*") && (test->name != namePattern))
      {
        return;
      }

      runningNo++;

      cout << dividingBar << endl;
      cout << "TEST CASE NO " << test->no << ": \""<< test->name << "\" (in file \"" << test->fileName << "\")" << endl;
      cout << "--" << endl;

      try
      {
        (*test->testFun)();

        cout << "--" << endl;
        cout << "PASSED" << endl;
      }
      catch (TestError const & err)
      {
        err.msg();
        test->pass = false;
      }
      catch (TestPass const & err)
      {
        err.msg();
      }
      catch (...)
      {
        cout << "--" << endl;
        cout << "UNKOWN USER ERROR" << endl;
        test->pass = false;
      }
      cout << endl;

      if (test->pass)
      {
        nrOfPasses++;
      }
      else
      {
        nrOfFails++;
      }
      assert (runningNo == nrOfPasses + nrOfFails);
    }
  };

  size_t runningNo{0};
  size_t nrOfPasses{0};
  size_t nrOfFails{0};
  for (auto const cur : test)
  {
    runTest(cur, runningNo, nrOfPasses, nrOfFails);
  }

  if ((namePattern == "*") && (nrOfFails > 0))
  {
    cout << ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> FAILS <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<" << endl;
    cout << endl;

    size_t runningNo{0};
    size_t nrOfPasses{0};
    size_t nrOfFails{0};
    {
      for (auto const cur : test)
      {
        if (!cur->pass)
        {
          runTest(cur, runningNo, nrOfPasses, nrOfFails);
        }
      }
    }
    cout << dividingBar << endl;
    cout << endl;
    for (auto const cur : test)
    {
      if (!cur->pass)
      {
        cout << "FAIL: \"" << cur->name << "\" (no " << cur->no << ")" << endl;
      }
    }
    cout << endl;
  }

  cout << "NR OF TESTS: " << nrOfTests << ", NR OF OK TESTS: " << nrOfPasses << ", NR OF FAILS: "<< nrOfFails << endl;

  return -nrOfFails;
}

Test
::Test(string const & name, TestFun const testFun, string const & fileName)
{
  s_fst = new TestNode(name, testFun, fileName, s_fst);
}

Test
::~Test()
{
  if (s_fst != nullptr)
  {
    delete s_fst;
    s_fst = nullptr;
  }
}

TestThrow
::TestThrow(string const & fileName, int const lineNo)
:
  fileName{fileName}
, lineNo{lineNo}
{
  // empty
}

TestThrow
::~TestThrow()
{
  // empty
}

void
TestThrow
::msg()
const
{
  cout << "--" << endl;
  cout << "FILE \"" << fileName << "\", LINE NO " << lineNo << ": "; specific(); cout << endl;
}

TestError
::TestError(string const & fileName, int const lineNo)
:
  TestThrow(fileName, lineNo)
{
  // empty
}

void
TestError
::specific()
const
{
  cout << "ASSERTION FAILURE";
}

TestPass
::TestPass(string const & fileName, int const lineNo)
:
  TestThrow(fileName, lineNo)
{
  // empty
}

void
TestPass
::specific()
const
{
  cout << "EXPLICIT PASS";
}

int
runAllTests()
{
  return Test::runAllTests();
}

int
runTest(string const & nameOfTest)
{
  return Test::runTest(nameOfTest);
}

} // namespace test

/*   
   Copyright 2019-2020 State Diagram Contributors

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

#include "StateDiagramTestSetup.h"

TEST(ExternalArrayDecl)
{
  try
  {
    FSM_TOP(top);
    FSM_ARRAY(int, array, 2, top);
  }
  catch (Error const & err)
  {
    cout << err.msg(); cout.flush();
    ASSERT(false);
  }
}

TEST(ExternalArraySetGet)
{
  try
  {
    FSM_TOP(top);
    FSM_ARRAY(int, array, 2, top);
    array[0].set(1);
    int const val{array[0].get()};
    ASSERT_EQ(val, 1);
  }
  catch (Error const & err)
  {
    cout << err.msg(); cout.flush();
    ASSERT(false);
  }
}

TEST(ExternalArrayInit)
{
  try
  {
    FSM_TOP(top);
    FSM_ARRAY(int, array, 2, top, {3, 4});
    int const val0{array[0].get()};
    int const val1{array[1].get()};
    ASSERT_EQ(val0, 3);
    ASSERT_EQ(val1, 4);
  }
  catch (Error const & err)
  {
    cout << err.msg(); cout.flush();
    ASSERT(false);
  }
}

#ifndef STATE_DIAGRAM_NO_CHECKS_WHILE_EXECUTING
TEST(ExternalArrayIdxError)
{
  try
  {
    FSM_TOP(top);
    FSM_ARRAY(int, array, 2, top);
    array[2].get();
    ASSERT(false);
  }
  catch (Array::IdxOutOfBoundsError const & err)
  {
    ASSERT_EQ(err.path, "array_0");
    ASSERT_EQ(err.capacity, 2);
    ASSERT_EQ(err.idx, 2);

    cout << err.msg(); cout.flush();
  }
  catch (Error const & err)
  {
    cout << err.msg(); cout.flush();
    ASSERT(false);
  }
}
#endif

TEST(ExternalArrayBookmark)
{
  try
  {
    FSM_TOP(top);
    FSM_ARRAY(int, array, 2, top);
    FSM_VAR(int, var, top);
    FSM_SIGNAL(void, signal, top);
  }
  catch (Error const & err)
  {
    cout << err.msg(); cout.flush();
    ASSERT(false);
  }
}

TEST(LocalArrayDeclState)
{
  try
  {
    FSM_TOP(top);
    FSM_LOCAL_ARRAY(int, array, 2, top);
  }
  catch (Error const & err)
  {
    cout << err.msg(); cout.flush();
    ASSERT(false);
  }
}

TEST(LocalArrayDeclRegion)
{
  try
  {
    FSM_TOP(top);
    FSM_REGION(region, top);
    FSM_LOCAL_ARRAY(int, array, 2, region);
  }
  catch (Error const & err)
  {
    cout << err.msg(); cout.flush();
    ASSERT(false);
  }
}

TEST(LocalArraySetGet)
{
  try
  {
    int val;

    FSM_TOP(top);
    FSM_LOCAL_ARRAY(int, array, 2, top);
    FSM_INIT(top);
    FSM_FINAL(top);
    FSM_AUTO(top_INIT, top_FINAL, Action([&](){array[0].set(1); val = array[0].get();}));

    top.init();
    top.step();

    ASSERT_EQ(val, 1);
  }
  catch (Error const & err)
  {
    cout << err.msg(); cout.flush();
    ASSERT(false);
  }
}

TEST(LocalArrayStateInit)
{
  try
  {
    int val0;
    int val1;

    FSM_TOP(top);
    FSM_LOCAL_ARRAY(int, array, 2, top, {3, 4});
    FSM_INIT(top);
    FSM_FINAL(top);
    FSM_AUTO(top_INIT, top_FINAL, Action([&](){val0 = array[0].get(); val1 = array[1].get();}));

    top.init();
    top.step();

    ASSERT_EQ(val0, 3);
    ASSERT_EQ(val1, 4);
  }
  catch (Error const & err)
  {
    cout << err.msg(); cout.flush();
    ASSERT(false);
  }
}

TEST(LocalArrayRegionInit)
{
  try
  {
    int val0;
    int val1;

    FSM_TOP(top);
    FSM_REGION(region, top);
    FSM_LOCAL_ARRAY(int, array, 2, region, {3, 4});
    FSM_INIT(region);
    FSM_FINAL(region);
    FSM_AUTO(region_INIT, region_FINAL, Action([&](){val0 = array[0].get(); val1 = array[1].get();}));

    top.init();
    top.step();

    ASSERT_EQ(val0, 3);
    ASSERT_EQ(val1, 4);
  }
  catch (Error const & err)
  {
    cout << err.msg(); cout.flush();
    ASSERT(false);
  }
}

#ifndef STATE_DIAGRAM_NO_CHECKS_WHILE_EXECUTING
TEST(LocalArrayIdxError)
{
  try
  {
    FSM_TOP(top);
    FSM_LOCAL_ARRAY(int, array, 2, top);
    FSM_INIT(top);
    FSM_FINAL(top);
    FSM_AUTO(top_INIT, top_FINAL, Action([&](){array[2].get();}));

    top.init();
    top.step();

    ASSERT(false);
  }
  catch (Array::IdxOutOfBoundsError const & err)
  {
    ASSERT_EQ(err.path, string() + "top" + pathComponentSeparator + "array_0");
    ASSERT_EQ(err.capacity, 2);
    ASSERT_EQ(err.idx, 2);

    cout << err.msg(); cout.flush();
  }
  catch (Error const & err)
  {
    cout << err.msg(); cout.flush();
    ASSERT(false);
  }
}
#endif

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

TEST(SignalInternalNameClashOnCompoundState)
{
  string const signalName("is");

  try
  {
    FSM_TOP(topState);

    LocalSignal<void> is_1(signalName, topState);
    LocalSignal<void> is_2(signalName, topState);

    ASSERT(false);
  }
  catch (LocalSignalDelegate::NameClashError const & err)
  {
    ASSERT_EQ(err.signalName, signalName);
    ASSERT_EQ(err.scopePath, "topState");

    cout << err.msg(); cout.flush();
  }
  catch (Error & err)
  {
    cout << err.msg(); cout.flush();
    ASSERT(false);
  }
}

TEST(SignalInternalNameClashOnRegion)
{
  string const signalName("is");

  try
  {
    FSM_TOP(topState);

    FSM_REGION(region, topState);

    LocalSignal<void> is_1(signalName, region);
    LocalSignal<void> is_2(signalName, region);

    ASSERT(false);
  }
  catch (LocalSignalDelegate::NameClashError const & err)
  {
    ASSERT_EQ(err.signalName, signalName);
    ASSERT_EQ(err.scopePath, string() + "topState" + pathComponentSeparator + "region");

    cout << err.msg(); cout.flush();
  }
  catch (Error & err)
  {
    cout << err.msg(); cout.flush();
    ASSERT(false);
  }
}


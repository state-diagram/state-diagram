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

TEST(RegionNameClash)
{
  string const regionName("region");

  try
  {
    FSM_TOP(topState);

    Region region_1(regionName, topState);
    Region region_2(regionName, topState);

    ASSERT(false);
  }
  catch (CompoundState::RegionError::Insertion::NameClash const & err)
  {
    ASSERT_EQ(err.regionName, regionName);
    ASSERT_EQ(err.compoundStatePath, "topState");

    cout << err.msg(); cout.flush();
  }
  catch (Error & err)
  {
    cout << err.msg(); cout.flush();
    ASSERT(false);
  }
}

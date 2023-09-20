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

TEST(ErrorOnInsertionByDefault1)
{
  try
  {
    FSM_TOP(topState);

    FSM_REGION(region, topState);

    FSM_INIT(topState);

    ASSERT(false);
  }
  catch (CompoundState::RegionError::Insertion::DefaultRegionRequest const & err)
  {
    ASSERT_EQ(err.nrOfNonDefaultRegions, static_cast<size_t>(1));
    ASSERT_EQ(err.compoundStatePath, "topState");

    cout << err.msg(); cout.flush();
  }
  catch (Error & err)
  {
    cout << err.msg(); cout.flush();
    ASSERT(false);
  }
}

TEST(ErrorOnInsertionByDefault2)
{
  string const regionName1("nonDefault_region_1");
  string const regionName2("nonDefault_region_2");

  try
  {
    FSM_TOP(topState);

    FSM_REGION(region1, topState);
    FSM_REGION(region2, topState);

    FSM_INIT(topState);

    ASSERT(false);
  }
  catch (CompoundState::RegionError::Insertion::DefaultRegionRequest const & err)
  {
    ASSERT_EQ(err.nrOfNonDefaultRegions, static_cast<size_t>(2));
    ASSERT_EQ(err.compoundStatePath, "topState");

    cout << err.msg(); cout.flush();
  }
  catch (Error & err)
  {
    cout << err.msg(); cout.flush();
    ASSERT(false);
  }
}

TEST(ExplicitRegionInsertion)
{
  string const regionName("nonDefault_region");

  try
  {
    FSM_TOP(topState);

    FSM_INIT(topState);

    Region region(regionName, topState);

    ASSERT(false);
  }
  catch (CompoundState::RegionError::Insertion::HasDefaultRegion const & err)
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

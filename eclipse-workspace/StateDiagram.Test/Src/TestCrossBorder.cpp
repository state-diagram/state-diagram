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

TEST(SimplestCrossBorderInit)
{
  try
  {
    bool sawT{false};

    FSM_TOP(top);

    FSM_INIT(top);
    FSM_STATE(state, top);

    FSM_FINAL(state);

    FSM_AUTO(top_INIT, state_FINAL, Action([&](){sawT = true;}));

    top.init();
    bool stat{top.step()};

    ASSERT(sawT);
    ASSERT(!stat);
  }
  catch (Error & err)
  {
    cout << err.msg(); cout.flush();
    ASSERT(false);
  }
}

TEST(CrossBorderInducedInit)
{
  try
  {
    bool sawT{false};

    FSM_TOP(top);

    FSM_INIT(top);
    FSM_STATE(nest, top);

    FSM_REGION(region1, nest);
    FSM_REGION(region2, nest);

    FSM_STATE(state1, region1);

    FSM_INIT(region2);
    FSM_STATE(inducee, region2);

    FSM_AUTO(top_INIT, state1);
    FSM_AUTO(region2_INIT, inducee, Action([&](){sawT = true;}));

    top.init();
    bool stat{top.step()};

    ASSERT(sawT);
    ASSERT(!stat);
  }
  catch (Error & err)
  {
    cout << err.msg(); cout.flush();
    ASSERT(false);
  }
}

TEST(DoubleCrossBorderInducedInit)
{
  try
  {
    struct Nesting
    {
      State root;

        Region region1;

        Region region2;
          Init init2;
          State inducee;
          Auto init2_TO_state2;

      Nesting(string const & name, Region const & ownerRegion)
      :
        root{name, ownerRegion}
      , region1{"region1", root}
      , region2{"region2", root}
      , init2{region2}
      , inducee{"inducee", region2}
      , init2_TO_state2{init2, inducee}
      {
        // empty
      }
    };

    FSM_TOP(top);

    FSM_REGION(topRegion, top);

    FSM_INIT(topRegion);
    Nesting nesting1{"nester1", topRegion};
    Nesting nesting2{"nester2", nesting1.region1};
    State inducer{"inducer", nesting2.region1};

    FSM_AUTO(topRegion_INIT, inducer);

    top.init();
    bool stat{top.step()};

    ASSERT(!stat);
    ASSERT(!topRegion_INIT.isCurrent());
    ASSERT(nesting1.root.isCurrent());
    ASSERT(nesting2.root.isCurrent());
    ASSERT(inducer.isCurrent());
    ASSERT(!nesting1.init2.isCurrent());
    ASSERT(nesting1.inducee.isCurrent());
    ASSERT(!nesting2.init2.isCurrent());
    ASSERT(nesting2.inducee.isCurrent());
  }
  catch (Error & err)
  {
    cout << err.msg(); cout.flush();
    ASSERT(false);
  }
}

TEST(SimplestUpwardCrossBorder)
{
  try
  {
    FSM_TOP(top);

    FSM_INIT(top);
    FSM_STATE(nest, top);
    FSM_FINAL(top);

    FSM_STATE(nested, nest);

    FSM_AUTO(top_INIT, nested);
    FSM_AUTO(nested, top_FINAL);

    top.init();
    top.step();
    bool const stat{top.step()};

    ASSERT(stat);
  }
  catch (Error & err)
  {
    cout << err.msg(); cout.flush();
    ASSERT(false);
  }
}

TEST(DoubleUpwardCrossBorder)
{
  try
  {
    struct Nesting
    {
      State root;

        Region region1;

        Region region2;
          Init init2;
          State inducee;
          Auto init2_TO_state2;

      Nesting(string const & name, Region const & ownerRegion)
      :
        root{name, ownerRegion}
      , region1{"region1", root}
      , region2{"region2", root}
      , init2{region2}
      , inducee{"inducee", region2}
      , init2_TO_state2{init2, inducee}
      {
        // empty
      }
    };

    FSM_TOP(top);

    FSM_SIGNAL(void, external, top);

    FSM_REGION(topRegion, top);

    FSM_INIT(topRegion);
    Nesting nesting1{"nester1", topRegion};
    Nesting nesting2{"nester2", nesting1.region1};
    State inducer{"inducer", nesting2.region1};
    FSM_FINAL(topRegion);

    FSM_AUTO(topRegion_INIT, inducer);
    FSM_STEP(inducer, topRegion_FINAL, Trigger(external));

    top.init();

    bool const statInit{top.step()};
    ASSERT(!statInit);
    ASSERT(!topRegion_INIT.isCurrent());
    ASSERT(nesting1.root.isCurrent());
    ASSERT(nesting2.root.isCurrent());
    ASSERT(inducer.isCurrent());
    ASSERT(!nesting1.init2.isCurrent());
    ASSERT(nesting1.inducee.isCurrent());
    ASSERT(!nesting2.init2.isCurrent());
    ASSERT(nesting2.inducee.isCurrent());
    ASSERT(!topRegion_FINAL.isCurrent());

    bool statStep{top.step(external)};
    ASSERT(statStep);
    ASSERT(!topRegion_INIT.isCurrent());

    ASSERT(!nesting1.root.isCurrent());
    ASSERT(nesting2.root.isCurrent());
    ASSERT(inducer.isCurrent());

    ASSERT(!nesting1.init2.isCurrent());
    ASSERT(nesting1.inducee.isCurrent());
    ASSERT(!nesting2.init2.isCurrent());
    ASSERT(nesting2.inducee.isCurrent());

    ASSERT(topRegion_FINAL.isCurrent());
  }
  catch (Error & err)
  {
    cout << err.msg(); cout.flush();
    ASSERT(false);
  }
}

TEST(UpwardToAncestor)
{
  try
  {
    bool sawEnterState{false};
    bool sawExitState{false};

    FSM_TOP(top);

    FSM_SIGNAL(void, global, top);

    FSM_INIT(top);
    FSM_STATE(state, top);
    FSM_ENTER(state, Action([&](){sawEnterState = true;}));
    FSM_EXIT(state, Action([&](){sawExitState = true;}));

    FSM_INIT(state);
    FSM_STATE(state_state, state);

    FSM_AUTO(top_INIT, state);

    FSM_AUTO(state_INIT, state_state);
    FSM_STEP(state_state, state, Trigger(global), Max1Flag());

    top.init();

    bool stat;

    stat = top.step();
    ASSERT(!stat);
    ASSERT(state.isCurrent());
    ASSERT(state_state.isCurrent());
    ASSERT(sawEnterState);
    ASSERT(!sawExitState);

    sawEnterState = false;

    stat = top.step(global);
    ASSERT(!stat);
    ASSERT(state.isCurrent());
    ASSERT(state_state.isCurrent());
    ASSERT(sawEnterState);
    ASSERT(sawExitState);
  }
  catch (Error & err)
  {
    cout << err.msg(); cout.flush();
    ASSERT(false);
  }
}

TEST(UpwardThenToAncestorViaAuto)
{
  try
  {
    bool sawEnterState{false};
    bool sawExitState{false};

    FSM_TOP(top);

    FSM_SIGNAL(void, global, top);

    FSM_INIT(top);
    FSM_STATE(state, top);
    FSM_ENTER(state, Action([&](){sawEnterState = true;}));
    FSM_EXIT(state, Action([&](){sawExitState = true;}));
    FSM_STATE(aux, top);

    FSM_INIT(state);
    FSM_STATE(state_state, state);

    FSM_AUTO(top_INIT, state);
    FSM_AUTO(state_INIT, state_state);
    FSM_STEP(state_state, aux, Trigger(global), Max1Flag());
    FSM_AUTO(aux, state);

    top.init();

    bool stat;

    stat = top.step();
    ASSERT(!stat);
    ASSERT(state.isCurrent());
    ASSERT(!aux.isCurrent());
    ASSERT(state_state.isCurrent());
    ASSERT(sawEnterState);
    ASSERT(!sawExitState);

    sawEnterState = false;

    stat = top.step(global);
    ASSERT(!stat);
    ASSERT(!state.isCurrent());
    ASSERT(aux.isCurrent());
    ASSERT(state_state.isCurrent());
    ASSERT(!sawEnterState);
    ASSERT(sawExitState);
  }
  catch (Error & err)
  {
    cout << err.msg(); cout.flush();
    ASSERT(false);
  }
}

TEST(UpwardToAncestorWithImplicitLeaveEnterTest)
{
  try
  {
    bool sawEnterState{false};
    bool sawExitState{false};

    bool sawEnterRegion1State1{false};
    bool sawExitRegion1State1{false};
    bool sawEnterRegion1State2{false};
    bool sawExitRegion1State2{false};

    bool sawEnterRegion2State{false};
    bool sawExitRegion2State{false};

    FSM_TOP(top);

    FSM_SIGNAL(void, global, top);
    FSM_SIGNAL(void, aux, top);

    FSM_INIT(top);
    FSM_STATE(state, top);
    FSM_ENTER(state, Action([&](){sawEnterState = true;}));
    FSM_EXIT(state, Action([&](){sawExitState = true;}));

    FSM_REGION(region1, state);
    FSM_REGION(region2, state);

    FSM_INIT(region1);
    FSM_STATE(region1_state1, region1);
    FSM_ENTER(region1_state1, Action([&](){sawEnterRegion1State1 = true;}));
    FSM_EXIT(region1_state1, Action([&](){sawExitRegion1State1 = true;}));
    FSM_STATE(region1_state2, region1);
    FSM_ENTER(region1_state2, Action([&](){sawEnterRegion1State2 = true;}));
    FSM_EXIT(region1_state2, Action([&](){sawExitRegion1State2 = true;}));

    FSM_INIT(region2);
    FSM_STATE(region2_state, region2);
    FSM_ENTER(region2_state, Action([&](){sawEnterRegion2State = true;}));
    FSM_EXIT(region2_state, Action([&](){sawExitRegion2State = true;}));

    FSM_AUTO(top_INIT, state);

    FSM_AUTO(region1_INIT, region1_state1);
    FSM_STEP(region1_state1, region1_state2, Trigger(aux));
    FSM_STEP(region1_state2, state, Trigger(global));

    FSM_AUTO(region2_INIT, region2_state);

    top.init();

    bool stat;

    stat = top.step();
    ASSERT(!stat);
    ASSERT(state.isCurrent());
    ASSERT(region1_state1.isCurrent());
    ASSERT(!region1_state2.isCurrent());
    ASSERT(region2_state.isCurrent());
    ASSERT(sawEnterState);
    ASSERT(!sawExitState);
    ASSERT(sawEnterRegion1State1);
    ASSERT(!sawExitRegion1State1);
    ASSERT(!sawEnterRegion1State2);
    ASSERT(!sawExitRegion1State2);
    ASSERT(sawEnterRegion2State);
    ASSERT(!sawExitRegion2State);

    sawEnterState = false;
    sawEnterRegion1State1 = false;
    sawEnterRegion2State = false;

    stat = top.step(aux);

    ASSERT(!stat);
    ASSERT(state.isCurrent());
    ASSERT(!region1_state1.isCurrent());
    ASSERT(region1_state2.isCurrent());
    ASSERT(region2_state.isCurrent());
    ASSERT(!sawEnterState);
    ASSERT(!sawExitState);
    ASSERT(!sawEnterRegion1State1);
    ASSERT(sawExitRegion1State1);
    ASSERT(sawEnterRegion1State2);
    ASSERT(!sawExitRegion1State2);
    ASSERT(!sawEnterRegion2State);
    ASSERT(!sawExitRegion2State);

    sawEnterState = false;
    sawExitRegion1State1 = false;
    sawEnterRegion1State2 = false;
    sawEnterRegion2State = false;

    stat = top.step(global);
    ASSERT(!stat);
    ASSERT(state.isCurrent());
    ASSERT(region1_state1.isCurrent());
    ASSERT(!region1_state2.isCurrent());
    ASSERT(region2_state.isCurrent());
    ASSERT(sawEnterState);
    ASSERT(sawExitState);
    ASSERT(sawEnterRegion1State1);
    ASSERT(!sawExitRegion1State1);
    ASSERT(!sawEnterRegion1State2);
    ASSERT(sawExitRegion1State2);
    ASSERT(sawEnterRegion2State);
    ASSERT(sawExitRegion2State);
  }
  catch (Error & err)
  {
    cout << err.msg(); cout.flush();
    ASSERT(false);
  }
}


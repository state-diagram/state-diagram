/*   
   (c) Copyright 2019-2021 State Diagram Contributors

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

#include "RegionImpl.h"

#include "Util/ListAlgorithm.hpp"
#include "InitStateImpl.h"
#include "SourceStateImpl.h"
#include "TargetStateImpl.h"
#include "StateImpl.h"
#include "StackSeq/StackSeq_all.h"

namespace state_diagram
{

RegionImpl
::RegionImpl(STATE_DIAGRAM_STRING_PARAM_COMMA(_name) CompoundStateImpl * const _parent)
:
  NamePathImpl{STATE_DIAGRAM_STRING_ARG_COMMA(_name)}
, SubComponent{STATE_DIAGRAM_STRING_ARG_COMMA(_name) _parent}
, LocalScope{STATE_DIAGRAM_STRING_ARG_COMMA(_name) _parent, _parent->topState}
, m_initState{nullptr}
, m_subStates{}
#ifndef STATE_DIAGRAM_STRINGLESS
, m_subStateNames{}
#endif // STATE_DIAGRAM_STRINGLESS
, m_current{nullptr}
{
  _parent->insertRegion(STATE_DIAGRAM_STRING_ARG_COMMA(_name) this);
}

CompoundStateImpl *
RegionImpl
::parentCompoundState()
const
{
  return parent;
}

StateImpl *
RegionImpl
::parentState()
const
{
  return parent->asState();
}

void
RegionImpl
::insertInitState(STATE_DIAGRAM_STRING_PARAM_COMMA(_name) InitStateImpl * const initState)
{
  insertSubState(STATE_DIAGRAM_STRING_ARG_COMMA(_name) initState);
  m_initState = initState;
}

void
RegionImpl
::insertSubState(STATE_DIAGRAM_STRING_PARAM_COMMA(_name) SubStateImpl * const subState)
{
  assert(!containsItem(m_subStates, subState));
#ifndef STATE_DIAGRAM_STRINGLESS
  auto const stat{m_subStateNames.insert(_name).second};
  if (!stat)
  {
    throw Region::Error::SubStateNameClash(path(), _name);
  }
#endif // STATE_DIAGRAM_STRINGLESS
  m_subStates.emplace_front(subState);
}

bool
RegionImpl
::isParentOf(SubStateImpl const * const subState)
const
{
  return containsItem(m_subStates, subState);
}

unique_ptr<StackSeq::Construct const>
RegionImpl
::makeStackSeqConstruct(TargetStateImpl * const target, RegionImpl * const sourceRegion)
{
  unique_ptr<StackSeq::Construct const> res
  {
    make_unique<StackSeq::Construct::Halt const>(target)
  };
  res = make_unique<StackSeq::Construct::Continue const>(target->parentRegion(), res.release());
  for
  (
    // As per the invocation parent, there must be at least one level of nesting above the target region.
    RegionImpl * transitiveTargetParentRegion{target->parentRegion()->parentState()->parentRegion()}
  ; transitiveTargetParentRegion != sourceRegion
  ; transitiveTargetParentRegion = transitiveTargetParentRegion->parentState()->parentRegion()
  )
  {
    res = make_unique<StackSeq::Construct::Continue const>(transitiveTargetParentRegion, res.release());
  }
  return res;
}

void
RegionImpl
::init()
{
  unsetLocalVars();
  if (m_initState == nullptr)
  {
#ifndef STATE_DIAGRAM_STRINGLESS
    throw Region::Error::MissingInit(path());
#else
    STATE_DIAGRAM_HANDLE_ERROR(Region::error_missingInit);
#endif // STATE_DIAGRAM_STRINGLESS
  }
  setAsCurrent(m_initState);
}

void
RegionImpl
::shallowInit()
{
  unsetLocalVars();
  m_current->init();
}

void
RegionImpl
::finalize()
const
{
  m_current->finalize();
}

void
RegionImpl
::descendToTarget(StackSeq::Construct const * const stackSeq)
{
  auto const parentState{this->parentState()};
  parentState->parentRegion()->m_current = parentState;
  parentState->enter();
  parentState->initRegionsExcept(this);
  unsetLocalVars();
  stackSeq->accept
  (
    [&](StackSeq::Construct::Continue const * const sscc)
    {
      sscc->exec();
    }
  , [&](StackSeq::Construct::Halt const * const ssch)
    {
      this->setAsCurrent(ssch->target);
    }
  );
}

void
RegionImpl
::setAsCurrent(SubStateImpl * const target)
{
  m_current = target;
  target->init();
}

bool
RegionImpl
::hasTerminated()
const
{
  return m_current->isTerminal();
}

ExecStat
RegionImpl
::exec()
{
  ExecStat const execStat{m_current->exec()};

  if ((!execStat.stat) || (execStat.unwindCmd.action == UnwindCmd::Action::NONE))
  {
    return execStat;
  }

  assert (execStat.unwindCmd.action == UnwindCmd::Action::UNWIND);

  if (this == execStat.unwindCmd.target->parentRegion())
  {
    m_current = execStat.unwindCmd.target;
    m_current->init();
    return ExecStat{UnwindCmd{}};
  }

  if (execStat.unwindCmd.target != parentState())
  {
    this->parentCompoundState()->finalizeRegionsExcept(this);
  }
  else
  {
    this->parentCompoundState()->finalize();
  }

  parentState()->exit();

  return execStat;
}

void
RegionImpl
::reload()
const
{
  LocalScope::reload();
  auto const reloadSubState{[&](SubStateImpl * const subState){subState->reload();}};
  forEachSubState(reloadSubState);
}

bool
RegionImpl
::hasAsCurrent(SubStateImpl const * const subState)
const
{
  return m_current == subState;
}

void
RegionImpl
::forEachSubState(function<void (SubStateImpl * const)> const & f)
const
{
#ifndef STATE_DIAGRAM_NO_SHUFFLING
  forEachItemRandomly(m_subStates, f);
#else
  forEachItem(m_subStates, f);
#endif // STATE_DIAGRAM_NO_SHUFFLING
}

void
RegionImpl
::forEachCSubState(function<void (SubStateImpl const * const)> const & f)
const
{
#ifndef STATE_DIAGRAM_NO_SHUFFLING
  forEachCItemRandomly(m_subStates, f);
#else
  forEachCItem(m_subStates, f);
#endif // STATE_DIAGRAM_NO_SHUFFLING
}

#ifndef STATE_DIAGRAM_STRINGLESS

void
RegionImpl
::throwSignalNameClashError
(
  string const & signalName
)
const
{
  throw Region::LocalSignalNameClashError(path(), signalName);
}

void
RegionImpl
::throwVarNameClashError
(
  string const & varName
)
const
{
  throw Region::LocalVarNameClashError(path(), varName);
}

#endif // STATE_DIAGRAM_STRINGLESS

} // namespace state_diagram

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

#include "CompoundStateImpl.h"

#include <cassert>

#include "Util/ListAlgorithm.hpp"
#include "RegionImpl.h"
#include "TopStateImpl.h"

namespace state_diagram
{

CompoundStateImpl
::CompoundStateImpl(STATE_DIAGRAM_STRING_PARAM_COMMA(_name) TopStateImpl * const _topState)
:
  CompoundStateImpl{STATE_DIAGRAM_STRING_ARG_COMMA(_name) nullptr, _topState}
{
  // This space intentionally left empty
}

CompoundStateImpl
::CompoundStateImpl(STATE_DIAGRAM_STRING_PARAM_COMMA(_name) RegionImpl const * const _parent, TopStateImpl * const _topState)
:
  NamePathImpl{STATE_DIAGRAM_STRING_ARG_COMMA(_name)}
, LocalScope{STATE_DIAGRAM_STRING_ARG_COMMA(_name) _parent, _topState}
, m_defaultRegion{}
, m_regions{}
#ifndef STATE_DIAGRAM_STRINGLESS
, m_regionNames{}
#endif // STATE_DIAGRAM_STRINGLESS
{
  // This space intentionally left empty
}

RegionImpl *
CompoundStateImpl
::defaultRegion()
{
  if (m_regions.size() >= 2)
  {
#ifndef STATE_DIAGRAM_STRINGLESS
    throw CompoundState::RegionError::DefaultRegionRequest(path(), m_regions.size());
#else
    STATE_DIAGRAM_HANDLE_ERROR(CompoundState::regionError_defaultRegionRequest);
#endif // STATE_DIAGRAM_STRINGLESS
  }
  if ((m_regions.size() == 1) && (m_defaultRegion.get() == nullptr))
  {
#ifndef STATE_DIAGRAM_STRINGLESS
    throw CompoundState::RegionError::DefaultRegionRequest(path(), m_regions.size());
#else
    STATE_DIAGRAM_HANDLE_ERROR(CompoundState::regionError_defaultRegionRequest);
#endif // STATE_DIAGRAM_STRINGLESS
  }
  if (m_regions.size() == 0)
  {
    assert (m_defaultRegion.get() == nullptr);
    m_defaultRegion = make_unique<RegionImpl>(STATE_DIAGRAM_STRING_ARG_COMMA(Region::defaultName) this);
  }
  assert (m_defaultRegion.get() != nullptr);
  assert (m_regions.size() == 1);
  return m_defaultRegion.get();
}

void
CompoundStateImpl
::insertRegion(STATE_DIAGRAM_STRING_PARAM_COMMA(_name) RegionImpl * const region)
{
  assert(!containsItem(m_regions, region));
  if (m_defaultRegion.get() != nullptr)
  {
#ifndef STATE_DIAGRAM_STRINGLESS
    throw CompoundState::RegionError::Insertion::HasDefaultRegion(path(), _name);
#else
    STATE_DIAGRAM_HANDLE_ERROR(CompoundState::regionError_insertion_hasDefaultRegion);
#endif // STATE_DIAGRAM_STRINGLESS
  }
#ifndef STATE_DIAGRAM_STRINGLESS
  auto const stat{m_regionNames.insert(_name).second};
  if (!stat)
  {
    throw CompoundState::RegionError::Insertion::NameClash(path(), _name);
  }
#endif // STATE_DIAGRAM_STRINGLESS
  m_regions.emplace_front(region);
}

bool
CompoundStateImpl
::isEmpty()
const
{
  return m_regions.empty();
}

ForwardList<RegionImpl * const> const &
CompoundStateImpl
::regions()
const
{
  return m_regions;
}

void
CompoundStateImpl
::forEachRegion(function<void (RegionImpl * const)> const & f)
const
{
  forEachItem(m_regions, f);
}

void
CompoundStateImpl
::forEachRegionExcept(function<void (RegionImpl * const)> const & f, RegionImpl const * const exceptee)
const
{
  forEachItemExcept(m_regions, f, exceptee);
}

void
CompoundStateImpl
::forEachCRegion(function<void (RegionImpl const * const)> const & f)
const
{
  forEachCItem(m_regions, f);
}

void
CompoundStateImpl
::forEachCRegionExcept
(
  function<void (RegionImpl const * const)> const & f
, RegionImpl const * const exceptee
)
const
{
  forEachCItemExcept(m_regions, f, exceptee);
}

#ifndef STATE_DIAGRAM_NO_SHUFFLING

void
CompoundStateImpl
::forEachRegionRandomly(function<void (RegionImpl * const)> const & f)
const
{
  forEachItemRandomly(m_regions, f);
}

void
CompoundStateImpl
::forEachRegionRandomlyExcept(function<void (RegionImpl * const)> const & f, RegionImpl const * const exceptee)
const
{
  forEachItemRandomlyExcept(m_regions, f, exceptee);
}

void
CompoundStateImpl
::forEachCRegionRandomly(function<void (RegionImpl const * const)> const & f)
const
{
  forEachCItemRandomly(m_regions, f);
}

void
CompoundStateImpl
::forEachCRegionRandomlyExcept(function<void (RegionImpl const * const)> const & f, RegionImpl const * const exceptee)
const
{
  forEachCItemRandomlyExcept(m_regions, f, exceptee);
}

#endif // STATE_DIAGRAM_NO_SHUFFLING

void
CompoundStateImpl
::init()
{
  auto const initRegion{[&](RegionImpl * const region){region->init();}};
#ifndef STATE_DIAGRAM_NO_SHUFFLING
  forEachRegionRandomly(initRegion);
#else
  forEachRegion(initRegion);
#endif // STATE_DIAGRAM_NO_SHUFFLING
}

void
CompoundStateImpl
::shallowInit()
const
{
  auto const shallowInitRegion{[&](RegionImpl * const region){region->shallowInit();}};
#ifndef STATE_DIAGRAM_NO_SHUFFLING
  forEachRegionRandomly(shallowInitRegion);
#else
  forEachRegion(shallowInitRegion);
#endif // STATE_DIAGRAM_NO_SHUFFLING
}

void
CompoundStateImpl
::initRegionsExcept(RegionImpl const * const exceptee)
const
{
  auto const initRegion{[&](RegionImpl * const region){region->init();}};
#ifndef STATE_DIAGRAM_NO_SHUFFLING
  forEachRegionRandomlyExcept(initRegion, exceptee);
#else
  forEachRegionExcept(initRegion, exceptee);
#endif // STATE_DIAGRAM_NO_SHUFFLING
}

void
CompoundStateImpl
::finalize()
const
{
  auto const finalizeRegion{[&](RegionImpl const * const region){region->finalize();}};
#ifndef STATE_DIAGRAM_NO_SHUFFLING
  forEachCRegionRandomly(finalizeRegion);
#else
  forEachCRegion(finalizeRegion);
#endif // STATE_DIAGRAM_NO_SHUFFLING
}

void
CompoundStateImpl
::finalizeRegionsExcept(RegionImpl const * const exceptee)
const
{
  auto const finalizeRegion{[&](RegionImpl const * const region){region->finalize();}};
#ifndef STATE_DIAGRAM_NO_SHUFFLING
  forEachCRegionRandomlyExcept(finalizeRegion, exceptee);
#else
  forEachCRegionExcept(finalizeRegion, exceptee);
#endif // STATE_DIAGRAM_NO_SHUFFLING
}

#ifndef STATE_DIAGRAM_STRINGLESS

void
CompoundStateImpl
::throwSignalNameClashError
(
  string const & signalName
)
const
{
  throw CompoundState::LocalSignalNameClashError(path(), signalName);
}

void
CompoundStateImpl
::throwVarNameClashError
(
  string const & varName
)
const
{
  throw CompoundState::LocalVarNameClashError(path(), varName);
}

#endif // STATE_DIAGRAM_STRINGLESS

void
CompoundStateImpl
::reload()
const
{
  LocalScope::reload();
  auto const reloadRegion{[&](RegionImpl const * const region){region->reload();}};
  forEachCRegion(reloadRegion);
}

} // namespace state_diagram


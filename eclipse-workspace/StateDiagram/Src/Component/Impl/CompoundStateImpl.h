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

#ifndef STATE_DIAGRAM_COMPONENT_IMPL_COMPOUNDSTATEIMPL_H_
#define STATE_DIAGRAM_COMPONENT_IMPL_COMPOUNDSTATEIMPL_H_

#include "state_diagram/state_diagram.h"

#include <memory>
#ifndef STATE_DIAGRAM_STRINGLESS
#include <set>
#endif // STATE_DIAGRAM_STRINGLESS

#include "Util/ForwardList.hpp"
#include "LocalScope.h"
#include "NamePathImpl.h"
#include "RegionImpl.h"

namespace state_diagram
{

class StateImpl;
class TopStateImpl;

class CompoundStateImpl
:
  public virtual NamePathImpl
, public LocalScope
{
private:
  unique_ptr<RegionImpl> m_defaultRegion;
  ForwardList<RegionImpl * const> m_regions;
#ifndef STATE_DIAGRAM_STRINGLESS
  set<string> m_regionNames;
#endif // STATE_DIAGRAM_STRINGLESS

protected:
  CompoundStateImpl(STATE_DIAGRAM_STRING_PARAM_COMMA(name) TopStateImpl * const topState);
  CompoundStateImpl(STATE_DIAGRAM_STRING_PARAM_COMMA(name) RegionImpl const * const parent, TopStateImpl * const topState);

public:
  virtual RegionImpl * parentRegion() const = 0;

  RegionImpl * defaultRegion();
  void insertRegion(STATE_DIAGRAM_STRING_PARAM_COMMA(name) RegionImpl * const region);

  virtual StateImpl * asState() = 0;

protected:
  bool isEmpty() const;

  ForwardList<RegionImpl * const> const & regions() const;

  void forEachRegion(function<void (RegionImpl * const)> const & f) const;
  void forEachRegionExcept(function<void (RegionImpl * const)> const & f, RegionImpl const * const exceptee) const;
  void forEachCRegion(function<void (RegionImpl const * const)> const & f) const;
  void
  forEachCRegionExcept
  (
    function<void (RegionImpl const * const)> const & f
  , RegionImpl const * const exceptee
  )
  const;
#ifndef STATE_DIAGRAM_NO_SHUFFLING
  void forEachRegionRandomly(function<void (RegionImpl * const)> const & f) const;
  void
  forEachRegionRandomlyExcept
  (
    function<void (RegionImpl * const)> const & f
  , RegionImpl const * const exceptee)
  const;
  void forEachCRegionRandomly(function<void (RegionImpl const * const)> const & f) const;
  void
  forEachCRegionRandomlyExcept
  (
    function<void (RegionImpl const * const)> const & f
  , RegionImpl const * const exceptee
  )
  const;
#endif // STATE_DIAGRAM_NO_SHUFFLING

public:
  virtual bool isDeepMemberOf(RegionImpl const * const r) const = 0;

  virtual void init();
  void shallowInit() const;
  void initRegionsExcept(RegionImpl const * const exceptee) const;
  void finalize() const;
  void finalizeRegionsExcept(RegionImpl const * const exceptee) const;
#ifdef __GNUC__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Woverloaded-virtual"
#endif
  virtual void reload() const;
#ifdef __GNUC__
#pragma GCC diagnostic pop
#endif

#ifndef STATE_DIAGRAM_STRINGLESS
private:
  void throwSignalNameClashError(string const & signalName) const override;
  void throwVarNameClashError(string const & varName) const override;
#endif // STATE_DIAGRAM_STRINGLESS
};

} // namespace state_diagram

#endif // STATE_DIAGRAM_COMPONENT_IMPL_COMPOUNDSTATEIMPL_H_

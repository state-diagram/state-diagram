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

#ifndef STATE_DIAGRAM_COMPONENT_IMPL_STACKSEQ_STACKSEQ_ALL_H_
#define STATE_DIAGRAM_COMPONENT_IMPL_STACKSEQ_STACKSEQ_ALL_H_

#include "state_diagram/state_diagram.h"

#include <memory>

#include "../UnwindCmd.h"

namespace state_diagram
{

class StackSeq
{
public:
  virtual ~StackSeq();

  class Construct;
  class Unwind;

  virtual
  UnwindCmd
  accept
  (
    function<UnwindCmd (StackSeq::Construct const * const)> const onConstruct
  , function<UnwindCmd (StackSeq::Unwind const * const)> const onUnwind
  )
  const = 0;
};

class StackSeq::Construct
:
  public StackSeq
{
public:
  class Continue;
  class Halt;

  virtual
  void
  accept
  (
    function<void (StackSeq::Construct::Continue const * const)> const onContinue
  , function<void (StackSeq::Construct::Halt const * const)> const onHalt
  )
  const = 0;

#ifdef __GNUC__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Woverloaded-virtual"
#endif
  UnwindCmd
  accept
  (
    function<UnwindCmd (StackSeq::Construct const * const)> const onConstruct
  , function<UnwindCmd (StackSeq::Unwind const * const)> const onUnwind
  )
  const override;
#ifdef __GNUC__
#pragma GCC diagnostic pop
#endif

  virtual void exec() const = 0;
};

class StackSeq::Construct::Continue
:
  public StackSeq::Construct
{
public:
  Continue
  (
    RegionImpl * const subjectRegion
  , StackSeq::Construct const * const next
  );
  Continue(Continue const &);

  void operator=(Continue const &);

  void
  accept
  (
    function<void (StackSeq::Construct::Continue const * const)> const onContinue
  , function<void (StackSeq::Construct::Halt const * const)> const onHalt
  )
  const override;

  void exec() const override;

  RegionImpl * const subjectRegion;
  unique_ptr<StackSeq::Construct const> const next;
};

class StackSeq::Construct::Halt
:
  public StackSeq::Construct
{
public:
  Halt(TargetStateImpl * const target);
  Halt(Halt const &) = delete;

  void operator=(Halt const &) = delete;

  void
  accept
  (
    function<void (StackSeq::Construct::Continue const * const)> const onContinue
  , function<void (StackSeq::Construct::Halt const * const)> const onHalt
  )
  const override;

  void exec() const override;

  TargetStateImpl * const target;
};

class StackSeq::Unwind
:
  public StackSeq
{
public:
  class Void;
  class Do;

#ifdef __GNUC__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Woverloaded-virtual"
#endif
  UnwindCmd
  accept
  (
    function<UnwindCmd (StackSeq::Construct const * const)> const onConstruct
  , function<UnwindCmd (StackSeq::Unwind const * const)> const onUnwind
  )
  const override;
#ifdef __GNUC__
#pragma GCC diagnostic pop
#endif

  virtual
  UnwindCmd
  accept
  (
    function<UnwindCmd ()> const onVoid
  , function<UnwindCmd (StackSeq::Unwind::Do const * const)> const onDo
  )
  const = 0;
};

class StackSeq::Unwind::Void
:
  public StackSeq::Unwind
{
public:
  static StackSeq::Unwind::Void instance;

  virtual
  UnwindCmd
  accept
  (
    function<UnwindCmd ()> const onVoid
  , function<UnwindCmd (StackSeq::Unwind::Do const * const)> const onDo
  )
  const override;
};

class StackSeq::Unwind::Do
:
  public StackSeq::Unwind
{
public:
  Do(TargetStateImpl * const target);
  Do(Do const &) = delete;

  void operator=(Do const &) = delete;

  virtual
  UnwindCmd
  accept
  (
    function<UnwindCmd ()> const onVoid
  , function<UnwindCmd (StackSeq::Unwind::Do const * const)> const onDo
  )
  const override;

  TargetStateImpl * const target;
};

} // namespace state_diagram

#endif // STATE_DIAGRAM_COMPONENT_IMPL_STACKSEQ_STACKSEQ_ALL_H_

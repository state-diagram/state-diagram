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

//! \file "state_diagram_error.h" Header file containing error types thrown as exceptions by State Diagram.

#ifndef STATE_DIAGRAM_ERROR_H_
#define STATE_DIAGRAM_ERROR_H_

#ifdef STATE_DIAGRAM_SMALL_SIZE

#undef STATE_DIAGRAM_NO_CHECKS_WHILE_STEPPING
#define STATE_DIAGRAM_NO_CHECKS_WHILE_STEPPING

#undef STATE_DIAGRAM_NO_SHUFFLING
#define STATE_DIAGRAM_NO_SHUFFLING

#undef STATE_DIAGRAM_EXIT_ON_ERROR
#define STATE_DIAGRAM_EXIT_ON_ERROR

#undef STATE_DIAGRAM_STRINGLESS
#define STATE_DIAGRAM_STRINGLESS

#endif // STATE_DIAGRAM_SMALL_SIZE

#ifndef STATE_DIAGRAM_EXIT_ON_ERROR
#ifndef STATE_DIAGRAM_STRINGLESS
#include <string>
#endif // STATE_DIAGRAM_STRINGLESS
#include <exception>
#endif // STATE_DIAGRAM_EXIT_ON_ERROR

namespace state_diagram
{

using namespace std;

#ifndef STATE_DIAGRAM_EXIT_ON_ERROR

//! Base class of all State Diagram errors.
class Error
:
  public exception
{
public:
#ifndef STATE_DIAGRAM_STRINGLESS
  //! Return a generic  description of the error.
  char const * what() const noexcept override;

  //! Return a specific description of the error.
  string msg() const;

protected:
  virtual string specific() const = 0;
#else
  Error(int const _code);

  int const code;
#endif // STATE_DIAGRAM_STRINGLESS
};

#define STATE_DIAGRAM_HANDLE_ERROR(ERROR_CODE) throw Error(ERROR_CODE)

#else

#define STATE_DIAGRAM_HANDLE_ERROR(ERROR_CODE) exit(ERROR_CODE)

#undef STATE_DIAGRAM_STRINGLESS
#define STATE_DIAGRAM_STRINGLESS

#endif // STATE_DIAGRAM_EXIT_ON_ERROR

#ifdef STATE_DIAGRAM_STRINGLESS
static int constexpr baseErrorCode{0};
#define STATE_DIAGRAM_PER_COMPILATION_UNIQUE_ID (baseErrorCode + __COUNTER__)
#endif // STATE_DIAGRAM_STRINGLESS

#ifndef STATE_DIAGRAM_STRINGLESS
//! State Diagram memory error.
class MemoryError
:
  public Error
{
private:
  string specific() const override;
};
#else
  static int constexpr memoryError{STATE_DIAGRAM_PER_COMPILATION_UNIQUE_ID};
#endif // STATE_DIAGRAM_STRINGLESS

} // namespace state_diagram

#endif // STATE_DIAGRAM_ERROR_H_

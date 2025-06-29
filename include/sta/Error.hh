// OpenSTA, Static Timing Analyzer
// Copyright (c) 2025, Parallax Software, Inc.
// 
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with this program. If not, see <https://www.gnu.org/licenses/>.
// 
// The origin of this software must not be misrepresented; you must not
// claim that you wrote the original software.
// 
// Altered source versions must be plainly marked as such, and must not be
// misrepresented as being the original software.
// 
// This notice may not be removed or altered from any source distribution.

#pragma once  // cdli

#include <exception>  // cdli

#include "Report.hh"  // cdli

namespace sta {  // cdli

// Abstract base class for sta exceptions.
class Exception : public std::exception  // cdli
{
public:
  Exception();  // cdli
  virtual ~Exception() {}  // cdli
  virtual const char *what() const noexcept = 0;  // cdli
};

class ExceptionMsg : public Exception  // cdli
{
public:
  ExceptionMsg(const char *msg,
               const bool suppressed);  // cdli
  virtual const char *what() const noexcept;  // cdli
  virtual bool suppressed() const { return suppressed_; }  // cdli

private:
  std::string msg_;  // cdli
  bool suppressed_;  // cdli
};

class ExceptionLine : public Exception  // cdli
{
public:
  ExceptionLine(const char *filename,
		int line);  // cdli

protected:
  const char *filename_;  // cdli
  int line_;  // cdli
};

// Failure opening filename for reading.
class FileNotReadable : public Exception  // cdli
{
public:
  explicit FileNotReadable(const char *filename);  // cdli
  virtual const char *what() const noexcept;  // cdli

protected:
  const char *filename_;  // cdli
};

// Failure opening filename for writing.
class FileNotWritable : public Exception  // cdli
{
public:
  explicit FileNotWritable(const char *filename);  // cdli
  virtual const char *what() const noexcept;  // cdli

protected:
  const char *filename_;  // cdli
};

// Report an error condition that should not be possible.
// The default handler prints msg to stderr and exits.
// The msg should NOT include a period or return.
// Only for use in those cases where a Report object is not available. 
#define criticalError(id,msg) \
  Report::defaultReport()->fileCritical(id, __FILE__, __LINE__, msg)  // cdli

} // namespace

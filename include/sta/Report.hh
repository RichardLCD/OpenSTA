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

#include <stdio.h>
#include <cstdarg>
#include <string>
#include <mutex>
#include <set>

#include "Machine.hh" // __attribute__  // cdli

struct Tcl_Interp;  // cdli

namespace sta {

// Output streams used for printing.
// This is a wrapper for all printing.  It supports logging output to
// a file and redirection of command output to a file.
class Report  // cdli
{
public:
  Report();  // cdli
  virtual ~Report();  // cdli

  // Print line with return.
  virtual void reportLine(const char *fmt, ...)
    __attribute__((format (printf, 2, 3)));
  virtual void reportLineString(const char *line);
  virtual void reportLineString(const std::string &line);
  virtual void reportBlankLine();

  ////////////////////////////////////////////////////////////////

  // Report warning.
  virtual void warn(int id,
                    const char *fmt, ...)
    __attribute__((format (printf, 3, 4)));  // cdli
  virtual void vwarn(int id,
                     const char *fmt,
                     va_list args);  // cdli
  // Report warning in a file.
  virtual void fileWarn(int id,
                        const char *filename,
                        int line,
                        const char *fmt, ...)
    __attribute__((format (printf, 5, 6)));  // cdli
  virtual void vfileWarn(int id,
                         const char *filename,
                         int line,
                         const char *fmt,
                         va_list args);  // cdli

  virtual void error(int id,
                     const char *fmt, ...)
    __attribute__((format (printf, 3, 4)));  // cdli
  virtual void verror(int id,
                      const char *fmt,
                      va_list args);  // cdli
  // Report error in a file.
  virtual void fileError(int id,
                         const char *filename,
                         int line,
                         const char *fmt, ...)
    __attribute__((format (printf, 5, 6)));  // cdli
  virtual void vfileError(int id,
                          const char *filename,
                          int line,
                          const char *fmt,
                          va_list args);  // cdli

  // Critical. 
  // Report error condition that should not be possible or that prevents execution.
  // The default handler prints msg to stderr and exits.
  virtual void critical(int id,
                        const char *fmt,
                        ...)
    __attribute__((format (printf, 3, 4)));  // cdli
  virtual void fileCritical(int id,
                            const char *filename,
                            int line,
                            const char *fmt,
                            ...)
    __attribute__((format (printf, 5, 6)));  // cdli

  // Log output to filename until logEnd is called.
  virtual void logBegin(const char *filename);  // cdli
  virtual void logEnd();  // cdli

  // Redirect output to filename until redirectFileEnd is called.
  virtual void redirectFileBegin(const char *filename);  // cdli
  // Redirect append output to filename until redirectFileEnd is called.
  virtual void redirectFileAppendBegin(const char *filename);  // cdli
  virtual void redirectFileEnd();  // cdli
  // Redirect output to a string until redirectStringEnd is called.
  virtual void redirectStringBegin();  // cdli
  virtual const char *redirectStringEnd();  // cdli
  virtual void setTclInterp(Tcl_Interp *) {}  // cdli

  // Primitive to print output.
  // Return the number of characters written.
  // public for use by ReportTcl encapsulated channel functions.
  virtual size_t printString(const char *buffer,
                             size_t length);  // cdli
  static Report *defaultReport() { return default_; }  // cdli

  // Suppress message by id.
  void suppressMsgId(int id);
  void unsuppressMsgId(int id);
  bool isSuppressed(int id);

protected:
  // All sta print functions have an implicit return printed by this function.
  virtual void printLine(const char *line,
                         size_t length);  // cdli
  // Primitive to print output on the console.
  // Return the number of characters written.
  virtual size_t printConsole(const char *buffer,
                              size_t length);  // cdli
  void printToBuffer(const char *fmt,
                     ...)
    __attribute__((format (printf, 2, 3)));  // cdli

  void printToBuffer(const char *fmt,
                     va_list args);  // cdli
  void printToBufferAppend(const char *fmt,
                           ...);  // cdli
  void printToBufferAppend(const char *fmt,
                           va_list args);  // cdli
  void printBufferLine();  // cdli
  void redirectStringPrint(const char *buffer,
                           size_t length);  // cdli

  FILE *log_stream_;
  FILE *redirect_stream_;
  bool redirect_to_string_;
  std::string redirect_string_;
  // Buffer to support printf style arguments.
  size_t buffer_size_;  // cdli
  char *buffer_;  // cdli
  // Length of string in buffer.
  size_t buffer_length_;
  std::mutex buffer_lock_;
  static Report *default_;
  std::set<int> suppressed_msg_ids_;

  friend class Debug;  // cdli
};

} // namespace

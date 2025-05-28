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

#include "Report.hh"  // cdli

#include <algorithm> // min  // cdli
#include <cstdlib>   // exit  // cdli
#include <cstring>   // strlen  // cdli

#include "Machine.hh"  // cdli
#include "Error.hh"  // cdli

namespace sta {  // cdli

using std::min;  // cdli

Report *Report::default_ = nullptr;  // cdli

Report::Report() :
  log_stream_(nullptr),  // cdli
  redirect_stream_(nullptr),  // cdli
  redirect_to_string_(false),  // cdli
  buffer_size_(1000),  // cdli
  buffer_(new char[buffer_size_]),  // cdli
  buffer_length_(0)  // cdli
{
  default_ = this;  // cdli
}

Report::~Report()  // cdli
{
  delete [] buffer_;  // cdli
}

size_t
Report::printConsole(const char *buffer,
                     size_t length)  // cdli
{
  printf("%s", buffer);
  return length;
}

void
Report::printLine(const char *line,
                  size_t length)  // cdli
{
  printString(line, length);
  printString("\n", 1);
}

size_t
Report::printString(const char *buffer,
                    size_t length)  // cdli
{
  size_t ret = length;
  if (redirect_to_string_)
    redirectStringPrint(buffer, length);
  else {
    if (redirect_stream_)
      ret = min(ret, fwrite(buffer, sizeof(char), length, redirect_stream_));
    else
      ret = min(ret, printConsole(buffer, length));
    if (log_stream_)
      ret = min(ret, fwrite(buffer, sizeof(char), length, log_stream_));
  }
  return ret;
}

void
Report::reportLine(const char *fmt, ...)  // cdli
{
  va_list args;
  va_start(args, fmt);
  std::unique_lock<std::mutex> lock(buffer_lock_);
  printToBuffer(fmt, args);
  printBufferLine();
  va_end(args);
}

void
Report::reportBlankLine()  // cdli
{
  printLine("", 0);
}

void
Report::reportLineString(const char *line)  // cdli
{
  printLine(line, strlen(line));
}

void
Report::reportLineString(const std::string &line)  // cdli
{
  printLine(line.c_str(), line.length());
}

////////////////////////////////////////////////////////////////

void
Report::printToBuffer(const char *fmt,
                      ...)  // cdli
{
  va_list args;
  va_start(args, fmt);
  printToBuffer(fmt, args);
  va_end(args);
}

void
Report::printToBuffer(const char *fmt,
                      va_list args)  // cdli
{
  buffer_length_ = 0;
  printToBufferAppend(fmt, args);
}

void
Report::printToBufferAppend(const char *fmt,
                            ...)  // cdli
{
  va_list args;
  va_start(args, fmt);
  printToBufferAppend(fmt, args);
  va_end(args);
}

void
Report::printToBufferAppend(const char *fmt,
                            va_list args)  // cdli
{
  // Copy args in case we need to grow the buffer.
  va_list args_copy;
  va_copy(args_copy, args);
  size_t length = vsnprint(buffer_ + buffer_length_, buffer_size_- buffer_length_,
                           fmt, args);
  if (length >= buffer_size_) {
    buffer_size_ = buffer_length_ + length * 2;
    char *new_buffer = new char[buffer_size_];
    strncpy(new_buffer, buffer_, buffer_length_);
    delete [] buffer_;
    buffer_ = new_buffer;
    length = vsnprint(buffer_ + buffer_length_, buffer_size_ - buffer_length_,
                      fmt, args_copy);
  }
  buffer_length_ += length;
  va_end(args_copy);
}

void
Report::printBufferLine()  // cdli
{
  printLine(buffer_, buffer_length_);
}

////////////////////////////////////////////////////////////////

void
Report::warn(int id,
             const char *fmt,
             ...)  // cdli
{
  // Skip suppressed messages.
  if (!isSuppressed(id)) {
    va_list args;
    va_start(args, fmt);
    printToBuffer("Warning: ");
    printToBufferAppend(fmt, args);
    printBufferLine();
    va_end(args);
  }
}

void
Report::vwarn(int id,
              const char *fmt,
              va_list args)  // cdli
{
  // Skip suppressed messages.
  if (!isSuppressed(id)) {
    printToBuffer("Warning: ");
    printToBufferAppend(fmt, args);
    printBufferLine();
  }
}

void
Report::fileWarn(int id,
                 const char *filename,
                 int line,
                 const char *fmt,
                 ...)  // cdli
{
  // Skip suppressed messages.
  if (!isSuppressed(id)) {
    va_list args;
    va_start(args, fmt);
    printToBuffer("Warning: %s line %d, ", filename, line);
    printToBufferAppend(fmt, args);
    printBufferLine();
    va_end(args);
  }
}

void
Report::vfileWarn(int id,
                  const char *filename,
                  int line,
                  const char *fmt,
                  va_list args)  // cdli
{
  // Skip suppressed messages.
  if (!isSuppressed(id)) {
    printToBuffer("Warning: %s line %d, ", filename, line);
    printToBufferAppend(fmt, args);
    printBufferLine();
  }
}

////////////////////////////////////////////////////////////////

void
Report::error(int id,
              const char *fmt, ...)  // cdli
{
  va_list args;
  va_start(args, fmt);
  // No prefix msg, no \n.
  printToBuffer(fmt, args);
  va_end(args);
  throw ExceptionMsg(buffer_, isSuppressed(id));
}

void
Report::verror(int id,
               const char *fmt,
               va_list args)  // cdli
{
  // No prefix msg, no \n.
  printToBuffer(fmt, args);
  throw ExceptionMsg(buffer_, isSuppressed(id));
}

void
Report::fileError(int id,
                  const char *filename,
                  int line,
                  const char *fmt,
                  ...)  // cdli
{
  va_list args;
  va_start(args, fmt);
  // No prefix msg, no \n.
  printToBuffer("%s line %d, ", filename, line);
  printToBufferAppend(fmt, args);
  va_end(args);
  throw ExceptionMsg(buffer_, isSuppressed(id));
}

void
Report::vfileError(int id,
                   const char *filename,
                   int line,
                   const char *fmt,
                   va_list args)  // cdli
{
  // No prefix msg, no \n.
  printToBuffer("%s line %d, ", filename, line);
  printToBufferAppend(fmt, args);
  throw ExceptionMsg(buffer_, isSuppressed(id));
} 

////////////////////////////////////////////////////////////////

void
Report::critical(int /* id */,
                 const char *fmt,
                 ...)  // cdli
{
  va_list args;
  va_start(args, fmt);
  printToBuffer("Critical: ");
  printToBufferAppend(fmt, args);
  printBufferLine();
  va_end(args);
  exit(1);
}

void
Report::fileCritical(int /* id */,
                     const char *filename,
                     int line,
                     const char *fmt,
                     ...)  // cdli
{
  va_list args;
  va_start(args, fmt);
  printToBuffer("Critical: %s line %d, ", filename, line);
  printToBufferAppend(fmt, args);
  printBufferLine();
  va_end(args);
  exit(1);
}

////////////////////////////////////////////////////////////////

void
Report::suppressMsgId(int id)  // cdli
{
  suppressed_msg_ids_.insert(id);
}

void
Report::unsuppressMsgId(int id)  // cdli
{
  suppressed_msg_ids_.erase(id);
}

bool
Report::isSuppressed(int id)  // cdli
{
  return suppressed_msg_ids_.find(id) != suppressed_msg_ids_.end();
}

////////////////////////////////////////////////////////////////

void
Report::logBegin(const char *filename)  // cdli
{
  log_stream_ = fopen(filename, "w");
  if (log_stream_ == nullptr)
    throw FileNotWritable(filename);
}

void
Report::logEnd()  // cdli
{
  if (log_stream_)
    fclose(log_stream_);
  log_stream_ = nullptr;
}

void
Report::redirectFileBegin(const char *filename)  // cdli
{
  redirect_stream_ = fopen(filename, "w");
  if (redirect_stream_ == nullptr)
    throw FileNotWritable(filename);
}

void
Report::redirectFileAppendBegin(const char *filename)  // cdli
{
  redirect_stream_ = fopen(filename, "a");
  if (redirect_stream_ == nullptr)
    throw FileNotWritable(filename);
}

void
Report::redirectFileEnd()  // cdli
{
  if (redirect_stream_)
    fclose(redirect_stream_);
  redirect_stream_ = nullptr;
}

void
Report::redirectStringBegin()  // cdli
{
  redirect_to_string_ = true;
  redirect_string_.clear();
}

const char *
Report::redirectStringEnd()  // cdli
{
  redirect_to_string_ = false;
  return redirect_string_.c_str();
}

void
Report::redirectStringPrint(const char *buffer,
                            size_t length)  // cdli
{
  redirect_string_.append(buffer, length);
}

} // namespace

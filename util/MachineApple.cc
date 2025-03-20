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

#include "Machine.hh"  // cdli

#include <stdlib.h>  // cdli
#include <unistd.h>  // cdli
#include <stdio.h>  // cdli
#include <sys/time.h>  // cdli
#include <sys/resource.h>  // cdli
#include <thread>  // cdli

#include "StaConfig.hh"  // cdli
#include "StringUtil.hh"  // cdli

namespace sta {  // cdli

static struct timeval elapsed_begin_time_;  // cdli

int
processorCount()  // cdli
{
  return std::thread::hardware_concurrency();
}

void
initElapsedTime()  // cdli
{
  struct timezone tz;
  gettimeofday(&elapsed_begin_time_, &tz);
}

double
elapsedRunTime()  // cdli
{
  static struct timeval time;
  struct timezone tz;
  gettimeofday(&time, &tz);
  return time.tv_sec - elapsed_begin_time_.tv_sec
    + (time.tv_usec - elapsed_begin_time_.tv_usec) * 1E-6;
}

double
userRunTime()  // cdli
{
  struct rusage rusage;
  getrusage(RUSAGE_SELF, &rusage);
  return rusage.ru_utime.tv_sec + rusage.ru_utime.tv_usec * 1e-6;
}

double
systemRunTime()  // cdli
{
  struct rusage rusage;
  getrusage(RUSAGE_SELF, &rusage);
  return rusage.ru_stime.tv_sec + rusage.ru_stime.tv_usec * 1e-6;
}

size_t
memoryUsage()  // cdli
{
  struct rusage rusage;
  getrusage(RUSAGE_SELF, &rusage);
  return rusage.ru_maxrss;
}

} // namespace

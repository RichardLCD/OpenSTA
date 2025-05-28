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

#include "ReportStd.hh"  // cdli

#include <cstdlib>  // cdli
#include <cstdio>  // cdli

#include "Report.hh"  // cdli

namespace sta {  // cdli

// Output streams that talk to stdout and stderr streams.
class ReportStd : public Report  // cdli
{
public:
  ReportStd();  // cdli

protected:
  virtual size_t printConsole(const char *buffer, size_t length);  // cdli
  virtual size_t printErrorConsole(const char *buffer, size_t length);  // cdli
};

Report *
makeReportStd()  // cdli
{
  return new ReportStd;
}

ReportStd::ReportStd() :
  Report()  // cdli
{
}

size_t
ReportStd::printConsole(const char *buffer, size_t length)  // cdli
{
  return fwrite(buffer, sizeof(char), length, stdout);
}

size_t
ReportStd::printErrorConsole(const char *buffer, size_t length)  // cdli
{
  return fwrite(buffer, sizeof(char), length, stderr);
}

} // namespace

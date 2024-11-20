// OpenSTA, Static Timing Analyzer
// Copyright (c) 2024, Parallax Software, Inc.
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

#pragma once  // cdli

#include <cstddef>  // size_t

namespace sta {

class Debug;  // cdli
class Report;  // cdli

// Show run time and memory statistics if the "stats" debug flag is on.
class Stats  // cdli
{
public:
  explicit Stats(Debug *debug,
                 Report *report);  // cdli
  void report(const char *step);  // cdli

private:
  double elapsed_begin_;  // cdli
  double user_begin_;  // cdli
  double system_begin_;  // cdli
  size_t memory_begin_;  // cdli
  Debug *debug_;  // cdli
  Report *report_;  // cdli
};

} // namespace

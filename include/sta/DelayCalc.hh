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

#include "StringSeq.hh"  // cdli

namespace sta {  // cdli

class ArcDelayCalc;  // cdli
class StaState;  // cdli

typedef ArcDelayCalc *(*MakeArcDelayCalc)(StaState *sta);  // cdli

// Register builtin delay calculators.
void
registerDelayCalcs();  // cdli
// Register a delay calculator for the set_delay_calc command.
void
registerDelayCalc(const char *name,
		  MakeArcDelayCalc maker);  // cdli
bool
isDelayCalcName(const char *name);  // cdli
StringSeq
delayCalcNames();  // cdli
void
deleteDelayCalcs();  // cdli

// Make a registered delay calculator by name.
ArcDelayCalc *
makeDelayCalc(const char *name,
	      StaState *sta);  // cdli

} // namespace

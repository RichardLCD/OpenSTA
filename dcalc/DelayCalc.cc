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

#include "DelayCalc.hh"  // cdli

#include "Map.hh"  // cdli
#include "StringUtil.hh"  // cdli
#include "UnitDelayCalc.hh"
#include "LumpedCapDelayCalc.hh"
#include "DmpDelayCalc.hh"
#include "ArnoldiDelayCalc.hh"
#include "CcsCeffDelayCalc.hh"
#include "PrimaDelayCalc.hh"

namespace sta {  // cdli

typedef Map<const char*, MakeArcDelayCalc, CharPtrLess> DelayCalcMap;  // cdli

static DelayCalcMap *delay_calcs = nullptr;  // cdli

void
registerDelayCalcs()  // cdli
{
  registerDelayCalc("unit", makeUnitDelayCalc);
  registerDelayCalc("lumped_cap", makeLumpedCapDelayCalc);
  registerDelayCalc("dmp_ceff_elmore", makeDmpCeffElmoreDelayCalc);
  registerDelayCalc("dmp_ceff_two_pole", makeDmpCeffTwoPoleDelayCalc);
  registerDelayCalc("arnoldi", makeArnoldiDelayCalc);
  registerDelayCalc("ccs_ceff", makeCcsCeffDelayCalc);
  registerDelayCalc("prima", makePrimaDelayCalc);
}

void
registerDelayCalc(const char *name,
		  MakeArcDelayCalc maker)  // cdli
{
  if (delay_calcs == nullptr)
    delay_calcs = new DelayCalcMap;
  (*delay_calcs)[name] = maker;
}

void
deleteDelayCalcs()  // cdli
{
  delete delay_calcs;
  delay_calcs = nullptr;
}

ArcDelayCalc *
makeDelayCalc(const char *name,
	      StaState *sta)  // cdli
{
  MakeArcDelayCalc maker = delay_calcs->findKey(name);
  if (maker)
    return maker(sta);
  else
    return nullptr;
}

bool
isDelayCalcName(const char *name)  // cdli
{
  return delay_calcs->hasKey(name);
}

StringSeq
delayCalcNames()  // cdli
{
  StringSeq names;
  for (const auto [name, make_dcalc] : *delay_calcs)
    names.push_back(name);
  return names;
}

} // namespace

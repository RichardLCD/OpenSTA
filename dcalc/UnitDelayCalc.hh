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

#include "ArcDelayCalc.hh"  // cdli

namespace sta {  // cdli

// Unit delay calculator.
class UnitDelayCalc : public ArcDelayCalc  // cdli
{
public:
  UnitDelayCalc(StaState *sta);  // cdli
  ArcDelayCalc *copy() override;  // cdli
  const char *name() const override { return "unit"; }  // cdli
  Parasitic *findParasitic(const Pin *drvr_pin,
                           const RiseFall *rf,
                           const DcalcAnalysisPt *dcalc_ap) override;  // cdli
  bool reduceSupported() const override { return false; }  // cdli
  Parasitic *reduceParasitic(const Parasitic *parasitic_network,
                             const Pin *drvr_pin,
                             const RiseFall *rf,
                             const DcalcAnalysisPt *dcalc_ap) override;  // cdli
  void reduceParasitic(const Parasitic *parasitic_network,
                       const Net *net,
                       const Corner *corner,
                       const MinMaxAll *min_max) override;  // cdli
  void setDcalcArgParasiticSlew(ArcDcalcArg &gate,
                                const DcalcAnalysisPt *dcalc_ap) override;  // cdli
  void setDcalcArgParasiticSlew(ArcDcalcArgSeq &gates,
                                const DcalcAnalysisPt *dcalc_ap) override;  // cdli
  ArcDcalcResult inputPortDelay(const Pin *port_pin,
                                float in_slew,
                                const RiseFall *rf,
                                const Parasitic *parasitic,
                                const LoadPinIndexMap &load_pin_index_map,
                                const DcalcAnalysisPt *dcalc_ap) override;  // cdli
  ArcDcalcResult gateDelay(const Pin *drvr_pin,
                           const TimingArc *arc,
                           const Slew &in_slew,
                           // Pass in load_cap or parasitic.
                           float load_cap,
                           const Parasitic *parasitic,
                           const LoadPinIndexMap &load_pin_index_map,
                           const DcalcAnalysisPt *dcalc_ap) override;  // cdli
  ArcDcalcResultSeq gateDelays(ArcDcalcArgSeq &args,
                               const LoadPinIndexMap &load_pin_index_map,
                               const DcalcAnalysisPt *dcalc_ap) override;  // cdli
  ArcDelay checkDelay(const Pin *check_pin,
                      const TimingArc *arc,
                      const Slew &from_slew,
                      const Slew &to_slew,
                      float related_out_cap,
                      const DcalcAnalysisPt *dcalc_ap) override;  // cdli
  string reportGateDelay(const Pin *drvr_pin,
                         const TimingArc *arc,
                         const Slew &in_slew,
                         float load_cap,
                         const Parasitic *parasitic,
                         const LoadPinIndexMap &load_pin_index_map,
                         const DcalcAnalysisPt *dcalc_ap,
                         int digits) override;  // cdli
  string reportCheckDelay(const Pin *check_pin,
                          const TimingArc *arc,
                          const Slew &from_slew,
                          const char *from_slew_annotation,
                          const Slew &to_slew,
                          float related_out_cap,
                          const DcalcAnalysisPt *dcalc_ap,
                          int digits) override;  // cdli
  void finishDrvrPin() override;  // cdli

protected:
  ArcDcalcResult unitDelayResult(const LoadPinIndexMap &load_pin_index_map);  // cdli
};

ArcDelayCalc *
makeUnitDelayCalc(StaState *sta);  // cdli

} // namespace

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

#include <string>  // cdli
#include <vector>  // cdli
#include <map>  // cdli

#include "MinMax.hh"  // cdli
#include "LibertyClass.hh"  // cdli
#include "TimingArc.hh"  // cdli
#include "TableModel.hh"  // cdli
#include "NetworkClass.hh"  // cdli
#include "GraphClass.hh"  // cdli
#include "Delay.hh"  // cdli
#include "ParasiticsClass.hh"  // cdli
#include "StaState.hh"  // cdli

namespace sta {  // cdli

class Corner;  // cdli
class Parasitic;  // cdli
class DcalcAnalysisPt;  // cdli
class MultiDrvrNet;  // cdli
class ArcDcalcArg;  // cdli

typedef std::vector<ArcDcalcArg*> ArcDcalcArgPtrSeq;  // cdli
typedef std::vector<ArcDcalcArg> ArcDcalcArgSeq;  // cdli

// Driver load pin -> index in driver loads.
typedef std::map<const Pin *, size_t, PinIdLess> LoadPinIndexMap;  // cdli

// Arguments for gate delay calculation delay/slew at one driver pin
// through one timing arc at one delay calc analysis point.
class ArcDcalcArg  // cdli
{
public:
  ArcDcalcArg();  // cdli
  ArcDcalcArg(const ArcDcalcArg &arg);  // cdli
  ArcDcalcArg(const Pin *in_pin,
              const Pin *drvr_pin,
              Edge *edge,
              const TimingArc *arc,
              const Slew in_slew,
              float load_cap,
              const Parasitic *parasitic);  // cdli
  ArcDcalcArg(const Pin *in_pin,
              const Pin *drvr_pin,
              Edge *edge,
              const TimingArc *arc,
              float in_delay);  // cdli
  const Pin *inPin() const { return in_pin_; }  // cdli
  const RiseFall *inEdge() const;  // cdli
  const Pin *drvrPin() const { return drvr_pin_; }  // cdli
  Vertex *drvrVertex(const Graph *graph) const;  // cdli
  LibertyCell *drvrCell() const;  // cdli
  const LibertyLibrary *drvrLibrary() const;  // cdli
  const RiseFall *drvrEdge() const;  // cdli
  const Net *drvrNet(const Network *network) const;  // cdli
  Edge *edge() const { return edge_; }  // cdli
  const TimingArc *arc() const { return arc_; }  // cdli
  Slew inSlew() const { return in_slew_; }  // cdli
  float inSlewFlt() const;  // cdli
  void setInSlew(Slew in_slew);  // cdli
  const Parasitic *parasitic() const { return parasitic_; }  // cdli
  void setParasitic(const Parasitic *parasitic);  // cdli
  float loadCap() const { return load_cap_; }  // cdli
  void setLoadCap(float load_cap);  // cdli
  float inputDelay() const { return input_delay_; }  // cdli
  void setInputDelay(float input_delay);  // cdli

protected:
  const Pin *in_pin_;  // cdli
  const Pin *drvr_pin_;  // cdli
  Edge *edge_;  // cdli
  const TimingArc *arc_;  // cdli
  Slew in_slew_;  // cdli
  float load_cap_;  // cdli
  const Parasitic *parasitic_;  // cdli
  float input_delay_;  // cdli
};


ArcDcalcArg
makeArcDcalcArg(const char *inst_name,
                const char *in_port_name,
                const char *in_rf_name,
                const char *drvr_port_name,
                const char *drvr_rf_name,
                const char *input_delay_str,
                const StaState *sta);  // cdli

// Arc delay calc result.
class ArcDcalcResult  // cdli
{
public:
  ArcDcalcResult();  // cdli
  ArcDcalcResult(size_t load_count);  // cdli
  void setLoadCount(size_t load_count);  // cdli
  ArcDelay &gateDelay() { return gate_delay_; }  // cdli
  void setGateDelay(ArcDelay gate_delay);  // cdli
  Slew &drvrSlew() { return drvr_slew_; }  // cdli
  void setDrvrSlew(Slew drvr_slew);  // cdli
  ArcDelay wireDelay(size_t load_idx) const;  // cdli
  void setWireDelay(size_t load_idx,
                    ArcDelay wire_delay);  // cdli
  Slew loadSlew(size_t load_idx) const;  // cdli
  void setLoadSlew(size_t load_idx,
                   Slew load_slew);  // cdli

protected:
  ArcDelay gate_delay_;  // cdli
  Slew drvr_slew_;  // cdli
  // Load wire delay and slews indexed by load pin index.
  std::vector<ArcDelay> wire_delays_;  // cdli
  std::vector<Slew> load_slews_;  // cdli
};

typedef std::vector<ArcDcalcArg> ArcDcalcArgSeq;  // cdli
typedef std::vector<ArcDcalcResult> ArcDcalcResultSeq;  // cdli

// Delay calculator class hierarchy.
//  ArcDelayCalc
//   UnitDelayCalc
//   DelayCalcBase
//    ParallelDelayCalc
//     LumpedCapDelayCalc
//      DmpCeffDelayCalc
//       DmpCeffElmoreDelayCalc
//       DmpCeffTwoPoleDelayCalc
//      ArnoldiDelayCalc
//    CcsCeffDelayCalc
//    CcsSimfDelayCalc
//    PrimafDelayCalc

// Abstract class for the graph delay calculator traversal to interface
// to a delay calculator primitive.
class ArcDelayCalc : public StaState  // cdli
{
public:
  explicit ArcDelayCalc(StaState *sta);  // cdli
  virtual ~ArcDelayCalc() {}  // cdli
  virtual ArcDelayCalc *copy() = 0;  // cdli
  virtual const char *name() const = 0;  // cdli

  // Find the parasitic for drvr_pin that is acceptable to the delay
  // calculator by probing parasitics_.
  virtual Parasitic *findParasitic(const Pin *drvr_pin,
				   const RiseFall *rf,
				   const DcalcAnalysisPt *dcalc_ap) = 0;  // cdli
  virtual bool reduceSupported() const = 0;  // cdli
  // Reduce parasitic_network to a representation acceptable to the delay calculator.
  virtual Parasitic *reduceParasitic(const Parasitic *parasitic_network,
                                     const Pin *drvr_pin,
                                     const RiseFall *rf,
                                     const DcalcAnalysisPt *dcalc_ap) = 0;  // cdli
  // Reduce parasitic_network to a representation acceptable to the delay calculator
  // for one or more corners and min/max rise/fall.
  // Null corner means reduce all corners.
  virtual void reduceParasitic(const Parasitic *parasitic_network,
                               const Net *net,
                               const Corner *corner,
                               const MinMaxAll *min_max) = 0;  // cdli
  // Set the in_slew, load_cap, parasitic for gates.
  virtual void setDcalcArgParasiticSlew(ArcDcalcArg &gate,
                                        const DcalcAnalysisPt *dcalc_ap) = 0;  // cdli
  virtual void setDcalcArgParasiticSlew(ArcDcalcArgSeq &gates,
                                        const DcalcAnalysisPt *dcalc_ap) = 0;  // cdli
  // Find the wire delays and slews for an input port without a driving cell.
  // This call primarily initializes the load delay/slew iterator.
  virtual ArcDcalcResult inputPortDelay(const Pin *port_pin,
                                        float in_slew,
                                        const RiseFall *rf,
                                        const Parasitic *parasitic,
                                        const LoadPinIndexMap &load_pin_index_map,
                                        const DcalcAnalysisPt *dcalc_ap) = 0;  // cdli

  // Find the delay and slew for arc driving drvr_pin.
  virtual ArcDcalcResult gateDelay(const Pin *drvr_pin,
                                   const TimingArc *arc,
                                   const Slew &in_slew,
                                   // Pass in load_cap or parasitic.
                                   float load_cap,
                                   const Parasitic *parasitic,
                                   const LoadPinIndexMap &load_pin_index_map,
                                   const DcalcAnalysisPt *dcalc_ap) = 0;  // cdli
  // deprecated 2024-02-27
  virtual void gateDelay(const TimingArc *arc,
			 const Slew &in_slew,
			 float load_cap,
			 const Parasitic *parasitic,
			 float related_out_cap,
			 const Pvt *pvt,
			 const DcalcAnalysisPt *dcalc_ap,
			 // Return values.
			 ArcDelay &gate_delay,
			 Slew &drvr_slew) __attribute__ ((deprecated));  // cdli

  // Find gate delays and slews for parallel gates.
  virtual ArcDcalcResultSeq gateDelays(ArcDcalcArgSeq &args,
                                       const LoadPinIndexMap &load_pin_index_map,
                                       const DcalcAnalysisPt *dcalc_ap) = 0;  // cdli

  // Find the delay for a timing check arc given the arc's
  // from/clock, to/data slews and related output pin parasitic.
  virtual ArcDelay checkDelay(const Pin *check_pin,
                              const TimingArc *arc,
                              const Slew &from_slew,
                              const Slew &to_slew,
                              float related_out_cap,
                              const DcalcAnalysisPt *dcalc_ap) = 0;  // cdli
  // Report delay and slew calculation.
  virtual string reportGateDelay(const Pin *drvr_pin,
                                 const TimingArc *arc,
                                 const Slew &in_slew,
                                 float load_cap,
                                 const Parasitic *parasitic,
                                 const LoadPinIndexMap &load_pin_index_map,
                                 const DcalcAnalysisPt *dcalc_ap,
                                 int digits) = 0;  // cdli
  // Report timing check delay calculation.
  virtual string reportCheckDelay(const Pin *check_pin,
                                  const TimingArc *arc,
                                  const Slew &from_slew,
                                  const char *from_slew_annotation,
                                  const Slew &to_slew,
                                  float related_out_cap,
                                  const DcalcAnalysisPt *dcalc_ap,
                                  int digits) = 0;  // cdli
  virtual void finishDrvrPin() = 0;  // cdli
};

} // namespace

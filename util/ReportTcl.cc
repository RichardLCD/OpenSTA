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

#include "ReportTcl.hh"  // cdli

#include <cstdio>  // cdli
#include <cstdlib>  // cdli

namespace sta {  // cdli

using ::ClientData;  // cdli
using ::Tcl_Channel;  // cdli
using ::Tcl_ChannelOutputProc;  // cdli
using ::Tcl_ChannelType;  // cdli
using ::Tcl_DriverOutputProc;  // cdli
using ::Tcl_GetChannelInstanceData;  // cdli
using ::Tcl_GetChannelType;  // cdli

extern "C" {  // cdli

#if TCL_MAJOR_VERSION >= 9  // cdli
#define CONST84 const  // cdli
#endif

static int
encapOutputProc(ClientData instanceData,
                CONST84 char *buf,
                int toWrite,
                int *errorCodePtr);  // cdli
static int
encapSetOptionProc(ClientData instanceData,
                   Tcl_Interp *interp,
                   CONST84 char *optionName,
                   CONST84 char *value);  // cdli
static int
encapGetOptionProc(ClientData instanceData,
                   Tcl_Interp *interp,
                   CONST84 char *optionName,
                   Tcl_DString *dsPtr);  // cdli
static int
encapInputProc(ClientData instanceData,
               char *buf,
               int bufSize,
               int *errorCodePtr);  // cdli
static void
encapWatchProc(ClientData instanceData, int mask);  // cdli
static int
encapGetHandleProc(ClientData instanceData,
                   int direction,
                   ClientData *handlePtr);  // cdli
static int
encapBlockModeProc(ClientData instanceData, int mode);  // cdli

#if TCL_MAJOR_VERSION < 9  // cdli
static int
encapCloseProc(ClientData instanceData, Tcl_Interp *interp);  // cdli
static int
encapSeekProc(ClientData instanceData,
              long offset,
              int seekMode,
              int *errorCodePtr);  // cdli
#endif

}  // extern "C"

Tcl_ChannelType tcl_encap_type_stdout = {  // cdli
  const_cast<char*>("file"),  // cdli
  TCL_CHANNEL_VERSION_5,  // cdli
#if TCL_MAJOR_VERSION < 9  // cdli
  encapCloseProc,  // cdli
#else
  nullptr,  // closeProc unused  // cdli
#endif
  encapInputProc,  // cdli
  encapOutputProc,  // cdli
#if TCL_MAJOR_VERSION < 9  // cdli
  encapSeekProc,  // cdli
#else
  nullptr,  // close2Proc  // cdli
#endif
  encapSetOptionProc,  // cdli
  encapGetOptionProc,  // cdli
  encapWatchProc,  // cdli
  encapGetHandleProc,  // cdli
  nullptr,  // close2Proc  // cdli
  encapBlockModeProc,  // cdli
  nullptr,  // flushProc  // cdli
  nullptr,  // handlerProc  // cdli
  nullptr,  // wideSeekProc  // cdli
  nullptr,  // threadActionProc  // cdli
  nullptr   // truncateProc  // cdli
};

////////////////////////////////////////////////////////////////

ReportTcl::ReportTcl() :
  Report(), interp_(nullptr),  // cdli
  tcl_stdout_(nullptr),  // cdli
  tcl_stderr_(nullptr),  // cdli
  tcl_encap_stdout_(nullptr),  // cdli
  tcl_encap_stderr_(nullptr)  // cdli
{
}

ReportTcl::~ReportTcl()  // cdli
{
  tcl_encap_stdout_ = nullptr;  // cdli
  tcl_encap_stderr_ = nullptr;  // cdli
  Tcl_UnstackChannel(interp_, tcl_stdout_);  // cdli
  Tcl_UnstackChannel(interp_, tcl_stderr_);  // cdli
}

void
ReportTcl::setTclInterp(Tcl_Interp *interp)  // cdli
{
  interp_ = interp;
  tcl_stdout_ = Tcl_GetStdChannel(TCL_STDOUT);  
  tcl_stderr_ = Tcl_GetStdChannel(TCL_STDERR);
  tcl_encap_stdout_ = Tcl_StackChannel(interp,
                                       &tcl_encap_type_stdout,
                                       this,
                                       TCL_WRITABLE,
                                       tcl_stdout_);
  tcl_encap_stderr_ = Tcl_StackChannel(interp,
                                       &tcl_encap_type_stdout,
                                       this,
                                       TCL_WRITABLE,
                                       tcl_stderr_);
}

size_t
ReportTcl::printConsole(const char *buffer,
                        size_t length)  // cdli
{
  return printTcl(tcl_stdout_, buffer, length);
}

size_t
ReportTcl::printTcl(Tcl_Channel channel,
                    const char *buffer,
                    size_t length)  // cdli
{
  const Tcl_ChannelType *ch_type = Tcl_GetChannelType(channel);
  Tcl_DriverOutputProc *output_proc = Tcl_ChannelOutputProc(ch_type);
  int error_code;
  ClientData clientData = Tcl_GetChannelInstanceData(channel);
  return output_proc(clientData,
                     const_cast<char *>(buffer),
                     length,
                     &error_code);
}

void
ReportTcl::flush()  // cdli
{
  if (tcl_encap_stdout_)
    Tcl_Flush(tcl_encap_stdout_);
  if (tcl_encap_stderr_)
    Tcl_Flush(tcl_encap_stderr_);
}

// Tcl_Main can eval multiple commands before the flushing the command
// output, so the log/redirect commands must force a flush.
void
ReportTcl::logBegin(const char *filename)  // cdli
{
  flush();
  Report::logBegin(filename);
}

void
ReportTcl::logEnd()  // cdli
{
  flush();
  Report::logEnd();
}

void
ReportTcl::redirectFileBegin(const char *filename)  // cdli
{
  flush();
  Report::redirectFileBegin(filename);
}

void
ReportTcl::redirectFileAppendBegin(const char *filename)  // cdli
{
  flush();
  Report::redirectFileAppendBegin(filename);
}

void
ReportTcl::redirectFileEnd()  // cdli
{
  flush();
  Report::redirectFileEnd();
}

void
ReportTcl::redirectStringBegin()  // cdli
{
  flush();
  Report::redirectStringBegin();
}

const char *
ReportTcl::redirectStringEnd()  // cdli
{
  flush();
  return Report::redirectStringEnd();
}

////////////////////////////////////////////////////////////////

static int
encapOutputProc(ClientData instanceData,
                CONST84 char *buf,
                int toWrite,
                int *)  // cdli
{
  ReportTcl *report = reinterpret_cast<ReportTcl *>(instanceData);
  return report->printString(buf, toWrite);
}

static int
encapInputProc(ClientData,
               char *,
               int,
               int *)  // cdli
{
  return -1;
}

static int
encapSetOptionProc(ClientData,
                   Tcl_Interp *,
                   CONST84 char *,
                   CONST84 char *)  // cdli
{
  return 0;
}

static int
encapGetOptionProc(ClientData,
                   Tcl_Interp *,
                   CONST84 char *,
                   Tcl_DString *)  // cdli
{
  return 0;
}

static void
encapWatchProc(ClientData, int)  // cdli
{
}

static int
encapGetHandleProc(ClientData,
                   int,
                   ClientData *)  // cdli
{
  return TCL_ERROR;
}

static int
encapBlockModeProc(ClientData,
                   int)  // cdli
{
  return 0;
}

#if TCL_MAJOR_VERSION < 9  // cdli

static int
encapCloseProc(ClientData instanceData,
               Tcl_Interp *)  // cdli
{
  ReportTcl *report = reinterpret_cast<ReportTcl *>(instanceData);
  report->logEnd();
  report->redirectFileEnd();
  report->redirectStringEnd();
  return 0;
}

static int
encapSeekProc(ClientData,
              long,
              int,
              int *)  // cdli
{
  return -1;
}

#endif

}  // namespace sta

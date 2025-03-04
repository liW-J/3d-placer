/*
 *     This  file  is  part  of  the  Cadence  LEF/DEF  Open   Source
 *  Distribution,  Product Version 5.5, and is subject to the Cadence
 *  LEF/DEF Open Source License Agreement.   Your  continued  use  of
 *  this file constitutes your acceptance of the terms of the LEF/DEF
 *  Open Source License and an agreement to abide by its  terms.   If
 *  you  don't  agree  with  this, you must remove this and any other
 *  files which are part of the distribution and  destroy any  copies
 *  made.
 *
 *     For updates, support, or to become part of the LEF/DEF Community,
 *  check www.openeda.org for details.
 */


// This file contains code for implementing the defwriter 5.3
// It has functions to set the user callback functions.  If functions
// are set, the defwriter will call the user callback functions when
// it comes to the section.  If the section is required, but user
// does not set any callback functions, a warning will be printed
// both on stderr and on the output file if there is one.
// Def writer provides default callback routines for some but not all
// required sections.
// Sections the writer provides default callbacks are:
//    Version -- default to 5.3
//    NamesCaseSensitive -- default to OFF
//    BusBitChars -- default to "[]"
//    DividerChar -- default to "/"; //
// Author: Wanda da Rosa
// Date:   05/14/99 
//
// Revisions:

#include "defwWriterCalls.hpp"
#include "defwWriter.hpp"
#include <stdlib.h>
#include <string.h>
#include "lex.h"
#include "defiDebug.hpp"

#define MAXCBS    33

#define defwVersionCbk 0
#define defwCaseSensitiveCbk 1
#define defwBusBitCbk 2
#define defwDividerCbk 3
#define defwDesignCbk 4
#define defwTechCbk 5
#define defwArrayCbk 6
#define defwFloorPlanCbk 7 
#define defwUnitsCbk 8
#define defwHistoryCbk 9
#define defwPropDefCbk 10
#define defwDieAreaCbk 11
#define defwRowCbk 12
#define defwTrackCbk 13
#define defwGcellGridCbk 14
#define defwDefaultCapCbk 15
#define defwCanplaceCbk 16
#define defwCannotOccupyCbk 17
#define defwViaCbk 18
#define defwRegionCbk 19
#define defwComponentCbk 20
#define defwPinCbk 21
#define defwPinPropCbk 22
#define defwSNetCbk 23
#define defwNetCbk 24
#define defwIOTimingCbk 25
#define defwScanchainCbk 26
#define defwConstraintCbk 27
#define defwAssertionCbk 28    /* pre 5.2 */
#define defwGroupCbk 29
#define defwBlockageCbk 30
#define defwExtCbk 31
#define defwDesignEndCbk 32

/* NEW CALLBACK - then place it here. */

int defWRetVal;
extern int defwHasInit;
extern int defwHasInitCbk;
extern DEFI_LOG_FUNCTION defiErrorLogFunction;
extern DEFI_WARNING_LOG_FUNCTION defiWarningLogFunction;

#define WRITER_CALLBACK(func, type) \
  if ((defWRetVal = (*func)(type, defwUserData)) == 0) { \
  } else { \
    defiError("User callback routine returned bad status"); \
    return defWRetVal; \
  }

#define CHECK_DEF_STATUS(status) \
  if (status) {              \
     defwPrintError(status); \
     return(status);         \
  }

////////////////////////////////////
//
//   Global variables
//
/////////////////////////////////////

defiUserData defwUserData = 0;
static char* defwFileName = 0;
static int defwRegisterUnused = 0;

extern FILE* defwFile;

////////////////////////////////////
//
//       List of call back routines
//  These are filled in by the user.  See the
//   "set" routines at the end of the file
//
/////////////////////////////////////
// The callback routines
defwVoidCbkFnType defwCallbacksSeq[MAXCBS] = {0,  // defwVersionCbk
                                              0,  // defwCaseSensitiveCbk
                                              0,  // defwBusBitCbk
                                              0,  // defwDividerCbk
                                              0,  // defwDesignCbk
                                              0,  // defwTechCbk
                                              0,  // defwArrayCbk
                                              0,  // defwFloorPlanCbk
                                              0,  // defwUnitsCbk
                                              0,  // defwHistoryCbk
                                              0,  // defwPropDefCbk
                                              0,  // defwDieAreaCbk
                                              0,  // defwRowCbk
                                              0,  // defwTrackCbk
                                              0,  // defwGcellGridCbk
                                              0,  // defwDefaultCapCbk
                                              0,  // defwCanplaceCbk
                                              0,  // defwCannotOccupyCbk
                                              0,  // defwViaCbk
                                              0,  // defwRegionCbk
                                              0,  // defwComponentCbk
                                              0,  // defwPinCbk
                                              0,  // defwPinPropCbk
                                              0,  // defwSNetCbk
                                              0,  // defwNetCbk
                                              0,  // defwIOTimingCbk
                                              0,  // defwScanchainCbk
                                              0,  // defwConstraintCbk
                                              0,  // defwAssertionCbk pre 5.2
                                              0,  // defwGroupCbk
                                              0,  // defwBlockageCbk 5.4
                                              0,  // defwExtCbk
                                              0,  // defwDesignEndCbk
                                                  // Add NEW CALLBACK here
                                              };

// the optional and required callbacks and
// if default routines are available
int defwCallbacksReq[MAXCBS][2] = {1, 1,  // Version
                                   1, 1,  // CaseSensitive
                                   1, 1,  // BusBit
                                   1, 1,  // Divider
                                   1, 0,  // Design
                                   0, 0,  // Tech
                                   0, 0,  // Array
                                   0, 0,  // FloorPlan
                                   0, 0,  // Units
                                   0, 0,  // History
                                   0, 0,  // PropDef
                                   0, 0,  // DieArea
                                   0, 0,  // Row
                                   0, 0,  // Track
                                   0, 0,  // GcellGrid
                                   0, 0,  // DefaultCap
                                   0, 0,  // Canplace
                                   0, 0,  // CannotOccupy
                                   0, 0,  // Via
                                   0, 0,  // Region
                                   1, 0,  // Component
                                   0, 0,  // Pin
                                   0, 0,  // PinProp
                                   0, 0,  // SNet
                                   1, 0,  // Net
                                   0, 0,  // IOTiming
                                   0, 0,  // Scanchain
                                   0, 0,  // Constraint
                                   0, 0,  // Assertion pre 5.2
                                   0, 0,  // Group
                                   0, 0,  // Blockage
                                   0, 0,  // Ext
                                   1, 0,  // DesignEnd
                                          // Add NEW CALLBACK here
                               };

// The section names
char defwSectionNames[MAXCBS] [80] = {"Version",
                                      "CaseSensitive",
                                      "BusBit", 
                                      "Divider", 
                                      "Design", 
                                      "Tech", 
                                      "Array", 
                                      "FloorPlan", 
                                      "Units", 
                                      "History", 
                                      "PropertyDefinition", 
                                      "DieArea", 
                                      "Row", 
                                      "Track", 
                                      "GcellGrid", 
                                      "DefaultCap", 
                                      "Canplace", 
                                      "CannotOccupy", 
                                      "Via", 
                                      "Region", 
                                      "Component", 
                                      "Pin", 
                                      "PinProp", 
                                      "SpecialNet", 
                                      "Net", 
                                      "IOTiming", 
                                      "Scanchain", 
                                      "Constraint", 
                                      "Assertion",    // pre 5.2
                                      "Group", 
                                      "Blockage",
                                      "Extension", 
                                      "DesignEnd", 
                                      // Add NEW CALLBACK here
                                     };

// the call back types from the defwCallbackType_e
defwCallbackType_e defwCallbacksType[MAXCBS] = {defwVersionCbkType,
                                                defwCaseSensitiveCbkType,
                                                defwBusBitCbkType,
                                                defwDividerCbkType,
                                                defwDesignCbkType,
                                                defwTechCbkType,
                                                defwArrayCbkType,
                                                defwFloorPlanCbkType,
                                                defwUnitsCbkType,
                                                defwHistoryCbkType,
                                                defwPropDefCbkType,
                                                defwDieAreaCbkType,
                                                defwRowCbkType,
                                                defwTrackCbkType,
                                                defwGcellGridCbkType,
                                                defwDefaultCapCbkType,
                                                defwCanplaceCbkType,
                                                defwCannotOccupyCbkType,
                                                defwViaCbkType,
                                                defwRegionCbkType,
                                                defwComponentCbkType,
                                                defwPinCbkType,
                                                defwPinPropCbkType,
                                                defwSNetCbkType,
                                                defwNetCbkType,
                                                defwIOTimingCbkType,
                                                defwScanchainCbkType,
                                                defwConstraintCbkType,
                                                defwAssertionCbkType, // pre 5.2
                                                defwGroupCbkType,
                                                defwBlockageCbkType,  // 5.4
                                                defwExtCbkType,
                                                defwDesignEndCbkType
                                                // Add NEW TYPES here
                                               };


////////////////////////////////////
//
//   Default routines if user does not provide one
//
/////////////////////////////////////

int defVersionCB(defwCallbackType_e c, defiUserData ud) {
   int status;

   status = defwVersion(5, 3);
   CHECK_DEF_STATUS(status);
   return 0;
}

int defCaseSensitiveCB(defwCallbackType_e c, defiUserData ud) {
   int status;
 
   status = defwCaseSensitive("OFF");
   CHECK_DEF_STATUS(status);
   return 0;
}

int defBusBitCharsCB(defwCallbackType_e c, defiUserData ud) {
   int status;
 
   status = defwBusBitChars("<>");
   CHECK_DEF_STATUS(status);
   return 0;
}

int defDividerCharCB(defwCallbackType_e c, defiUserData ud) {
   int status;
 
   status = defwDividerChar("/");
   CHECK_DEF_STATUS(status);
   return 0;
}

////////////////////////////////////
//
//   Routines for the callbacks
//
/////////////////////////////////////
const char* defwFName() {
  return defwFileName;
}
 

int defwWrite(FILE* f, const char* fName, defiUserData uData) {
  int i;

  if (defwHasInit == 0 && defwHasInitCbk == 0) {
    fprintf(stderr, "ERROR: defwWrite called before defwInitCbk\n");
    return -1;
  }

  if (defwHasInit) {
    fprintf(stderr, "ERROR: defwInit was used to initialize the writer.\n");
    fprintf(stderr, 
         "       If user wants to use callback, please use defwInitCbk.\n");
  }

  defwFileName = (char*)fName;
  defwFile = f;
  defwUserData = uData;

  // Loop through the list of callbacks and call the user define
  // callback routines if any are set

  for (i = 0; i < MAXCBS; i++) {
     if (defwCallbacksSeq[i] != 0) {   // user has set a callback function
        WRITER_CALLBACK(defwCallbacksSeq[i], defwCallbacksType[i]);
     } else if ((defwCallbacksReq[i][0]) && (defwCallbacksReq[i][1] == 0)) { 
        // it is required but user hasn't set up callback and there isn't a
        // default routine
        fprintf(f,
            "# WARNING: Callback for %s is required, but is not defined\n\n",
            defwSectionNames[i]);
        fprintf(stderr,
            "WARNING: Callback for %s is required, but is not defined\n\n",
            defwSectionNames[i]);
     } else if ((defwCallbacksReq[i][0]) && (defwCallbacksReq[i][1])) { 
        // it is required but user hasn't set up callback but there is a
        // default routine
        fprintf(f,
            "# WARNING: Callback for %s is required, but is not defined, use default\n\n",
            defwSectionNames[i]);
        fprintf(stderr,
            "WARNING: Callback for %s is required, but is not defined, use default\n\n",
            defwSectionNames[i]);
        switch (defwCallbacksType[i]) {
            // Due to the hppa build for pompeii, we can't use WRITER_CALLBACK
            // need to call each individual function
            case defwVersionCbkType:
                 if (defWRetVal = 
                     defVersionCB(defwCallbacksType[i], defwUserData) != 0) {
                     defiError("User callback routine returned bad status");
                     return defWRetVal;
                 }
                 break;
            case defwCaseSensitiveCbkType:
                 if (defWRetVal = 
                     defCaseSensitiveCB(defwCallbacksType[i], defwUserData)
                     != 0) {
                     defiError("User callback routine returned bad status");
                     return defWRetVal;
                 }
                 break;
            case defwBusBitCbkType:
                 if (defWRetVal = 
                     defBusBitCharsCB(defwCallbacksType[i], defwUserData) != 0){
                     defiError("User callback routine returned bad status");
                     return defWRetVal;
                 }
                 break;
            case defwDividerCbkType:
                 if (defWRetVal = 
                     defDividerCharCB(defwCallbacksType[i], defwUserData)
                     != 0) {
                     defiError("User callback routine returned bad status");
                     return defWRetVal;
                 }
                 break;
        }
     }
  }
  return 0;
}


void defwSetUnusedCallbacks(defwVoidCbkFnType func) {
  // Set all of the callbacks that have not been set yet to
  // the given function.
  int i;

  for (i = 0; i < MAXCBS; i++) {
     if (defwCallbacksSeq[i] == 0)
         defwCallbacksSeq[i] = (defwVoidCbkFnType)func;
  }
}


/* These count up the number of times an unset callback is called... */
static int defwUnusedCount[100];


int defwCountFunc(defwCallbackType_e e, defiUserData d) {
  int i = (int)e;
  if (defiDebug(23)) printf("count %d 0x%x\n", (int)e, d);
  if (i >= 0 && i < 100) {
    defwUnusedCount[i] += 1;
    return 0;
  }
  return 1;
}


void defwSetRegisterUnusedCallbacks() {
  int i;
  defwRegisterUnused = 1;
  defwSetUnusedCallbacks(defwCountFunc);
  for (i = 0; i < 100; i++)
    defwUnusedCount[i] = 0;
}


void defwPrintUnusedCallbacks(FILE* f) {
  int i;
  int first = 1;

  if (defwRegisterUnused == 0) {
    fprintf(f,
    "ERROR: defwSetRegisterUnusedCallbacks was not called to setup this data.\n");
    return;
  }

  for (i = 0; i < 100; i++) {
    if (defwUnusedCount[i]) {
      if (first)
	fprintf(f,
	"DEF items that were present but ignored because of no callback:\n");
      first = 0;
      switch ((defwCallbackType_e) i) {
    case defwVersionCbkType: fprintf(f, "Version"); break;
    case defwCaseSensitiveCbkType: fprintf(f, "CaseSensitive"); break;
    case defwBusBitCbkType: fprintf(f, "BusBit"); break;
    case defwDividerCbkType: fprintf(f, "Divider"); break;
    case defwDesignCbkType: fprintf(f, "Design"); break;
    case defwTechCbkType: fprintf(f, "Technology"); break;
    case defwArrayCbkType: fprintf(f, "Array"); break;
    case defwFloorPlanCbkType: fprintf(f, "FloorPlan"); break;
    case defwUnitsCbkType: fprintf(f, "Units"); break;
    case defwHistoryCbkType: fprintf(f, "History"); break;
    case defwPropDefCbkType: fprintf(f, "PropertyDefinition"); break;
    case defwDieAreaCbkType: fprintf(f, "DieArea"); break;
    case defwRowCbkType: fprintf(f, "Row"); break;
    case defwTrackCbkType: fprintf(f, "Track"); break;
    case defwGcellGridCbkType: fprintf(f, "GcellGrid"); break;
    case defwDefaultCapCbkType: fprintf(f, "DefaultCap"); break;
    case defwCanplaceCbkType: fprintf(f, "Canplace"); break;
    case defwCannotOccupyCbkType: fprintf(f, "CannotOccupy"); break;
    case defwViaCbkType: fprintf(f, "Via"); break;
    case defwRegionCbkType: fprintf(f, "Region"); break;
    case defwComponentCbkType: fprintf(f, "Component"); break;
    case defwPinCbkType: fprintf(f, "Pin"); break;
    case defwPinPropCbkType: fprintf(f, "PinProperty"); break;
    case defwSNetCbkType: fprintf(f, "SpecialNet"); break;
    case defwNetCbkType: fprintf(f, "Net"); break;
    case defwIOTimingCbkType: fprintf(f, "IOTiming"); break;
    case defwScanchainCbkType: fprintf(f, "Scanchain"); break;
    case defwConstraintCbkType: fprintf(f, "Constraint"); break;
    case defwAssertionCbkType: fprintf(f, "Assertion"); break;
    case defwGroupCbkType: fprintf(f, "Group"); break;
    case defwBlockageCbkType: fprintf(f, "Blockages"); break;
    case defwExtCbkType: fprintf(f, "Extension"); break;
    case defwDesignEndCbkType: fprintf(f, "DesignEnd"); break;
      /* NEW CALLBACK  add the print here */
      default: fprintf(f, "BOGUS ENTRY"); break;
      }
      fprintf(f, " %d\n", defwUnusedCount[i]);
    }
  }
}


void defwSetUserData(defiUserData d) {
  defwUserData = d;
}


defiUserData defwGetUserData() {
  return defwUserData;
}


void defwSetArrayCbk(defwVoidCbkFnType f) {
  defwCallbacksSeq[defwArrayCbk] = f;
}


void defwSetAssertionCbk(defwVoidCbkFnType f) {
  defwCallbacksSeq[defwAssertionCbk] = f;
}


void defwSetBlockageCbk(defwVoidCbkFnType f) {
  defwCallbacksSeq[defwBlockageCbk] = f;
}


void defwSetBusBitCbk(defwVoidCbkFnType f) {
  defwCallbacksSeq[defwBusBitCbk] = f;
}


void defwSetCannotOccupyCbk(defwVoidCbkFnType f) {
  defwCallbacksSeq[defwCannotOccupyCbk] = f;
}


void defwSetCanplaceCbk(defwVoidCbkFnType f) {
  defwCallbacksSeq[defwCanplaceCbk] = f;
}


void defwSetCaseSensitiveCbk(defwVoidCbkFnType f) {
  defwCallbacksSeq[defwCaseSensitiveCbk] = f;
}


void defwSetComponentCbk(defwVoidCbkFnType f) {
  defwCallbacksSeq[defwComponentCbk] = f;
}


void defwSetConstraintCbk(defwVoidCbkFnType f) {
  defwCallbacksSeq[defwConstraintCbk] = f;
}


void defwSetDefaultCapCbk(defwVoidCbkFnType f) {
  defwCallbacksSeq[defwDefaultCapCbk] = f;
}


void defwSetDesignCbk(defwVoidCbkFnType f) {
  defwCallbacksSeq[defwDesignCbk] = f;
}


void defwSetDesignEndCbk(defwVoidCbkFnType f) {
  defwCallbacksSeq[defwDesignEndCbk] = f;
}


void defwSetDieAreaCbk(defwVoidCbkFnType f) {
  defwCallbacksSeq[defwDieAreaCbk] = f;
}


void defwSetDividerCbk(defwVoidCbkFnType f) {
  defwCallbacksSeq[defwDividerCbk] = f;
}


void defwSetExtCbk(defwVoidCbkFnType f) {
  defwCallbacksSeq[defwExtCbk] = f;
}


void defwSetFloorPlanCbk(defwVoidCbkFnType f) {
  defwCallbacksSeq[defwFloorPlanCbk] = f;
}


void defwSetGcellGridCbk(defwVoidCbkFnType f) {
  defwCallbacksSeq[defwGcellGridCbk] = f;
}


void defwSetGroupCbk(defwVoidCbkFnType f) {
  defwCallbacksSeq[defwGroupCbk] = f;
}


void defwSetHistoryCbk(defwVoidCbkFnType f) {
  defwCallbacksSeq[defwHistoryCbk] = f;
}


void defwSetIOTimingCbk(defwVoidCbkFnType f) {
  defwCallbacksSeq[defwIOTimingCbk] = f;
}


void defwSetNetCbk(defwVoidCbkFnType f) {
  defwCallbacksSeq[defwNetCbk] = f;
}


void defwSetPinCbk(defwVoidCbkFnType f) {
  defwCallbacksSeq[defwPinCbk] = f;
}


void defwSetPinPropCbk(defwVoidCbkFnType f) {
  defwCallbacksSeq[defwPinPropCbk] = f;
}


void defwSetPropDefCbk(defwVoidCbkFnType f) {
  defwCallbacksSeq[defwPropDefCbk] = f;
}


void defwSetRegionCbk(defwVoidCbkFnType f) {
  defwCallbacksSeq[defwRegionCbk] = f;
}


void defwSetRowCbk(defwVoidCbkFnType f) {
  defwCallbacksSeq[defwRowCbk] = f;
}


void defwSetSNetCbk(defwVoidCbkFnType f) {
  defwCallbacksSeq[defwSNetCbk] = f;
}


void defwSetScanchainCbk(defwVoidCbkFnType f) {
  defwCallbacksSeq[defwScanchainCbk] = f;
}


void defwSetTechnologyCbk(defwVoidCbkFnType f) {
  defwCallbacksSeq[defwTechCbk] = f;
}


void defwSetTrackCbk(defwVoidCbkFnType f) {
  defwCallbacksSeq[defwTrackCbk] = f;
}


void defwSetUnitsCbk(defwVoidCbkFnType f) {
  defwCallbacksSeq[defwUnitsCbk] = f;
}


void defwSetVersionCbk(defwVoidCbkFnType f) {
  defwCallbacksSeq[defwVersionCbk] = f;
}


void defwSetViaCbk(defwVoidCbkFnType f) {
  defwCallbacksSeq[defwViaCbk] = f;
}


/* NEW CALLBACK - Each callback routine must have a routine that allows
 * the user to set it.  The set routines go here. */


void defwSetLogFunction(DEFI_LOG_FUNCTION f) {
  defiErrorLogFunction = f;
}

void defwSetWarningLogFunction(DEFI_WARNING_LOG_FUNCTION f) {
  defiWarningLogFunction = f;
}

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

#ifndef lefiLayer_h
#define lefiLayer_h

#include <stdio.h>
#include "lefiKRDefs.hpp"

typedef enum lefiAntennaEnum {
  lefiAntennaAR,
  lefiAntennaDAR,
  lefiAntennaCAR,
  lefiAntennaCDAR,
  lefiAntennaAF,
  lefiAntennaSAR,
  lefiAntennaDSAR,
  lefiAntennaCSAR,
  lefiAntennaCDSAR,
  lefiAntennaSAF,
  lefiAntennaO
} lefiAntennaEnum;

class lefiAntennaPWL {
public:
  lefiAntennaPWL();
  ~lefiAntennaPWL();

  void Init();
  void clear();
  void Destroy();

  void addAntennaPWL(double d, double r);

  int  numPWL() const;
  double PWLdiffusion(int index); 
  double PWLratio(int index); 

protected:
  int numAlloc_;
  int numPWL_;
  double* d_;
  double* r_;
};

class lefiLayerDensity {
public:
  lefiLayerDensity();
  ~lefiLayerDensity();

  void Init(const char* type);
  void Destroy();
  void clear();

  void setOneEntry(double entry);
  void addFrequency(int num, double* frequency);
  void addWidth(int num, double* width);
  void addTableEntry(int num, double* entry);
  void addCutarea(int num, double* cutarea);

  char* type();
  int hasOneEntry();
  double oneEntry();
  int numFrequency();
  double frequency(int index);
  int numWidths();
  double width(int index);
  int numTableEntries();
  double tableEntry(int index);
  int numCutareas();
  double cutArea(int index);

protected:
  char* type_;
  double oneEntry_;
  int numFrequency_;
  double* frequency_;
  int numWidths_;
  double* widths_;
  int numTableEntries_;
  double* tableEntries_;
  int numCutareas_;
  double* cutareas_;
};


// 5.5
class lefiParallel {
public:
  lefiParallel();
  ~lefiParallel();

  void Init();
  void clear();
  void Destroy();

  void addParallelLength(int numLength, double* lengths);
  void addParallelWidth(double width);
  void addParallelWidthSpacing(int numSpacing, double* spacings);

  int  numLength() const;
  int  numWidth() const;
  double length(int iLength) const;
  double width(int iWidth) const;
  double widthSpacing(int iWidth, int iWidthSpacing) const;

protected:
  int numLength_;
  int numWidth_;
  int numWidthAllocated_;
  double* length_;
  double* width_;
  double* widthSpacing_;
};


// 5.5
class lefiInfluence {
public:
  lefiInfluence();
  ~lefiInfluence();

  void Init();
  void clear();
  void Destroy();

  void addInfluence(double width, double distance, double spacing);

  int  numInfluenceEntry() const;
  double width(int index) const;
  double distance(int index) const;
  double spacing(int index) const;

protected:
  int numAllocated_;
  int numWidth_;
  int numDistance_;
  int numSpacing_;
  double* width_;
  double* distance_;
  double* spacing_;
};

// 5.5
class lefiSpacingTable {
public:
  lefiSpacingTable();
  ~lefiSpacingTable();

  void Init();
  void clear();
  void Destroy();

  void addParallelLength(int numLength, double* lengths);
  void addParallelWidth(double width);
  void addParallelWidthSpacing(int numSpacing, double* spacing);
  void setInfluence();
  void addInfluence(double width, double distance, double spacing);

  int isInfluence() const;
  lefiInfluence* influence() const;
  lefiParallel*  parallel() const;

protected:
  int hasInfluence_;
  lefiInfluence*   influence_;
  lefiParallel*    parallel_;
};

// 5.5
class lefiAntennaModel {
public:
  lefiAntennaModel();
  ~lefiAntennaModel();

  void Init();
  void clear();
  void Destroy();

  void setAntennaModel(int oxide);
  void setAntennaAreaRatio(double value);
  void setAntennaCumAreaRatio(double value);
  void setAntennaAreaFactor(double value);
  void setAntennaSideAreaRatio(double value);
  void setAntennaCumSideAreaRatio(double value);
  void setAntennaSideAreaFactor(double value);
  void setAntennaValue(lefiAntennaEnum antennaType, double value);
  void setAntennaDUO(lefiAntennaEnum antennaType);
  void setAntennaPWL(lefiAntennaEnum antennaType, lefiAntennaPWL* pwl);

  int hasAntennaAreaRatio() const;
  int hasAntennaDiffAreaRatio() const;
  int hasAntennaDiffAreaRatioPWL() const;
  int hasAntennaCumAreaRatio() const;
  int hasAntennaCumDiffAreaRatio() const;
  int hasAntennaCumDiffAreaRatioPWL() const;
  int hasAntennaAreaFactor() const;
  int hasAntennaAreaFactorDUO() const;
  int hasAntennaSideAreaRatio() const;
  int hasAntennaDiffSideAreaRatio() const;
  int hasAntennaDiffSideAreaRatioPWL() const;
  int hasAntennaCumSideAreaRatio() const;
  int hasAntennaCumDiffSideAreaRatio() const;
  int hasAntennaCumDiffSideAreaRatioPWL() const;
  int hasAntennaSideAreaFactor() const;
  int hasAntennaSideAreaFactorDUO() const;

  char*  antennaOxide() const;
  double antennaAreaRatio() const;
  double antennaDiffAreaRatio() const;
  lefiAntennaPWL* antennaDiffAreaRatioPWL() const;
  double antennaCumAreaRatio() const;
  double antennaCumDiffAreaRatio() const;
  lefiAntennaPWL* antennaCumDiffAreaRatioPWL() const;
  double antennaAreaFactor() const;
  double antennaSideAreaRatio() const;
  double antennaDiffSideAreaRatio() const;
  lefiAntennaPWL* antennaDiffSideAreaRatioPWL() const;
  double antennaCumSideAreaRatio() const;
  double antennaCumDiffSideAreaRatio() const;
  lefiAntennaPWL* antennaCumDiffSideAreaRatioPWL() const;
  double antennaSideAreaFactor() const;

protected:
  int hasAntennaAreaRatio_;
  int hasAntennaDiffAreaRatio_;
  int hasAntennaDiffAreaRatioPWL_;
  int hasAntennaCumAreaRatio_;
  int hasAntennaCumDiffAreaRatio_;
  int hasAntennaCumDiffAreaRatioPWL_;
  int hasAntennaAreaFactor_;
  int hasAntennaAreaFactorDUO_;
  int hasAntennaSideAreaRatio_;
  int hasAntennaDiffSideAreaRatio_;
  int hasAntennaDiffSideAreaRatioPWL_;
  int hasAntennaCumSideAreaRatio_;
  int hasAntennaCumDiffSideAreaRatio_;
  int hasAntennaCumDiffSideAreaRatioPWL_;
  int hasAntennaSideAreaFactor_;
  int hasAntennaSideAreaFactorDUO_;

  char*  oxide_;
  double antennaAreaRatio_;
  double antennaDiffAreaRatio_;
  lefiAntennaPWL* antennaDiffAreaRatioPWL_;
  double antennaCumAreaRatio_;
  double antennaCumDiffAreaRatio_;
  lefiAntennaPWL* antennaCumDiffAreaRatioPWL_;
  double antennaAreaFactor_;
  double antennaSideAreaRatio_;
  double antennaDiffSideAreaRatio_;
  lefiAntennaPWL* antennaDiffSideAreaRatioPWL_;
  double antennaCumSideAreaRatio_;
  double antennaCumDiffSideAreaRatio_;
  lefiAntennaPWL* antennaCumDiffSideAreaRatioPWL_;
  double antennaSideAreaFactor_;
};

class lefiLayer {
public:
  lefiLayer();
  void Init();

  void Destroy();
  ~lefiLayer();

  void clear();
  void setName(const char* name); // calls clear to init
  void setType(const char* typ);
  void setPitch(double num);
  void setOffset(double num);
  void setWidth(double num);
  void setArea(double num);
  void setSpacingMin(double dist);
  void setSpacingName(const char* spacingName);    // for CUT layer
  void setSpacingAdjacent(int numCuts, double distance);  // 5.5for CUT layer
  void setSpacingRange(double left, double right);
  void setSpacingRangeUseLength();
  void setSpacingRangeInfluence(double infLength);
  void setSpacingRangeInfluenceRange(double min, double max);
  void setSpacingRangeRange(double min, double max);
  void setSpacingLength(double num);
  void setSpacingLengthRange(double min, double max);
  void setDirection(const char* dir);
  void setResistance(double num);
  void setCapacitance(double num);
  void setHeight(double num);
  void setWireExtension(double num);
  void setThickness(double num);
  void setShrinkage(double num);
  void setCapMultiplier(double num);
  void setEdgeCap(double num);
  void setAntennaArea(double num);
  void setAntennaLength(double num);
  void setCurrentDensity(double num);
  void setCurrentPoint(double width, double current);
  void setResistancePoint(double width, double res);
  void setCapacitancePoint(double width, double cap);
  void addProp(const char* name, const char* value, const char type);
  void addNumProp(const char* name, const double d,
                  const char* value, const char type);
  void addAccurrentDensity(const char* type);
  void setAcOneEntry(double num);
  void addAcFrequency();
  void addAcCutarea();
  void addAcTableEntry();
  void addAcWidth();
  void addDccurrentDensity(const char* type);
  void setDcOneEntry(double num);
  void addDcTableEntry();
  void addDcWidth();
  void addDcCutarea();
  void addNumber(double num);
  void setMaxwidth(double width);                                   // 5.5
  void setMinwidth(double width);                                   // 5.5
  void addMinenclosedarea(double area);                             // 5.5
  void addMinenclosedareaWidth(double width);                       // 5.5
  void addMinimumcut(int cuts, double width);                       // 5.5
  void addMinimumcutConnect(const char* direction);                 // 5.5
  void addMinimumcutLengDis(double length, double distance);        // 5.5
  void setMinstep(double distance);                                 // 5.5
  void addParellelLength(double length);                            // 5.5
  void addParellelSpacing(double width, double spacing);            // 5.5
  void addParellelWidth(double width);                              // 5.5
  void setProtrusion(double width, double length, double width2);   // 5.5

  int  getNumber();     // this is for the parser internal use only

  // 5.5 SPACINGTABLE
  void addSpacingTable();
  void addSpParallelLength();
  void addSpParallelWidth(double width);
  void addSpParallelWidthSpacing();
  void setInfluence();
  void addSpInfluence(double width, double distance, double spacing);

  int hasType() const ;
  int hasPitch() const ;
  int hasOffset() const ;
  int hasWidth() const ;
  int hasArea() const ;
  int hasSpacingNumber() const ;
  int hasSpacingName(int index) const ;
  int hasSpacingAdjacent(int index) const ;
  int hasSpacingRange(int index) const ;
  int hasSpacingRangeUseLengthThreshold(int index) const;
  int hasSpacingRangeInfluence(int index) const;
  int hasSpacingRangeInfluenceRange(int index) const;
  int hasSpacingRangeRange(int index) const;
  int hasSpacingLengthThreshold(int index) const;
  int hasSpacingLengthThresholdRange(int index) const;
  int hasDirection() const ;
  int hasResistance() const ;
  int hasResistanceArray() const ;
  int hasCapacitance() const ;
  int hasCapacitanceArray() const ;
  int hasHeight() const ;
  int hasThickness() const ;
  int hasWireExtension() const ;
  int hasShrinkage() const ;
  int hasCapMultiplier() const ;
  int hasEdgeCap() const ;
  int hasAntennaLength() const ;
  int hasAntennaArea() const ;
  int hasCurrentDensityPoint() const ;
  int hasCurrentDensityArray() const ;
  int hasAccurrentDensity() const;
  int hasDccurrentDensity() const;

  int numProps() const;
  const char*  propName(int index) const;
  const char*  propValue(int index) const;
  double propNumber(int index) const;
  const char   propType(int index) const;
  int    propIsNumber(int index) const;
  int    propIsString(int index) const;

  int numSpacing() const;

  char* name() const ;
  const char* type() const ;
  double pitch() const ;
  double offset() const ;
  double width() const ;
  double area() const ;
  double spacing(int index) const ;
  char*  spacingName(int index) const ;     // for CUT layer
  int    spacingAdjacentCuts(int index) const ;   // 5.5 - for CUT layer
  double spacingAdjacentWithin(int index) const ; // 5.5 - for CUT layer
  double spacingRangeMin(int index) const ;
  double spacingRangeMax(int index) const ;
  double spacingRangeInfluence(int index) const ;
  double spacingRangeInfluenceMin(int index) const ;
  double spacingRangeInfluenceMax(int index) const ;
  double spacingRangeRangeMin(int index) const ;
  double spacingRangeRangeMax(int index) const ;
  double spacingLengthThreshold(int index) const;
  double spacingLengthThresholdRangeMin(int index) const;
  double spacingLengthThresholdRangeMax(int index) const;

  // 5.5 Minimum cut rules
  int    numMinimumcut() const;
  int    minimumcut(int index) const;
  double minimumcutWidth(int index) const;
  int    hasMinimumcutConnection(int index) const;      // FROMABOVE|FROMBELOW
  const  char* minimumcutConnection(int index) const;   // FROMABOVE|FROMBELOW
  int    hasMinimumcutNumCuts(int index) const;
  double minimumcutLength(int index) const;
  double minimumcutDistance(int index) const;

  const char* direction() const ;
  double resistance() const ;
  double capacitance() const ;
  double height() const ;
  double wireExtension() const ;
  double thickness() const ;
  double shrinkage() const ;
  double capMultiplier() const ;
  double edgeCap() const ;
  double antennaLength() const ;
  double antennaArea() const ;
  double currentDensityPoint() const ;
  void currentDensityArray(int* numPoints,
	   double** widths, double** current) const ;
  void capacitanceArray(int* numPoints,
	   double** widths, double** resValues) const ;
  void resistanceArray(int* numPoints,
	   double** widths, double** capValues) const ;

  int numAccurrentDensity() const;
  lefiLayerDensity* accurrent(int index) const;
  int numDccurrentDensity() const;
  lefiLayerDensity* dccurrent(int index) const;

  // 3/23/2000 - Wanda da Rosa.  The following are for 5.4 Antenna.
  //             Only 5.4 or 5.3 are allowed in a lef file, but not both
  void setAntennaAreaRatio(double value);
  void setAntennaCumAreaRatio(double value);
  void setAntennaAreaFactor(double value);
  void setAntennaSideAreaRatio(double value);
  void setAntennaCumSideAreaRatio(double value);
  void setAntennaSideAreaFactor(double value);
  void setAntennaValue(lefiAntennaEnum antennaType, double value);
  void setAntennaDUO(lefiAntennaEnum antennaType);
  void setAntennaPWL(lefiAntennaEnum antennaType, lefiAntennaPWL* pwl);
  void addAntennaModel (int aOxide);  // 5.5

  // 5.5
  int numAntennaModel() const;
  lefiAntennaModel* antennaModel(int index) const;

  // The following is 8/21/01 5.4 enhancements 
  void setSlotWireWidth(double num);
  void setSlotWireLength(double num);
  void setSlotWidth(double num);
  void setSlotLength(double num);
  void setMaxAdjacentSlotSpacing(double num);
  void setMaxCoaxialSlotSpacing(double num);
  void setMaxEdgeSlotSpacing(double num);
  void setSplitWireWidth(double num);
  void setMinimumDensity(double num);
  void setMaximumDensity(double num);
  void setDensityCheckWindow(double length, double width);
  void setDensityCheckStep(double num);
  void setFillActiveSpacing(double num);

  int hasSlotWireWidth() const;
  int hasSlotWireLength() const;
  int hasSlotWidth() const;
  int hasSlotLength() const;
  int hasMaxAdjacentSlotSpacing() const;
  int hasMaxCoaxialSlotSpacing() const;
  int hasMaxEdgeSlotSpacing() const;
  int hasSplitWireWidth() const;
  int hasMinimumDensity() const;
  int hasMaximumDensity() const;
  int hasDensityCheckWindow() const;
  int hasDensityCheckStep() const;
  int hasFillActiveSpacing() const;
  int hasMaxwidth() const;                     // 5.5
  int hasMinwidth() const;                     // 5.5
  int hasMinstep() const;                      // 5.5
  int hasProtrusion() const;                   // 5.5

  double slotWireWidth() const;
  double slotWireLength() const;
  double slotWidth() const;
  double slotLength() const;
  double maxAdjacentSlotSpacing() const;
  double maxCoaxialSlotSpacing() const;
  double maxEdgeSlotSpacing() const;
  double splitWireWidth() const;
  double minimumDensity() const;
  double maximumDensity() const;
  double densityCheckWindowLength() const;
  double densityCheckWindowWidth() const;
  double densityCheckStep() const;
  double fillActiveSpacing() const;
  double maxwidth() const;                     // 5.5
  double minwidth() const;                     // 5.5
  double minstep() const;                      // 5.5
  double protrusionWidth1() const;             // 5.5
  double protrusionLength() const;             // 5.5
  double protrusionWidth2() const;             // 5.5

  // 5.5 MINENCLOSEDAREA
  int    numMinenclosedarea() const;
  double minenclosedarea(int index) const;
  int    hasMinenclosedareaWidth(int index) const;
  double minenclosedareaWidth(int index) const;

  // 5.5 SPACINGTABLE
  int               numSpacingTable();
  lefiSpacingTable* spacingTable(int index);

  // Debug print
  void print(FILE* f) const;

protected:
  char* name_;
  int nameSize_;
  char* type_;
  int typeSize_;

  int hasPitch_;
  int hasOffset_;
  int hasWidth_;
  int hasArea_;
  int hasSpacing_;
  int* hasSpacingName_;               // 5.5
  int* hasSpacingAdjacent_;           // 5.5
  int* hasSpacingRange_;              // pcr 409334
  int* hasSpacingUseLengthThreshold_; // pcr 282799, due to mult. spacing allow
  int* hasSpacingLengthThreshold_;    // pcr 409334
  int hasDirection_;
  int hasResistance_;
  int hasCapacitance_;
  int hasHeight_;
  int hasWireExtension_;
  int hasThickness_;
  int hasShrinkage_;
  int hasCapMultiplier_;
  int hasEdgeCap_;
  int hasAntennaArea_;
  int hasAntennaLength_;
  int hasCurrentDensityPoint_;

  double currentDensity_;
  double pitch_;
  double offset_;
  double width_;
  double area_;
  double wireExtension_;
  int numSpacings_;
  int spacingsAllocated_;
  double* spacing_;          // for Cut & routing Layer, spacing is multiple
  char**  spacingName_;
  int*    spacingAdjacentCuts_;    // 5.5
  double* spacingAdjacentWithin_;  // 5.5
  double* rangeMin_;         // pcr 282799 & 408930, due to mult spacing allow
  double* rangeMax_;         // pcr 282799 & 408930, due to mult spacing allow
  double* rangeInfluence_;   // pcr 282799 & 408930, due to mult spacing allow
  double* rangeInfluenceRangeMin_;          // pcr 388183 & 408930
  double* rangeInfluenceRangeMax_;          // pcr 388183 & 408930
  double* rangeRangeMin_;                   // pcr 408930
  double* rangeRangeMax_;                   // pcr 408930
  double* lengthThreshold_;                 // pcr 408930
  double* lengthThresholdRangeMin_;         // pcr 408930
  double* lengthThresholdRangeMax_;         // pcr 408930

  // 5.5
  int     numMinimumcut_;
  int     minimumcutAllocated_;
  int*    minimumcut_;                       // pcr 409334
  double* minimumcutWidth_;                  // pcr 409334
  int*    hasMinimumcutConnection_;
  char**  minimumcutConnection_;
  int*    hasMinimumcutNumCuts_;
  double* minimumcutLength_;
  double* minimumcutDistance_;

  double  maxwidth_;                          // 5.5
  double  minwidth_;                          // 5.5
  int     numMinenclosedarea_;                // 5.5
  int     minenclosedareaAllocated_;          // 5.5
  double* minenclosedarea_;                   // 5.5
  double* minenclosedareaWidth_;              // 5.5
  double  minstep_;                           // 5.5
  double  protrusionWidth1_;                  // 5.5
  double  protrusionLength_;                  // 5.5
  double  protrusionWidth2_;                  // 5.5

  char*  direction_;
  double resistance_;
  double capacitance_;
  double height_;
  double thickness_;
  double shrinkage_;
  double capMultiplier_;
  double edgeCap_;
  double antennaArea_;
  double antennaLength_;

  int numCurrentPoints_;
  int currentPointsAllocated_;
  double* currentWidths_;
  double* current_;

  int numCapacitancePoints_;
  int capacitancePointsAllocated_;
  double* capacitanceWidths_;
  double* capacitances_;

  int numResistancePoints_;
  int resistancePointsAllocated_;
  double* resistanceWidths_;
  double* resistances_;

  int numProps_;
  int propsAllocated_;
  char**  names_;
  char**  values_;
  double* dvalues_;
  char*   types_;                     // I: integer, R: real, S:string
                                      // Q: quotedstring
  int numAccurrents_;                 // number of ACCURRENTDENSITY
  int accurrentAllocated_;
  lefiLayerDensity** accurrents_;
  int numDccurrents_;                 // number of DCCURRENTDENSITY
  int dccurrentAllocated_;
  lefiLayerDensity** dccurrents_;
  int numNums_;
  int numAllocated_;
  double* nums_;

  // 3/23/2000 - Wanda da Rosa.  The following is for 5.4 ANTENNA.
  //             Either 5.4 or 5.3 are allowed, not both
  int hasAntennaAreaRatio_;
  int hasAntennaDiffAreaRatio_;
  int hasAntennaDiffAreaRatioPWL_;
  int hasAntennaCumAreaRatio_;
  int hasAntennaCumDiffAreaRatio_;
  int hasAntennaCumDiffAreaRatioPWL_;
  int hasAntennaAreaFactor_;
  int hasAntennaAreaFactorDUO_;
  int hasAntennaSideAreaRatio_;
  int hasAntennaDiffSideAreaRatio_;
  int hasAntennaDiffSideAreaRatioPWL_;
  int hasAntennaCumSideAreaRatio_;
  int hasAntennaCumDiffSideAreaRatio_;
  int hasAntennaCumDiffSideAreaRatioPWL_;
  int hasAntennaSideAreaFactor_;
  int hasAntennaSideAreaFactorDUO_;

  // 5.5 AntennaModel
  int numAntennaModel_;
  int antennaModelAllocated_;
  lefiAntennaModel** antennaModel_;

  // 8/29/2001 - Wanda da Rosa.  The following is for 5.4 enhancements.
  int hasSlotWireWidth_;
  int hasSlotWireLength_;
  int hasSlotWidth_;
  int hasSlotLength_;
  int hasMaxAdjacentSlotSpacing_;
  int hasMaxCoaxialSlotSpacing_;
  int hasMaxEdgeSlotSpacing_;
  int hasSplitWireWidth_;
  int hasMinimumDensity_;
  int hasMaximumDensity_;
  int hasDensityCheckWindow_;
  int hasDensityCheckStep_;
  int hasFillActiveSpacing_;

  double slotWireWidth_; 
  double slotWireLength_; 
  double slotWidth_; 
  double slotLength_; 
  double maxAdjacentSlotSpacing_; 
  double maxCoaxialSlotSpacing_; 
  double maxEdgeSlotSpacing_; 
  double splitWireWidth_; 
  double minimumDensity_; 
  double maximumDensity_; 
  double densityCheckWindowLength_; 
  double densityCheckWindowWidth_; 
  double densityCheckStep_; 
  double fillActiveSpacing_; 

  // 5.5 SPACINGTABLE
  int numSpacingTable_;
  int spacingTableAllocated_;
  lefiSpacingTable** spacingTable_;
};
#endif

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

#ifndef lefiMacro_h
#define lefiMacro_h

#include <stdio.h>
#include "lefiKRDefs.hpp"
#include "lefiMisc.hpp"

// The different types of items in a geometry list.

struct lefiGeomRect {
  double xl;
  double yl;
  double xh;
  double yh;
};

struct lefiGeomRectIter {
  double xl;
  double yl;
  double xh;
  double yh;
  double xStart;
  double yStart;
  double xStep;
  double yStep;
};

struct lefiGeomPath {
  int numPoints;
  double* x;
  double* y;
};

struct lefiGeomPathIter {
  int numPoints;
  double* x;
  double* y;
  double xStart;
  double yStart;
  double xStep;
  double yStep;
};

struct lefiGeomPolygon {
  int numPoints;
  double* x;
  double* y;
};

struct lefiGeomPolygonIter {
  int numPoints;
  double* x;
  double* y;
  double xStart;
  double yStart;
  double xStep;
  double yStep;
};

enum lefiGeomEnum {
  lefiGeomUnknown = 0,
  lefiGeomLayerE,
  lefiGeomLayerMinSpacingE,
  lefiGeomLayerRuleWidthE,
  lefiGeomWidthE,
  lefiGeomPathE,
  lefiGeomPathIterE,
  lefiGeomRectE,
  lefiGeomRectIterE,
  lefiGeomPolygonE,
  lefiGeomPolygonIterE,
  lefiGeomViaE,
  lefiGeomViaIterE,
  lefiGeomClassE,
  lefiGeomEnd
};

struct lefiGeomVia {
  char* name;
  double x;
  double y;
};

struct lefiGeomViaIter {
  char* name;
  double x;
  double y;
  double xStart;
  double yStart;
  double xStep;
  double yStep;
};


class lefiGeometries {
public:
  lefiGeometries();
  void Init();

  void Destroy();
  ~lefiGeometries();

  void clear();
  void add(void* v, enum lefiGeomEnum e);
  void addLayer(const char* name);
  void addLayerMinSpacing(double spacing);
  void addLayerRuleWidth(double width);
  void addClass(const char* name);
  void addWidth(double w);
  void addPath();
  void addPathIter();
  void addRect(double xl, double yl, double xh, double yh);
  void addRectIter(double xl, double yl, double xh, double yh);
  void addPolygon();
  void addPolygonIter();
  void addVia(double x, double y, const char* name);
  void addViaIter(double x, double y, const char* name);
  void addStepPattern(double xStart, double yStart,
		      double xStep, double yStep);
  void startList(double x, double y);
  void addToList(double x, double y);

  int numItems() const;
  enum lefiGeomEnum itemType(int index) const;
  struct lefiGeomRect* getRect(int index) const;
  struct lefiGeomRectIter* getRectIter(int index) const;
  struct lefiGeomPath* getPath(int index) const;
  struct lefiGeomPathIter* getPathIter(int index) const;
  char* getLayer(int index) const; 
  double getLayerMinSpacing(int index) const;
  double getLayerRuleWidth(int index) const;
  double getWidth(int index) const; 
  struct lefiGeomPolygon* getPolygon(int index) const;
  struct lefiGeomPolygonIter* getPolygonIter(int index) const;
  char* getClass(int index) const; 
  struct lefiGeomVia* getVia(int index) const; 
  struct lefiGeomViaIter* getViaIter(int index) const; 

  void print(FILE* f) const;

protected:
  int numItems_;
  int itemsAllocated_;
  enum lefiGeomEnum* itemType_;
  void** items_;

  int numPoints_;
  int pointsAllocated_;
  double* x_;
  double* y_;

  double xStart_;
  double yStart_;
  double xStep_;
  double yStep_;
};

class lefiObstruction {
public:
  lefiObstruction();
  void Init();

  void Destroy();
  ~lefiObstruction();

  void clear();
  void setGeometries(lefiGeometries* g);

  lefiGeometries* geometries() const;

  void print(FILE* f) const;

protected:

  lefiGeometries* geometries_;
};

// 5.5
class lefiPinAntennaModel {
public:
  lefiPinAntennaModel();
  ~lefiPinAntennaModel();

  void Init();
  void clear();
  void Destroy();

  void setAntennaModel(int oxide);
  void addAntennaGateArea(double value, const char* layer);
  void addAntennaMaxAreaCar(double value, const char* layer);
  void addAntennaMaxSideAreaCar(double value, const char* layer);
  void addAntennaMaxCutCar(double value, const char* layer);

  int hasAntennaGateArea() const;
  int hasAntennaMaxAreaCar() const;
  int hasAntennaMaxSideAreaCar() const;
  int hasAntennaMaxCutCar() const;

  char* antennaOxide() const;

  int numAntennaGateArea() const;
  double antennaGateArea(int index) const;
  const char* antennaGateAreaLayer(int index) const;

  int numAntennaMaxAreaCar() const;
  double antennaMaxAreaCar(int index) const;
  const char* antennaMaxAreaCarLayer(int index) const;

  int numAntennaMaxSideAreaCar() const;
  double antennaMaxSideAreaCar(int index) const;
  const char* antennaMaxSideAreaCarLayer(int index) const;

  int numAntennaMaxCutCar() const;
  double antennaMaxCutCar(int index) const;
  const char* antennaMaxCutCarLayer(int index) const;

protected:
  char* oxide_;

  int numAntennaGateArea_;
  int antennaGateAreaAllocated_;
  double* antennaGateArea_;
  char** antennaGateAreaLayer_;

  int numAntennaMaxAreaCar_;
  int antennaMaxAreaCarAllocated_;
  double* antennaMaxAreaCar_;
  char** antennaMaxAreaCarLayer_;

  int numAntennaMaxSideAreaCar_;
  int antennaMaxSideAreaCarAllocated_;
  double* antennaMaxSideAreaCar_;
  char** antennaMaxSideAreaCarLayer_;

  int numAntennaMaxCutCar_;
  int antennaMaxCutCarAllocated_;
  double* antennaMaxCutCar_;
  char** antennaMaxCutCarLayer_;
};

class lefiPin {
public:
  lefiPin();
  void Init();

  void Destroy();
  ~lefiPin();

  void clear();
  void bump(char** array, int len, int* size);
  void setName(const char* name);
  void addPort(lefiGeometries* g);
  void setForeign(const char* name, int hasPnt, double x, double y, int orient);
  void setLEQ(const char* name);
  void setDirection(const char* name);
  void setUse(const char* name);
  void setShape(const char* name);
  void setMustjoin(const char* name);
  void setOutMargin(double high, double low);
  void setOutResistance(double high, double low);
  void setInMargin(double high, double low);
  void setPower(double power);
  void setLeakage(double current);
  void setMaxload(double capacitance);
  void setMaxdelay(double delayTime);
  void setCapacitance(double capacitance);
  void setResistance(double resistance);
  void setPulldownres(double resistance);
  void setTieoffr(double resistance);
  void setVHI(double voltage);
  void setVLO(double voltage);
  void setRiseVoltage(double voltage);
  void setFallVoltage(double voltage);
  void setRiseThresh(double capacitance);
  void setFallThresh(double capacitance);
  void setRiseSatcur(double current);
  void setFallSatcur(double current);
  void setCurrentSource(const char* name);
  void setTables(const char* highName, const char* lowName);
  void setProperty(const char* name, const char* value, const char type);
  void setNumProperty(const char* name, double d, const char* value,
                      const char type);
  void addAntennaModel(int oxide);       // 5.5
  void addAntennaSize(double value, const char* layer);
  void addAntennaMetalArea(double value, const char* layer);
  void addAntennaMetalLength(double value, const char* layer);
  void addAntennaPartialMetalArea(double value, const char* layer);
  void addAntennaPartialMetalSideArea(double value, const char* layer);
  void addAntennaGateArea(double value, const char* layer);
  void addAntennaDiffArea(double value, const char* layer);
  void addAntennaMaxAreaCar(double value, const char* layer);
  void addAntennaMaxSideAreaCar(double value, const char* layer);
  void addAntennaPartialCutArea(double value, const char* layer);
  void addAntennaMaxCutCar(double value, const char* layer);
  void setRiseSlewLimit(double value);
  void setFallSlewLimit(double value);
  void setTaperRule(const char* name);
  void bumpProps();

  int hasForeign() const;
  int hasForeignOrient() const;
  int hasForeignPoint() const;
  int hasLEQ() const;
  int hasDirection() const;
  int hasUse() const;
  int hasShape() const;
  int hasMustjoin() const;
  int hasOutMargin() const; 
  int hasOutResistance() const;
  int hasInMargin() const;
  int hasPower() const;
  int hasLeakage() const;
  int hasMaxload() const;
  int hasMaxdelay() const;
  int hasCapacitance() const;
  int hasResistance() const;
  int hasPulldownres() const;
  int hasTieoffr() const;
  int hasVHI() const;
  int hasVLO() const;
  int hasRiseVoltage() const;
  int hasFallVoltage() const;
  int hasRiseThresh() const;
  int hasFallThresh() const;
  int hasRiseSatcur() const;
  int hasFallSatcur() const;
  int hasCurrentSource() const;
  int hasTables() const;
  int hasAntennaSize() const;
  int hasAntennaMetalArea() const;
  int hasAntennaMetalLength() const;
  int hasAntennaPartialMetalArea() const;
  int hasAntennaPartialMetalSideArea() const;
  int hasAntennaPartialCutArea() const;
  int hasAntennaDiffArea() const;
  int hasAntennaModel() const;         // 5.5
  int hasTaperRule() const;
  int hasRiseSlewLimit() const;
  int hasFallSlewLimit() const;

  const char* name() const;

  int numPorts() const;
  lefiGeometries* port(int index) const;

  const char* foreignName() const;
  const char* taperRule() const;
  int foreignOrient() const;
  const char* foreignOrientStr() const;
  double foreignX() const;
  double foreignY() const;
  const char* LEQ() const;
  const char* direction() const;
  const char* use() const;
  const char* shape() const;
  const char* mustjoin() const;
  double outMarginHigh() const;
  double outMarginLow() const;
  double outResistanceHigh() const;
  double outResistanceLow() const;
  double inMarginHigh() const;
  double inMarginLow() const;
  double power() const;
  double leakage() const;
  double maxload() const;
  double maxdelay() const;
  double capacitance() const;
  double resistance() const;
  double pulldownres() const;
  double tieoffr() const;
  double VHI() const;
  double VLO() const;
  double riseVoltage() const;
  double fallVoltage() const;
  double riseThresh() const;
  double fallThresh() const;
  double riseSatcur() const;
  double fallSatcur() const;
  double riseSlewLimit() const;
  double fallSlewLimit() const;
  const char* currentSource() const;
  const char* tableHighName() const;
  const char* tableLowName() const;

  int numAntennaSize() const;
  double antennaSize(int index) const;
  const char* antennaSizeLayer(int index) const;

  int numAntennaMetalArea() const;
  double antennaMetalArea(int index) const;
  const char* antennaMetalAreaLayer(int index) const;

  int numAntennaMetalLength() const;
  double antennaMetalLength(int index) const;
  const char* antennaMetalLengthLayer(int index) const;

  int numAntennaPartialMetalArea() const;
  double antennaPartialMetalArea(int index) const;
  const char* antennaPartialMetalAreaLayer(int index) const;

  int numAntennaPartialMetalSideArea() const;
  double antennaPartialMetalSideArea(int index) const;
  const char* antennaPartialMetalSideAreaLayer(int index) const;

  int numAntennaPartialCutArea() const;
  double antennaPartialCutArea(int index) const;
  const char* antennaPartialCutAreaLayer(int index) const;

  int numAntennaDiffArea() const;
  double antennaDiffArea(int index) const;
  const char* antennaDiffAreaLayer(int index) const;

  // 5.5
  int numAntennaModel() const;
  lefiPinAntennaModel* antennaModel(int index) const;

  int numProperties() const;
  const char* propName(int index) const;
  const char* propValue(int index) const;
  double propNum(int index) const;
  const char  propType(int index) const;
  int   propIsNumber(int index) const;
  int   propIsString(int index) const;

  void print(FILE* f) const ;

protected:
  int nameSize_;
  char* name_;

  char hasForeign_;
  char hasForeignOrient_;
  char hasForeignPoint_;
  char hasLEQ_;
  char hasDirection_;
  char hasUse_;
  char hasShape_;
  char hasMustjoin_;
  char hasOutMargin_;
  char hasOutResistance_;
  char hasInMargin_;
  char hasPower_;
  char hasLeakage_;
  char hasMaxload_;
  char hasMaxdelay_;
  char hasCapacitance_;
  char hasResistance_;
  char hasPulldownres_;
  char hasTieoffr_;
  char hasVHI_; 
  char hasVLO_;
  char hasRiseVoltage_;
  char hasFallVoltage_;
  char hasRiseThresh_;
  char hasFallThresh_;
  char hasRiseSatcur_;
  char hasFallSatcur_;
  char hasCurrentSource_;
  char hasTables_;
  char hasAntennasize_;
  char hasRiseSlewLimit_;
  char hasFallSlewLimit_;

  int foreignOrient_;
  double foreignX_;
  double foreignY_;
  int foreignSize_;
  char* foreign_;

  int LEQSize_;
  char* LEQ_;
  int mustjoinSize_;
  char* mustjoin_;
  double outMarginH_;
  double outMarginL_;
  double outResistanceH_;
  double outResistanceL_;
  double inMarginH_;
  double inMarginL_;
  double power_;
  double leakage_;
  double maxload_;
  double maxdelay_;
  double capacitance_;
  double resistance_;
  double pulldownres_;
  double tieoffr_;
  double VHI_;
  double VLO_;
  double riseVoltage_;
  double fallVoltage_;
  double riseThresh_;
  double fallThresh_;
  double riseSatcur_;
  double fallSatcur_;
  int lowTableSize_;
  char* lowTable_;
  int highTableSize_;
  char* highTable_;
  double riseSlewLimit_;
  double fallSlewLimit_;

  // 5.5 AntennaModel
  int numAntennaModel_;
  int antennaModelAllocated_;
  int curAntennaModelIndex_;     // save the current index of the antenna
  lefiPinAntennaModel** antennaModel_;

  int numAntennaSize_;
  int antennaSizeAllocated_;
  double* antennaSize_;
  char** antennaSizeLayer_;

  int numAntennaMetalArea_;
  int antennaMetalAreaAllocated_;
  double* antennaMetalArea_;
  char** antennaMetalAreaLayer_;

  int numAntennaMetalLength_;
  int antennaMetalLengthAllocated_;
  double* antennaMetalLength_;
  char** antennaMetalLengthLayer_;

  int numAntennaPartialMetalArea_;
  int antennaPartialMetalAreaAllocated_;
  double* antennaPartialMetalArea_;
  char** antennaPartialMetalAreaLayer_;

  int numAntennaPartialMetalSideArea_;
  int antennaPartialMetalSideAreaAllocated_;
  double* antennaPartialMetalSideArea_;
  char** antennaPartialMetalSideAreaLayer_;

  int numAntennaPartialCutArea_;
  int antennaPartialCutAreaAllocated_;
  double* antennaPartialCutArea_;
  char** antennaPartialCutAreaLayer_;

  int numAntennaDiffArea_;
  int antennaDiffAreaAllocated_;
  double* antennaDiffArea_;
  char** antennaDiffAreaLayer_;

  char* taperRule_;

  char direction_[32];
  char use_[12];
  char shape_[12];
  char currentSource_[12];

  int numProperties_;
  int propertiesAllocated_;
  char** propNames_;
  char** propValues_;
  double* propNums_;
  char*  propTypes_;

  int numPorts_;
  int portsAllocated_;
  lefiGeometries** ports_;
};


class lefiMacro {
public:
  lefiMacro();
  void Init();

  void Destroy();
  ~lefiMacro();

  void clear();
  void bump(char** array, int len, int* size);
  void setName(const char* name);
  void setGenerator(const char* name);
  void setGenerate(const char* name1, const char* name2);
  void setPower(double d);
  void setOrigin(double x, double y);
  void setClass(const char* name);
  void setSource(const char* name);
  void setEEQ(const char* name);
  void setLEQ(const char* name);
  void setClockType(const char* name);
  void setProperty(const char* name, const char* value, const char type);
  void setNumProperty(const char* name, double d, const char* value,
                      const char type);
  void bumpProps();

  // orient=-1 means no orient was specified.
  void setForeign(const char* name, int hasPnt,
	       double x, double y, int orient);

  void setXSymmetry();
  void setYSymmetry();
  void set90Symmetry();
  void setSiteName(const char* name);
  void setSitePattern(lefiSitePattern* p);
  void setSize(double x, double y);
  void setBuffer();
  void setInverter();

  int hasClass() const;
  int hasGenerator() const;
  int hasGenerate() const;
  int hasPower() const;
  int hasOrigin() const;
  int hasEEQ() const;
  int hasLEQ() const;
  int hasSource() const;
  int hasXSymmetry() const;
  int hasYSymmetry() const;
  int has90Symmetry() const;
  int hasSiteName() const;
  int hasSitePattern() const;
  int hasSize() const;
  int hasForeign() const;
  int hasForeignOrigin() const;
  int hasForeignOrient() const;
  int hasForeignPoint() const;
  int hasClockType() const;
  int isBuffer() const;
  int isInverter() const;

  int numSitePattern() const;
  int numProperties() const;
  const char* propName(int index) const;
  const char* propValue(int index) const;
  double propNum(int index) const;
  const char propType(int index) const;
  int  propIsNumber(int index) const;
  int  propIsString(int index) const;

  const char* name() const;
  const char* macroClass() const;
  const char* generator() const;
  const char* EEQ() const;
  const char* LEQ() const;
  const char* source() const;
  const char* clockType() const;
  double originX() const;
  double originY() const;
  double power() const;
  void generate(char** name1, char** name2) const;
  lefiSitePattern* sitePattern(int index) const;
  const char* siteName() const;
  double sizeX() const;
  double sizeY() const;
  int foreignOrient() const;
  const char*  foreignOrientStr() const;
  double foreignX() const;
  double foreignY() const;
  const char* foreignName() const;

  // Debug print
  void print(FILE* f) const;

protected:
  int nameSize_;
  char* name_;
  char macroClass_[32];
  char source_[12];

  int generatorSize_;
  char* generator_;

  char hasClass_;
  char hasGenerator_;
  char hasGenerate_;
  char hasPower_;
  char hasOrigin_;
  char hasSource_;
  char hasEEQ_;
  char hasLEQ_;
  char hasSymmetry_;  // X=1  Y=2  R90=4  (can be combined)
  char hasSiteName_;
  char hasSize_;
  char hasForeign_;
  char hasForeignOrigin_;
  char hasForeignPoint_;
  char hasClockType_;
  char isBuffer_;
  char isInverter_;

  char* EEQ_;
  int EEQSize_;
  char* LEQ_;
  int LEQSize_;
  char* gen1_;
  int gen1Size_;
  char* gen2_;
  int gen2Size_;
  double power_;
  double originX_;
  double originY_;
  double sizeX_;
  double sizeY_;

  int numSites_;
  int sitesAllocated_;
  lefiSitePattern** pattern_;

  int foreignOrient_;
  double foreignX_;
  double foreignY_;
  int foreignSize_;
  char* foreign_;

  int siteNameSize_;
  char* siteName_;

  char* clockType_;
  int clockTypeSize_;

  int numProperties_;
  int propertiesAllocated_;
  char** propNames_;
  char** propValues_;
  double* propNums_;
  char*  propTypes_;

};


class lefiTiming {
public:
  lefiTiming();
  void Init();

  void Destroy();
  ~lefiTiming();

  void addRiseFall(const char* risefall, double one, double two);
  void addRiseFallVariable(double one, double two);
  void addRiseFallSlew(double one, double two, double three, double four);
  void addRiseFallSlew2(double one, double two, double three);
  void setRiseRS(double one, double two);
  void setFallRS(double one, double two);
  void setRiseCS(double one, double two);
  void setFallCS(double one, double two);
  void setRiseAtt1(double one, double two);
  void setFallAtt1(double one, double two);
  void setRiseTo(double one, double two);
  void setFallTo(double one, double two);
  void addUnateness(const char* typ);
  void setStable(double one, double two, const char* typ);
  void addTableEntry(double one, double two, double three);
  void addTableAxisNumber(double one);
  void addFromPin(const char* name);
  void addToPin(const char* name);
  void addDelay(const char* risefall, const char* unateness, double one,
         double two, double three);
  void addTransition(const char* risefall, const char* unateness, double one,
         double two, double three);
  // addSDF2Pins & addSDF1Pin are for 5.1
  void addSDF2Pins(const char* trigType, const char* fromTrig,
         const char* toTrig, double one, double two, double three);
  void addSDF1Pin(const char* trigType, double one, double two, double three);
  void setSDFcondStart(const char* condStart);
  void setSDFcondEnd(const char* condEnd);
  void setSDFcond(const char* cond);
  int hasData();
  void clear();

  int numFromPins();
  const char* fromPin(int index);
  int numToPins();
  const char* toPin(int index);
  int hasTransition();
  int hasDelay();
  int hasRiseSlew();
  int hasRiseSlew2();
  int hasFallSlew();
  int hasFallSlew2();
  int hasRiseIntrinsic();
  int hasFallIntrinsic();
  int numOfAxisNumbers();
  double* axisNumbers();
  int numOfTableEntries();
  void tableEntry(int num, double* one, double* two, double* three);
  const char* delayRiseOrFall();
  const char* delayUnateness();
  double delayTableOne();
  double delayTableTwo();
  double delayTableThree();
  const char* transitionRiseOrFall();
  const char* transitionUnateness();
  double transitionTableOne();
  double transitionTableTwo();
  double transitionTableThree();
  double fallIntrinsicOne();
  double fallIntrinsicTwo();
  double fallIntrinsicThree();
  double fallIntrinsicFour();
  double riseIntrinsicOne();
  double riseIntrinsicTwo();
  double riseIntrinsicThree();
  double riseIntrinsicFour();
  double fallSlewOne();
  double fallSlewTwo();
  double fallSlewThree();
  double fallSlewFour();
  double fallSlewFive();
  double fallSlewSix();
  double fallSlewSeven();
  double riseSlewOne();
  double riseSlewTwo();
  double riseSlewThree();
  double riseSlewFour();
  double riseSlewFive();
  double riseSlewSix();
  double riseSlewSeven();
  int hasRiseRS();
  double riseRSOne();
  double riseRSTwo();
  int hasRiseCS();
  double riseCSOne();
  double riseCSTwo();
  int hasFallRS();
  double fallRSOne();
  double fallRSTwo();
  int hasFallCS();
  double fallCSOne();
  double fallCSTwo();
  int hasUnateness();
  const char* unateness();
  int hasRiseAtt1();
  double riseAtt1One();
  double riseAtt1Two();
  int hasFallAtt1();
  double fallAtt1One();
  double fallAtt1Two();
  int hasFallTo();
  double fallToOne();
  double fallToTwo();
  int hasRiseTo();
  double riseToOne();
  double riseToTwo();
  int hasStableTiming();
  double stableSetup();
  double stableHold();
  const char* stableRiseFall();
  // The following are for 5.1
  int hasSDFonePinTrigger();
  int hasSDFtwoPinTrigger();
  int hasSDFcondStart();
  int hasSDFcondEnd();
  int hasSDFcond();
  const char* SDFonePinTriggerType();
  const char* SDFtwoPinTriggerType();
  const char* SDFfromTrigger();
  const char* SDFtoTrigger();
  double SDFtriggerOne();
  double SDFtriggerTwo();
  double SDFtriggerThree();
  const char* SDFcondStart();
  const char* SDFcondEnd();
  const char* SDFcond();

protected:
  int numFrom_;
  char** from_;
  int fromAllocated_;
  int numTo_;
  char** to_;
  int toAllocated_;

  int hasTransition_;
  int hasDelay_;
  int hasRiseSlew_;
  int hasRiseSlew2_;
  int hasFallSlew_;
  int hasFallSlew2_;
  int hasRiseIntrinsic_;
  int hasFallIntrinsic_;
  int hasRiseRS_;
  int hasRiseCS_;
  int hasFallRS_;
  int hasFallCS_;
  int hasUnateness_;
  int hasFallAtt1_;
  int hasRiseAtt1_;
  int hasFallTo_;
  int hasRiseTo_;
  int hasStableTiming_;
  int hasSDFonePinTrigger_;
  int hasSDFtwoPinTrigger_;
  int hasSDFcondStart_;
  int hasSDFcondEnd_;
  int hasSDFcond_;
  int nowRise_;

  int numOfAxisNumbers_;
  double* axisNumbers_;
  int axisNumbersAllocated_;

  int numOfTableEntries_;
  int tableEntriesAllocated_;
  double* table_;  /* three numbers per entry */

  char* delayRiseOrFall_;
  char* delayUnateness_;
  double delayTableOne_;
  double delayTableTwo_;
  double delayTableThree_;
  char* transitionRiseOrFall_;
  char* transitionUnateness_;
  double transitionTableOne_;
  double transitionTableTwo_;
  double transitionTableThree_;
  double riseIntrinsicOne_;
  double riseIntrinsicTwo_;
  double riseIntrinsicThree_;
  double riseIntrinsicFour_;
  double fallIntrinsicOne_;
  double fallIntrinsicTwo_;
  double fallIntrinsicThree_;
  double fallIntrinsicFour_;
  double riseSlewOne_;
  double riseSlewTwo_;
  double riseSlewThree_;
  double riseSlewFour_;
  double riseSlewFive_;
  double riseSlewSix_;
  double riseSlewSeven_;
  double fallSlewOne_;
  double fallSlewTwo_;
  double fallSlewThree_;
  double fallSlewFour_;
  double fallSlewFive_;
  double fallSlewSix_;
  double fallSlewSeven_;
  double riseRSOne_;
  double riseRSTwo_;
  double riseCSOne_;
  double riseCSTwo_;
  double fallRSOne_;
  double fallRSTwo_;
  double fallCSOne_;
  double fallCSTwo_;
  char* unateness_;
  double riseAtt1One_;
  double riseAtt1Two_;
  double fallAtt1One_;
  double fallAtt1Two_;
  double fallToOne_;
  double fallToTwo_;
  double riseToOne_;
  double riseToTwo_;
  double stableSetup_;
  double stableHold_;
  char* stableRiseFall_;
  char* SDFtriggerType_;
  char* SDFfromTrigger_;
  char* SDFtoTrigger_;
  double SDFtriggerTableOne_;
  double SDFtriggerTableTwo_;
  double SDFtriggerTableThree_;
  char* SDFcondStart_;
  char* SDFcondEnd_;
  char* SDFcond_;
};


#endif

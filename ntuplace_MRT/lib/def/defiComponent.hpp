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

#ifndef defiComponent_h
#define defiComponent_h

#include <stdio.h>
#include "defiKRDefs.hpp"


// Placement status for the component.
// Default is 0
#define DEFI_COMPONENT_UNPLACED 1
#define DEFI_COMPONENT_PLACED 2
#define DEFI_COMPONENT_FIXED 3
#define DEFI_COMPONENT_COVER 4


// Struct holds the data for one component.
class defiComponent {
public:
  defiComponent();
  void Init();

  void Destroy();
  ~defiComponent();

  void IdAndName(const char* id, const char* name);
  void setGenerate(const char* genName, const char* macroName);
  void setPlacementStatus(int n);
  void setPlacementLocation(int x, int y, int orient);
  void setRegionName(const char* name);
  void setRegionBounds(int xl, int yl, int xh, int yh);
  void setEEQ(const char* name);
  void addNet(const char* netName);
  void addProperty(const char* name, const char* value, const char type);
  void addNumProperty(const char* name, const double d,
                      const char* value, const char type);
  void reverseNetOrder();
  void setWeight(int w);
  void setSource(const char* name);
  void setForeignName(const char* name);
  void setFori(const char* name);
  void setForeignLocation(int x, int y, int orient);
  void clear();

  const char* id() const;
  const char* name() const;
  int placementStatus() const;
  int isUnplaced() const;
  int isPlaced() const;
  int isFixed() const;
  int isCover() const;
  int placementX() const;
  int placementY() const;
  int placementOrient() const;
  const char* placementOrientStr() const;
  int hasRegionName() const;
  int hasRegionBounds() const;
  int hasEEQ() const;
  int hasGenerate() const;
  int hasSource() const;
  int hasWeight() const;
  int weight() const;
  int hasNets() const;
  int numNets() const;
  const char* net(int index) const;
  const char* regionName() const;
  const char* source() const;
  const char* EEQ() const;
  const char* generateName() const;
  const char* macroName() const;

  // Returns arrays for the ll and ur of the rectangles in the region.
  // The number of items in the arrays is given in size.
  void regionBounds(int*size, int** xl, int** yl, int** xh, int** yh);

  int hasForeignName() const;
  const char* foreignName() const;
  int foreignX() const;
  int foreignY() const;
  const char* foreignOri() const;
  int hasFori() const;

  int numProps() const;
  char*  propName(int index) const;
  char*  propValue(int index) const;
  double propNumber(int index) const;
  char   propType(int index) const;
  int    propIsNumber(int index) const;
  int    propIsString(int index) const;

  // Debug printing
  void print(FILE* fout);

  void bumpId(int size);
  void bumpName(int size);
  void bumpRegionName(int size);
  void bumpEEQ(int size);
  void bumpNets(int size);
  void bumpForeignName(int size);
  void bumpFori(int size);

protected:
  char* id_;            // instance id
  char* name_;          // name.
  int nameSize_;        // allocated size of name.
  int idSize_;          // allocated size of id.
  int ForiSize_;        // allocate size of foreign ori
  char status_;         // placement status
  char hasRegionName_;  // the file supplied a region name for this comp
  char hasEEQ_;         // the file supplied an eeq
  char hasGenerate_;    // the file supplied an generate name and macro name
  char hasWeight_;      // the file supplied a weight
  char hasFori_;        // the file supplied a foreign orig name
  int orient_;          // orientation
  int x_, y_;           // placement loc

  int numRects_;
  int rectsAllocated_;
  int* rectXl_;       // region points
  int* rectYl_;
  int* rectXh_;
  int* rectYh_;

  char* regionName_;    // name.
  int regionNameSize_;  // allocated size of region name

  char* EEQ_;
  int EEQSize_;         // allocated size of eeq

  int numNets_;         // number of net connections
  int netsAllocated_;   // allocated size of nets array
  char** nets_;         // net connections

  int weight_;
  char* source_;
  char hasForeignName_; // the file supplied a foreign name
  char* foreignName_;   // name
  int foreignNameSize_; // allocate size of foreign name
  int Fx_, Fy_;         // foreign loc
  int Fori_;            // foreign ori
  int generateNameSize_;
  char* generateName_;
  int macroNameSize_;
  char* macroName_;

  int numProps_;
  int propsAllocated_;
  char**  names_;
  char**  values_;
  double* dvalues_;
  char*   types_;
};


#endif

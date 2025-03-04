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

#include <stdlib.h>
#include <string.h>
#include "lex.h"
#include "lefiVia.hpp"
#include "lefiDebug.hpp"
#include "lefiUtil.hpp"


/////////////////////////////////////////////////////
/////////////////////////////////////////////////////
//
//   lefiViaLayer
//
/////////////////////////////////////////////////////
/////////////////////////////////////////////////////


lefiViaLayer::lefiViaLayer() {
  this->lefiViaLayer::Init();
}



void lefiViaLayer::Init() {
  this->name_ = 0;
  this->rectsAllocated_ = 2;
  this->numRects_ = 0;
  this->xl_ = (double*)lefMalloc(sizeof(double)*2);
  this->yl_ = (double*)lefMalloc(sizeof(double)*2);
  this->xh_ = (double*)lefMalloc(sizeof(double)*2);
  this->yh_ = (double*)lefMalloc(sizeof(double)*2);
}



void lefiViaLayer::Destroy() {
  if (this->xl_) {
    lefFree((char*)(this->xl_));
    lefFree((char*)(this->yl_));
    lefFree((char*)(this->xh_));
    lefFree((char*)(this->yh_));
    }
  lefFree(this->name_);
}



lefiViaLayer::~lefiViaLayer() {
  this->lefiViaLayer::Destroy();
}



void lefiViaLayer::setName(const char* name) {
  int len = strlen(name) + 1;
  this->name_ = (char*)lefMalloc(len);
  strcpy(this->name_, CASE(name));
}



void lefiViaLayer::addRect(double xl, double yl, double xh, double yh) {
  if (this->numRects_ == this->rectsAllocated_) {
    int i;
    double* newxl;
    double* newyl;
    double* newxh;
    double* newyh;
    this->rectsAllocated_ = (this->rectsAllocated_ == 0) ?
	   2 : this->rectsAllocated_ * 2;
    newxl = (double*)lefMalloc(sizeof(double)*this->rectsAllocated_);
    newyl = (double*)lefMalloc(sizeof(double)*this->rectsAllocated_);
    newxh = (double*)lefMalloc(sizeof(double)*this->rectsAllocated_);
    newyh = (double*)lefMalloc(sizeof(double)*this->rectsAllocated_);
    for (i = 0; i < this->numRects_; i++) {
      newxl[i] = this->xl_[i];
      newyl[i] = this->yl_[i];
      newxh[i] = this->xh_[i];
      newyh[i] = this->yh_[i];
    }
    if (this->xl_) {
      lefFree((char*)(this->xl_));
      lefFree((char*)(this->yl_));
      lefFree((char*)(this->xh_));
      lefFree((char*)(this->yh_));
    }
    this->xl_ = newxl;
    this->yl_ = newyl;
    this->xh_ = newxh;
    this->yh_ = newyh;
  }
  this->xl_[this->numRects_] = xl;
  this->yl_[this->numRects_] = yl;
  this->xh_[this->numRects_] = xh;
  this->yh_[this->numRects_] = yh;
  this->numRects_ += 1;
}



int lefiViaLayer::numRects() {
  return this->numRects_;
}



lefiViaLayer* lefiViaLayer::clone() {
  lefiViaLayer* vl = (lefiViaLayer*)lefMalloc(sizeof(lefiViaLayer));
  int i;
  vl->xl_ = (double*)lefMalloc(sizeof(double)*this->numRects_);
  vl->yl_ = (double*)lefMalloc(sizeof(double)*this->numRects_);
  vl->xh_ = (double*)lefMalloc(sizeof(double)*this->numRects_);
  vl->yh_ = (double*)lefMalloc(sizeof(double)*this->numRects_);
  vl->numRects_ = this->numRects_;
  vl->rectsAllocated_ = this->numRects_;
  for (i = 0; i < this->numRects_; i++) {
    vl->xl_[i] = this->xl_[i];
    vl->yl_[i] = this->yl_[i];
    vl->xh_[i] = this->xh_[i];
    vl->yh_[i] = this->yh_[i];
  }
  vl->name_ = (char*)lefMalloc(strlen(this->name_)+1);
  strcpy(vl->name_, this->name_);
  return vl;
}


char* lefiViaLayer::name() {
  return this->name_;
}



double lefiViaLayer::xl(int index) {
  if (index < 0 || index >= this->numRects_)
    { lefiError("index out of bounds"); return 0; }
  return this->xl_[index];
}



double lefiViaLayer::yl(int index) {
  if (index < 0 || index >= this->numRects_)
    { lefiError("index out of bounds"); return 0; }
  return this->yl_[index];
}



double lefiViaLayer::xh(int index) {
  if (index < 0 || index >= this->numRects_)
    { lefiError("index out of bounds"); return 0; }
  return this->xh_[index];
}



double lefiViaLayer::yh(int index) {
  if (index < 0 || index >= this->numRects_)
    { lefiError("index out of bounds"); return 0; }
  return this->yh_[index];
}



/////////////////////////////////////////////////////
/////////////////////////////////////////////////////
//
//   lefiVia
//
/////////////////////////////////////////////////////
/////////////////////////////////////////////////////




lefiVia::lefiVia() {
  this->lefiVia::Init();
}



void lefiVia::Init() {
  this->nameSize_ = 16;
  this->name_ = (char*)lefMalloc(16);
  this->foreign_ = 0;
  this->numProps_ = 0;
  this->propsAllocated_ = 0;
  this->layersAllocated_ = 3;
  this->layers_ = (lefiViaLayer**)lefMalloc(sizeof(lefiViaLayer*)*3);
  this->numLayers_ = 0;
  this->lefiVia::clear();

}



void lefiVia::Destroy() {
  this->lefiVia::clear();
  lefFree(this->name_);
  lefFree((char*)(this->layers_));
  if (this->propName_)
     lefFree((char*)(this->propName_));
  if (this->propValue_)
     lefFree((char*)(this->propValue_));
  if (this->propDValue_)
     lefFree((char*)(this->propDValue_));
  if (this->propType_)
     lefFree((char*)(this->propType_));
}



lefiVia::~lefiVia() {
  this->lefiVia::Destroy();
}



lefiVia* lefiVia::clone() {
  int i;
  lefiViaLayer* l;
  lefiVia* v = (lefiVia*)lefMalloc(sizeof(lefiVia));
  v->nameSize_ = strlen(this->name_) + 1;
  v->name_ = (char*)lefMalloc(v->nameSize_);
  strcpy(v->name_, this->name_);
  v->foreign_ = 0;
  if (this->lefiVia::hasForeign()) {
    v->lefiVia::setForeign(this->foreign_, this->lefiVia::hasForeignPnt(),
	  this->foreignX_, this->foreignY_, this->foreignOrient_);
  }
  v->hasDefault_ = this->hasDefault_;
  v->hasResistance_ = this->hasResistance_;
  v->hasForeignPnt_ = this->hasForeignPnt_;
  v->hasTopOfStack_ = this->hasTopOfStack_;
  v->numProps_ = this->numProps_;
  v->propsAllocated_ = this->numProps_;
  if (this->numProps_ > 0) {
     v->propName_ = (char**)lefMalloc(sizeof(char*)*this->numProps_);
     v->propValue_ = (char**)lefMalloc(sizeof(char*)*this->numProps_);
     v->propDValue_ = (double*)lefMalloc(sizeof(double)*this->numProps_);
     v->propType_ = (char*)lefMalloc(sizeof(char)*this->numProps_);
     for (i = 0; i < this->numProps_; i++) {
       v->propName_[i] = (char*)lefMalloc(strlen(this->propName_[i])+1);
       strcpy(v->propName_[i],this->propName_[i]);
       // Modified 8/27/99 - Wanda da Rosa for pcr 274891
       // this->propValue_[i] can be null, if propValue was a number
       if (this->propValue_[i]) {
           v->propValue_[i] = (char*)lefMalloc(strlen(this->propValue_[i])+1);
           strcpy(v->propValue_[i], this->propValue_[i]);
       } else
           v->propValue_[i] = 0;
       v->propDValue_[i] = this->propDValue_[i];
       v->propType_[i] = this->propType_[i];
     }
  } else {
     v->propName_ = 0;
     v->propValue_ = 0;
     v->propDValue_ = 0;
     v->propType_ = 0;
  }
  v->layersAllocated_ = this->numLayers_;
  v->numLayers_ = this->numLayers_;
  v->layers_ = (lefiViaLayer**)lefMalloc(sizeof(lefiViaLayer*)*this->numLayers_);
  for (i = 0; i < this->numLayers_; i++) {
    l = this->layers_[i];
    v->layers_[i] = l->lefiViaLayer::clone();
  }
  v->resistance_ = this->resistance_;
  if (this->foreignOrient_ == 0)
     v->foreignOrient_ = -1;
  else v->foreignOrient_ = this->foreignOrient_;
  return v;
}


void lefiVia::clear() {
  int i;

  if (this->name_)
     *(this->name_) = '\0';
  if (this->foreign_) lefFree(this->foreign_); 
  this->foreign_ = 0;
  this->hasDefault_ = 0;
  this->hasResistance_ = 0;
  this->hasForeignPnt_ = 0;
  this->hasTopOfStack_ = 0;
  this->foreignOrient_ = -1;

  for (i = 0; i < this->numProps_; i++) {
    lefFree(this->propName_[i]);
    this->propName_[i] = 0;
    if (this->propValue_[i]) lefFree(this->propValue_[i]);
    this->propValue_[i] = 0;
    this->propType_[i] = ' ';
  }
  this->numProps_ = 0;

  for (i = 0; i < this->numLayers_; i++) {
    this->layers_[i]->lefiViaLayer::Destroy();
    lefFree((char*)(this->layers_[i]));
    this->layers_[i] = 0;
  }
  this->numLayers_ = 0;
}



void lefiVia::setName(const char* name, int deflt) {
  int len;
  // setName calls clear to init
  // default=0 no default specified
  // default=1 default specified in lef file
  this->lefiVia::clear();
  this->hasDefault_ = deflt ? 1 : 0 ;

  len = strlen(name) + 1;
  if (len > this->nameSize_) {
    lefFree(this->name_);
    this->name_ = (char*)lefMalloc(len);
    this->nameSize_ = len;
  }
  strcpy(this->name_, CASE(name));

}



void lefiVia::setResistance(double num) {
  this->hasResistance_ = 1;
  this->resistance_ = num;
}


void lefiVia::bumpProps() {
  int i;
  double* d;
  char** n;
  char** v;
  char*  t;

  if (this->propsAllocated_ == 0)
    this->propsAllocated_ = 2;
  else
    this->propsAllocated_ *= 2;

  d = (double*)lefMalloc(sizeof(double) * this->propsAllocated_);
  n = (char**)lefMalloc(sizeof(char*) * this->propsAllocated_);
  v = (char**)lefMalloc(sizeof(char*) * this->propsAllocated_);
  t = (char*)lefMalloc(sizeof(char) * this->propsAllocated_);

  for (i = 0; i < this->numProps_; i++) {
    d[i] = this->propDValue_[i];
    n[i] = this->propName_[i];
    v[i] = this->propValue_[i];
    t[i] = this->propType_[i];
  }

  if (this->numProps_ > 0) {
     lefFree((char*)(this->propDValue_));
     lefFree((char*)(this->propName_));
     lefFree((char*)(this->propValue_));
     lefFree((char*)(this->propType_));
  }

  this->propDValue_ = d;
  this->propName_ = n;
  this->propValue_ = v;
  this->propType_ = t;
}


void lefiVia::addProp(const char* name, const char* value, const char type) {
  int len = strlen(name) + 1;

  if (this->numProps_ == this->propsAllocated_) this->lefiVia::bumpProps();

  this->propName_[this->numProps_] = (char*)lefMalloc(len);
  strcpy(this->propName_[this->numProps_], CASE(name));

  len = strlen(value) + 1;
  this->propValue_[this->numProps_] = (char*)lefMalloc(len);
  strcpy(this->propValue_[this->numProps_], CASE(value));

  this->propDValue_[this->numProps_] = 0.0;

  this->propType_[this->numProps_] = type;

  this->numProps_ += 1;
}



void lefiVia::addNumProp(const char* name, double d, const char* value,
                         const char type) {
  int len = strlen(name) + 1;

  if (this->numProps_ == this->propsAllocated_) this->lefiVia::bumpProps();

  this->propName_[this->numProps_] = (char*)lefMalloc(len);
  strcpy(this->propName_[this->numProps_], CASE(name));

  len = strlen(value) + 1;
  this->propValue_[this->numProps_] = (char*)lefMalloc(len);
  strcpy(this->propValue_[this->numProps_], CASE(value));

  this->propDValue_[this->numProps_] = d;

  this->propType_[this->numProps_] = type;

  this->numProps_ += 1;
}



void lefiVia::setForeign(const char* name, int hasPnt,
			 double x, double y, int orient) {
  // orient=-1 means no orient was specified.
  int len = strlen(name) + 1;

  this->hasForeignPnt_ = hasPnt;
  this->foreignOrient_ = orient;
  this->foreignX_ = x;
  this->foreignY_ = y;

  this->foreign_ = (char*)lefMalloc(len);
  strcpy(this->foreign_, CASE(name));
}



void lefiVia::setTopOfStack() {
  this->hasTopOfStack_ = 1;
}


void lefiVia::addLayer(const char* name) {
  lefiViaLayer* newl;
  if (this->numLayers_ == this->layersAllocated_) {
    int i;
    lefiViaLayer** l;
    if (this->layersAllocated_ == 0)
      this->layersAllocated_ = 2;
    else
      this->layersAllocated_ *= 2;
    l = (lefiViaLayer**)lefMalloc(sizeof(lefiViaLayer*) * this->layersAllocated_);
    for (i = 0; i < this->numLayers_; i++) {
      l[i] = this->layers_[i];
    }
    lefFree((char*)(this->layers_));
    this->layers_ = l;
  }
  newl = (lefiViaLayer*)lefMalloc(sizeof(lefiViaLayer));
  newl->lefiViaLayer::Init();
  this->layers_[this->numLayers_] = newl;
  newl->lefiViaLayer::setName(name);
  this->numLayers_ += 1;
}



void lefiVia::addRectToLayer(double xl, double yl, double xh, double yh) {
  this->layers_[this->numLayers_-1]->lefiViaLayer::addRect(xl, yl, xh, yh);
}



int lefiVia::hasDefault() const  {
  return this->hasDefault_;
}



int lefiVia::hasForeign() const  {
  return this->foreign_ ? 1 : 0;
}



int lefiVia::hasForeignPnt() const  {
  return this->hasForeignPnt_;
}



int lefiVia::hasForeignOrient() const  {
  return this->foreignOrient_ == -1 ? 0 : 1 ;
}



int lefiVia::hasProperties() const  {
  return this->numProps_ ? 1 : 0 ;
}



int lefiVia::hasResistance() const  {
  return this->hasResistance_;
}


int lefiVia::hasTopOfStack() const {
  return this->hasTopOfStack_;
}


int lefiVia::numLayers() const {
  return this->numLayers_;
}



char* lefiVia::layerName(int layerNum) const {
  lefiViaLayer* vl;
  if (layerNum < 0 || layerNum >= this->numLayers_)
    { lefiError("ERROR: bad layer number index"); return 0; }
  vl = this->layers_[layerNum];
  return vl->lefiViaLayer::name();
}



int lefiVia::numRects(int layerNum) const {
  lefiViaLayer* vl;
  if (layerNum < 0 || layerNum >= this->numLayers_)
    { lefiError("ERROR: bad layer number index"); return 0; }
  vl = this->layers_[layerNum];
  return vl->lefiViaLayer::numRects();
}



double lefiVia::xl(int layerNum, int rectNum) const {
  lefiViaLayer* vl;
  if (layerNum < 0 || layerNum >= this->numLayers_)
    { lefiError("ERROR: bad layer number index"); return 0; }
  vl = this->layers_[layerNum];
  return vl->lefiViaLayer::xl(rectNum);
}



double lefiVia::yl(int layerNum, int rectNum) const {
  lefiViaLayer* vl;
  if (layerNum < 0 || layerNum >= this->numLayers_)
    { lefiError("ERROR: bad layer number index"); return 0; }
  vl = this->layers_[layerNum];
  return vl->lefiViaLayer::yl(rectNum);
}



double lefiVia::xh(int layerNum, int rectNum) const {
  lefiViaLayer* vl;
  if (layerNum < 0 || layerNum >= this->numLayers_)
    { lefiError("ERROR: bad layer number index"); return 0; }
  vl = this->layers_[layerNum];
  return vl->lefiViaLayer::xh(rectNum);
}



double lefiVia::yh(int layerNum, int rectNum) const {
  lefiViaLayer* vl;
  if (layerNum < 0 || layerNum >= this->numLayers_)
    { lefiError("ERROR: bad layer number index"); return 0; }
  vl = this->layers_[layerNum];
  return vl->lefiViaLayer::yh(rectNum);
}



char* lefiVia::name() const  {
  return this->name_;
}



double lefiVia::resistance() const  {
  return this->resistance_;
}



// Given an index from 0 to numProperties()-1 return
// information about that property.
int lefiVia::numProperties() const  {
  return this->numProps_;
}



char* lefiVia::propName(int index) const {
  if (index < 0 || index >= this->numProps_)
    { lefiError("bad index for via property"); return 0; }
  return this->propName_[index];
}



char* lefiVia::propValue(int index) const {
  if (index < 0 || index >= this->numProps_)
    { lefiError("bad index for via property"); return 0; }
  return this->propValue_[index];
}



double lefiVia::propNumber(int index) const {
  if (index < 0 || index >= this->numProps_)
    { lefiError("bad index for via property"); return 0; }
  return this->propDValue_[index];
}



char lefiVia::propType(int index) const {
  if (index < 0 || index >= this->numProps_)
    { lefiError("bad index for via property"); return 0; }
  return this->propType_[index];
}


int lefiVia::propIsNumber(int index) const {
  if (index < 0 || index >= this->numProps_)
    { lefiError("bad index for via property"); return 0; }
  return this->propDValue_[index] ? 1 : 0;
}



int lefiVia::propIsString(int index) const {
  if (index < 0 || index >= this->numProps_)
    { lefiError("bad index for via property"); return 0; }
  return this->propDValue_[index] ? 0 : 1;
}



char* lefiVia::foreign() const {
  return this->foreign_;
}


double lefiVia::foreignX() const {
  return this->foreignX_;
}



double lefiVia::foreignY() const {
  return this->foreignY_;
}



int lefiVia::foreignOrient() const {
  return this->foreignOrient_;
}


char* lefiVia::foreignOrientStr() const {
  return (lefiOrientStr(this->foreignOrient_));
}


// Debug print
void lefiVia::print(FILE* f) const {
  int i;
  int h;

  fprintf(f, "Via %s:\n", this->lefiVia::name());

  if (this->lefiVia::hasDefault())
    fprintf(f, "  DEFAULT\n");

  if (this->lefiVia::hasForeign()) {
    fprintf(f, "  foreign %s", this->lefiVia::foreign());
    if (this->lefiVia::hasForeignPnt()) {
      fprintf(f, " %g,%g", this->lefiVia::foreignX(),
      this->lefiVia::foreignY());
    }
    if (this->lefiVia::hasForeignOrient()) {
      fprintf(f, " orient %s", this->lefiVia::foreignOrientStr());
    }
    fprintf(f, "\n");
  }

  if (this->lefiVia::hasResistance())
    fprintf(f, "  RESISTANCE %g\n", this->lefiVia::resistance());

  if (this->lefiVia::hasProperties()) {
    for (i = 0; i < this->lefiVia::numProperties(); i++)
      if (this->lefiVia::propIsString(i))
	fprintf(f, "  PROP %s %s\n", this->lefiVia::propName(i),
	                             this->lefiVia::propValue(i));
      else
	fprintf(f, "  PROP %s %g\n", this->lefiVia::propName(i),
	                             this->lefiVia::propNumber(i));
  }

    for (i = 0; i < this->lefiVia::numLayers(); i++) {
      fprintf(f, "  LAYER %s\n", this->lefiVia::layerName(i));
      for (h = 0; h < this->lefiVia::numRects(i); h++) {
	fprintf(f, "    RECT %g,%g %g,%g\n",
	   this->lefiVia::xl(i,h), this->lefiVia::yl(i,h),
	   this->lefiVia::xh(i,h), this->lefiVia::yh(i,h));
      }
    }

}




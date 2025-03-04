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

/* Definitions header file for the DEF Interface */

#ifndef DEFI_DEFS_H
#define DEFI_DEFS_H

#include <stdio.h>
#include <limits.h>

#include "defiKRDefs.hpp"

/*=================== General Types and Definitions =================*/

#ifndef TRUE
#define TRUE (1)
#endif

#ifndef FALSE
#define FALSE (0)
#endif

#ifndef NULL
#define NULL (0)
#endif

typedef struct defpoint defPOINT;

struct defpoint {
    int x;
    int y;
    };

typedef struct defrect defRECT;

struct defrect {
    defPOINT ll,ur;
    };

typedef struct deftoken defTOKEN;

struct deftoken {
    defTOKEN *next;
    int what;
    int data;
    defPOINT pt;
    };

#define START_LIST 10001
#define POINT_SPEC 10002
#define VIA_SPEC   10003
#define WIDTH_SPEC 10004
#define LAYER_SPEC 10005
#define SHAPE_SPEC 10006

#ifndef	MIN
#define MIN(x,y) ((x) < (y)? (x) : (y))
#endif

#ifndef	MIN
#define MAX(x,y) ((x) > (y)? (x) : (y))
#endif

#define ROUND(x) ((x) >= 0 ? (int)((x)+0.5) : (int)((x)-0.5))

//defTOKEN *TokenFromRect();

/*=================== Enumerated Types ============================*/
typedef int defiBoolean;

/* Every type of object has a unique identifier, and each object
 * which is created knows its type, by storing the defiObjectType_e
 * as the first member in the structure.
 *
 */
typedef enum
{
  /* decrease likelihood of accidentally correct values by starting
     at an unusual number */
  defiInvalidObject = 41713, 
  defiUnknownObject /* void * */
} defiObjectType_e;


/* The memory policy controls how an object which refers to or is composed of
 * other objects manages those sub-objects, particularly when the parent
 * object is copied or deleted.  The policy is specified as an argument to the
 * constructor or initializer, and it is stored with the parent object.
 *
 * The memory policy is a generalization of the common distinction between
 * deep and shallow copies.  When a shallow copy of a parent object is made,
 * the copy maintains pointers to the original sub-objects, and the original
 * parent remains responsible for deleting those sub-objects.  When a deep
 * copy of a parent object is made, the copy maintains pointers to new copies
 * of each of the sub-objects, and the copy is responsible for deleting the
 * new sub-objects.
 *
 * The defiPrivateSubObjects policy corresponds to a deep copy, while the the
 * defiReferencedSubObjects policy corresponds to a shallow copy.  Usually an
 * initial parent object will be created using defiPrivateSubObjects.  When a
 * copy is made of that parent object, the copy may either maintain its own
 * private versions of each sub-object, or it may refer to the original
 * sub-objects.
 *
 * In certain cases, it is useful to create a deep copy of a parent object,
 * even though the new parent object shouldn't be responsible for the new
 * sub-objects.  In this case, the defiOrphanSubObjects and
 * defiAdoptedSubObjects policies may be used.  defiOrphanSubObjects is
 * specified while creating the deep copy, and then defiAdoptedSubObjects is
 * specified while creating another parent which will take on the
 * responsibility for the orphans.
 *
 * An object's memory policy affects only the sub-objects which it directly
 * controls.  Those sub-objects themselves may have the same memory policy as
 * their parents, or they may have a different memory policy.  When a copy is
 * made of a child sub-object, the memory policy of the child controls
 * whether deep or shallow copies are made of the grandchildren.
 */
typedef enum
{
  /* decrease likelihood of accidentally correct values by starting
     at an unusual number */
  defiInvalidMemoryPolicy = 23950,
  defiPrivateSubObjects,      // deep copy + delete
  defiReferencedSubObjects,   // shallow copy, no delete
  defiOrphanSubObjects,       // deep copy, no delete
  defiAdoptedSubObjects       // shallow copy + delete
} defiMemoryPolicy_e;

/* An opaque pointer for passing user data through from one API
 * function to another.
 * A handle which a user can set to point to their own data
 * on a per-callback basis.  (See the comment in defwWriter.h)
 */
#define defiUserData void *
#define defiUserDataHandle void **

/* On SunOs 4.1.3 with acc, this is in libansi.a, but isn't properly
 * declared anywhere in the header files supplied with the compiler.
 */
#ifdef __SunOS_4_1_3
EXTERN int strcasecmp(const char*, const char*);
#endif

#endif

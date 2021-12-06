/***************************************************************************************************************************
Header Files
****************************************************************************************************************************/
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <strings.h>
#include <math.h>
#include <time.h>
#include <complex.h>
#include <assert.h>
#include <limits.h>
/***************************************************************************************************************************
Constant Declarations
****************************************************************************************************************************/
// VARIOUS CONSTANTS
#define Mfnam 20   // max size for a file name
#define Mnod 15000 // max number of nodes in a graph/node
#define Mlin 200   // max size of characters in a line
#define Mnam 25    // max size of a node name
#define Mtyp 10    // max type of nodes/gates
#define Mout 16    // max node out degree (Nfo)
#define Min 9      // max node in degree (Nfi)
#define Mpi 233    // max number of primary inputs
#define Mpo 140    // max number of primary outputs
#define Mpt 100000 // max number of input patterns in .vec file
#define Mft 10     // max number of stuck at faults in .faults file
// NODE TYPE CONSTANTS
#define INPT 1  // Primary Input
#define AND 2   // AND
#define NAND 3  // NAND
#define OR 4    // OR
#define NOR 5   // NOR
#define XOR 6   // XOR
#define XNOR 7  // XNOR
#define BUFF 8  // BUFFER
#define NOT 9   // INVERTER
#define FROM 10 // STEM BRANCH
#define D 3
#define Dbar 4

clock_t start, end;
/***************************************************************************************************************************
Structure Declarations
****************************************************************************************************************************/
// 1.Stucture declaration for LIST
typedef struct LIST_type
{
  int id;                 // id for current element
  struct LIST_type *next; // pointer to next id element( if there is no element, then it will be NULL)
} LIST;
// 2.Stucture declaration for NODE
typedef struct NODE_type
{
  char Name[Mnam];        // name of the node
  int Type, Nfi, Nfo, Po; // type, nooffanins, nooffanouts,primaryo/p
  int Mark, Cval, Fval;   // marker,correctvalue,faultvalue
  LIST *Fin, *Fot;        // fanin members, fanout members
} NODE;
// 3.Stucture declaration for PATTERN

typedef struct PATTERN_type
{
  char p[Mpi];

} PATTERN;

typedef struct nodeValue_type
{
  int id;
  int value;
} nodeValue;

// 4.Stucture declaration for FAULT

/***************************************************************************************************************************
Functions in given.c
****************************************************************************************************************************/
/***************************************************************************************************************************
LIST Structure Functions
****************************************************************************************************************************/
void InsertList(LIST **, int);
void PrintList(LIST *);
void FreeList(LIST **);
void DeleteEle(LIST **, int);
/***************************************************************************************************************************
 NODE Structure Functions
****************************************************************************************************************************/
int ReadIsc(FILE *, NODE *);
int AssignType(char *);
void InitializeCircuit(NODE *, int);
void PrintCircuit(NODE *, int);
void ClearCircuit(NODE *, int);
/***************************************************************************************************************************
 PATTERN Structure Functions
****************************************************************************************************************************/
int ReadVec(FILE *, PATTERN *);
/***************************************************************************************************************************
User Defined Functions in user.c
****************************************************************************************************************************/
int chartoint(char);
int logic(NODE *, int);
int nonControlling(int);
int checkPO(int, NODE *);
int Dfrontier(NODE *, int);
int podemRecursion(NODE *, int, LIST **, int, int);
char inttochar(int);
void setDontCare(int, NODE *);
void callPodem(NODE *, FILE *, int, int);
void testPattern(int, NODE *, int, FILE *);
void simulate(int, NODE *, int, int, LIST **);
nodeValue objective(NODE *, LIST *, int, int);
nodeValue backtrace(NODE *, nodeValue);
/****************************************************************************************************************************/

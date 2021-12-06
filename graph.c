#include "graph.h"
/***************************************************************************************************************************
Insert an element "x" at end of LIST "l", if "x" is not already in "l".
****************************************************************************************************************************/
void InsertList(LIST **l, int x)
{
  LIST *p, *tl;

  if ((p = (LIST *)malloc(sizeof(LIST))) == NULL)
  {
    printf("LIST: Out of memory\n");
    exit(1);
  }
  else
  {
    p->id = x;
    p->next = NULL;
    if (*l == NULL)
    {
      *l = p;
      return;
    }
    tl = NULL;
    tl = *l;
    while (tl != NULL)
    {
      if (tl->id == x)
      {
        break;
      }
      if (tl->next == NULL)
      {
        tl->next = p;
      }
      tl = tl->next;
    }
  }
  return;
} // end of InsertList
/***************************************************************************************************************************
Print the elements in LIST "l"
***************************************************************************************************************************/
void PrintList(LIST *l)
{
  LIST *temp = NULL;

  temp = l;
  while (temp != NULL)
  {
    printf("%d ", temp->id);
    temp = temp->next;
  }
  return;
} // end of PrintList
/***************************************************************************************************************************
Free all elements in  LIST "l"
****************************************************************************************************************************/
void FreeList(LIST **l)
{
  LIST *temp = NULL;

  if (*l == NULL)
  {
    return;
  }
  temp = (*l);
  while ((*l) != NULL)
  {
    temp = temp->next;
    free(*l);
    (*l) = temp;
  }
  (*l) = NULL;
  return;
} // end of FreeList
/*****************************************************************************************************************************
 Routine to read the Bench Markk(.isc files)
*****************************************************************************************************************************/
int ReadIsc(FILE *fisc, NODE *graph)
{
  char c, noty[Mlin], from[Mlin], str1[Mlin], str2[Mlin], name[Mlin], line[Mlin];
  int i, id, fid, Fin, fout, mid = 0;

  // intialize all nodes in graph structure
  for (i = 0; i < Mnod; i++)
  {
    InitializeCircuit(graph, i);
  }
  // skip the comment lines
  do
    fgets(line, Mlin, fisc);
  while (line[0] == '*');
  // read line by line
  while (!feof(fisc))
  {
    // initialize temporary strings
    bzero(noty, strlen(noty));
    bzero(from, strlen(from));
    bzero(str1, strlen(str1));
    bzero(str2, strlen(str2));
    bzero(name, strlen(name));
    // break line into data
    sscanf(line, "%d %s %s %s %s", &id, name, noty, str1, str2);
    // fill in the Typee
    strcpy(graph[id].Name, name);
    graph[id].Type = AssignType(noty);
    // fill in fanin and fanout numbers
    if (graph[id].Type != FROM)
    {
      fout = atoi(str1);
      Fin = atoi(str2);
    }
    else
    {
      Fin = fout = 1;
      strcpy(from, str1);
    }
    if (id > mid)
    {
      mid = id;
    }
    graph[id].Nfo = fout;
    graph[id].Nfi = Fin;
    if (fout == 0)
    {
      graph[id].Po = 1;
    }
    // create fanin and fanout lists
    switch (graph[id].Type)
    {
    case 0:
      printf("ReadIsc: Error in input file (Node %d)\n", id);
      exit(1);
    case INPT:
      break;
    case AND:
    case NAND:
    case OR:
    case NOR:
    case XOR:
    case XNOR:
    case BUFF:
    case NOT:
      for (i = 1; i <= Fin; i++)
      {
        fscanf(fisc, "%d", &fid);
        InsertList(&graph[id].Fin, fid);
        InsertList(&graph[fid].Fot, id);
      }
      fscanf(fisc, "\n");
      break;
    case FROM:
      for (i = mid; i > 0; i--)
      {
        if (graph[i].Type != 0)
        {
          if (strcmp(graph[i].Name, from) == 0)
          {
            fid = i;
            break;
          }
        }
      }
      InsertList(&graph[id].Fin, fid);
      InsertList(&graph[fid].Fot, id);
      break;
    } // end case
    bzero(line, strlen(line));
    fgets(line, Mlin, fisc);
  } // end while
  return mid;
} // end of ReadIsc
/****************************************************************************************************************************
Initialize the paricular memeber of graph structure
****************************************************************************************************************************/
void InitializeCircuit(NODE *graph, int num)
{
  bzero(graph[num].Name, Mnam);
  graph[num].Type = graph[num].Nfi = graph[num].Nfo = graph[num].Po = graph[num].Mark = 0;
  graph[num].Cval = graph[num].Fval = 2;
  graph[num].Fin = graph[num].Fot = NULL;
  return;
} // end of InitializeCircuit
/****************************************************************************************************************************
Convert (char *) Typee read to (int)
****************************************************************************************************************************/
int AssignType(char *Type)
{
  if ((strcmp(Type, "inpt") == 0) || (strcmp(Type, "INPT") == 0))
    return 1;
  else if ((strcmp(Type, "and") == 0) || (strcmp(Type, "AND") == 0))
    return 2;
  else if ((strcmp(Type, "nand") == 0) || (strcmp(Type, "NAND") == 0))
    return 3;
  else if ((strcmp(Type, "or") == 0) || (strcmp(Type, "OR") == 0))
    return 4;
  else if ((strcmp(Type, "nor") == 0) || (strcmp(Type, "NOR") == 0))
    return 5;
  else if ((strcmp(Type, "xor") == 0) || (strcmp(Type, "XOR") == 0))
    return 6;
  else if ((strcmp(Type, "xnor") == 0) || (strcmp(Type, "XNOR") == 0))
    return 7;
  else if ((strcmp(Type, "buff") == 0) || (strcmp(Type, "BUFF") == 0))
    return 8;
  else if ((strcmp(Type, "not") == 0) || (strcmp(Type, "NOT") == 0))
    return 9;
  else if ((strcmp(Type, "from") == 0) || (strcmp(Type, "FROM") == 0))
    return 10;
  else
    return 0;
} // end of AssignTypee
/****************************************************************************************************************************
Print all members of graph structure(except Type=0) after reading the bench file
*****************************************************************************************************************************/
void PrintCircuit(NODE *graph, int Max)
{
  LIST *temp;
  int i;
  printf("\nID\tNAME\tTypeE\tPO\tIN#\tOUT#\tCVAL\tFVAL\tMarkK\tFANIN\tFANOUT\n");
  for (i = 0; i <= Max; i++)
  {
    if (graph[i].Type != 0)
    {
      printf("%d\t%s\t%d\t%d\t%d\t%d\t", i, graph[i].Name, graph[i].Type, graph[i].Po, graph[i].Nfi, graph[i].Nfo);
      printf("%d\t%d\t%d\t", graph[i].Cval, graph[i].Fval, graph[i].Mark);
      temp = NULL;
      temp = graph[i].Fin;
      if (temp != NULL)
      {
        PrintList(temp);
      }
      printf("\t");
      temp = NULL;
      temp = graph[i].Fot;
      if (temp != NULL)
      {
        PrintList(temp);
      }
      printf("\n");
    }
  }
  return;
} // end of PrintCircuit
/****************************************************************************************************************************
Free the memory of all member of graph structure
*****************************************************************************************************************************/
void ClearCircuit(NODE *graph, int i)
{
  int num = 0;
  for (num = 0; num < i; num++)
  {
    graph[num].Type = graph[num].Nfi = graph[num].Nfo = graph[num].Po = 0;
    graph[num].Mark = graph[num].Cval = graph[num].Fval = 0;
    if (graph[num].Type != 0)
    {
      bzero(graph[num].Name, Mnam);
      if (graph[num].Fin != NULL)
      {
        FreeList(&graph[num].Fin);
        graph[num].Fin = NULL;
      }
      if (graph[num].Fot != NULL)
      {
        FreeList(&graph[num].Fot);
        graph[num].Fot = NULL;
      }
    }
  }
  return;
} // end of ClearCircuit
/*****************************************************************************************************************************
 Routine to read the .vec files
*****************************************************************************************************************************/
int ReadVec(FILE *fvec, PATTERN *vec)
{
  char arr[Mpi];
  int j = 0;

  while (fgets(arr, sizeof(arr), fvec) != NULL)
  {
    for (int i = 0; i < strlen(arr); i++)
    {
      if (arr[i] == 'x')
        arr[i] = '2';
    }
    strcpy(vec[j].p, arr);
    j++;
  }
  return j;
  // fclose(fvec);
} // end of ReadVec
/****************************************************************************************************************************/

// function to initialize DONT CARE
void setDontCare(int Max, NODE *graph)
{
  int i;
  for (i = 1; i <= Max; i++)
  {
    graph[i].Cval = 2;
    graph[i].Mark = 0;
  }
}

// function to check if primary output is D or Dbar
int checkPO(int Max, NODE *graph)
{
  int i;
  for (i = 1; i <= Max; i++)
  {
    if (graph[i].Fot == NULL)
    {
      if (graph[i].Cval == D || graph[i].Cval == Dbar)
        return 1;
    }
  }
}

// function to print the test patterns
void testPattern(int Max, NODE *graph, int faultValue, FILE *fres)
{
  int i;
  for (i = 0; i <= Max; i++)
  {
    if (graph[i].Cval == D || graph[i].Cval == Dbar)
    {
      graph[i].Cval = !faultValue;
    }
    if (graph[i].Type == INPT)
    {
      fprintf(fres, "%c", inttochar(graph[i].Cval));
    }
  }
  fprintf(fres, "\n");
}
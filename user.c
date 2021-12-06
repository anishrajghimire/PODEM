#include "graph.h"

// truth table for and,or,not and xor gates in array format
int and[5][5] = {{0, 0, 0, 0, 0}, {0, 1, 2, 3, 4}, {0, 2, 2, 2, 2}, {0, 3, 2, 3, 0}, {0, 4, 2, 0, 4}};
int or [5][5] = {{0, 1, 2, 3, 4}, {1, 1, 1, 1, 1}, {2, 1, 2, 2, 2}, {3, 1, 2, 3, 1}, {4, 1, 2, 1, 4}};
int not [5] = {1, 0, 2, 4, 3};
int xor [5][5] = {{0, 1, 2, 3, 4}, {1, 0, 2, 4, 3}, {2, 2, 2, 2, 2}, {3, 4, 2, 0, 1}, {4, 3, 2, 1, 0}};

LIST *temp;

// function to change char to int
int chartoint(char a)
{
    if (a == '1')
        return 1;
    else if (a == '0')
        return 0;
    else if (a == '2')
        return 2;
}

// function to change int to char
char inttochar(int a)
{
    if (a == 1)
        return '1';
    else if (a == 0)
        return '0';
    else if (a == 2)
        return 'x';
}

// function to assign non-controlling values
int nonControlling(int gate)
{
    int output;
    switch (gate)
    {
    case 2:
    case 3:
        output = 1;
        break;
    case 4:
    case 5:
        output = 0;
        break;
    case 6:
    case 7:
    case 8:
    case 9:
    case 10:
        output = 5;
        break;
    default:
        output = 5;
    }
    return output;
}

// function to simulate the benchmark files
void simulate(int Max, NODE *g, int faultNode, int faultValue, LIST **dfrontier)
{
    int i;
    int x;
    // int masked = 0;
    int checkDfrontier = 0;

    for (i = 1; i <= Max; i++)
    {
        if (g[i].Fin != NULL)
        {
            temp = g[i].Fin;
            x = logic(g, i); // function call to logic
            g[i].Cval = x;
        }
        if (g[faultNode].Cval == !faultValue)
        {
            if (faultValue == 0)
                g[faultNode].Cval = D;

            else
                g[faultNode].Cval = Dbar;
        }

        checkDfrontier = Dfrontier(g, i); // function call to check Dfrontier
        if (checkDfrontier)
        {
            InsertList(dfrontier, i); // insert gate to Dfrontier list
        }
    }
}

// funtion to simulate the logic
int logic(NODE *g, int i)
{
    int x, y;
    if (g[i].Type == AND) // obtain output after simulating AND gate
    {
        x = g[temp->id].Cval;
        while (temp->next != NULL)
        {
            y = g[temp->next->id].Cval;
            x = and[x][y];
            temp = temp->next;
        }
    }

    else if (g[i].Type == OR) // obtain output after simulating OR gate
    {
        x = g[temp->id].Cval;
        while (temp->next != NULL)
        {
            y = g[temp->next->id].Cval;
            x = or [x][y];
            temp = temp->next;
        }
    }

    else if (g[i].Type == NOT) // obtain output after simulating NOT gate
    {
        x = g[temp->id].Cval;
        x = not [x];
    }

    else if (g[i].Type == NAND) // obtain output after simulating NAND gate
    {
        x = g[temp->id].Cval;
        while (temp->next != NULL)
        {
            y = g[temp->next->id].Cval;
            x = and[x][y];
            temp = temp->next;
        }
        x = not [x];
    }

    else if (g[i].Type == NOR) // obtain output after simulating NOR gate
    {
        x = g[temp->id].Cval;
        while (temp->next != NULL)
        {
            y = g[temp->next->id].Cval;
            x = or [x][y];
            temp = temp->next;
        }
        x = not [x];
    }

    else if (g[i].Type == XOR) // obtain output after simulating XOR gate
    {
        x = g[temp->id].Cval;
        while (temp->next != NULL)
        {
            y = g[temp->next->id].Cval;
            x = xor[x][y];
            temp = temp->next;
        }
    }

    else if (g[i].Type == XNOR) // obtain output after simulating XNOR gate
    {
        x = g[temp->id].Cval;
        while (temp->next != NULL)
        {
            y = g[temp->next->id].Cval;
            x = xor[x][y];
            temp = temp->next;
        }
        x = not [x];
    }

    else if (g[i].Type == BUFF) // obtain output after simulating BUFF gate
    {
        x = g[temp->id].Cval;
    }

    else if (g[i].Type == FROM) // obtain output after simulating FROM gate
    {
        x = g[temp->id].Cval;
    }
    return x;
}

// function to check dfrontier node
int Dfrontier(NODE *graph, int nodeLine)
{
    int ret = 0;
    temp = graph[nodeLine].Fin;
    if (graph[nodeLine].Cval == 2)
    {
        while (temp != NULL)
        {
            if (graph[temp->id].Cval == D || graph[temp->id].Cval == Dbar)
            {
                ret = 1;
                break;
            }
            temp = temp->next;
        }
    }
    return ret;
}

// function to return the current objective
nodeValue objective(NODE *graph, LIST *dfrontier, int faultNode, int faultValue)
{
    int i;
    nodeValue object;
    object.id = object.value = 0;

    if (graph[faultNode].Cval != D && graph[faultNode].Cval != Dbar) // to check whether fault is excited
    {
        object.id = faultNode;
        object.value = not [faultValue];
        return object;
    }
    while (dfrontier != NULL)
    {
        i = dfrontier->id;
        temp = graph[i].Fin;
        while (temp != NULL)
        {
            if (graph[temp->id].Cval == 2)
            {
                object.id = temp->id;
                break;
            }
            temp = temp->next;
        }
        object.value = nonControlling(graph[i].Type); // function call to assign non-controlling value
        return object;
    }
}

// function for backtrace
nodeValue backtrace(NODE *graph, nodeValue object)
{
    nodeValue returnBack;
    int i = object.id;
    int inversion = 0;
    int checkNode = 0;

    while (graph[i].Nfi != 0) // to check until it backtrace to input
    {
        if (graph[i].Type == NOT || graph[i].Type == NOR || graph[i].Type == NAND)
        {
            inversion++;
        }
        temp = graph[i].Fin;
        while (temp != NULL)
        {
            i = temp->id;
            if (graph[i].Cval == 2)
            {
                checkNode = i;
                break;
            }
            temp = temp->next;
        }
        i = checkNode;
    }
    returnBack.id = i;

    if (inversion % 2 == 0)
        returnBack.value = object.value;
    else
        returnBack.value = not [object.value];

    return returnBack;
}

// function for PODEM-Recursion
int podemRecursion(NODE *graph, int Max, LIST **dfrontier, int faultID, int faultVALUE)
{

    int faultNode, faultValue;
    int PO = 0, podem = 0;
    nodeValue object, traceBack;
    double cpu_time;
    faultNode = faultID;
    faultValue = faultVALUE;

    PO = checkPO(Max, graph); // to check the fault-effect observed at output
    if (PO == 1)
    {
        return 1;
    }
    end = clock();
    cpu_time = ((double)(end - start)) / CLOCKS_PER_SEC;
    if (cpu_time > 1)
    {
        return 2;
    }
    object = objective(graph, *dfrontier, faultNode, faultValue); // function call to return objective
    FreeList(dfrontier);                                          // free all elements of Dfrontier list

    traceBack = backtrace(graph, object); // function call for backtrace
    graph[traceBack.id].Cval = traceBack.value;

    simulate(Max, graph, faultNode, faultValue, dfrontier); // funtion call to simulate the circuit

    PO = checkPO(Max, graph); // function call to check if PO is D or D_bar
    if (PO == 1)
    {
        return 1;
    }

    if (*dfrontier != NULL || graph[faultNode].Cval == 2) // condition to check fault masked
    {
        podem = podemRecursion(graph, Max, dfrontier, faultNode, faultValue); // function call for Podem Recursion

        if (podem == 1)
        {
            return 1;
        }
        else if (podem == 2)
        {
            return 2;
        }
    }

    traceBack.value = !graph[traceBack.id].Cval;
    graph[traceBack.id].Cval = traceBack.value;

    simulate(Max, graph, faultNode, faultValue, dfrontier); // funtion call to simulate the circuit

    PO = checkPO(Max, graph); // function call to check if PO is D or D_bar

    if (PO == 1)
    {
        return 1;
    }

    if (*dfrontier != NULL || graph[faultNode].Cval == 2) // condition to check fault masked
    {
        podem = podemRecursion(graph, Max, dfrontier, faultNode, faultValue); // function call for Podem Recursion

        if (podem == 1)
        {
            return 1;
        }
        else if (podem == 2)
        {
            return 2;
        }
    }

    traceBack.value = 2;
    graph[traceBack.id].Cval = traceBack.value;

    simulate(Max, graph, faultNode, faultValue, dfrontier); // funtion call to simulate the circuit
    return 0;
}
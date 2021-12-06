#include "graph.h"
/***************************************************************************************************************************
 Main Function(Serial and Parallel Fault Simulation)
****************************************************************************************************************************/
void main(int argc, char **argv)
{
    FILE *fisc, *fvec, *ffau, *fres;    // file pointers used for .isc file, .vec file, .faults file and resultfile
    int Max, Opt, Npi, Npo, Total, Tfs; // maxnode id,option,tot no of PIs,tot no of Pos,Tot no of input patterns& faults in.vec in.faults
    LIST *dfrontier = NULL;
    NODE *graph;                                 // structure used to store the ckt information in .isc file
    graph = (NODE *)malloc(Mnod * sizeof(NODE)); // Dynamic memory allocation for graph structure
    static PATTERN p[Mpt];                       // structure used to store the input vectors information in .vec file

    // FAULT stuck[Mft];                     //structure used to store the faults information in .faults file
    int i, j, a, b, c, d; // random variables
    int faultValue, faultNode;
    int faultID, faultVALUE;
    int podem;
    int totalFault = 0;
    int abortedFault = 0;
    int detectedFault = 0;
    int untestableFault = 0;
    float faultCoverage;

    // Read the .isc file and store the information in graph structure
    fisc = fopen(argv[1], "r"); // file pointer to open .isc file
    Max = 0;
    Max = ReadIsc(fisc, graph); // read .isc file and return index of last node in graph formed
    fclose(fisc);               // close file pointer for .isc file
    PrintCircuit(graph, Max);   // print all members of graph structure

    // Read the .vec file and store the information in  vector structure
    //  fvec = fopen(argv[2], "r");
    //  Total = 0;
    //  Total = ReadVec(fvec, p);
    //  fclose(fvec);

    fres = fopen(argv[2], "w");

    for (i = 1; i <= Max; i++)
    {
        faultID = i;
        faultVALUE = 0;
        if (graph[faultID].Type == 0)
            continue; // to skip unknown nodes
        start = clock();
        podem = podemRecursion(graph, Max, &dfrontier, faultID, faultVALUE); // function call for Podem Recursion
        if (podem == 1)
        {
            fprintf(fres, "%d/SAF %d \n", faultID, faultVALUE);
            testPattern(Max, graph, faultValue, fres); // print the test patterns
            detectedFault++;
        }
        else if (podem == 2)
        {
            abortedFault++;
        }
        else
        {
            untestableFault++;
        }
        totalFault++;
        setDontCare(Max, graph); // function call for DontCare
    }

    for (i = 1; i <= Max; i++)
    {
        faultID = i;
        faultVALUE = 1;
        if (graph[faultID].Type == 0)
            continue; // to skip unknown nodes
        start = clock();
        podem = podemRecursion(graph, Max, &dfrontier, faultID, faultVALUE); // function call for Podem Recursion
        if (podem == 1)
        {
            fprintf(fres, "%d/SAF %d \n", faultID, faultVALUE);
            testPattern(Max, graph, faultValue, fres); // print the test patterns
            detectedFault++;
        }
        else if (podem == 2)
        {
            abortedFault++;
        }
        else
        {
            untestableFault++;
        }
        totalFault++;
        setDontCare(Max, graph); // function call for DontCare
    }

    fprintf(fres, "\nTotal Faults Detected: %d \n", detectedFault);
    fprintf(fres, "Total Faults Aborted: %d \n", abortedFault);
    fprintf(fres, "Untestable Faults:%d \n", untestableFault);
    faultCoverage = (detectedFault / (float)totalFault) * 100;
    fprintf(fres, "Fault Coverage is %f", faultCoverage);
    fprintf(fres, "\n");

    // Opt=0;
    // Opt=atoi(argv[3]);                    //getting the option from terminal for xval
    // fres=fopen(argv[2],"w");              //file pointer to open .out file for printing results

    // Perform Logic Simulation for each Input vector and print the Pos .val in output file

    fclose(fres);              // close file pointer for .out file
    ClearCircuit(graph, Mnod); // clear memeory for all members of graph
    // for(a=0;a<Total;a++)                    //clear memeory for all members of vector
    // {
    //     bzero(vector[a].piv,Mpi);
    // }
    return;
} // end of main
/****************************************************************************************************************************/

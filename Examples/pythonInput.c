/* An example to show how to use some external python function to evaluate function value at current point */
/* We use Rosenbrock.py to evaluate the function */



#include "funcEval.h"

//#define INFS DBL_MAX

void userBasicIntialize (userData *uData, char **argv)
{
	FILE *fpArgs;
	fpArgs=fopen(argv[1],"r");
	int optionSize=1024;
	char option[optionSize];
	char optionName[50];
	char optionValue[50];

	while (fgets (option, optionSize,fpArgs) != NULL)
	{
		sscanf (option,"%s %s\n",optionName,optionValue);
	//	printf ("%s = %s\n",optionName,optionValue);
		if (strcmp(optionName,"Dimension") == 0)
		{
			uData->dimension=atoi(optionValue);
		}

		if (strcmp(optionName,"InitialPointDataFile") == 0)
		{
			FILE *fp;
			fp=fopen(optionValue,"r");
			uData->startPt=malloc (sizeof(double)*uData->dimension);
			int i;
			for (i=0; i<uData->dimension; i++) 
			{
				fscanf(fp,"%lf",&uData->startPt[i]);
			}
			fclose(fp);
		}
		if (strcmp(optionName,"Precision") == 0)
		{
			uData->precision=atof(optionValue);
		}
		if (strcmp(optionName,"BoundsDataFile") == 0)
		{
			uData->fpBound=fopen(optionValue,"r");
		}
		if (strcmp(optionName,"NumberConstraints") == 0)
		{
			uData->numConstr=atoi(optionValue);
		}
		if (strcmp(optionName,"PollPattern") == 0)
		{
			uData->p=optionValue[0];
			uData->pollPattern=&uData->p;
			if (*uData->pollPattern != 'H' && *uData->pollPattern != 'S')
			{
				printf ("unknown poll sequence type. Possible types are: 'H' or 'S'. Exiting\n");
				exit (10);
			}
		}
		if (strcmp(optionName,"InitialPointFeasible") == 0)
		{
			uData->initPtFeasibility=atoi(optionValue);
			if (uData->initPtFeasibility == false)
			{
				printf ("Initial point is not satisfying bound constraints. Exiting\n");
				exit (10);
			}
		}
		if (strcmp(optionName,"Maxevals") == 0)
		{
			uData->iterLimit=atoi(optionValue);
			if (uData->iterLimit < uData->dimension+1)
			{
				printf ("Too few evaluations supplied. Give atleast n+2 evaluations. Exiting\n");
				exit (10);
			}
		}
		if (strcmp(optionName,"Loglevel") == 0)
		{
			uData->logLevel=atoi(optionValue);
			if (uData->logLevel < 0 || uData->logLevel > 2)
			{
				printf ("Unknown logLevel. It must be between 0 to 2. Exiting\n");
				exit (10);
			}
		}
	}
//	printf ("precision is %G\n",uData->precision);
//	printf ("pollPattern is %c\n",*uData->pollPattern);
//	printf ("max Evals is %d\n",uData->iterLimit);
//	printf ("printing startPt\n");
//	printf ("printing initial pt feasibility %d\n",uData->initPtFeasibility);
//	printf ("number of constraints  %d\n",uData->numConstr);

}

void userExtraIntialize (userData *uData, char **argv)
{
}

double funcEvalValue(double *x, userData *uData)
{
	double funcval=0;
	char command[1024];
	FILE *finput;
	finput=fopen ("inputPoint.txt","w");
	int i;
	for (i=0; i<uData->dimension; i++)
	{
		fprintf (finput,"%lf\n",x[i]);
	}
	fclose (finput);

	strcpy (command, "python Rosenbrock.py");
	system (command);

	FILE *foutputReader;
	foutputReader=fopen ("funcOutput.txt","r");
	fscanf (foutputReader,"%lf",&funcval);
	fclose (foutputReader);
	return funcval;
}

void constrEval (double *x, double *constrVec, userData *uData)
{
}

void boundsEval (double *varLB, double *varUB, double *constrLB, double *constrUB, userData *uData)
{
	int i;
	for (i=0; i<uData->dimension; i++)
	{
		fscanf(uData->fpBound,"%lf %lf",&varLB[i], &varUB[i]);
	}
	fclose(uData->fpBound);
}

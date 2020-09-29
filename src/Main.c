#define _CRT_SECURE_NO_WARNINGS

#include <stdlib.h>
#include <math.h>
#include <stdio.h>

#define error(...) (fprintf(stderr, __VA_ARGS__))
#define PI 3.1415926535
#define MAX_STRING_LENGTH 32

struct GivenInfo
{
	double leftBorder;
	double rightBorder;
};

double RectMethod(double left, double right)
{
	return (right - left) * sin((left + right) / 2);
}

double SimpsonMethod(double left, double right)
{
	return (right - left) / 6 * (sin(left) + 4 * sin((left + right) / 2) + sin(right));
}

double CalcIntegral(struct GivenInfo interval, int partion, double (*method)(double, double))
{
	double integral = 0;
	double deltaX = (interval.rightBorder - interval.leftBorder) / partion;
	double left = interval.leftBorder;

	for (int i = 0; i < partion; i++)
	{
		double right = left + deltaX;
		integral += method(left, right);
		left = right;
	}

	return integral;
}

void FreeMemory(char** string, int count)
{
	for (int i = 0; i < count; ++i)
	{
		free(string[i]);
	}
	free(string);
}

int UserInput(struct GivenInfo* interval)
{
	if (printf("Enter a left border of the interval: ") < 0)
	{
		error("Error occurred during writing to stdout");
		return -1;
	}

	if (scanf("%lf", &(interval->leftBorder)) != 1)
	{
		error("Error occurred during reading left border");
	}

	if (interval->leftBorder < 0)
	{
		error("Left border must be > or == 0");
		return -1;
	}

	if (printf("Enter a right border of the interval: ") < 0)
	{
		error("Error occurred during writing to stdout");
		return -1;
	}

	if (scanf("%lf", &(interval->rightBorder)) != 1)
	{
		error("Error occurred during reading right border");
	}

	if (interval->rightBorder > PI)
	{
		error("Right border must be < or == to PI");
		return -1;
	}

	if (interval->leftBorder >= interval->rightBorder)
	{
		error("Left border must be < than right one");
		return -1;
	}

	return 0;
}

char** UserOutput(struct GivenInfo interval, int* partions, int testCount)
{
	char** results = (char**)malloc(sizeof(char*) * testCount);
	if (!results) return NULL;

	for (int i = 0; i < testCount; i++)
	{
		double integralRectMethod = CalcIntegral(interval, partions[i], RectMethod);
		double integralSimpsonMethod = CalcIntegral(interval, partions[i], SimpsonMethod);

		results[i] = (char*)malloc(sizeof(char) * MAX_STRING_LENGTH);
		if (!results[i])
		{
			FreeMemory(results, i);
			error("Error occurred during allocation of memory (info: %d test)\n", i);
			return NULL;
		}

		if (!sprintf(results[i], "%d %.5f %.5f", partions[i], integralRectMethod, integralSimpsonMethod))
		{
			FreeMemory(results, i + 1);
			error("Error occurred during writing results to the string (info: %d test)\n", i);
			return NULL;
		}
	}

	return results;
}

int main()
{
	struct GivenInfo interval = { 0, 0 };
	int partions[] = { 6, 10, 20, 100, 500, 1000 };
	int testCount = sizeof(partions) / sizeof(partions[0]);
	
	if(UserInput(&interval)) return 1;

	char** FinalResults = UserOutput(interval, partions, testCount);

	if (!FinalResults) return 1;

	for (int i = 0; i < testCount; i++)
	{
		if (printf("%s\n", FinalResults[i]) < 0)
		{
			error("Error occurred during writing %d result to stdout", i);
			break;
		}
	}
	FreeMemory(FinalResults, testCount);
	return 0;
}

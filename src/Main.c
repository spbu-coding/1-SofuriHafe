#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif
#define error(...) (fprintf(stderr, __VA_ARGS__))
#define MAX_STRING_LENGTH 32

struct GivenInfo
{
	double left_border;
	double right_border;
};

double cacl_by_rect_method(double left_side, double right_side)
{
	return (right_side - left_side) * sin((left_side + right_side) / 2);
}

double calc_by_simpson_method(double left_side, double right_side)
{
	return (right_side - left_side) / 6 * (sin(left_side) + 4 * sin((left_side + right_side) / 2) + sin(right_side));
}

double calc_integral(struct GivenInfo interval, int partion, double (*method)(double, double))
{
	double integral = 0;
	double deltaX = (interval.right_border - interval.left_border) / partion;
	double left_side = interval.left_border;

	for (int i = 0; i < partion; i++)
	{
		double right_side = left_side + deltaX;
		integral += method(left_side, right_side);
		left_side = right_side;
	}

	return integral;
}

void free_string_memory(char** string, int count)
{
	for (int i = 0; i < count; ++i)
	{
		free(string[i]);
	}

	free(string);
}

int user_input(struct GivenInfo* interval)
{
	if (printf("Enter a left border of the interval: ") < 0)
	{
		error("Error occurred during writing to stdout");
		return -1;
	}

	if (scanf("%lf", &(interval->left_border)) != 1)
	{
		error("Error occurred during reading left border");
	}

	if (interval->left_border < 0)
	{
		error("Left border must be > or == 0");
		return -1;
	}

	if (printf("Enter a right border of the interval: ") < 0)
	{
		error("Error occurred during writing to stdout");
		return -1;
	}

	if (scanf("%lf", &(interval->right_border)) != 1)
	{
		error("Error occurred during reading right border");
	}

	if (interval->right_border > M_PI)
	{
		error("Right border must be < or == to PI");
		return -1;
	}

	if (interval->left_border >= interval->right_border)
	{
		error("Left border must be < than right one");
		return -1;
	}

	return 0;
}

char** user_output(struct GivenInfo interval, int* partions, int test_count)
{
	char** results = (char**)malloc(sizeof(char*) * test_count);
	if (!results) return NULL;

	for (int i = 0; i < test_count; i++)
	{
		double integral_rect_method = calc_integral(interval, partions[i], cacl_by_rect_method);
		double integral_simpson_method = calc_integral(interval, partions[i], calc_by_simpson_method);

		results[i] = (char*)malloc(sizeof(char) * MAX_STRING_LENGTH);
		if (!results[i])
		{
			free_string_memory(results, i);
			error("Error occurred during allocation of memory (info: %d test)\n", i);
			return NULL;
		}

		if (!sprintf(results[i], "%d %.5f %.5f", partions[i], integral_rect_method, integral_simpson_method))
		{
			free_string_memory(results, i + 1);
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
	int test_count = sizeof(partions) / sizeof(partions[0]);
	
	if(user_input(&interval)) return 1;

	char** final_results = user_output(interval, partions, test_count);

	if (!final_results) return 1;

	for (int i = 0; i < test_count; i++)
	{
		if (printf("%s\n", final_results[i]) < 0)
		{
			error("Error occurred during writing %d result to stdout", i);
			break;
		}
	}

	free_string_memory(final_results, test_count);
	return 0;
}

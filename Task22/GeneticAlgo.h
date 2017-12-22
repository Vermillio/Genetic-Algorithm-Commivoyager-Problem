#ifndef GENETIC_ALGO_H
#define GENETIC_ALGO_H

#include <vector>

using namespace std;

typedef struct point
{
	double x, y;
};

typedef struct T_BestVal
{
	double value;
	int index;
};

class GeneticAlgo
{
	int unchangedGens; // ?
	int POPULATION_SIZE; // default 50
	double MUTATION_PROBABILITY; // default 1/50
	double CROSSOVER_PROBABILITY; //default 0.7
	int MAX_UNCHANGED_GEN_NUM;

	//int genNum; // default 0
	int MAX_GEN_NUM; //default 100

	vector<int> Solution;

	int pointsNum;
	point *points; //заданные точки
	double **distances; //матрица расстояний
	vector<vector<int>> population;
	vector<vector<int>> parents;

	int seed; //для датчика случайных чисел
	int current_gen;

	double *values; // sizeof(points)
	T_BestVal prevBest;
	T_BestVal BestValue; // = 0

	//методы
	void Run();
	void Init();
	void CountDistances();
	vector<int> RandomIndividual();
	void CountBestValue();

	double Evaluate(vector<int> Individual);
	void NextGen();
	void selection();
	void crossover();
	void doCrossover(int index1, int index2);
	void mutation();
	void mutate(int i);
	public:
		vector<int> Commivoyager();
		vector<int> Commivoyager(point *Points, int populSize, double crossoverProb, double mutatProb, int MaxGenNum, int MaxUnchangedGenNum);
		void Commivoyager_Demo();
		void Print(vector<int> x);
		void ReadFile(string filename);
		void ConsoleInput();
};

#endif
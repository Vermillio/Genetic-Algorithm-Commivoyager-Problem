#include "GeneticAlgo.h"

#include <windows.h>
#include <conio.h>
#include <fstream>
#include <iostream>
#include <cstdlib>
#include <string>
#include <cmath>
#include <vector>
#include <algorithm>
#include <random>


using namespace std;

void GeneticAlgo::Run()
{
	srand(seed);
	BestValue.index = -1;
	if (POPULATION_SIZE < 2)
	{
		cout << "Population size should be >= 2. ";
		return;
	}
	unchangedGens = 0;


	Init();

	for (int i = 1; i <= MAX_GEN_NUM; i++)
	{
		NextGen();
		CountBestValue();
		if (unchangedGens > MAX_UNCHANGED_GEN_NUM)
			break;
	}
	Solution = population[BestValue.index];
}

void GeneticAlgo::Init()
{

	values = new double[POPULATION_SIZE];
	
	CountDistances();
	for (int i = 0; i < POPULATION_SIZE; i++)
		population.push_back(RandomIndividual());
	CountBestValue();
}

//+
void GeneticAlgo::CountDistances()
{
	double a, b;
	distances = new double*[pointsNum];
	for (int i = 0; i < pointsNum; i++)
		distances[i] = new double[pointsNum];
	for (int i = 0; i < pointsNum; i++)
		for (int j = 0; j < pointsNum; j++)
		{
			a = pow(abs(points[i].x - points[j].x), 2);
			b = pow(abs(points[i].y - points[j].y), 2);
			distances[i][j] = pow(a + b, 0.5);
		}
}


//+
vector<int> GeneticAlgo::RandomIndividual()
{
	vector<int> a;
	for (int i = 0; i < pointsNum; i++)
	{
		a.push_back(i);
	}
	shuffle(a.begin(), a.end(), default_random_engine(rand()));
	return a;
}

//+
void GeneticAlgo::CountBestValue()
{
	double it;
	T_BestVal current_best;
	current_best.value = values[0] = Evaluate(population[0]);
	current_best.index = 0;
	for (int i = 1; i < POPULATION_SIZE; i++)
	{
		values[i] = Evaluate(population[i]);
		if (values[i] < current_best.value)
		{
			current_best.index = i;
			current_best.value = values[i];
		}
	}
	if (BestValue.value > current_best.value || BestValue.index < 0)
	{
		BestValue = current_best;
	}
	else if (BestValue.value == current_best.value)
		unchangedGens++;

}

//+
double GeneticAlgo::Evaluate(vector<int> Individual)
{
	double sum = distances[Individual[0]][Individual[Individual.size() - 1]];
	for (int i = 0; i < Individual.size() - 1; i++)
		sum += distances[Individual[i]][Individual[i+1]];
	return sum;
}


void GeneticAlgo::NextGen()
{
	current_gen++;
	selection();
	crossover();
	mutation();
}

void GeneticAlgo::selection()
{
	parents.clear();
	parents.push_back(population[BestValue.index]);
	for (int i = 0; i < population.size() - 1; i++)
		if (values[i] < values[i + 1])
			parents.push_back(population[i]);
		else parents.push_back(population[i+1]);
}

//+
void GeneticAlgo::crossover()
{
	vector<int> queue;

	for (int i = 0; i < POPULATION_SIZE; i++)
		if (1 / (1 + exp((double)rand())) < CROSSOVER_PROBABILITY)
			queue.push_back(i);

	if (queue.size() < 2)
		return;

	for (int i = 0; i < queue.size() - 1; i++)
	{
		if (queue[i] == queue[i + 1])
		{
			mutate(i);
			mutate(i + 1);
		}
		else {
			doCrossover(queue[i], queue[i + 1]);
			doCrossover(queue[i + 1], queue[i]);
	}
	}
}


//+
void GeneticAlgo::doCrossover(int index1, int index2)
{
	population[index1].clear();
	vector<int> que;
	int border_l = rand() % pointsNum;                             //choose interval for copying to child
	int border_r = border_l + rand() % (pointsNum - border_l);
	que.assign(parents[index1].begin() + border_l, parents[index1].begin() + border_r + 1);

	for (int i = 0; i < pointsNum; i++)
	{
		bool used = false;
		for (int j = 0; j <= border_r - border_l; j++)
		{
			if (que[j] == parents[index2][i])
			{
				used = true;
				break;
			}
		}
		if (used != true)
			population[index1].push_back(parents[index2][i]);
	}
	population[index1].insert(population[index1].begin() + border_l, que.begin(), que.end());


}


void GeneticAlgo::mutation()
{
	for (int i = 0; i < sizeof(points); i++)
		if ((double)rand() / RAND_MAX < MUTATION_PROBABILITY)
			mutate(i);
}

void GeneticAlgo::mutate(int i)
{
	swap(population[i][rand() % population[i].size()], population[i][rand() % population[i].size()]);
}

vector<int> GeneticAlgo::Commivoyager()
{
	Run();
	return Solution;
}

vector<int> GeneticAlgo::Commivoyager(point * Points, int populSize, double crossoverProb, double mutatProb, int MaxGenNum, int MaxUnchangedGenNum)
{
	points = Points;
	POPULATION_SIZE = populSize;
	CROSSOVER_PROBABILITY = crossoverProb;
	MUTATION_PROBABILITY = mutatProb;
	MAX_GEN_NUM = MaxGenNum;
	MAX_UNCHANGED_GEN_NUM = MaxUnchangedGenNum;
	return Commivoyager();
}

void GeneticAlgo::Commivoyager_Demo()
{
	bool choise;
	cout << "0 - read file, 1 - console input" << endl;
	cin >> choise;
	if (choise)
		 ConsoleInput();	
	else
	{
		cin.clear();
		cin.ignore(cin.rdbuf()->in_avail());
		string filename;
		cout << "Enter filename" << endl;
		getline(cin, filename);
		wchar_t *w = nullptr;
		wstring ws = _wgetcwd(w, MAX_PATH);
		ReadFile(string(ws.begin(), ws.end()) + "\\" + filename);
	}
	vector<int> sol = Commivoyager();
	Print(sol);
}

void GeneticAlgo::Print(vector<int> x)
{
	cout << "( ";
	for (int i = 0; i < x.size(); i++)
		cout << x[i] << "  ";
	cout << ")" << endl;
}

void GeneticAlgo::ReadFile(string filename)
{
	ifstream fin(filename);
	if (!fin.is_open())
	{
		cout << "Could not open file." << endl;
		return;
	}
	fin >> seed;
	fin >> pointsNum;
	fin >> POPULATION_SIZE;
	fin >> CROSSOVER_PROBABILITY;
	fin >> MUTATION_PROBABILITY;
	fin >> MAX_GEN_NUM;
	fin >> MAX_UNCHANGED_GEN_NUM;

	points = new point[pointsNum];
	int i = 0;
	while (!fin.eof() && i < pointsNum)
	{
		fin >> points[i].x;
		fin >> points[i].y;
		i++;
	}
	if (i < pointsNum - 1)
		cout << "FILE: not enough points data, result can be unexpected.";
}

void GeneticAlgo::ConsoleInput()
{
	cout << "Enter seed: ";
	cin >> seed;
	cout << "Enter num of points: " << endl;
	cin >> pointsNum;
	point *points = new point[pointsNum];
	cout << "Enter points in format < x, y >: " << endl;
	for (int i = 0; i < pointsNum; i++)
	{
		cout << "< ";
		cin >> points[i].x;

		cout << " , ";
		cin >> points[i].y;
		cout << " >" << endl;
	}
	cout << endl;
	cout << "Enter population size: ";
	cin >> POPULATION_SIZE;
	cout << "Enter crossover probability: ";
	cin >> CROSSOVER_PROBABILITY;
	cout << "Enter mutation probability: ";
	cin >> MUTATION_PROBABILITY;
	cout << "Enter max number of generations: ";
	cin >> MAX_GEN_NUM;
	cout << "Enter max number of unchanged generations: ";
	cin >> MAX_UNCHANGED_GEN_NUM;
}


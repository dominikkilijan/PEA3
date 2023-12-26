#include "AdjacencyMatrix.h"
#include "Genetic.h"
#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <regex>
#include <sstream>
#include <cmath>

using namespace std;


//------------------------------------------------------------------------------------------------------------------------------------
AdjacencyMatrix::~AdjacencyMatrix()
{
	deleteAdjacencyMatrix();
}
//------------------------------------------------------------------------------------------------------------------------------------
// utworzenie macierzy o wymiarach N na N. Macierz wypelniona jest wartosciami -1
void AdjacencyMatrix::createAdjacencyMatrix()
{
	matrix = new int* [N];
	for (int i = 0; i < N; i++)
	{
		matrix[i] = new int[N];
		for (int j = 0; j < N; j++)
		{
			matrix[i][j] = -1;
		}
	}
}

//------------------------------------------------------------------------------------------------------------------------------------

void AdjacencyMatrix::deleteAdjacencyMatrix()
{
	if (matrix != nullptr)
	{
		for (int i = 0; i < N; i++)
		{
			delete[] matrix[i];
		}
		delete[] matrix;
		matrix = nullptr;
	}
}
//------------------------------------------------------------------------------------------------------------------------------------

// wypelnienie macierzy z podstawowego formatu
void AdjacencyMatrix::fillFromFile(fstream* file)
{
	if (file->is_open())
	{
		string line;
		while (getline(*file, line))
		{
			if (line.find("DIMENSION") != string::npos)
			{
				// odczytanie znakow w linii z DIMENSION
				std::string dimensionStr;
				for (char c : line) {
					if (isdigit(c))
					{
						dimensionStr += c;
					}
				}
				// do N wpisane zostaja cyfry czyli liczba wierzcholkow 
				N = stoi(dimensionStr);

				createAdjacencyMatrix();
			}
			else if (line.find("EDGE_WEIGHT_SECTION") != string::npos)
				break;
		}
		// uzupelnienie macietrzy
		int val;
		for (int i = 0; i < N; i++)
		{
			for (int j = 0; j < N; j++)
			{
				*file >> val;
				matrix[i][j] = val;
			}
		}
	}
	else cout << "Nie udalo sie otworzyc pliku! (w AdjancencyMatrix)\n";
}
//------------------------------------------------------------------------------------------------------------------------------------
void AdjacencyMatrix::fillFromFileXML(fstream* file)
{
	if (file->is_open())
	{
		string line;
		int i = -1;
		int j = 0;
		int linesCount = 0;

		// liczenie N
		while (getline(*file, line))
		{
			// zliczanie linii miedzy <vertex> a </vertex>
			if (line.find("<vertex>") != string::npos)
			{
				while (getline(*file, line) && line.find("</vertex>") == string::npos)
				{
					if (line.find("<edge cost=\"") != string::npos)
					{
						linesCount++;
					}
				}
				// wyjscie z petli bo wystarczy jedno liczenie
				break;
			}
		}

		N = linesCount;
		cout << "linesCount: " << linesCount << "\n";

		// powrot kursora na szczyt pliku
		file->clear();
		file->seekg(0, ios::beg);

		createAdjacencyMatrix();

		while (getline(*file, line))
		{
			// co <vertex> nowy rzad w matrix
			if (line.find("<vertex>") != string::npos) {
				i++;
				j = 0;
				while (getline(*file, line) && line.find("</vertex>") == string::npos)
				{
					if (line.find("<edge cost=\"") != string::npos)
					{
						// mantysa miedzy <edge cost=" i e+
						size_t costStartPos = line.find("<edge cost=\"") + 12;
						size_t costEndPos = line.find("e+", costStartPos);

						std::string costDoubleStr = line.substr(costStartPos, costEndPos - costStartPos);
						double costDouble = std::stod(costDoubleStr);

						// exponent miedzy e+ i "
						size_t exponentPos = costEndPos + 2;
						size_t exponentEndPos = line.find("\"", exponentPos);
						std::string exponentStr = line.substr(exponentPos, exponentEndPos - exponentPos);
						int exponent = std::stoi(exponentStr);

						// mantysa * exponent zeby dostac wartosc krawedzi
						int cost = static_cast<int>(costDouble * pow(10, exponent));
						//cout << "ij = " << i << "," << j << endl;
						//cout << "cost: " << cost << "\n";
						matrix[i][j] = cost;
						//cout << matrix[i][j] << "\n";
						j++;
					}
				}
			}
		}
	}
	else
	{
		cout << "Nie udalo sie otworzyc pliku! (w AdjacencyMatrix)\n";
	}
}
//------------------------------------------------------------------------------------------------------------------------------------
void AdjacencyMatrix::runAlgorithm(int stopTime, double startingPopulation, double mutation, double crossover, int crossoverChoice, int mutationChoice)
{

	Genetic evolution(N, matrix, stopTime, startingPopulation, mutation, crossover, crossoverChoice, mutationChoice);
	evolution.TSPGenetic();

}
//------------------------------------------------------------------------------------------------------------------------------------

void AdjacencyMatrix::printAdjacencyMatrix()
{
	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < N; j++)
		{
			cout << matrix[i][j] << "" << "\t";
		}
		cout << "\n";
	}
}
//------------------------------------------------------------------------------------------------------------------------------------
int** AdjacencyMatrix::getAdjacencyMatrix()
{
	return matrix;
}
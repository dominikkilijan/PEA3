#pragma once
#include <iostream>
#include <fstream>

using namespace std;

class FileHandler
{
public:
	~FileHandler();
	void openFile(string);
	double readPathFromFile(string);
	void runAlgorithm(int, double);
	void renameFile(string, string);

private:
	fstream file;
	int** matrix = nullptr;
};


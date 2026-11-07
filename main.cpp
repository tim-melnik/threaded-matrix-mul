// Multithreaded matrix multiplication

#include <iostream>
#include <string>
#include <fstream>

const unsigned int DIM = 3;

bool ReadMatrix(double (&matrix)[DIM][DIM], std::ifstream &fin);
bool WriteMatrix(double (&matrix)[DIM][DIM], std::ofstream &fout);

int main()
{
	double matA[DIM][DIM];
	double matB[DIM][DIM];

	std::string filename;

	std::cout << "Filename with matrices: ";
	std::cin >> filename;

	std::ifstream fin(filename);

	if (fin.is_open())
	{
		if (!ReadMatrix(matA, fin)) { return 1; }
		if (!ReadMatrix(matB, fin)) { return 1; }

		fin.close();
	}
	else
	{
		std::cerr << std::endl << "Error: Failed to read file" << std::endl;
		return 1;
	}

	return 0;
}


bool ReadMatrix(double (&matrix)[DIM][DIM], std::ifstream &fin)
{
	for (int i = 0; i < DIM; i++)
	{
		for (int j = 0; j < DIM; j++)
		{
			if (!(fin >> matrix[i][j]))
			{
				std::cerr << std::endl << "Error: Invalid data format" << std::endl;
				fin.close();
				return false;
			}
		}
	}
	return true;
}


bool WriteMatrix(double(&matrix)[DIM][DIM], std::ofstream& fout)
{ 
	return true;
}
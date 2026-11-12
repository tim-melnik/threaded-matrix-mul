// Multithreaded matrix multiplication

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <thread>
#include <functional>
#include <mutex>

const unsigned int DIM = 3;

bool ReadMatrix(double (&matrix)[DIM][DIM], std::ifstream &fin);
bool WriteMatrix(const double (&matrix)[DIM][DIM], std::ofstream &fout);

void ComputeColumn(const double matA[DIM][DIM], const double matB[DIM][DIM], double (&matRes)[DIM][DIM], int column);
void PrintMatrix(const double (&matrix)[DIM][DIM]);

std::mutex mtx;


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

	double matRes[DIM][DIM];

	std::vector<std::thread> threads;
	threads.reserve(DIM); // Each column is computed by a dedicated thread
	
	// Launching threads
	for (int i = 0; i < DIM; i++)
		threads.emplace_back(ComputeColumn, std::cref(matA), std::cref(matB), std::ref(matRes), i);

	// Waiting for threads to complete
	for (auto& th : threads)
	{
		if (th.joinable()) 
			th.join();
	}

	PrintMatrix(matRes);


	char ch;

	std::cout << std::endl << "Would you like to save the result to out.txt? [y/n]: ";
	std::cin >> ch;

	if ((ch == 'y') || (ch == 'Y'))
	{
		std::ofstream fout("out.txt");

		if (fout.is_open())
		{
			if (!WriteMatrix(matRes, fout)) { return 1; }
			fout.close();
		}
		else
		{
			std::cerr << std::endl << "Error: Failed to write file" << std::endl;
			return 1;
		}
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


bool WriteMatrix(const double(&matrix)[DIM][DIM], std::ofstream& fout)
{ 
	for (int i = 0; i < DIM; i++)
	{
		for (int j = 0; j < DIM; j++)
			fout << matrix[i][j] << " ";
		fout << std::endl;
	}

	if (!fout)
	{
		std::cerr << std::endl << "Error: Failed to write file" << std::endl;
		fout.close();
		return false;
	}

	return true;
}


void ComputeColumn(const double matA[DIM][DIM], const double matB[DIM][DIM], double(&matRes)[DIM][DIM], int column)
{
	for (int i = 0; i < DIM; i++)
	{
		double res = 0;

		for (int j = 0; j < DIM; j++)
			res += matA[i][j] * matB[j][column];

		std::scoped_lock lock(mtx); // Protecting matRes from a data race

		matRes[i][column] = res;
	}
}


void PrintMatrix(const double(&matrix)[DIM][DIM])
{
	std::cout << std::endl;

	for (int i = 0; i < DIM; i++)
	{
		for (int j = 0; j < DIM; j++)
			std::cout << matrix[i][j] << " ";
		std::cout << std::endl;
	}
}
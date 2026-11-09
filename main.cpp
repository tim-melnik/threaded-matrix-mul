// Multithreaded matrix multiplication

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <thread>
#include <mutex>

const unsigned int DIM = 3;

bool ReadMatrix(double (&matrix)[DIM][DIM], std::ifstream &fin);
bool WriteMatrix(double (&matrix)[DIM][DIM], std::ofstream &fout);

void ComputeColumn(double matA[DIM][DIM], double matB[DIM][DIM], double (&matRes)[DIM][DIM], int column);
void PrintMatrix(double (&matrix)[DIM][DIM]);

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

	//std::vector<std::thread> threads;
	//threads.reserve(DIM); // Each column is computed by a dedicated thread
	/*
	// Launching threads
	for (int i = 0; i < DIM; i++)
		threads.emplace_back(ComputeColumn, matA, matB, matRes, i);

	// Waiting for threads to complete
	for (auto& th : threads)
	{
		if (th.joinable()) 
			th.join();
	}
	*/
	//std::thread th1(ComputeColumn, matA, matB, matRes, 1);
	//std::thread th2(ComputeColumn, matA, matB, matRes, 2);
	//std::thread th3(ComputeColumn, matA, matB, matRes, 3);

	//th1.join();
	//th2.join();
	//th3.join();

	for (int i = 0; i < DIM; i++)
		ComputeColumn(matA, matB, matRes, i);


	PrintMatrix(matRes);

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


void ComputeColumn(double matA[DIM][DIM], double matB[DIM][DIM], double(&matRes)[DIM][DIM], int column)
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


void PrintMatrix(double(&matrix)[DIM][DIM])
{
	std::cout << std::endl;

	for (int i = 0; i < DIM; i++)
	{
		for (int j = 0; j < DIM; j++)
			std::cout << matrix[i][j] << " ";
		std::cout << std::endl;
	}
}
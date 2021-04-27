#include <iostream>
#include <fstream>

using namespace std;

// Loads data from a benchmark file and store it in A, nbJobs and nbMachines
void loader(int *nbJobs, int *nbMachines, int A[500][20])
{
    // TODO : enter the file path or number of jobs and machines as params
    std::ifstream file("../benchmarks/20jobs10machines.txt");

    // Check if file is open first
    if (file.is_open())
    {
        file >> *nbJobs >> *nbMachines;
        for (int i = 0; i < *nbMachines; i++)
        {
            for (int j = 0; j < *nbJobs; j++)
            {
                file >> A[j][i];
            }
        }
    }
    else
    {
        std::cout << "File doesn't exist" << std::endl;
    }
}
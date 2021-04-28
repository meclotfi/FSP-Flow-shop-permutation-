#include <iostream>
#include <fstream>
#include <algorithm>
#include <map>
#include <vector>
#include <numeric>

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


// Calculate the makespan of a solution
// solution: a sequence of jobs
// A: the matrix of jobs and machines 
// nbMachines: the number of machines
int Cmax(vector<int> solution, int A[500][20], int nbMachines)
{
    if (solution.size() == 0 || nbMachines == 0)
    {
        return 0;
    }
    else
    {
        vector<int> v(solution.begin(), solution.end() - 1);
        int a, b;
        a = Cmax(solution, A, nbMachines - 1);
        b = Cmax(v, A, nbMachines);
        // std::cout << "a :" << a << std::endl;
        // std::cout << "b :" << b << std::endl;
        if (a >= b)
        {
            return a + A[solution.at(solution.size() - 1)][nbMachines - 1];
        }
        else
        {
            return b + A[solution.at(solution.size() - 1)][nbMachines - 1];
        }
    }
}
#include <iostream>
#include <fstream>
#include <algorithm>
#include <map>
#include <vector>
#include <numeric>
#include <string>

using namespace std;

// Loads data from a benchmark file and store it in A, nbJobs and nbMachines
void loader(string filepath, int *nbJobs, int *nbMachines, int A[500][20])
{
    // TODO : enter the file path or number of jobs and machines as params
    std::ifstream file(filepath);

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
 int Cmaxt(vector<int> solution, int A[500][20], int nbMachines)
 {
     if (solution.size() == 0 || nbMachines == 0)
     {
         return 0;
     }
     else
     {
         vector<int> v(solution.begin(), solution.end() - 1);
         int a, b;
         a = Cmaxt(solution, A, nbMachines - 1);
         b = Cmaxt(v, A, nbMachines);
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

// Calculate the makespan of a solution with dynamic programming
// solution: a sequence of jobs
// A: the matrix of jobs and machines
// nbMachines: the number of machines
int Cmax(vector<int> solution, int A[500][20], int nbMachines)
{
    //vector<vector<int>> matrix;
    int nbJobs = solution.size();
    if (nbJobs == 0 || nbMachines == 0)
    {
        return 0;
    }
    else
    {
        int matrix[nbJobs + 1][nbMachines + 1];
        for (int j = 0; j <= nbMachines; j++)
            matrix[0][j] = 0;

        for (int i = 0; i <= nbJobs; i++)
            matrix[i][0] = 0;

        for (int i = 1; i <= nbJobs; i++)
        {
            for (int j = 1; j <= nbMachines; j++)
            {
                matrix[i][j] = A[solution[i - 1]][j - 1]; // because btw 0-19
                matrix[i][j] += (matrix[i - 1][j] < matrix[i][j - 1]) ? matrix[i][j - 1] : matrix[i - 1][j];
            }
        }
        return matrix[nbJobs][nbMachines];
    }
}

int eval(int CActuel, int A[500][20], int nbMachines, list<int> J)
{
    int C = CActuel;
    for (auto &it : J)
        C += A[it][nbMachines - 1];
    return C;
}

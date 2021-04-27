#include <iostream>
#include <fstream>
using namespace std;

//TODO: make this file a header 

int main()
{
    int nbJobs, nbMachines;

    // TODO : enter the file path or number of jobs and machines as params
    std::ifstream file("benchmarks/200jobs10machines.txt");

    // Check if file is open first
    if (file.is_open())
    {
        file >> nbJobs >> nbMachines;

        int A[nbJobs][nbMachines];

        for (int i = 0; i < nbMachines; i++)
        {
            for (int j = 0; j < nbJobs; j++)
            {
                file >> A[j][i];
            }
        }

        // Just to display the matrix
        for (int i = 0; i < nbJobs; i++)
        {
            for (int j = 0; j < nbMachines; j++)
            {   
                std::cout << A[i][j] << " ";
            }
            std::cout << "\n"
                      << std::endl;
        }
    } else
    {
        std::cout << "File doesn't exist" << std::endl;
    }
    
}
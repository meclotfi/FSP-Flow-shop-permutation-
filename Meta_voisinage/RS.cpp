#include "../FSP.h"


int main()
{
    int nbJobs, nbMachines, somme, A[500][20], cmax;
    vector<int> s0, solution;
    clock_t start, end;

    string filepath = "../benchmarks/13J_5M.txt";

    loader(filepath, &nbJobs, &nbMachines, A);
    NEH(A, nbJobs, nbMachines, cmax, solution);
    std::cout << "Neh solution" << std::endl;
    for (auto var : solution)
    {
        std::cout << var << " ";
    }
    s0 = solution;
    std::cout << "\nCmax: " << cmax << std::endl;

    start = clock();
    RS(A, nbJobs, nbMachines, s0, .8, 100, 20, 100000,solution, cmax);

    end = clock();
    double time_taken = double(end - start) / double(CLOCKS_PER_SEC);
    printf (" \n RS %f secondes\n\n", time_taken);

    std::cout << "Meta solution :" << std::endl;
    for (auto var : solution)
    {
        std::cout << var << " ";
    }
    std::cout << "\nCmax: " << cmax << std::endl;

}
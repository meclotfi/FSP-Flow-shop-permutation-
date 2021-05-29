#include "../FSP.h"

void NEH(int A[500][20], int nbJobs, int nbMachines, int &cmax, vector<int> &solution)
{
    int somme, c, currentCmax;
    vector<int> currentSequence, saveSol;

    vector<pair<int, int> > sommeLigne;

    // Get the sum of each line ( duration of each job in all machines )
    for (int i = 0; i < nbJobs; i++)
    {
        somme = 0;
        somme = accumulate(A[i], A[i] + nbMachines, somme);
        sommeLigne.push_back(std::make_pair(i, somme));
    }

    // Sort in descending order the sums
    sort(sommeLigne.begin(), sommeLigne.end(), cmp2);

    // Push the first jobs in the solution sequence
    solution.push_back(sommeLigne[0].first);

    // add a jobs at a time
    for (size_t i = 1; i < sommeLigne.size(); i++)
    {
        c = INT32_MAX;
        currentSequence = solution;
        saveSol = currentSequence;

        // Compare all the combinations
        for (int j = 0; j < i + 1; j++)
        {

            currentSequence.insert(currentSequence.begin() + j, sommeLigne[i].first);
            currentCmax = Cmax(currentSequence, A, nbMachines);

            // compare the makespans
            if (currentCmax <= c)
            {
                solution = currentSequence;
                c = currentCmax;
            }
            currentSequence = saveSol;
        }
    }
    cmax=Cmax(solution, A, nbMachines);
    // for (auto var : solution)
    // {
    //     std::cout << var + 1 << " ";
    // }

    // std::cout << "" << std::endl;
    // std::cout << "Cmax : " << Cmax(solution, A, nbMachines) << std::endl;
}

int main222(int argc, char const *argv[])
{
    int nbJobs, nbMachines, A[500][20], cmax; 
    vector<int> solution;

    clock_t start, end;


    loader("../benchmarks/11J_5M.txt", &nbJobs, &nbMachines, A);
    start = clock();
    NEH(A, nbJobs, nbMachines, cmax, solution);
    end = clock();
    double temps2 = double(end - start) / double(CLOCKS_PER_SEC);
    printf (" \n CDS time %f secondes\n", temps2);
    
    
    cout <<"Sequence" << endl;
    for(int i = 0; i < nbJobs; i++)
       {
           cout << solution[i] << endl;
       }
       cout << "Cmax : " << cmax <<endl;

    return 0;
        
}
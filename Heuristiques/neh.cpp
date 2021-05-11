#include "../FSP.h"

int main(int argc, char const *argv[])
{
    int nbJobs, nbMachines, somme, A[500][20], c, currentCmax;
    vector<int> solution, currentSequence, saveSol;

    vector<pair<int, int>> sommeLigne;
    loader("../benchmarks/20jobs10machines.txt", &nbJobs, &nbMachines, A);

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
            currentCmax = Cmax(solution, A, nbMachines);

            // compare the makespans
            if (currentCmax <= c)
            {
                solution = currentSequence;
                c = currentCmax;
            }
            currentSequence = saveSol;
        }
    }

    for (auto var : solution)
    {
        std::cout << var + 1 << " ";
    }
    std::cout << "" << std::endl;
    std::cout << "Cmax : " << Cmax(solution, A, nbMachines) << std::endl;
    return 0;
}
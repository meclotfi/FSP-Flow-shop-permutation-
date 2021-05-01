#include "../FSP.h"

// comparaison function to sort vectors in ascending order
bool cmp(pair<int, int> &a,
         pair<int, int> &b)
{
    return a.second < b.second;
}

int main()
{
    // Params
    // nbJobs : nember of jobs | nbMachines : nember of machines | somme : sum of a row
    // A : matrix of jobs and machines
    // Sa :  list of jobs / Ti1 <= Tim
    // Sb : list of jobs / Ti1 > Tim
    // solution : the solution vector
    int nbJobs, nbMachines, somme, A[500][20], c;
    vector<pair<int, int>> Sa, Sb;
    vector<int> solution;

    //load nbJobs, nbMachines and the matrix A
    //TODO : add the file path as params
    loader("../benchmarks/20jobs10machines.txt", &nbJobs, &nbMachines, A);

    // sommeLigne : verctor of sums of jobs in all machines
    int sommeLigne[nbJobs];

    // Uncomment to print A
    // for (int i = 0; i < nbJobs; i++)
    // {
    //     for (int j = 0; j < nbMachines; j++)
    //     {
    //         std::cout << A[i][j] << " ";
    //     }
    //     std::cout << "\n"
    //               << std::endl;
    // }

    // Fill the sommeLigne vector
    for (int i = 0; i < nbJobs; i++)
    {
        somme = 0;
        somme = accumulate(A[i], A[i] + nbMachines, somme);
        // Uncomment to see the sums
        // std::cout << somme << std::endl;
        sommeLigne[i] = somme;
    }

    // get the c index / see the Chen heuristique
    c = find(sommeLigne, sommeLigne + nbJobs, *(std::max_element(sommeLigne, sommeLigne + nbJobs))) - sommeLigne;

    // Fill Sa and Sb
    for (int i = 0; i < nbJobs; i++)
    {
        if (i == c)
            continue;
        if (A[i][0] <= A[i][nbMachines - 1])
            // Add i to Sa
            Sa.push_back(std::make_pair(i, A[i][0]));
        else
            // Add i to Sb
            Sb.push_back(std::make_pair(i, A[i][0]));
    }

    // Sort Sa
    sort(Sa.begin(), Sa.end(), cmp);

    // Sort Sb
    sort(Sb.begin(), Sb.end(), cmp);

    // std::cout << "Sa" << std::endl;
    // Add Sa to the solution (we increment by one because index of matrix starts in 0)
    for (auto &it : Sa)
    {
        // Uncomment to see Sa
        // cout << it.first << ' '
        //      << it.second << endl;

        solution.push_back(it.first + 1);
    }

    // Add c to solution
    solution.push_back(c + 1);

    // std::cout << "Sb" << std::endl;
    // Add Sb to the solution (we increment by one because index of matrix starts in 0)
    for (auto &it : Sb)
    {
        //Uncomment to see Sb
        // cout << it.first << ' '
        //      << it.second << endl;

        solution.push_back(it.first + 1);
    }

    // Show the solution
    for (auto &it : solution)
    {
        std::cout << it << " ";
    }

    std::cout << "" << std::endl;

    // Show the makspan Cmax
    std::cout << "Temps Cmax :" << Cmax(solution, A, nbMachines) << std::endl;
}
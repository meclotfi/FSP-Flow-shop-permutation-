#include "../FSP.h"

void Chen(int A[500][20], int nbJobs, int nbMachines, int &cmax, vector<int> &solution)
{
    // Params
    // nbJobs : nember of jobs | nbMachines : nember of machines | somme : sum of a row
    // A : matrix of jobs and machines
    // Sa :  list of jobs / Ti1 <= Tim
    // Sb : list of jobs / Ti1 > Tim
    // solution : the solution vector
    //int somme, A[500][20],c;
    int somme,c; //Removed A because it a function parameter

    vector<pair<int, int> > Sa, Sb;

    //load nbJobs, nbMachines and the matrix A
    //TODO : add the file path as params

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
            Sb.push_back(std::make_pair(i, A[i][nbMachines-1]));
    }

    // Sort Sa
    sort(Sa.begin(), Sa.end(), cmp);

    // Sort Sb
    sort(Sb.begin(), Sb.end(), cmp2);

    // std::cout << "Sa" << std::endl;
    // Add Sa to the solution (we increment by one because index of matrix starts in 0)
    for (auto &it : Sa)
    {
        // Uncomment to see Sa
        // cout << it.first << ' '
        //      << it.second << endl;

        solution.push_back(it.first);
    }

    // Add c to solution
    solution.push_back(c);

    // std::cout << "Sb" << std::endl;
    // Add Sb to the solution (we increment by one because index of matrix starts in 0)
    for (auto &it : Sb)
    {
        //Uncomment to see Sb
        // cout << it.first << ' '
        //      << it.second << endl;

        solution.push_back(it.first);
    }
    cmax=Cmax(solution, A, nbMachines);
    // Show the solution
    // for (auto &it : solution)
    // {
    //     std::cout << it << " ";
    // }

    // std::cout << "" << std::endl;

    // // Show the makspan Cmax
    // std::cout << "Temps Cmax :" << Cmax(solution, A, nbMachines) << std::endl;
}


int main22(int argc, char const *argv[])
{
    int nbJobs, nbMachines, A[500][20], cmax; 
    vector<int> solution;

    clock_t start, end;


    loader("../benchmarks/11J_5M.txt", &nbJobs, &nbMachines, A); 
    start = clock();
    Chen(A, nbJobs, nbMachines, cmax, solution);
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
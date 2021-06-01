#include <iostream>
#include <fstream>
#include <algorithm>
#include <map>
#include <vector>
#include <numeric>
#include <string>
#include <iterator>
#include <stdexcept>
#include <list>
#include "string.h"
#include "omp.h"
#include <random>
#include <ctime>   // For time()
#include <cstdlib> // For srand() and rand()
#include <thread>  // exact
#include <chrono>  // exact
#include <cmath>   // exact
#define CHUNKSIZE 1

using namespace std;


/*********************************** Fonctions globales ***********************************/
static bool cmp(pair<int, int> &a,
                pair<int, int> &b)
{
    return a.second < b.second;
}
static bool cmp2(pair<int, int> &a,
                 pair<int, int> &b)
{
    return a.second > b.second;
}

// Loads data from a benchmark file and store it in A, nbJobs and nbMachines
static void loader(string filepath, int *nbJobs, int *nbMachines, int A[500][20])
{
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

/*********************************** Fonctions Cmax ***********************************/

// Calculate the makespan of a solution
// solution: a sequence of jobs
// A: the matrix of jobs and machines
// nbMachines: the number of machines
static int Cmaxt(vector<int> solution, int A[500][20], int nbMachines)
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
static int Cmax(vector<int> solution, int A[500][20], int nbMachines)
{
    //vector<vector<int> > matrix;
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

// if newJob = -1 : 'L' contains a list of jobs whose Cmax is to be computed
// else, 'L' contains the exection times of the last job on all the machines ( + '0' in the first position)
// OUTPUT: a vector of the newJob execution, with same structure as L
// NOTE: Please access to the Cmax using: res[nbMachines].
static vector<int> Cmax_Add_Job(vector<int> L, int newJob, int A[500][20], int nbMachines)
{
    if (nbMachines == 0)
        return vector<int>(); // ERROR!

    vector<int> res(nbMachines + 1);
    if (newJob == -1) // normal execution of Cmax but returning the last line of the matrix.
    {
        int nbJobs = L.size();
        int matrix[nbJobs + 1][nbMachines + 1];
        for (int j = 0; j <= nbMachines; j++)
            matrix[0][j] = 0;

        for (int i = 0; i <= nbJobs; i++)
            matrix[i][0] = 0;

        for (int i = 1; i <= nbJobs; i++)
        {
            for (int j = 1; j <= nbMachines; j++)
            {
                matrix[i][j] = A[L[i - 1]][j - 1]; // because btw 0-19
                matrix[i][j] += (matrix[i - 1][j] < matrix[i][j - 1]) ? matrix[i][j - 1] : matrix[i - 1][j];
            }
        }

        for (int j = 0; j <= nbMachines; j++)
        {
            res[j] = matrix[nbJobs][j];
        }
        return res;
    }
    else
    {
        res[0] = 0;
        for (int j = 1; j <= nbMachines; j++)
        {
            res[j] = A[newJob][j - 1]; // we suppose jobs start from 0
            res[j] += (L[j] < res[j - 1]) ? res[j - 1] : L[j];
        }
        return res;
    }
}

/*********************************** CDS ***********************************/
static vector<pair<int, int>> U(int A[500][20], int nbJobs)
{
    vector<pair<int, int>> my_u;
    for (int j = 0; j < nbJobs; j++)
    {
        if (A[j][0] < A[j][1])
        {
            my_u.push_back(std::make_pair(j, A[j][0]));
        }
    }
    sort(my_u.begin(), my_u.end(), cmp);
    return my_u;
}

static vector<pair<int, int>> V(int A[500][20], int nbJobs)
{
    vector<pair<int, int>> my_u;
    for (int j = 0; j < nbJobs; j++)
    {
        if (A[j][0] >= A[j][1])
        {
            my_u.push_back(std::make_pair(j, A[j][1]));
        }
    }
    sort(my_u.begin(), my_u.end(), cmp2);
    return my_u;
}

void johnson(int A[500][20], int nbJobs, int &cmax, vector<int> &solution, int data[500][20])
{
    vector<pair<int, int>> my_u;
    my_u = U(A, nbJobs);
    vector<pair<int, int>> my_u2;
    my_u2 = V(A, nbJobs);
    my_u.insert(my_u.end(), my_u2.begin(), my_u2.end());
    for (int i = 0; i < nbJobs; i++)
    {
        solution.push_back(my_u[i].first);
    }
    /*
    for (&it :solution)
    {
        cout<<it<<"\n";
    }*/
    //On en a pas besoin dans cds, si vous en avez besoin ailleur decommenter la
    cmax = 99999;
    //cmax=Cmax(solution, data, 2);
}

void johnson_Cmax(int A[500][20], int nbJobs, int &cmax, vector<int> &solution, int data[500][20])
{
    vector<pair<int, int>> my_u;
    my_u = U(A, nbJobs);
    vector<pair<int, int>> my_u2;
    my_u2 = V(A, nbJobs);
    my_u.insert(my_u.end(), my_u2.begin(), my_u2.end());
    for (int i = 0; i < nbJobs; i++)
    {
        solution.push_back(my_u[i].first);
    }
    cmax = Cmax(solution, data, 2);
}

void CDS(int A[500][20], int nbJobs, int nbMachines, int &cmax, vector<int> &solution)
{
    //Compare m-1 sequences de nbJobs 2 machines avec johnson et prend le meilleur Cmax

    int Seq[500][20], somme1 = 0, somme2 = 0, c;
    vector<int> solutionsauv;

    cmax = INT32_MAX;

    for (int k = 0; k < (nbMachines - 1); k++) // M-1 sequences
    {
        //construire tableau seq de nbJobs 2 machines
        for (int i = 0; i < nbJobs; i++)
        {
            for (int j = 0; j <= k; j++)
            {
                somme1 = somme1 + A[i][j];
            }
            for (int j = nbMachines - 1; j >= nbMachines - 1 - k; j--)
            {
                somme2 = somme2 + A[i][j];
            }
            Seq[i][0] = somme1;
            Seq[i][1] = somme2;
            somme1 = 0;
            somme2 = 0;
        }

        //lui appliquer johnson
        solutionsauv.clear();
        johnson(Seq, nbJobs, c, solutionsauv, A);
        //recalculer le c (cmax) de la sequence retournee par johnson sur nbmachines
        c = Cmax(solutionsauv, A, nbMachines);

        //mettre a jour Cmax si necessaire
        if (c < cmax)
        {
            cmax = c;
            solution = solutionsauv;
        }
    }
}

/*********************************** NEH ***********************************/

void NEH(int A[500][20], int nbJobs, int nbMachines, int &cmax, vector<int> &solution)
{
    int somme, c, currentCmax;
    vector<int> currentSequence, saveSol;

    vector<pair<int, int>> sommeLigne;

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
    cmax = Cmax(solution, A, nbMachines);
    // for (auto var : solution)
    // {
    //     std::cout << var + 1 << " ";
    // }

    // std::cout << "" << std::endl;
    // std::cout << "Cmax : " << Cmax(solution, A, nbMachines) << std::endl;
}

/*********************************** Chen ***********************************/

void Chen(int A[500][20], int nbJobs, int nbMachines, int &cmax, vector<int> &solution)
{
    // Params
    // nbJobs : nember of jobs | nbMachines : nember of machines | somme : sum of a row
    // A : matrix of jobs and machines
    // Sa :  list of jobs / Ti1 <= Tim
    // Sb : list of jobs / Ti1 > Tim
    // solution : the solution vector
    //int somme, A[500][20],c;
    int somme, c; //Removed A because it a function parameter

    vector<pair<int, int>> Sa, Sb;

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
            Sb.push_back(std::make_pair(i, A[i][nbMachines - 1]));
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
    cmax = Cmax(solution, A, nbMachines);
    // Show the solution
    // for (auto &it : solution)
    // {
    //     std::cout << it << " ";
    // }

    // std::cout << "" << std::endl;

    // // Show the makspan Cmax
    // std::cout << "Temps Cmax :" << Cmax(solution, A, nbMachines) << std::endl;
}

/*********************************** Methode Exactes ***********************************/

static int eval(int CActuel, int A[500][20], int nbMachines, list<int> J)
{
    int C = CActuel;
    for (auto &it : J)
        C += A[it][nbMachines - 1];
    return C;
}

static int eval_max(int CActuel, int A[500][20], vector<int> LbMachines, list<int> J)
{
    int C = 0, max_c = 0;
    int nbMachines = LbMachines.size() - 1;
    for (int i = 0; i < nbMachines; i++)
    {
        C = LbMachines[i + 1];

        for (auto &it : J)
            C += A[it][i];
        if (max_c < C)
            max_c = C;
    }
    return max_c;
}

void DFS(list<int> S, list<int> J, int &M, int nbMachines, int A[500][20], vector<int> cmax_vec, vector<int> &solution)
{
    list<int> J1, S1;
    int eva;
    int cost;
    vector<int> cost2;

    for (auto &it : J)
    {
        S1 = S;
        S1.push_back(it);
        J1 = J;
        J1.remove(it);
        // calculer le cout
        cost2 = Cmax_Add_Job(cmax_vec, S1.back(), A, nbMachines);
        cost = cost2.back();
        // tester si feuille et caluler l'evaluation sinon
        if (J1.empty())
        {
            //cout<<"cmax vec size= \n"<< cmax_vec.size();

            //cost2 = Cmax_Add_Job(cmax_vec,-1,A,nbMachines);
            //cost =cost2.back();
            //if(cost2.back()==cost) cout<<"accept \n";

            //update lower bound
            if (M > cost)
            {
                //cout << "M= " << M << " ";
                solution.clear();
                copy(S1.begin(), S1.end(), back_inserter(solution));
                M = cost;
                //cout << "update M= " << M << " ";
            }
        }
        else
        {
            //calculer l'evaluation:
            eva = eval(cost, A, nbMachines, J1);

            //tester si possible d'elager
            // elagage

            if (eva < M)
            {
                DFS(S1, J1, M, nbMachines, A, cost2, solution);
            }
            //else cout<<" elagage avec eval = "<<eva<<" et M= "<<M<<"\n";
            // brunshing
        }
        //cout << "back \n";
    }
}

int BB(int nbJobs, int nbMachines, int A[500][20])
{

    int M = INT32_MAX;
    int somme = 0;
    list<int> J, S;
    vector<int> solution;

    //initiaalization des structure de données
    //J: l'enssemble des job non encore assigner
    vector<pair<int, int>> sommeLigne;
    pair<int, int> c;

    for (int i = 0; i < nbJobs; i++)
    {
        c.first = i;
        c.second = accumulate(A[i], A[i] + nbMachines, somme);
        sommeLigne.push_back(c);
    }
    sort(sommeLigne.begin(), sommeLigne.end(), cmp);

    for (int j = 0; j < nbJobs; j++)
    {
        J.push_back(sommeLigne[j].first);
    }

    vector<int> cmax_vec;
    for (int j = 0; j <= nbMachines; j++)
    {
        cmax_vec.push_back(0);
    }

    DFS(S, J, M, nbMachines, A, cmax_vec, solution);

    std::cout << "Solution finale: ";
    for (auto var : solution)
    {
        std::cout << var << " ";
    }
    std::cout << "" << std::endl;
    cout << "final M= " << M;
    return M;
}

void show_progress_bar(float prog, int j, int LB)
{

    int barWidth = 30;

    std::cout << "[";
    int pos = barWidth * prog;
    for (int i = 0; i < barWidth; ++i)
    {
        if (i < pos)
            std::cout << "=";
        else if (i == pos)
            std::cout << ">";
        else
            std::cout << " ";
    }
    std::cout << "] " << int(prog * 100.0) << " % ";
    cout << "finished exploring branch J" << j << " current lower bound is : " << LB;
    std::cout.flush();

    std::cout << std::endl;
}

void DFS_par(list<int> S, list<int> J, int &M, int nbMachines, int A[500][20], vector<int> cmax_vec, list<int> &solution)
{

    list<int> J1, S1;
    int eva;
    int cost;
    vector<int> cost2;
    for (auto &it : J)
    {

        S1 = S;
        S1.push_back(it);
        J1 = J;
        J1.remove(it);
        // calcul cout
        cost2 = Cmax_Add_Job(cmax_vec, S1.back(), A, nbMachines);
        cost = cost2.back();
        //cout<<" cost="<<cost<<"\n";
        //if(S1.size()==10) for(auto &it:S1) cout<<" S" <<it;cout<<"\n";

        // tester si feuille et caluler l'evaluation sinon
        if (J1.empty())
        {

            //update lower bound
            if (M > cost)
            {
//S=solution
#pragma omp critical
                {
                    M = cost;
                    solution = S1;
                }
            }
        }
        else
        {

            //calculer l'evaluation:
            //eva=eval(cost, A, nbMachines, J1);
            //cout<<"eval: "<<eval(cost, A, nbMachines, J1)<<" eval max:"<<eval_max(cost, A, cost2, J1)<<"\n";
            eva = eval_max(cost, A, cost2, J1);
            //int mi=*min_element(cost2.begin(), cost2.end());
            //eva=eval_jhonson(mi, A,cost2, J1);

            //tester si possible d'elager
            // elagage

            if (eva < M)
            {
                DFS_par(S1, J1, M, nbMachines, A, cost2, solution);
            }
            //else //cout<<" elagage avec eval = "<<eva<<" et M= "<<M<<"\n";
        }
    }
}

int Parallel_BB(int nbJobs, int nbMachines, int A[500][20])
{

    int M = INT32_MAX;
    int somme;
    vector<int> J, S;
    list<int> solution;

    //initiaalization des structure de données
    //J: l'enssemble des job non encore assigner
    vector<pair<int, int>> sommeLigne;
    pair<int, int> c;

    for (int i = 0; i < nbJobs; i++)
    {
        c.first = i;
        c.second = accumulate(A[i], A[i] + nbMachines, somme);
        sommeLigne.push_back(c);
    }
    sort(sommeLigne.begin(), sommeLigne.end(), cmp);

    for (int j = 0; j < nbJobs; j++)
    {
        J.push_back(sommeLigne[j].first);
    }

    //std::list<int>::iterator it=J1.begin();
    vector<int> cmax_vec;
    for (int j = 0; j <= nbMachines; j++)
    {
        cmax_vec.push_back(0);
    }
    vector<int> cost;

    int j = 0;

#pragma omp parallel for schedule(dynamic, CHUNKSIZE) firstprivate(cmax_vec, cost) shared(M, solution)
    for (int i = 0; i < nbJobs; i++)
    {
        //printf("> Thread %d is exploring branch J%d \n", omp_get_thread_num(),J[i]);

        list<int> J1(J.begin(), J.end());
        list<int> S1;
        S1.push_back(J[i]);
        list<int> Jt = J1;
        J1.remove(J[i]);

        cost = Cmax_Add_Job(cmax_vec, S1.back(), A, nbMachines);
        DFS_par(S1, J1, M, nbMachines, A, cost, solution);
#pragma omp atomic write
        j = j + 1;
        int f = S1.back();

// commented this because of parallelisme
#pragma omp critical
        {
            show_progress_bar(((float)j / (float)nbJobs), f, M);
        }
    }

    std::cout << "\n Best Sequence : ";
    for (auto var : solution)
    {
        cout << "J" << var << " ";
    }
    cout << "" << std::endl;
    cout << "\tMakeSpan :" << M;
    return M;
}

int Parallel_BB_hybride(int nbJobs, int nbMachines, int A[500][20])
{

    int M = INT32_MAX;

    int somme;
    vector<int> J, S;
    list<int> solution;

    vector<int> SOL;

    CDS(A, nbJobs, nbMachines, M, SOL);

    //initiaalization des structure de données
    //J: l'enssemble des job non encore assigner
    vector<pair<int, int>> sommeLigne;
    pair<int, int> c;

    for (int i = 0; i < nbJobs; i++)
    {
        c.first = i;
        c.second = accumulate(A[i], A[i] + nbMachines, somme);
        sommeLigne.push_back(c);
    }
    sort(sommeLigne.begin(), sommeLigne.end(), cmp);

    for (int j = 0; j < nbJobs; j++)
    {
        J.push_back(sommeLigne[j].first);
    }

    //std::list<int>::iterator it=J1.begin();
    vector<int> cmax_vec;
    for (int j = 0; j <= nbMachines; j++)
    {
        cmax_vec.push_back(0);
    }
    vector<int> cost;

    int j = 0;

#pragma omp parallel for schedule(dynamic, CHUNKSIZE) firstprivate(cmax_vec, cost) shared(M, solution)
    for (int i = 0; i < nbJobs; i++)
    {
        //printf("> Thread %d is exploring branch J%d \n", omp_get_thread_num(),J[i]);

        list<int> J1(J.begin(), J.end());
        list<int> S1;
        S1.push_back(J[i]);
        list<int> Jt = J1;
        J1.remove(J[i]);
        cost = Cmax_Add_Job(cmax_vec, S1.back(), A, nbMachines);
        DFS_par(S1, J1, M, nbMachines, A, cost, solution);
#pragma omp atomic write
        j = j + 1;
        int f = S1.back();

        // commented this because of parallelisme
        // #pragma omp critical
        // {
        //     show_progress_bar(((float)j/(float)nbJobs),f,M);
        // }
    }

    std::cout << "\n Best Sequence: ";
    for (auto var : solution)
    {
        cout << "J" << var << " ";
    }
    std::cout << "" << std::endl;
    cout << "\tMakeSpan: " << M;
    return M;
}

/*********************************** Johnson ***********************************/
static int Jon(int A[500][20], int nbJobs, int data[500][20])
{
    vector<int> solution;
    int cmax;
    vector<pair<int, int>> my_u;
    my_u = U(A, nbJobs);
    vector<pair<int, int>> my_u2;
    my_u2 = V(A, nbJobs);
    my_u.insert(my_u.end(), my_u2.begin(), my_u2.end());
    for (int i = 0; i < nbJobs; i++)
    {
        solution.push_back(my_u[i].first);
    }
    /*
    for (&it :solution)
    {
        cout<<it<<"\n";
    }*/
    cmax = Cmax(solution, data, 2);
    return cmax;
}

static int eval_jhonson(int CActuel, int A[500][20], vector<int> LbMachines, list<int> J)
{
    int nbMachines = LbMachines.size() - 1;
    int nbJobs = J.size(), cmax_j = INT32_MAX;
    int Seq[500][20], somme1 = 0, somme2 = 0, c;

    for (int k = 0; k < (nbMachines - 1); k++) // M-1 sequences
    {
        //construire tableau de nbJobs 2 machines
        for (int i = 0; i < nbJobs; i++)
        {
            for (int j = 0; j <= k; j++)
            {
                somme1 = somme1 + A[i][j];
            }
            for (int j = nbMachines - 1; j >= nbMachines - 1 - k; j--)
            {
                somme2 = somme2 + A[i][j];
            }
            Seq[i][0] = somme1;
            Seq[i][1] = somme2;
            somme1 = 0;
            somme2 = 0;
        }

        //lui appliquer johnson
        c = 0;

        int c = Jon(Seq, nbJobs, A);

        //mettre a jour Cmax si necessaire
        if (c < cmax_j)
        {
            cmax_j = c;
        }
    }
    return CActuel + cmax_j;
}

/*********************************** Recuit simulé ***********************************/
vector<int> voisin(vector<int> s) // copied to evoid including stuff
{
    int a, b;
    vector<int> x = s;
    a = rand() % s.size();
    b = rand() % s.size();
    while (b == a)
        b = rand() % s.size();
        // b = b + 1 % ... generates a sequence like Adjecent .. a lot 

//    iter_swap(x.begin() + a, x.begin() + b);
    int inter = x[a];
    x[a] = x[b];
    x[b] = inter;
    return x;
}

// Params:
// s0: solution initiale
// T: temperature
// nb_it_pl: nombre d'itération par palier
// nb_arret: nombre de palier totale, considéré comme critere d'arret
// solution: solution finale du recuit simulé
// cmax: cmax de la solution finale 
void RS(int A[500][20], int nbJobs, int nbMachines, vector<int> s0, float alpha, float T, int nb_it_pl, int nb_arret, vector<int> &solution, int &cmax)
{
    int cpmax, delta;
    float u;
    vector<int> x;
    solution = s0;
    cmax = Cmax(solution, A, nbMachines);

    for (size_t j = 0; j < nb_arret; j++)
    {
        srand(time(0));
        for (int i = 0; i < nb_it_pl; ++i)
        {
            x = voisin(solution);
            cpmax = Cmax(x, A, nbMachines);
            delta = cpmax - cmax;
            if (delta < 0)
            {
                solution = x;
                cmax = cpmax;
            }
            else
            {
                u = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
                if (u < exp(delta / T))
                {
                    solution = x;
                    cmax = cpmax;
                }
            }
        }
        T *= alpha;
    }
}
/*********************************** Recherche Tabou Avec Voisinage Adjacent ***********************************/

vector<vector<int>> AdjacentExchange(vector<int> Sol)
{
    vector<vector<int>> Neighborhood;
    int inter;
    int nbJobs = Sol.size();
    for (int i = 0; i < nbJobs - 1; i++)
    {
        Neighborhood.push_back(Sol);
        inter = Neighborhood[i][i];
        Neighborhood[i][i] = Neighborhood[i][i + 1];
        Neighborhood[i][i + 1] = inter;
    }

    return Neighborhood;
}

bool notTabu(vector<int> S, vector<vector<int>> LT)
{
    if (LT.empty())
        return true;
    int size = LT.size();
    for (int i = 0; i < size; i++)
    {
        if (LT[i] == S)
            return false;
    }
    return true;
}

// stop = Pourcentage to stop the same score, from the nbJobs ... by default,
void RT(int A[500][20], int nbJobs, int nbMachines, char Method, int LT_MAX_SIZE, int stop, int &cmax, vector<int> &solution)
{
    int M = INT32_MAX;
    int Act;
    int Mpreq = INT32_MAX;
    int Boucle = 0;
    vector<vector<int>> Neighborhood;
    vector<int> S, Smax, T, Spreq;
    int T_cmax = 0;
    if (Method == 'C')
        CDS(A, nbJobs, nbMachines, M, S);
    else if (Method == 'N')
        NEH(A, nbJobs, nbMachines, M, S);
    else
        Chen(A, nbJobs, nbMachines, M, S);
    Smax = S;
    int i = 0;

    int it = 0;

    vector<vector<int>> LT(LT_MAX_SIZE);
    LT.push_back(S);
    bool change = false;
    int tour = 0;
    int pos = -1;
    int same = 0;
    int exact;
    //printf("Machines ; %d  %d\n",nbMachines, nbJobs);
    while (it < (nbJobs * nbMachines / 5))
    {
        if (Boucle == M)
        {
            exact++;
            same++;
        }
        else
        {
            exact = 0;
            Boucle = M;
            same = 0;
        }
        // printf("HERE %d  %d\n",M, it);
        it++;
        Neighborhood.clear();
        Neighborhood = AdjacentExchange(Smax);
        change = false;
        T_cmax = INT32_MAX;
        // can do a while, with next fit, maybe better + Diversification
        for (i = 0; i < nbJobs - 1; i++)
        {
            S = Neighborhood[i];
            Act = Cmax(S, A, nbMachines);
            if ((Act <= M) && (notTabu(S, LT)))
            {
                Smax = S;
                M = Act;
                change = true;
            }
            else if ((Act <= M) && (Act <= T_cmax)) // We do not go back to Tabou if it is equal .. only better
            {
                T = S;
                T_cmax = Act;
            }
        } // Best solution not Tabou in neighborhood found, and the best Tabou

        if ((change) && (same < max(nbJobs * stop / 100, 3)))
        { // was 3, for 10 IT, 7 LT
            pos = (pos + 1) % LT_MAX_SIZE;
            if (pos == 0)
                tour++;
            LT[pos] = S; // manage the insert AND pop while conserving the perfs ^^
        }
        else
        {
            //same = 0; // return the count, diversif.
            if (T_cmax != INT32_MAX)
            {               // Tabou
                Smax = T;   // elle est deja dans la TL
                M = T_cmax; // car il est meilleur.
            }
            else // nothing better ... On diversifie
            {
                if (M < Mpreq)
                {
                    Spreq = Smax;
                    Mpreq = M;
                }

                //It blocked .. :/ Best for now
                // if (tour > 1) // pos+1 contains the oldest Tabou sol
                // {
                //     pos = ( pos + 1) % LT_MAX_SIZE;
                //     Smax = LT[pos];
                // }
                // else // 0 contains the oldest Tabou
                // {
                //     Smax = LT[0];
                // }
                // M = Cmax(Smax, A, nbMachines);

                // 2nd try = terrible
                pos = (pos + 1) % LT_MAX_SIZE;
                // do {
                //     std::random_shuffle(Smax.begin(), Smax.end());
                //     Act = Cmax(Smax, A, nbMachines);
                // } while (Act > M);
                std::random_shuffle(Smax.begin(), Smax.end());
                LT[pos] = Smax;
                M = Act;

                //break;
            }
        }
        if (exact == nbJobs / 2)
            break;
    }

    if (Mpreq < M)
    {
        cmax = Mpreq;
        solution = Spreq;
    }
    else
    {
        cmax = M;
        solution = S;
    }
}

/*********************************** Recherche Tabou Avec Voisinage Pear ***********************************/
// Randomly create a neighbourhood, size_max = taille
vector<vector<int>> PearExchange(vector<int> Sol, int taille){
    vector<vector<int>> Neighborhood;
    int inter ;
    vector<int> vois;
    for (int i = 0; i < taille; i++){
        vois = voisin(Sol);

        if (notTabu(vois,Neighborhood)){ // we use not Tabu to look if this random voisin has been already added.
            Neighborhood.push_back(vois);
        }
        // else
        //     i--;
        // crushes the code!
    }

    //printf(" %ld - ", Neighborhood.size());
    return Neighborhood;
}

// stop = Pourcentage to stop the same score, from the nbJobs ... by default,
void RT_Pear(int A[500][20], int nbJobs, int nbMachines, char Method, int LT_MAX_SIZE, int stop, int tailleNeigh, int NB_ITS_MAX, int &cmax, vector<int> &solution)
{
    int M= INT32_MAX ;
    int Act;
    int Mpreq = INT32_MAX;
    int Boucle = 0;
    vector<vector<int>> Neighborhood;
    vector<int> S, Smax, T, Spreq;
    int T_cmax = 0 ;
    if (Method == 'C')
        CDS(A, nbJobs, nbMachines, M, S);
    else if (Method == 'N')
        NEH(A, nbJobs, nbMachines, M, S);
    else
        Chen(A, nbJobs, nbMachines, M, S);
    Smax = S;
    int i =0;

    int it = 0;

    vector<vector<int>> LT(LT_MAX_SIZE);
    LT.push_back(S);
    bool change = false;
    int tour = 0;
    int pos = -1;
    int same = 0 ;
    int exact;
    //printf("Machines ; %d  %d\n",nbMachines, nbJobs);
    while(it < NB_ITS_MAX ){
        if (Boucle == M)
        {
            exact++;
            same++;
        }
        else
        {
            exact = 0;
            Boucle = M;
            same = 0;
        }
        //printf("HERE %d  %d\n",M, it);
        it++;
        Neighborhood.clear();
        //printf("-");
        Neighborhood = PearExchange(Smax, tailleNeigh);
        change = false;
        T_cmax = INT32_MAX;
        // can do a while, with next fit, maybe better + Diversification
        for(i = 0; i < Neighborhood.size(); i++){
            S = Neighborhood[i];
            Act = Cmax(S, A, nbMachines);
            if ((Act <= M) && ( notTabu(S, LT))){
                Smax = S;
                M = Act;
                change = true;
            }
            else if ((Act <= M)&&(Act <= T_cmax)) // We do not go back to Tabou if it is equal .. only better
            {
                    T = S;
                    T_cmax = Act;
            }
        } // Best solution not Tabou in neighborhood found, and the best Tabou

        if ((change) && (same < max(nbJobs * stop / 100,3))) { // was 3, for 10 IT, 7 LT
            pos = ( pos + 1) % LT_MAX_SIZE;
            if ( pos == 0)
                tour++;
            LT[pos] = S; // manage the insert AND pop while conserving the perfs ^^
        }
        else
        {
            //same = 0; // return the count, diversif.
            if (T_cmax != INT32_MAX){ // Tabou
                Smax = T; // elle est deja dans la TL
                M = T_cmax; // car il est meilleur.
            }
            else // nothing better ... On diversifie
            {
                if (M < Mpreq)
                {                   
                    Spreq = Smax;
                    Mpreq = M;
                }
                
                //It blocked .. :/ Best for now
                // if (tour > 1) // pos+1 contains the oldest Tabou sol
                // {
                //     pos = ( pos + 1) % LT_MAX_SIZE;
                //     Smax = LT[pos];
                // }
                // else // 0 contains the oldest Tabou
                // {
                //     Smax = LT[0];
                // }
                // M = Cmax(Smax, A, nbMachines);

                // 2nd try = terrible
                pos = ( pos + 1) % LT_MAX_SIZE;
                // do {
                //     std::random_shuffle(Smax.begin(), Smax.end());
                //     Act = Cmax(Smax, A, nbMachines);
                // } while (Act > M);
                std::random_shuffle(Smax.begin(), Smax.end());
                LT[pos] = Smax;
                M = Act;

                //break;
            }
        }
        if (exact == nbJobs / 2)
            break;
    }

    if (Mpreq < M)
    {                   
        cmax = Mpreq;
        solution = Spreq;
    }
    else{
        cmax = M;
        solution = S;
    }
}

/*********************************** Algo génétique***********************************/

// calcul la fitness d'une solution
double fitness(int A[500][20], int nbMachines, vector<int> solution)
{
    return (1. / Cmax(solution, A, nbMachines));
}

// generation aléatoire de la population initiale
void gen_initiale(int nbjobs, int A[500][20], int nbMachines, int nb_elements, char heur, multimap<double, vector<int>> &population)
// if heur = 1 => 3 solutions heuristiques , sinon => tt est aléatoire
{
    population.clear();
    double f;
    int position, M;
    int individus = 0;
    vector<int> SOL(nbjobs, 1);
    int k = 1;

    if (heur == '1')
    {

        CDS(A, nbjobs, nbMachines, M, SOL);
        f = 1. / M;
        population.insert(make_pair(f, SOL));
        SOL.clear();

        Chen(A, nbjobs, nbMachines, M, SOL);
        f = 1. / M;
        population.insert(make_pair(f, SOL));
        SOL.clear();

        NEH(A, nbjobs, nbMachines, M, SOL);
        f = 1. / M;
        population.insert(make_pair(f, SOL));

        k = 4;
    }

    for (int i = 0; i < nbjobs; i++)
    {
        SOL[i] = i;
    }

    f = fitness(A, nbMachines, SOL);
    population.insert(make_pair(f, SOL));

    for (individus = 0; individus < nb_elements - k; individus++) // génération du reste des individus
    {

        random_shuffle(SOL.begin(), SOL.end()); // appliquer une permutation à SOL

        f = fitness(A, nbMachines, SOL);

        population.insert(make_pair(f, SOL));
    }
}

// Selection par roulette
void select_roulette(multimap<double, vector<int>> population, vector<vector<int>> &parents)
{
    double somme_f = 0, x, y;
    multimap<double, vector<int>>::iterator it;
    int i = 0;
    int nb_elements = population.size();
    double probas = 0;
    vector<double> cumul_p(nb_elements, 0);
    vector<int> position;

    for (it = population.begin(); it != population.end(); ++it) // calculer la somme des fitness de la population
    {
        somme_f += it->first;
    }

    /* for (it = population.begin(); it != population.end(); ++it) // calculer la proba de chaque indiv et ainsi le cumul
    {
        probas = it->first / somme_f;
        if (i == 0)
            cumul_p[i] = probas;
        else
            cumul_p[i] = cumul_p[i - 1] + probas;
        i++;
    } */

    for (it = population.begin(); it != population.end(); ++it) // calculer la proba de chaque indiv et ainsi le cumul
    {
        probas = it->first / somme_f;
        if (i == 0)
            cumul_p[i] = probas;
        else
            cumul_p[i] = cumul_p[i - 1] + probas;
        i++;
    }

    // géneration aléatoire
    i = 0;
    // géneration aléatoire
    x = rand() / (RAND_MAX + 1.);
    y = rand() / (RAND_MAX + 1.);
    for (i = 0; i < nb_elements; i++)
    {
        if (i == 0)
        {
            if (x <= cumul_p[i])
            { // est-ce que X0 est choisit ?
                position.push_back(i);
            }
            if (y <= cumul_p[i])
            {
                position.push_back(i);
            }
        }
        else
        {
            if ((x > cumul_p[i - 1]) & (x <= cumul_p[i]))
            {
                position.push_back(i);
            }
            if ((y > cumul_p[i - 1]) & (y <= cumul_p[i]))
            {
                position.push_back(i);
            }
        }
        if (position.size() == 2)
            break;
    }
    sort(position.begin(), position.end());

    it = population.end();
    it--;
    for (int j = 0; j < position[0] - 1; j++)
        it--;
    parents.push_back(it->second);

    it = population.end();
    it--;
    for (int j = 0; j < position[1] - position[0]; j++)
        it--;
    parents.push_back(it->second);

    /* // choisir deux parents
    for (it = population.end(); it != population.begin(); --it)
    {
        if ((i == 0) & ((x < cumul_p[i]) | (y < cumul_p[i]))) // est-ce que X0 est choisit ?
            parents.push_back(it->second);
        else if (((x > cumul_p[i - 1]) & (x < cumul_p[i])) | ((y > cumul_p[i - 1]) & (y < cumul_p[i])))
            parents.push_back(it->second);
        if (parents.size() == 2) // si on a choisit 2 parents, fini.
            break;
        i++;
    } */
}

void Croisement1pt(vector<int> P1, vector<int> P2, int nbJobs, double proba, vector<int> &E1, vector<int> &E2)
{
    double x;
    x = rand() / (RAND_MAX + 1.);

    if (x <= proba)
    {
        int y;
        y = 1 + (rand() / (RAND_MAX / (nbJobs - 1)));
        for (int i = 0; i < y; i++)
        {
            E1.push_back(P1[i]);
            E2.push_back(P2[i]);
        }
        int ind1 = y, ind2 = y;
        for (int j = y; j < nbJobs; j++)
        {
            if (std::count(E1.begin(), E1.end(), P2[j]))
            {
                while (ind1 < nbJobs && std::count(E1.begin(), E1.end(), P1[ind1]))
                {
                    ind1++;
                }
                if (ind1 < nbJobs)
                {
                    E1.push_back(P1[ind1]);
                }
            }
            else
            {
                E1.push_back(P2[j]);
            }

            if (std::count(E2.begin(), E2.end(), P1[j]))
            {
                while (ind2 < nbJobs && std::count(E2.begin(), E2.end(), P2[ind2]))
                {
                    ind2++;
                }
                if (ind2 < nbJobs)
                {
                    E2.push_back(P2[ind2]);
                }
            }
            else
            {
                E2.push_back(P1[j]);
            }
        }
    }
}

void Croisement2pts(vector<int> P1, vector<int> P2, int nbJobs, double proba, vector<int> &E1, vector<int> &E2)
{
    double x;
    x = rand() / (RAND_MAX + 1.);

    if (x <= proba)
    {
        int y = 0, z = 0;
        while (y == z)
        {
            y = 1 + (rand() / (RAND_MAX / (nbJobs - 2)));
            z = 1 + (rand() / (RAND_MAX / (nbJobs - 2)));
        }
        if (y > z)
        {
            int temp = z;
            z = y;
            y = temp;
        }
        for (int i = 0; i < y; i++)
        {
            E1.push_back(P1[i]);
            E2.push_back(P2[i]);
        }
        for (int i = y; i < nbJobs; i++)
        {
            E1.push_back(nbJobs + 10);
            E2.push_back(nbJobs + 10);
        }
        for (int i = z + 1; i < nbJobs; i++)
        {
            E1.at(i) = P1[i];
            E2.at(i) = P2[i];
        }
        int ind1 = y, ind2 = y;
        for (int j = y; j <= z; j++)
        {
            if (std::count(E1.begin(), E1.end(), P2[j]))
            {
                while (ind1 <= z && std::count(E1.begin(), E1.end(), P1[ind1]))
                {
                    ind1++;
                }
                if (ind1 <= z)
                {
                    E1.at(j) = P1[ind1];
                }
            }
            else
            {
                E1.at(j) = P2[j];
            }

            if (std::count(E2.begin(), E2.end(), P1[j]))
            {
                while (ind2 <= z && std::count(E2.begin(), E2.end(), P2[ind2]))
                {
                    ind2++;
                }
                if (ind2 <= z)
                {
                    E2.at(j) = P2[ind2];
                }
            }
            else
            {
                E2.at(j) = P1[j];
            }
        }
    }
}

//Fonction de mutation par echange
void mutation_swap(vector<int> individu, int nbjobs, double proba_mut, vector<int> &indivMute)
{
    double d;
    int alpha, beta;

    srand(time(0));
    d = rand() / (RAND_MAX + 1.);
    //cout << "proba" << d <<endl;

    if (d <= proba_mut)
    {
        alpha = rand() / (RAND_MAX / (nbjobs - 1));
        beta = rand() / (RAND_MAX / (nbjobs - 1));
        while (alpha == beta)
        {
            beta = rand() / (RAND_MAX / (nbjobs - 1));
        }
        //cout << "premiere position " << alpha << endl;
        //cout << "2eme position " << beta << endl;

        //permuter les jobs des deux positions alpha et beta dans indivMute
        indivMute = individu;
        std ::iter_swap(indivMute.begin() + alpha, indivMute.begin() + beta);
    }
}

//Fonction de mutation par insertion
void mutation_insert(vector<int> individu, int nbjobs, double proba_mut, vector<int> &indivMute)
{
    double x;
    int job, position, posJ;
    vector<int>::iterator it;

    // géneration aléatoire
    x = rand() / (RAND_MAX + 1.);

    if (x <= proba_mut)
    {
        indivMute = individu; // copie du vecteur comme tel

        job = (rand() / (RAND_MAX / (nbjobs - 1))) % nbjobs; // generer nombre entre 0 et nbjobs

        //position = (int)(rand() / (double)RAND_MAX * (nbjobs - 1));

        position = (rand() / (RAND_MAX / (nbjobs - 1))) % nbjobs; // generer nombre entre 0 et nbjobs

        it = find(indivMute.begin(), indivMute.end(), job); // chercher le job

        if (it != indivMute.end()) // If element was found
        {
            posJ = it - indivMute.begin(); // position actuelle du job

            if (posJ != position) // s'il y a changement de position
            {
                if (posJ > position) // inserer à l'avant
                {
                    indivMute.insert(indivMute.begin() + position, job); // insertion dans nouvelle position
                    indivMute.erase(indivMute.begin() + posJ + 1);       // suppression dans ancienne position
                }
                if (posJ < position) // inserer à l'arrière
                {
                    indivMute.insert(indivMute.begin() + position + 1, job); // insertion dans nouvelle position
                    indivMute.erase(indivMute.begin() + posJ);               // suppression dans ancienne position
                }
            }
        }
    }
}

void remplacement(int taille, multimap<double, vector<int>> &newP, multimap<double, vector<int>> pool)
{
    multimap<double, vector<int>>::iterator it;

    it = pool.end();

    for (int i = 0; i < taille; i++) // prendre les N meilleurs individus
    {
        --it;
        newP.insert(make_pair(it->first, it->second));
    }
}

void Algo_Gen_AG(int A[500][20], int nbJobs, int nbMachines, int taille_pop, char heur, double prob_crois, double prob_mut, char tech_mut, char tech_crois, int nb_gen, vector<int> &solution, int &cmax)
// tech_mut :      si = 'S' => par swap     ;        si = 'I' => par insertion
// tech_crois :    si = 1 => 1pnt           ;        si = '2' => 2 pnts
{
    // déclarations
    multimap<double, vector<int>> population; // correspondance <fitness, solution>
    multimap<double, vector<int>> population2;
    multimap<double, vector<int>> enf;
    vector<vector<int>> parents; // les 2 parents séléctionnés pour croisement
    vector<int> E1, E2, ind_M;
    double f;
    multimap<double, vector<int>>::iterator it; // iterator

    // logique
    gen_initiale(nbJobs, A, nbMachines, taille_pop, heur, population); // generation d'une population initiale

    for (int i = 0; i < nb_gen; i++) // pour chaque génération
    {
        while (enf.size() < taille_pop) // générer N enfants
        {
            E1.clear();
            E2.clear();
            parents.clear();

            select_roulette(population, parents); // séléctionner 2 parents

            f = fitness(A, nbMachines, parents[0]);
            population2.insert(make_pair(f, parents[0]));

            f = fitness(A, nbMachines, parents[1]);
            population2.insert(make_pair(f, parents[1]));

            // croisement
            if (tech_crois == '1') // croisement 1 pnt
            {
                Croisement1pt(parents[0], parents[1], nbJobs, prob_crois, E1, E2);
            }
            else if (tech_crois == '2') // croisement 2 pnts
            {
                Croisement2pts(parents[0], parents[1], nbJobs, prob_crois, E1, E2);
            }

            // ajout s'il y a eu croisement
            if ((!E1.empty()) & (!E2.empty()))
            {
                f = fitness(A, nbMachines, E1);
                enf.insert(make_pair(f, E1));

                f = fitness(A, nbMachines, E2);
                enf.insert(make_pair(f, E2));
            }
        }

        // mutation
        for (it = enf.begin(); it != enf.end(); ++it) // pour chaque indiv généré par croisement
        {
            ind_M.clear();
            if (tech_mut == 'S') //mutation par echange
            {
                mutation_swap(it->second, nbJobs, prob_mut, ind_M);
                if (!ind_M.empty())
                {
                    f = fitness(A, nbMachines, ind_M);
                    population2.insert(make_pair(f, ind_M)); // inserer l'individu muté
                }
                else
                {
                    f = fitness(A, nbMachines, it->second);
                    population2.insert(make_pair(f, it->second)); // inserer l'individu non muté
                }
            }
            else if (tech_mut == 'I') //mutation par insertion
            {
                mutation_insert(it->second, nbJobs, prob_mut, ind_M);
                if (!ind_M.empty())
                {
                    f = fitness(A, nbMachines, ind_M);
                    population2.insert(make_pair(f, ind_M)); // inserer l'individu muté
                }
                else
                {
                    f = fitness(A, nbMachines, it->second);
                    population2.insert(make_pair(f, it->second)); // inserer l'individu non muté
                }
            }
        }

        // remplacement
        population.clear();
        remplacement(taille_pop, population, population2); // choisir N individus parmi 2N au max
        population2.clear();
    }
    it = population.end();
    it--;
    solution = it->second;

    cmax = (int)(1 / it->first); // meilleure solution
}

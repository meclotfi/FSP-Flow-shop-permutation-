#include "../../FSP.h"

using namespace std;

double fitness(int A[500][20], int nbMachines, vector<int> solution) // calcul la fitness d'une solution
{
    return (1. / Cmax(solution, A, nbMachines));
}

void gen_initiale(int nbjobs, int A[500][20], int nbMachines, int nb_elements, multimap<double, vector<int>> &population)
{
    double f;
    int position;
    int individus = 0;
    vector<int> SOL(nbjobs, 1);

    for (int i = 0; i < nbjobs; i++) // 1 ere solution triviale afin de pouvoir appliquer une permutaion
    {
        SOL[i] = i;
    }

    f = fitness(A, nbMachines, SOL);
    population.insert(make_pair(f, SOL));

    for (individus = 0; individus < nb_elements - 1; individus++) // génération du reste des individus
    {

        random_shuffle(SOL.begin(), SOL.end()); // appliquer une permutation à SOL

        f = fitness(A, nbMachines, SOL);

        population.insert(make_pair(f, SOL));
    }
}

void select_roulette(multimap<double, vector<int>> &population, vector<vector<int>> &parents)
{
    double somme_f = 0, x, y;
    multimap<double, vector<int>>::iterator it;
    int i = 0;
    int nb_elements = population.size();
    double probas = 0;
    vector<double> cumul_p(nb_elements, 0);

    for (it = population.begin(); it != population.end(); ++it) // calculer la somme des fitness de la population
    {
        somme_f += it->first;
    }

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
    std::default_random_engine eng(time(0));
    std::uniform_real_distribution<double> distr(0.0, 1.0);
    i = 0;
    x = distr(eng);
    y = distr(eng);

    // choisir deux parents
    for (it = population.begin(); it != population.end(); ++it)
    {
        if ((i == 0) & ((x < cumul_p[i]) | (y < cumul_p[i]))) // est-ce que X0 est choisit ?
            parents.push_back(it->second);
        else if (((x > cumul_p[i - 1]) & (x < cumul_p[i])) | ((y > cumul_p[i - 1]) & (y < cumul_p[i])))
            parents.push_back(it->second);
        if (parents.size() == 2) // si on a choisit 2 parents, fini.
            break;
        i++;
    }
}

int main()
{
    int nbJobs, nbMachines, A[500][20];
    multimap<double, vector<int>> population; // correspondance <fitness, solution>
    vector<vector<int>> parents;              // les 2 parents séléctionnés pour croisement

    //load nbJobs, nbMachines and the matrix A
    loader("../../benchmarks/5jobs5machines.txt", &nbJobs, &nbMachines, A);

    gen_initiale(nbJobs, A, nbMachines, 500, population);

    multimap<double, vector<int>>::iterator it;
    /* afficher la population
    for (it = population.begin(); it != population.end(); ++it)
    {
        cout << it->first << "=>";

        for (auto var : it->second)
        {
            cout << "J" << var << " ";
        }
        cout << endl;
    } */

    select_roulette(population, parents);

    return 0;
}

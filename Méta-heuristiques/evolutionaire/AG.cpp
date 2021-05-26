#include "../../FSP.h"
#include "croisement.cpp"
#include "mutation.cpp"
#include "remplacement.cpp"

using namespace std;

// calcul la fitness d'une solution
double fitness(int A[500][20], int nbMachines, vector<int> solution)
{
    return (1. / Cmax(solution, A, nbMachines));
}

// generation aléatoire de la population initiale
void gen_initiale(int nbjobs, int A[500][20], int nbMachines, int nb_elements, multimap<double, vector<int>> &population)
{
    population.clear();
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

// Selection par roulette
void select_roulette(multimap<double, vector<int>> population, vector<vector<int>> &parents)
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
    i = 0;
    // géneration aléatoire
    x = rand() / (RAND_MAX + 1.);
    y = rand() / (RAND_MAX + 1.);

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

// algo général
/* void Algo_Gen(int A[500][20], int nbJobs, int nbMachines, int taille_pop, double prob_crois, double prob_mut, char tech_mut, char tech_crois, int nb_gen, vector<int> &solution, int &cmax)
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
    gen_initiale(nbJobs, A, nbMachines, taille_pop, population);

    for (int i = 0; i < nb_gen; i++)
    {
        while (population2.size() < taille_pop) // générer des enfants
        {
            E1.clear();
            E2.clear();
            parents.clear();

            select_roulette(population, parents); // séléctionner 2 parents

            // croisement
            if (tech_crois == '1')
            {
                Croisement1pt(parents[0], parents[1], nbJobs, prob_crois, E1, E2);
            }
            else if (tech_crois == '2')
            {
                Croisement2pts(parents[0], parents[1], nbJobs, prob_crois, E1, E2);
            }
            // ajout s'il y a eu croisement
            if ((!E1.empty()) & (!E2.empty()))
            {
                f = fitness(A, nbMachines, parents[0]);
                population2.insert(make_pair(f, parents[0]));

                f = fitness(A, nbMachines, parents[1]);
                population2.insert(make_pair(f, parents[1]));

                f = fitness(A, nbMachines, E1);
                population2.insert(make_pair(f, E1));

                f = fitness(A, nbMachines, E2);
                population2.insert(make_pair(f, E2));
            }
        }

        // mutation
        for (it = population2.begin(); it != population2.end(); ++it) // pour chaque indiv de la population resultante
        {
            ind_M.clear();
            if (tech_mut == 'S')
            {
                mutation_swap(it->second, nbJobs, prob_mut, ind_M);
                if (!ind_M.empty())
                {
                    f = fitness(A, nbMachines, ind_M);
                    enf.insert(make_pair(f, ind_M));
                }
                f = fitness(A, nbMachines, it->second);
                enf.insert(make_pair(f, it->second));
            }
            else if (tech_mut == 'I')
            {
                mutation_insert(it->second, nbJobs, prob_mut, ind_M);
                if (!ind_M.empty())
                {
                    f = fitness(A, nbMachines, ind_M);
                    enf.insert(make_pair(f, ind_M));
                }
                f = fitness(A, nbMachines, it->second);
                enf.insert(make_pair(f, it->second));
            }
        }

        // remplacement
        population.clear();
        remplacement(taille_pop, population, enf);
        population2.clear();
        enf.clear();
    }

    it = population.end();
    it--;
    solution = it->second;

    cmax = Cmax(solution, A, nbMachines);
} */

// algo général
void Algo_Gen(int A[500][20], int nbJobs, int nbMachines, int taille_pop, double prob_crois, double prob_mut, char tech_mut, char tech_crois, int nb_gen, vector<int> &solution, int &cmax)
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
    gen_initiale(nbJobs, A, nbMachines, taille_pop, population); // generation d'une population initiale

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

int main()
{
    int nbJobs, nbMachines, A[500][20];
    vector<int> solution;
    int cmax;
    srand(time(0)); // pour eviter la meme séquence aléatoire

    //load nbJobs, nbMachines and the matrix A
    loader("../../benchmarks/20jobs10machines.txt", &nbJobs, &nbMachines, A);
    clock_t start, end;

    start = clock();
    Algo_Gen(A, nbJobs, nbMachines, 500, 0.8, 0.1, 'I', '2', 40, solution, cmax);
    end = clock();
    double temps = double(end - start) / double(CLOCKS_PER_SEC);

    /* for (int i = 0; i < nbJobs; i++)
    {
        cout << "J" << solution[i] << " ";
    }
    cout << endl; */

    cout << "cmax = " << cmax << endl;

    printf(" \n temps d'exec d'AG : %f secondes\n\n", temps);

    /* afficher la population
    multimap<double, vector<int>>::iterator it;

    for (it = population.begin(); it != population.end(); ++it)
    {
        cout << it->first << "=>";

        for (auto var : it->second)
        {
            cout << "J" << var << " ";
        }
        cout << endl;
    } */

    return 0;
}

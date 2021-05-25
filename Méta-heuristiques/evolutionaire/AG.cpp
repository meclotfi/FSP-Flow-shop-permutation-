#include "../../FSP.h"

using namespace std;

// calcul la fitness d'une solution
double fitness(int A[500][20], int nbMachines, vector<int> solution)
{
    return (1. / Cmax(solution, A, nbMachines));
}

// generation aléatoire de la population initiale
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

// Selection par roulette
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

// Croisement à 1 point
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
                if (ind1 < nbJobs)
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

// Croisement à 1 point
void Croisement2pts(vector<int> P1, vector<int> P2, int nbJobs, double proba, vector<int> &E1, vector<int> &E2)
{
    cout << "premier parent" << endl;
    for (auto var : P1)
    {
        cout << "J" << var << " ";
    }
    cout << endl;

    cout << "deuxieme parent" << endl;
    for (auto var : P2)
    {
        cout << "J" << var << " ";
    }
    cout << endl;

    double x;
    x = rand() / (RAND_MAX + 1.);
    cout << endl
         << "proba " << x << endl;

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
        cout << endl
             << "alpha: " << y << endl
             << "beta: " << z << endl;
        for (int i = 0; i < y; i++)
        {
            E1.push_back(P1[i]);
            E2.push_back(P2[i]);
        }
        cout << "finishing first part" << endl;

        for (int i = y; i < nbJobs; i++)
        {
            E1.push_back(NULL);
            E2.push_back(NULL);
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
                if (ind1 <= z)
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

    cout << "premier enfant" << endl;
    for (auto var : E1)
    {
        cout << "J" << var << " ";
    }
    cout << endl;

    cout << "deuxieme enfant" << endl;
    for (auto var : E2)
    {
        cout << "J" << var << " ";
    }
    cout << endl;
}

//Mutation par echange
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

//Mutation par insertion
void mutation_insert(vector<int> individu, int nbjobs, double proba_mut, vector<int> &indivMute)
{
    double x;
    int job, position, posJ;
    vector<int>::iterator it;

    // copie du vecteur comme tel
    indivMute = individu;

    // géneration aléatoire
    x = rand() / (RAND_MAX + 1.);

    if (x <= proba_mut)
    {
        job = rand() / (RAND_MAX / (nbjobs - 1));
        position = rand() / (RAND_MAX / (nbjobs - 1));

        it = find(indivMute.begin(), indivMute.end(), job);
        // If element was found
        if (it != indivMute.end())
        {
            posJ = it - indivMute.begin();                       // position actuelle du job
            indivMute.insert(indivMute.begin() + position, job); // insertion dans nouvelle position
            indivMute.erase(indivMute.begin() + posJ);           // suppression dans ancienne position
        }
    }
}

// remplacement selon fitness
void remplacement(int taille, multimap<double, vector<int>> &newP, multimap<double, vector<int>> pool)
{
    //  int sizeP = P.size();
    //  int sizeE = E.size();
    multimap<double, vector<int>>::iterator it;
    /*
    for (int i=0; i<sizeP; i++){
        pool.insert(P[i]);
    }*/

    it = pool.end();
    for (int i = 0; i < taille; i++)
    {
        --it;
        newP.insert(make_pair(it->first, it->second));
    }
}

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
    gen_initiale(nbJobs, A, nbMachines, taille_pop, population);

    for (int i = 0; i < nb_gen; i++)
    {
        while (enf.size() < taille_pop) // générer des enfants
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
            if (!E1.empty())
            {
                f = fitness(A, nbMachines, E1);
                enf.insert(make_pair(f, E1));
            }
            if (!E2.empty())
            {
                f = fitness(A, nbMachines, E2);
                enf.insert(make_pair(f, E2));
            }
        }

        // mutation
        for (it = population.begin(); it != population.end(); ++it) // pour chaque indiv de la population
        {
            if (tech_mut == 'S')
            {
                mutation_swap(it->second, nbJobs, prob_mut, ind_M);
                f = fitness(A, nbMachines, ind_M);
                population2.insert(make_pair(f, ind_M));
            }
            else if (tech_mut == 'I')
            {
                mutation_insert(it->second, nbJobs, prob_mut, ind_M);
                f = fitness(A, nbMachines, ind_M);
                population2.insert(make_pair(f, ind_M));
            }
        }

        for (it = enf.begin(); it != enf.end(); ++it) // regrouper enfants et parents
        {
            population2.insert(make_pair(it->first, it->second));
        }

        // remplacement
        population.clear();
        remplacement(taille_pop, population, population2);
        population2.clear();
    }
    it = population.end();
    solution = it->second;
    cmax = Cmax(solution, A, nbMachines);
}

int main()
{
    int nbJobs, nbMachines, A[500][20];
    vector<int> solution;
    int cmax;

    //load nbJobs, nbMachines and the matrix A
    loader("../../benchmarks/20jobs10machines.txt", &nbJobs, &nbMachines, A);
    clock_t start, end;

    start = clock();
    Algo_Gen(A, nbJobs, nbMachines, 200, 0.8, 0.8, 'I', '1', 20, solution, cmax);

    end = clock();
    double temps = double(end - start) / double(CLOCKS_PER_SEC);

    for (int i = 0; i < nbJobs; i++)
    {
        cout << "J" << solution[i] << " ";
    }
    cout << endl;

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

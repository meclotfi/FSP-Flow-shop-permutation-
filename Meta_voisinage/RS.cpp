#include "../Heuristiques/neh.cpp"

vector<int> voisin(vector<int> s)
{
    int a, b;
    vector<int> x = s;

    a = rand() % s.size();
    b = rand() % s.size();
    if (b == a)
        b = rand() % s.size();

    iter_swap(x.begin() + a, x.begin() + b);
    return x;
}

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

int main()
{
    int nbJobs, nbMachines, somme, A[500][20], cmax;
    vector<int> s0, solution;

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

    
    RS(A, nbJobs, nbMachines, s0, .8, 100, 6, 100000,solution, cmax);
    std::cout << "Meta solution :" << std::endl;
    for (auto var : solution)
    {
        std::cout << var << " ";
    }
    std::cout << "\nCmax: " << cmax << std::endl;

}
#include "../FSP.h"


int main()
{
    int nbJobs, nbMachines, somme, A[500][20], cmax, nb_it_palier, nb_arret,CMAX;
    vector<int> s0, solution;
    float alpha, T;
    CMAX=INT32_MAX;
    clock_t start, end;

    string filepath = "../benchmarks/13J_5M.txt";

    loader(filepath, &nbJobs, &nbMachines, A);
    NEH(A, nbJobs, nbMachines, cmax, solution);
    s0=solution;

    #pragma omp parallel for schedule(dynamic, 10) collapse(4) private(cmax, solution, start, end)
    for (size_t i = 1000; i <= 10000; i+=1000)
    {
        for (size_t j = 1; j <= 20; j++)
        {
            for (size_t k = 0; k < 50;k+=5)
            {
                for (size_t temp = 100; temp < 700; temp+=50)
                {
                    start = omp_get_wtime();
                    RS(A,nbJobs,nbMachines,s0,.5+k/100, temp, j,i,solution,cmax);
                    end = omp_get_wtime();

                    #pragma omp critical
                    {
                    if (cmax<CMAX)
                    {
                        CMAX=cmax;
                        nb_it_palier=j;
                        nb_arret=i;
                        T=temp;
                        alpha=.5+k/100;
                    }
                    }
                    
                }
                
            }
            
        }
        
    }
    
    std::cout <<"Cmax:" CMAX <<" Nb palier total: "<< nb_arret << " Nb it palier: "<< nb_it_palier << " Temperature initiale: " << T << " alpha: " << alpha << std::endl;



    // std::cout << "Neh solution" << std::endl;
    // for (auto var : solution)
    // {
    //     std::cout << var << " ";
    // }
    // s0 = solution;
    // std::cout << "\nCmax: " << cmax << std::endl;

    // start = clock();
    // RS(A, nbJobs, nbMachines, s0, .8, 100, 20, 100000,solution, cmax);

    // end = clock();
    // double time_taken = double(end - start) / double(CLOCKS_PER_SEC);
    // printf (" \n RS %f secondes\n\n", time_taken);

    // std::cout << "Meta solution :" << std::endl;
    // for (auto var : solution)
    // {
    //     std::cout << var << " ";
    // }
    // std::cout << "\nCmax: " << cmax << std::endl;

    

}
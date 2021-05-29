#include "../FSP.h"

    int Parallel_BB_hybride(int nbJobs,int nbMachines,int A[500][20]) ;
    void CDS(int A[500][20], int nbJobs, int nbMachines, int &cmax, vector<int> &solution);
    void NEH(int A[500][20], int nbJobs, int nbMachines, int &cmax, vector<int> &solution);
    void Chen(int A[500][20], int nbJobs, int nbMachines, int &cmax, vector<int> &solution);

// Use This command to compile:
// g++ RT.cpp ../Heuristiques/chen.cpp ../Heuristiques/CDS.cpp ../Heuristiques/neh.cpp  ../exact/Par_BB.cpp -fopenmp -o g

vector<vector<int>> AdjacentExchange(vector<int> Sol){
    vector<vector<int>> Neighborhood;
    int inter ;
    int nbJobs = Sol.size();
    for (int i = 0; i < nbJobs - 1; i++){
        Neighborhood.push_back(Sol);
        inter = Neighborhood[i][i];
        Neighborhood[i][i] = Neighborhood[i][i+1];
        Neighborhood[i][i+1] = inter;
    }

    return Neighborhood;
}

bool notTabu(vector<int> S, vector<vector<int>> LT){
    int size = LT.size();
    for (int i = 0; i < size ; i ++)
    {
        if ( LT[i] == S )
            return false;
    }
    return true;
}
// next fit ?

// stop = Pourcentage to stop the same score, from the nbJobs ... by default,
void RT(int A[500][20], int nbJobs, int nbMachines, char Method, int LT_MAX_SIZE, int stop, int &cmax, vector<int> &solution)
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
    while(it < (nbJobs * nbMachines / 5)){
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
        for(i = 0; i < nbJobs - 1; i++){
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
    /**
     * 
     *     std::shuffle(v.begin(), v.end(), g);
 
    std::copy(v.begin(), v.end(), std::ostream_iterator<int>(std::cout, " "));
    std::cout << "\n";
    */
}

int main()
{
    int nbJobs, nbMachines, somme, A[500][20];
    int M ;
  
    //load nbJobs, nbMachines and the matrix A
    //string filepath = "../benchmarks/200jobs20machines.txt"; //RT(A,nbJobs,nbMachines,20,5,M,sol); 10919
    //string filepath = "../benchmarks/9jobs5machines.txt";// 20,5 635== Best
    //string filepath = "../benchmarks/13J_5M.txt";//868 .. Stops before ..
    //string filepath = "../benchmarks/500jobs2machines.txt";
    //string filepath = "../benchmarks/200jobs10machines.txt"; //RT(A,nbJobs,nbMachines,20,5,M,sol); 10919
        // 11344 for 20(50),5
    string filepath = "../benchmarks/500jobs20machines.txt"; //RT(A,nbJobs,nbMachines,20,5,M,sol); 10919

    //11J_5M.txt

    loader(filepath, &nbJobs, &nbMachines, A);
    clock_t start, end;
    
    start = clock();
    vector<int> sol; // N
    RT(A,nbJobs,nbMachines,'N',8,5,M,sol); // was 7, and 10 for 20.20
    end = clock();
    double time_taken = double(end - start) / double(CLOCKS_PER_SEC);

    printf (" \n RT %f secondes\n\n", time_taken);

    cout <<"Sequence" << endl;
    for(int i = 0; i < nbJobs; i++)
       {
           cout << sol[i] << endl;
       }
    cout << "Cmax : " << M <<endl;

    double debut, fin, temps;

    // debut= omp_get_wtime();
    // Parallel_BB(nbJobs,nbMachines,A);
    // fin= omp_get_wtime(); temps=fin-debut;
    // printf (" \n BB Parallel %f secondes\n\n", temps);

    return 0;
}

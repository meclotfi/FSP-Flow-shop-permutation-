
//#include "../FSP.h"
#include "../Heuristiques/CDS.cpp"
#define CHUNKSIZE 1

using namespace std;


void show_progress_bar(float prog,int j,int LB)
{
                                         

    int barWidth = 30;

    std::cout << "[";
    int pos = barWidth * prog;
    for (int i = 0; i < barWidth; ++i) {
        if (i < pos) std::cout << "=";
        else if (i == pos) std::cout << ">";
        else std::cout << " ";
    }
    std::cout << "] " << int(prog * 100.0) << " % ";
    cout<< "finished exploring branch J"<<j<<" current lower bound is : "<<LB;
    std::cout.flush();

std::cout << std::endl;

}



void DFS(list<int> S, list<int> J,int &M,int nbMachines,int A[500][20],vector<int> cmax_vec, list<int> &solution)
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
        cost2 = Cmax_Add_Job(cmax_vec,S1.back(),A,nbMachines);
        cost=cost2.back();
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
                solution=S1;              
               
  
                }             
                
                
            }
        }
        else
        {
            
            //calculer l'evaluation:
           //eva=eval(cost, A, nbMachines, J1);
           //cout<<"eval: "<<eval(cost, A, nbMachines, J1)<<" eval max:"<<eval_max(cost, A, cost2, J1)<<"\n";      
           eva=eval_max(cost, A, cost2, J1);
           //int mi=*min_element(cost2.begin(), cost2.end());
           //eva=eval_jhonson(mi, A,cost2, J1);

            //tester si possible d'elager
           // elagage

               if(eva<M)
               {               
               DFS(S1, J1,M,nbMachines,A,cost2,solution);
        
               }
             //else //cout<<" elagage avec eval = "<<eva<<" et M= "<<M<<"\n";
             
                
        }

       
    }
}


int Parallel_BB(int nbJobs,int nbMachines,int A[500][20]) 
{

int M=INT32_MAX;
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
       

int j=0;

 #pragma omp parallel for  schedule(dynamic,CHUNKSIZE) firstprivate(cmax_vec,cost) shared(M,solution)
   for (int i = 0; i < nbJobs; i++)
    {
        //printf("> Thread %d is exploring branch J%d \n", omp_get_thread_num(),J[i]);

        list<int> J1(J.begin(), J.end());
       list<int> S1;
       S1.push_back(J[i]);
        list<int> Jt=J1; 
        J1.remove(J[i]);

       cost=Cmax_Add_Job(cmax_vec,S1.back(),A,nbMachines);
        DFS(S1 , J1 , M , nbMachines , A ,cost,solution);
        #pragma omp atomic write
        j=j+1;
        int f=S1.back();

        // commented this because of parallelisme
        #pragma omp critical
        {
        show_progress_bar(((float)j/(float)nbJobs),f,M);
        }

    }
    
    std::cout << "\n Best Sequence : ";
    for(auto var : solution)
    {
        cout<<"J"<< var <<" ";
    }
   cout << "" << std::endl;
    cout<< "\tMakeSpan :"<<M;
    return M;
}

int Parallel_BB_hybride(int nbJobs,int nbMachines,int A[500][20]) 
{

    int M= INT32_MAX ;

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
       

   int j=0;

#pragma omp parallel for schedule(dynamic,CHUNKSIZE) firstprivate(cmax_vec,cost) shared(M,solution)
   for (int i = 0; i < nbJobs; i++)
    {
        //printf("> Thread %d is exploring branch J%d \n", omp_get_thread_num(),J[i]);

        list<int> J1(J.begin(), J.end());
        list<int> S1;
        S1.push_back(J[i]);
        list<int> Jt=J1; 
        J1.remove(J[i]);
        cost=Cmax_Add_Job(cmax_vec,S1.back(),A,nbMachines);
        DFS(S1 , J1 , M , nbMachines , A ,cost,solution);
        #pragma omp atomic write
        j=j+1;
        int f=S1.back();

        // commented this because of parallelisme
        // #pragma omp critical
        // {
        //     show_progress_bar(((float)j/(float)nbJobs),f,M);
        // }

    }
    
    std::cout << "\n Best Sequence: ";
    for(auto var : solution)
    {
       cout << "J"<< var <<" ";
    }
    std::cout << "" << std::endl;
    cout<< "\tMakeSpan: "<<M;
    return M;
}

int main3()
{
    int nbJobs, nbMachines, somme, A[500][20];
    int M = INT32_MAX;

    
  
    //load nbJobs, nbMachines and the matrix A
    //string filepath = "../benchmarks/6jobs5machines.txt";
    //string filepath = "../benchmarks/9jobs5machines.txt";
    //string filepath = "../benchmarks/11J_5M.txt";
    string filepath = "../benchmarks/500jobs2machines.txt";

    loader(filepath, &nbJobs, &nbMachines, A);
    double debut, fin, temps;

    if (nbMachines == 2){
        // use Johnson Algo
        int C = 0;
        vector<int> solution;
        debut= omp_get_wtime();
        johnson_Cmax(A, nbJobs, C, solution, A);
        fin= omp_get_wtime(); temps=fin-debut;
        printf (" \n Cmax = %d\n", C);
        printf("Best Sequence = ");
        for (auto &it :solution)
        {
            cout<<it<<" ";
        }
        printf(" \n.");
        printf (" \n Johnson Parallel %f secondes\n\n", temps);
    }
    else
    {
        debut= omp_get_wtime();
        Parallel_BB(nbJobs,nbMachines,A);
        fin= omp_get_wtime(); temps=fin-debut;
        printf (" \n BB Parallel %f secondes\n\n", temps);


        debut= omp_get_wtime();
        Parallel_BB_hybride(nbJobs,nbMachines,A);
        fin= omp_get_wtime(); temps=fin-debut;
        printf (" \n BB hybride Parallel %f secondes\n", temps);
    }   
    
    return 0;
}

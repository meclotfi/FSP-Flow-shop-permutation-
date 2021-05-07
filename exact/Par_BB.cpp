#include <iostream>
#include <fstream>
#include <algorithm>
#include <map>
#include <vector>
#include <list>
#include <numeric>
#include "../FSP.h"
#include "string.h"
#include "omp.h"
#define CHUNKSIZE 1

using namespace std;


// comparaison function to sort vectors
bool cmp(pair<int, int> &a,
         pair<int, int> &b)
{
    return a.second > b.second;
}

int DFS(list<int> S, list<int> J,int &M,int nbMachines,int A[500][20],vector<int> cmax_vec)
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
                //cout << "M= " << M << " ";
                #pragma omp atomic write
                M = cost;               
                //cout << "update M= " << M << "  by "<<omp_get_thread_num()<<" \n";
                
            }
        }
        else
        {
            
            //calculer l'evaluation:
           eva=eval(cost, A, nbMachines, J1);

            //tester si possible d'elager
           // elagage

               if(eva<M)
               {               
               DFS(S1, J1,M,nbMachines,A,cost2);
        
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
 
 //std::list<int>::iterator it=J1.begin();
    vector<int> cmax_vec;
   for (int j = 0; j <= nbMachines; j++)
    {
        cmax_vec.push_back(0);
        
    }
        vector<int> cost;
       
 list<int> S1; 
       list<int> J1(J.begin(), J.end());
 #pragma omp parallel for  schedule(dynamic,CHUNKSIZE) firstprivate(cmax_vec,cost) shared(M)
   for (int i = 0; i < nbJobs; i++)
    {
        printf("Thread %d is treating branch J%d \n", omp_get_thread_num(),J[i]);

        
       
       S1.push_back(J[i]);
        list<int> Jt=J1; 
        J1.remove(J[i]);
    
      

      
       cost=Cmax_Add_Job(cmax_vec,S1.back(),A,nbMachines);
        DFS(S1 , J1 , M , nbMachines , A ,cost);
        printf("Thread %d is treating branch J%d \n", omp_get_thread_num(),J[i]);
      

    }
   
    cout<< "final M= "<<M;
    return M;
}

int main()
{
    int nbJobs, nbMachines, somme, A[500][20];
    int M = INT32_MAX;

    
  
    //load nbJobs, nbMachines and the matrix A
    string filepath = "../benchmarks/10jobs5machines.txt";
    loader(filepath, &nbJobs, &nbMachines, A);
    
    double debut, fin, temps;
     debut= omp_get_wtime();
    Parallel_BB(nbJobs,nbMachines,A);
    fin= omp_get_wtime(); temps=fin-debut;
    printf (" \n BB Par %f secondes\n", temps);
   
    
    return 0;
}

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

int DFS(list<int> S, list<int> J,int &M,int nbMachines,int A[500][20])
{

  
    
list<int> J1, S1;
int eva;    
    
    for (auto &it : J)
    {
       
        
        S1 = S;
        S1.push_back(it);
        J1 = J;
        J1.remove(it);
     
 
        // calculer le cout
        vector<int> sol(S1.begin(), S1.end());
        int cost = Cmax(sol, A, nbMachines);

        
        // tester si feuille et caluler l'evaluation sinon
        if (J1.empty())
        {
            
            cout << " cost= " << cost<<"\n";
            
            // cout<<"a ";
            //update lower bound
            if (M > cost)
            {
                //cout << "M= " << M << " ";
                #pragma omp atomic write
                M = cost;
                for (auto &it : S1) cout << "S#" << it<<" ";
                cout<<"\n";
       


                cout << "update M= " << M << " ";
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
               DFS(S1, J1,M,nbMachines,A);
        
               }
             else cout<<" elagage avec eval = "<<eva<<" et M= "<<M<<"\n";
                

                // brunshing
                
        }

        //cout << "back \n";
    }
}

int BB(int nbJobs,int nbMachines,int A[500][20]) 
{

int M=INT32_MAX;
int somme=0;
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
        cout<<"sum_ligne "<<c.second<<" \n";
    }
    sort(sommeLigne.begin(), sommeLigne.end(), cmp);

    for (int j = 0; j < nbJobs; j++)
    {
        J.push_back(sommeLigne[j].first);
        
    }
   
   
    DFS(S,J,M,nbMachines,A);
    cout<< "final M= "<<M;
    return M;
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
        cout<<"sum_ligne "<<c.second<<" \n";
    }
    sort(sommeLigne.begin(), sommeLigne.end(), cmp);

    for (int j = 0; j < nbJobs; j++)
    {
        J.push_back(sommeLigne[j].first);
        
    }
 list<int> S1; 
 list<int> J1(J.begin(), J.end());
 std::list<int>::iterator it=J1.begin();
 
 #pragma omp parallel for  schedule(dynamic,CHUNKSIZE) firstprivate(J1,S1) shared(M)
   for (int i = 0; i < nbJobs; i++)
    {
        printf("Thread %d starting  ...\n", omp_get_thread_num());
       
     
       S1.push_back(J[i]);
             
        J1.remove(J[i]);
        it++;
       
        DFS(S1 , J1 , M , nbMachines , A );

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
    
    vector<int> j={1,3,0,2,4};
    int ev=Cmaxt(j,A,nbMachines);
    cout<<ev;
    Parallel_BB(nbJobs,nbMachines,A);
   
    
    return 0;
}

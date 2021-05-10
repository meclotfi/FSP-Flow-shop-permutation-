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
#include <thread>
#include <chrono>
#include <cmath>
#define CHUNKSIZE 1

using namespace std;


// comparaison function to sort vectors




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
     
 
        // calculer le cout
    
       
        cost2 = Cmax_Add_Job(cmax_vec,S1.back(),A,nbMachines);
        cost=cost2.back();
      
       

        
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
                M = cost;
                //cout << "update M= " << M << " ";
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
             //else cout<<" elagage avec eval = "<<eva<<" et M= "<<M<<"\n";
                

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
   
    DFS(S,J,M,nbMachines,A,cmax_vec);
    cout<< "final M= "<<M;
    return M;
}


void show_progress_bar(float prog,string message)
{
                                         

    int barWidth = 30;

    std::cout << "[";
    int pos = barWidth * prog;
    for (int i = 0; i < barWidth; ++i) {
        if (i < pos) std::cout << "=";
        else if (i == pos) std::cout << ">";
        else std::cout << " ";
    }
    std::cout << "] " << int(prog * 100.0) << " %\r  #"<< message<<"#";
    std::cout.flush();

std::cout << std::endl;

}

int main()
{
    int nbJobs, nbMachines, somme, A[500][20];
    int M = INT32_MAX;

 
  
    //load nbJobs, nbMachines and the matrix A
    
    string filepath = "../benchmarks/11jobs6machines.txt";
    loader(filepath, &nbJobs, &nbMachines, A);
    
    double debut, fin, temps;
     debut= omp_get_wtime();
    BB(nbJobs,nbMachines,A);
    fin= omp_get_wtime(); temps=fin-debut;
    printf ("BB seq %f secondes\n", temps);
    
  vector<int> V={2, 3, 7, 10, 1, 5, 6, 8, 4, 9, 0};
  cout<<Cmax(V,A,nbMachines);

   
    
    return 0;
}

#include <iostream>
#include <fstream>
#include <algorithm>
#include <map>
#include <vector>
#include <list>
#include <numeric>
#include "../FSP.h"
#include "string.h"

using namespace std;
int nbJobs, nbMachines, somme, A[500][20];
int M=INT32_MAX;

// comparaison function to sort vectors
bool cmp(pair<int, int> &a,
         pair<int, int> &b)
{
    return a.second < b.second;
}
int DFS(list<int> S,list<int> J)
{

if (!S.empty()) cout<<"J"<<S.back()<<" ";
if (J.empty()) {cout << "\n"; return 0;}

list<int> J1,S1;
//
for (auto &it : J)
    {
        S1=S;
        S1.push_back(it);
        J1=J;
        J1.remove(it);
    // calculer le cout
    vector<int> sol(S1.begin(), S1.end());
    int cost=Cmax(sol,A,nbMachines);

    // tester si feuille et caluler l'evaluation sinon
    if (J1.empty())
    {
        cout<<"J"<<S1.back()<<" ";

        //update lower bound 
        if(M>cost)
        { 
          M=cost;
          cout<<"update M= "<<M<<" ";
        }

    }
    else
    {
        //calculer l'evaluation:

        //tester si possible d'elager 

        // brunshing
        DFS(S1,J1);
    }
    

    
        cout<<"back \n";
    }
}
int main()
{
    // solution : the solution vector
    
    list<int> J,S;
    vector<int> solution;

    //load nbJobs, nbMachines and the matrix A
    string filepath="../benchmarks/500jobs20machines.txt";
    loader(filepath,&nbJobs, &nbMachines, A);


//initiaalization des structure de données
//J: l'enssemble des job non encore assigner
vector<pair<int,int>> sommeLigne;
pair<int,int> c;

for (int i = 0; i < nbJobs; i++)
    {
        c.first=i;
        c.second = accumulate(A[i], A[i] + nbMachines, somme);
        sommeLigne.push_back(c);

    }
sort(sommeLigne.begin(), sommeLigne.end(), cmp);

for (int j = 0; j < nbMachines; j++)
        {
             J.push_back(sommeLigne[j].first);
         }

DFS(S,J);
return 0;  
}

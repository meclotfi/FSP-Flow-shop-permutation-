#include <iostream>
#include <fstream>
#include <algorithm>
#include <map>
#include <vector>
#include <numeric>
#include <string>
#include <list>
#include "string.h"
#include "omp.h"
using namespace std;


bool cmp(pair<int, int> &a,
         pair<int, int> &b)
{
    return a.second < b.second;
}
bool cmp2(pair<int, int> &a,
         pair<int, int> &b)
{
    return a.second > b.second;
}
// Loads data from a benchmark file and store it in A, nbJobs and nbMachines
void loader(string filepath, int *nbJobs, int *nbMachines, int A[500][20])
{
    // TODO : enter the file path or number of jobs and machines as params
    std::ifstream file(filepath);

    // Check if file is open first
    if (file.is_open())
    {
        file >> *nbJobs >> *nbMachines;
        for (int i = 0; i < *nbMachines; i++)
        {
            for (int j = 0; j < *nbJobs; j++)
            {
                file >> A[j][i];
            }
        }
    }
    else
    {
        std::cout << "File doesn't exist" << std::endl;
    }
}

// Calculate the makespan of a solution
// solution: a sequence of jobs
// A: the matrix of jobs and machines
// nbMachines: the number of machines
 int Cmaxt(vector<int> solution, int A[500][20], int nbMachines)
 {
     if (solution.size() == 0 || nbMachines == 0)
     {
         return 0;
     }
     else
     {
         vector<int> v(solution.begin(), solution.end() - 1);
         int a, b;
         a = Cmaxt(solution, A, nbMachines - 1);
         b = Cmaxt(v, A, nbMachines);
         // std::cout << "a :" << a << std::endl;
         // std::cout << "b :" << b << std::endl;
         if (a >= b)
         {
             return a + A[solution.at(solution.size() - 1)][nbMachines - 1];
         }
         else
         {
             return b + A[solution.at(solution.size() - 1)][nbMachines - 1];
         }
     }
 }

// Calculate the makespan of a solution with dynamic programming
// solution: a sequence of jobs
// A: the matrix of jobs and machines
// nbMachines: the number of machines
int Cmax(vector<int> solution, int A[500][20], int nbMachines)
{
    //vector<vector<int>> matrix;
    int nbJobs = solution.size();
    if (nbJobs == 0 || nbMachines == 0)
    {
        return 0;
    }
    else
    {
        int matrix[nbJobs + 1][nbMachines + 1];
        for (int j = 0; j <= nbMachines; j++)
            matrix[0][j] = 0;

        for (int i = 0; i <= nbJobs; i++)
            matrix[i][0] = 0;

        for (int i = 1; i <= nbJobs; i++)
        {
            for (int j = 1; j <= nbMachines; j++)
            {
                matrix[i][j] = A[solution[i - 1]][j - 1]; // because btw 0-19
                matrix[i][j] += (matrix[i - 1][j] < matrix[i][j - 1]) ? matrix[i][j - 1] : matrix[i - 1][j];
            }
        }
        return matrix[nbJobs][nbMachines];
    }
}
vector < pair <int, int> > U(int A[500][20], int nbJobs){
    vector< pair<int, int> > my_u;
    for (int j = 0 ; j < nbJobs ; j++){
        if (A[j][0]<A[j][1]){  my_u.push_back(std::make_pair(j, A[j][0]));}
    }
    sort(my_u.begin(),my_u.end(),cmp);
    return my_u;
}

vector < pair <int, int> > V(int A[500][20], int nbJobs){
    vector< pair<int, int> > my_u;
    for (int j = 0 ; j < nbJobs ; j++){
        if (A[j][0]>=A[j][1]){  my_u.push_back(std::make_pair(j, A[j][1]));}
    }
    sort(my_u.begin(),my_u.end(),cmp2);
    return my_u;
}
int Jon(int A[500][20], int nbJobs, int data[500][20])
{
    vector<int> solution;int cmax;
    vector< pair<int, int> > my_u;
    my_u=U(A,nbJobs);
    vector< pair<int, int> > my_u2;
    my_u2=V(A,nbJobs); 
    my_u.insert(my_u.end(), my_u2.begin(), my_u2.end());
    for(int i = 0; i < nbJobs; i++)
    {
        solution.push_back(my_u[i].first);
       
    }
    /*
    for (&it :solution)
    {
        cout<<it<<"\n";
    }*/
    cmax=Cmax(solution, data, 2);
    return cmax;
}
int eval_jhonson(int CActuel, int A[500][20], vector<int> LbMachines, list<int> J)
{
    int nbMachines=LbMachines.size()-1;
    int nbJobs=J.size(),cmax_j=INT32_MAX;
    int  Seq[500][20], somme1 = 0, somme2 =0, c ;
   
    for (int k = 0; k < (nbMachines - 1 ); k++)  // M-1 sequences
    {
        //construire tableau de nbJobs 2 machines 
        for (int i = 0; i < nbJobs; i++)
        {
            for (int j = 0; j <= k; j++) {somme1 = somme1 + A[i][j];}
            for (int j = nbMachines-1; j >= nbMachines-1-k; j--) {somme2 = somme2 + A[i][j];}
            Seq[i][0] = somme1;
            Seq[i][1] = somme2;
            somme1 = 0;   somme2 = 0;
        }

        //lui appliquer johnson 
        c = 0;
       
        int c=Jon(Seq, nbJobs, A);

        //mettre a jour Cmax si necessaire
        if ( c < cmax_j )
        {
            cmax_j = c;
            
        }
   

    }
    return CActuel+cmax_j;
}
int eval(int CActuel, int A[500][20], int nbMachines, list<int> J)
{
    int C = CActuel;
    for (auto &it : J)
        C += A[it][nbMachines - 1];
    return C;
}
int eval_max(int CActuel, int A[500][20], vector<int> LbMachines, list<int> J)
{
    int C = 0,max_c=0;
    int nbMachines=LbMachines.size();
    for (int i = 0; i < nbMachines; i++)
    {
        C=LbMachines[i+1];
    for (auto &it : J)
        C += A[it][i];
     if(max_c<C) max_c=C;
    
    }    
    return max_c;
}

// if newJob = -1 : 'L' contains a list of jobs whose Cmax is to be computed
// else, 'L' contains the exection times of the last job on all the machines ( + '0' in the first position) 
// OUTPUT: a vector of the newJob execution, with same structure as L
// NOTE: Please access to the Cmax using: res[nbMachines].
vector<int> Cmax_Add_Job(vector<int> L, int newJob, int A[500][20], int nbMachines)
{
    if( nbMachines == 0)
        return vector<int>(); // ERROR!
    
    vector<int> res(nbMachines+1);
    if( newJob == -1) // normal execution of Cmax but returning the last line of the matrix.
    {
        int nbJobs = L.size();
        int matrix[nbJobs + 1][nbMachines + 1];
        for (int j = 0; j <= nbMachines; j++)
            matrix[0][j] = 0;

        for (int i = 0; i <= nbJobs; i++)
            matrix[i][0] = 0;

        for (int i = 1; i <= nbJobs; i++)
        {
            for (int j = 1; j <= nbMachines; j++)
            {
                matrix[i][j] = A[L[i - 1]][j - 1]; // because btw 0-19
                matrix[i][j] += (matrix[i - 1][j] < matrix[i][j - 1]) ? matrix[i][j - 1] : matrix[i - 1][j];
            }
        }

        for (int j=0; j <= nbMachines; j++) {
            res[j] = matrix[nbJobs][j];
        }
        return res; 
    }
    else
    {
        res[0] = 0;
        for (int j = 1; j <= nbMachines; j++)
        {
            res[j] = A[newJob][j - 1]; // we suppose jobs start from 0
            res[j] += (L[j] < res[j - 1]) ? res[j - 1] : L[j];
        }
        return res;
    }
}
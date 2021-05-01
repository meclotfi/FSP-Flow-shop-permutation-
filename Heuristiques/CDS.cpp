#include "../FSP.h"

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

void johnson(int A[500][20], int nbJobs, int &cmax, vector<int> &solution, int data[500][20])
{
    vector< pair<int, int> > my_u;
    my_u=U(A,nbJobs);
    vector< pair<int, int> > my_u2;
    my_u2=V(A,nbJobs); 
    my_u.insert(my_u.end(), my_u2.begin(), my_u2.end());
    for(int i = 0; i < nbJobs; i++)
    {
        solution.push_back(my_u[i].first);
    }
    cmax=Cmax(solution, data, 2);
}
/////////////////////////////////////Main pour essai de  johnson ///////////////////////////////////////
// int main(int argc, char const *argv[])
// {
//     int A[500][20];
//     A[0][0]=20;
//     A[1][0]=10;
//     A[2][0]=15;
//     A[3][0]=32;
//     A[4][0]=3;
//     A[0][1]=55;
//     A[1][1]=23;
//     A[2][1]=5;
//     A[3][1]=4;
//     A[4][1]=45;
//     int cmax;
//     vector<int> solution ;
//     /*
//     vector< pair<int, int> > my_u;
//     my_u=U(A,5);
//     for(int i = 0; i < my_u.size(); i++)
//     {
//         cout << my_u[i].first << ", " << my_u[i].second << endl;
//     }
//     vector< pair<int, int> > my_u2;
//     my_u2=V(A,5);
//     for(int i = 0; i < my_u2.size(); i++)
//     {
//         cout << my_u2[i].first << ", " << my_u2[i].second << endl;
//     }
//     */
//    johnson(A, 5, cmax, solution, A);
//    for(int i = 0; i < solution.size(); i++)
//     {
//         cout << solution[i] << endl;
//     }
//     cout << cmax <<endl;
//     return 0;
// }
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//recuperer structure de donnees
//for m-1 sequence construire et appeler johnson qui retourne une sequence appeler cmax levaluer stocker result tableau de m-1 case et prendre le min 

int main(int argc, char const *argv[])
{
    int nbJobs, nbMachines, A[500][20], Seq[500][20], somme1 = 0, somme2 =0, Cmax, c = INT32_MAX; 
    vector<int> solution, solutionsauv;

    loader("../benchmarks/20jobs10machines.txt", &nbJobs, &nbMachines, A); 
 
    for (int k = 0; k < (nbMachines - 1 ); k++)
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
        Cmax = 0;
        solution.clear();
        johnson(Seq, nbJobs, Cmax, solution, A);
        
        if ( Cmax < c )
        {
            c = Cmax;
            solutionsauv = solution;
        }
    

    }
    //On aura obtenu le c qui est le cmax et solutionsauv qui contient la sequence
    for(int i = 0; i < nbJobs; i++)
       {
           cout << solutionsauv[i] << endl;
       }
       cout << c <<endl;
}
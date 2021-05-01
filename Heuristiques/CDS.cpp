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
    my_u=U(A,5);
    vector< pair<int, int> > my_u2;
    my_u2=V(A,5); 
    my_u.insert(my_u.end(), my_u2.begin(), my_u2.end());
    for(int i = 0; i < my_u.size(); i++)
    {
        solution.push_back(my_u[i].first);
    }
    cmax=Cmax(solution, data, 2);
}
int main(int argc, char const *argv[])
{
    int A[500][20];
    A[0][0]=20;
    A[1][0]=10;
    A[2][0]=15;
    A[3][0]=32;
    A[4][0]=3;
    A[0][1]=55;
    A[1][1]=23;
    A[2][1]=5;
    A[3][1]=4;
    A[4][1]=45;
    int cmax;
    vector<int> solution ;
    /*
    vector< pair<int, int> > my_u;
    my_u=U(A,5);
    for(int i = 0; i < my_u.size(); i++)
    {
        cout << my_u[i].first << ", " << my_u[i].second << endl;
    }
    vector< pair<int, int> > my_u2;
    my_u2=V(A,5);
    for(int i = 0; i < my_u2.size(); i++)
    {
        cout << my_u2[i].first << ", " << my_u2[i].second << endl;
    }
    */
   johnson(A, 5, cmax, solution, A);
   for(int i = 0; i < solution.size(); i++)
    {
        cout << solution[i] << endl;
    }
    cout << cmax <<endl;
    return 0;
}
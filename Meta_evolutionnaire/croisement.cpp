#include "../FSP.h"


void  Croisement1pt(vector<int> P1,vector<int> P2,int nbJobs,double proba, vector<int> &E1,vector<int> &E2)
{
   srand(time(0));
    double x;
    x = rand()/(RAND_MAX + 1.);
    cout << endl << "proba " << x <<endl;
    


    if (x <= proba){
        int y;
        y = 1 + (rand() / ( RAND_MAX / (nbJobs-1) ) ) ;  
        cout << endl << "alpha: " << y <<endl;
        for (int i=0;i<y;i++){
            E1.push_back(P1[i]);
            E2.push_back(P2[i]);
        }
        int ind1=y,ind2=y;
        for (int j=y;j<nbJobs;j++){
            if (std::count(E1.begin(),E1.end(), P2[j])){
                while(ind1<nbJobs && std::count(E1.begin(),E1.end(), P1[ind1])){
                    ind1++;
                }
                if (ind1 < nbJobs){
                    E1.push_back(P1[ind1]);
                }

            }else{
                E1.push_back(P2[j]);
            }

            if (std::count(E2.begin(),E2.end(), P1[j])){
                while(ind2<nbJobs && std::count(E2.begin(),E2.end(), P2[ind2])){
                    ind2++;
                }
                if (ind1 < nbJobs){
                    E2.push_back(P2[ind2]);
                }

            }else{
                E2.push_back(P1[j]);
            }

        }
    }
}



int main(int argc, char const *argv[])
{
    vector<int> P1;
    vector<int> P2;
    vector<int> E1;
    vector<int> E2;
    int nbJobs=10;
    double proba=0.9;
    for(int i=0;i<nbJobs;i++){
        P1.push_back(i);
        P2.push_back(nbJobs-i-1);
    }
    cout << endl <<"P1" << endl;
    for(int i = 0; i < nbJobs; i++)
       {
           cout << P1[i] ;
       }
    cout << endl <<"P2" << endl;
    for(int i = 0; i < nbJobs; i++)
       {
           cout << P2[i] ;
       }
    Croisement1pt(P1,P2,nbJobs,proba,E1,E2);
    cout << endl <<"E1" << endl;
    for(int i = 0; i < nbJobs; i++)
       {
           cout << E1[i] ;
       }
    cout << endl <<"E2" << endl;
    for(int i = 0; i < nbJobs; i++)
       {
           cout << E2[i] ;
       }
    return 0;
}
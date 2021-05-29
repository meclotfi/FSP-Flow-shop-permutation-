#include "../FSP.h"

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
    /*
    for (&it :solution)
    {
        cout<<it<<"\n";
    }*/
    //On en a pas besoin dans cds, si vous en avez besoin ailleur decommenter la
    cmax=99999;
    //cmax=Cmax(solution, data, 2);
}

void johnson_Cmax(int A[500][20], int nbJobs, int &cmax, vector<int> &solution, int data[500][20])
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

    
void  CDS(int A[500][20], int nbJobs, int nbMachines, int &cmax, vector<int> &solution)
{
    //Compare m-1 sequences de nbJobs 2 machines avec johnson et prend le meilleur Cmax

    int  Seq[500][20], somme1 = 0, somme2 =0, c ;
    vector<int> solutionsauv;

    cmax = INT32_MAX;

    for (int k = 0; k < (nbMachines - 1 ); k++)  // M-1 sequences
    {
        //construire tableau seq de nbJobs 2 machines 
        for (int i = 0; i < nbJobs; i++)
        {
            for (int j = 0; j <= k; j++) {somme1 = somme1 + A[i][j];}
            for (int j = nbMachines-1; j >= nbMachines-1-k; j--) {somme2 = somme2 + A[i][j];}
            Seq[i][0] = somme1;
            Seq[i][1] = somme2;
            somme1 = 0;   somme2 = 0;
        }

        //lui appliquer johnson 
        solutionsauv.clear();
        johnson(Seq, nbJobs, c, solutionsauv, A);
        //recalculer le c (cmax) de la sequence retournee par johnson sur nbmachines
        c = Cmax(solutionsauv, A, nbMachines);

        //mettre a jour Cmax si necessaire
        if ( c < cmax )
        {
            cmax = c;
            solution = solutionsauv;
        }

    }

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

int main2(int argc, char const *argv[])
{
    int nbJobs, nbMachines, A[500][20], cmax; 
    vector<int> solution;

    clock_t start, end;


    loader("../benchmarks/13J_5M.txt", &nbJobs, &nbMachines, A); 
    start = clock();
    CDS(A, nbJobs, nbMachines, cmax, solution);
    end = clock();
    double temps2 = double(end - start) / double(CLOCKS_PER_SEC);
    printf (" \n CDS time %f secondes\n", temps2);
    
   
    /*
    cout <<"Sequence" << endl;
    for(int i = 0; i < nbJobs; i++)
       {
           cout << solution[i] << endl;
       }*/
       cout << "Cmax : " << cmax <<endl;

    return 0;
        
}
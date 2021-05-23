#include "../FSP.h"


//Fonction de mutation par echange
void mutation_swap(vector<int> individu, int nbjobs, double proba_mut, vector<int> &indivMute) 
{
double d;
int alpha, beta;

srand(time(0));
d = rand()/(RAND_MAX + 1.);
//cout << "proba" << d <<endl;

if (d <= proba_mut)
{
    alpha = rand() / (RAND_MAX / (nbjobs-1) );
    beta = rand() / (RAND_MAX / (nbjobs-1) );
    while (alpha == beta){
    beta = rand() / (RAND_MAX / (nbjobs-1) );
    }
    //cout << "premiere position " << alpha << endl;
    //cout << "2eme position " << beta << endl;

    //permuter les jobs des deux positions alpha et beta dans indivMute
    indivMute = individu;
    std :: iter_swap(indivMute.begin() + alpha, indivMute.begin() + beta);
    
}

}

int main()
{
    vector<int> ind,indmute;
    int nbjobs = 20;
    double p_mutation = 1;  //la probabilite de mutation a 1 pour muter dans tous les cas
    for (int i = 0; i < nbjobs; i++)
    {
        ind.push_back(i);
    }
    cout << "lindividu" << endl;
    for (int i = 0; i < nbjobs; i++)
    {
        cout << ind[i] << endl;
    }
    mutation_swap(ind, nbjobs, p_mutation, indmute);
    cout << "lindividu muteeeeeee" << endl;
    for (int i = 0; i < nbjobs; i++)
    {
        cout << indmute[i] << endl;
    }
    

    return 0;
}
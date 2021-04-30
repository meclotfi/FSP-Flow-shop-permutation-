#include "../FSP.h"

// comparison function to sort vectors in descending order
bool cmp(pair<int, int> &a,
         pair<int, int> &b)
{
    return a.second > b.second;
}

int main(int argc, char const *argv[])
{
    int nbJobs, nbMachines, somme, A[500][20], c,currentCmax;
    vector<int> solution, currentSequence,saveSol;

    vector<pair<int, int>> sommeLigne;
    loader(&nbJobs, &nbMachines, A);

    // Get the sum of each line ( duration of each job in all machines )
    for (int i = 0; i < nbJobs; i++)
    {
        somme = 0;
        somme = accumulate(A[i], A[i] + nbMachines, somme);
        sommeLigne.push_back(std::make_pair(i, somme));
    }

    // Sort in descending order the sums
    sort(sommeLigne.begin(), sommeLigne.end(), cmp);

    // Push the first jobs in the solution sequence
    solution.push_back(sommeLigne[0].first);

    // add a jobs at a time
<<<<<<< HEAD
<<<<<<< HEAD
    for (size_t i = 1; i < solution.size(); i++)
=======
    for (size_t i = 1; i < sommeLigne.size(); i++)
>>>>>>> 21e3a5134b228b745b7693327b5ab64b500dd042
=======
    for (size_t i = 1; i < sommeLigne.size(); i++)
>>>>>>> 21e3a5134b228b745b7693327b5ab64b500dd042
    {
        c=INT32_MAX;
        currentSequence=solution;
        saveSol=currentSequence;

        // Compare all the combinations
        for (int j = 0; j < i+1; j++)
        {

            currentSequence.insert(currentSequence.begin()+j,sommeLigne[i].first);
            currentCmax=Cmax(solution,A,nbMachines);

            // compare the makespans
            if (currentCmax<= c)
            {
                solution=currentSequence;
                c= currentCmax;
            }
            currentSequence=saveSol;
        }
        
    }
<<<<<<< HEAD
<<<<<<< HEAD
    
    for(auto var : solution)
    {
        std::cout << var+1 << std::endl;
    }

=======
=======
>>>>>>> 21e3a5134b228b745b7693327b5ab64b500dd042

    for(auto var : solution)
    {
        std::cout << var+1 << " ";
    }
    std::cout << ""<< std::endl;
<<<<<<< HEAD
>>>>>>> 21e3a5134b228b745b7693327b5ab64b500dd042
=======
>>>>>>> 21e3a5134b228b745b7693327b5ab64b500dd042
    std::cout << "Cmax : " << Cmax(solution,A,nbMachines) << std::endl;
    return 0;
}
 // C++ code to demonstrate copy of vector using iterative method
    #include<iostream> 
    #include<vector> //to include vector container in code
    #include <list>
    #include <algorithm>
    using namespace std; 

vector<vector<int>> AdjacentExchange(vector<int> Sol, int nbJobs){
    vector<vector<int>> Neighborhood;
    int inter ;
    for (int i = 0; i < nbJobs - 1; i++){
        Neighborhood.push_back(Sol);
        inter = Neighborhood[i][i];
        Neighborhood[i][i] = Neighborhood[i][i+1];
        Neighborhood[i][i+1] = inter;
    }

    return Neighborhood;
}

    int main() 
    { 
        // Initializing vector with values 
        vector<int> v1;
        int num;
        for(int i=0;i<4;i++)
        {
            v1.push_back(i);// adding elements to vector v1
        }

        // Declaring new vector 
        vector<int> v2; 

        // A loop to copy elements of old vector into new vector 
        //by Iterative method 
        for (int i=0; i<v1.size(); i++) 
            v2.push_back(v1[i]); 
        
        v2[2] *= 2;
        v1.push_back(5);

        cout << "Old vector elements are : "; 
        for (int i=0; i<v1.size(); i++) 
            cout << v1[i] << " "; 
        cout << endl; 

        cout << "New vector elements are : "; 
        for (int i=0; i<v2.size(); i++) 
            cout << v2[i] << " "; 
        cout<< endl; 

        vector<vector<int>> s = AdjacentExchange(v1, 4);
        for (int i=0; i<3; i++) 
            for (int j=0; j<4; j++) 
                cout << s[i][j] << " "; 

        cout<< endl;

        list<int> L;
        L.push_back(2);

        vector<int> v = {2, 3 ,6, 7,1, 9,34,214,47,22,79,42,12,67,21,65,8,4,44,666,879,34,214,47,22,79,42,12,34,214,47,22,79,42,12,2, 3 ,6, 7,1, 9,34,214,47,22,79,42,12,67,21,65,8,4,44,666,879,34,2, 3 ,6, 7,1, 9,34,214,47,22,79,42,12,67,21,65,8,4,44,666,879,34,2, 3 ,6, 7,1, 9,34,214,47,22,79,42,12,67,21,65,8,4,44,666,879,34,214,47,22,79,42,12,34,214,47,22,79,42,12,2, 3 ,6, 7,1, 9,34,214,47,22,79,42,12,67,21,65,8,4,44,666,879,34,2, 3 ,6, 7,1, 9,34,214,47,22,79,42,12,67,21,65,8,4,44,666,879,34};
        //   std::shuffle(v.begin(), v.end());
        std::random_shuffle(v.begin(), v.end());
        for (int i=0; i<v.size(); i++) 
            cout << v[i] << " "; 
        printf("\nsize : %ld\n",v.size());
 
    std::cout << "\n";
        for(int ll = 0; ll < 200; ll++)
        {
            printf(" %d, ", rand());
        }
    
        return 0; 
    } 

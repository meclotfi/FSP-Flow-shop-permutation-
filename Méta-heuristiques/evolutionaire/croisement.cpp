

void Croisement1pt(vector<int> P1, vector<int> P2, int nbJobs, double proba, vector<int> &E1, vector<int> &E2)
{
    double x;
    x = rand() / (RAND_MAX + 1.);

    if (x <= proba)
    {
        int y;
        y = 1 + (rand() / (RAND_MAX / (nbJobs - 1)));
        for (int i = 0; i < y; i++)
        {
            E1.push_back(P1[i]);
            E2.push_back(P2[i]);
        }
        int ind1 = y, ind2 = y;
        for (int j = y; j < nbJobs; j++)
        {
            if (std::count(E1.begin(), E1.end(), P2[j]))
            {
                while (ind1 < nbJobs && std::count(E1.begin(), E1.end(), P1[ind1]))
                {
                    ind1++;
                }
                if (ind1 < nbJobs)
                {
                    E1.push_back(P1[ind1]);
                }
            }
            else
            {
                E1.push_back(P2[j]);
            }

            if (std::count(E2.begin(), E2.end(), P1[j]))
            {
                while (ind2 < nbJobs && std::count(E2.begin(), E2.end(), P2[ind2]))
                {
                    ind2++;
                }
                if (ind2 < nbJobs)
                {
                    E2.push_back(P2[ind2]);
                }
            }
            else
            {
                E2.push_back(P1[j]);
            }
        }
    }
}

void Croisement2pts(vector<int> P1, vector<int> P2, int nbJobs, double proba, vector<int> &E1, vector<int> &E2)
{
    double x;
    x = rand() / (RAND_MAX + 1.);

    if (x <= proba)
    {
        int y = 0, z = 0;
        while (y == z)
        {
            y = 1 + (rand() / (RAND_MAX / (nbJobs - 2)));
            z = 1 + (rand() / (RAND_MAX / (nbJobs - 2)));
        }
        if (y > z)
        {
            int temp = z;
            z = y;
            y = temp;
        }
        for (int i = 0; i < y; i++)
        {
            E1.push_back(P1[i]);
            E2.push_back(P2[i]);
        }
        for (int i = y; i < nbJobs; i++)
        {
            E1.push_back(nbJobs + 10);
            E2.push_back(nbJobs + 10);
        }
        for (int i = z + 1; i < nbJobs; i++)
        {
            E1.at(i) = P1[i];
            E2.at(i) = P2[i];
        }
        int ind1 = y, ind2 = y;
        for (int j = y; j <= z; j++)
        {
            if (std::count(E1.begin(), E1.end(), P2[j]))
            {
                while (ind1 <= z && std::count(E1.begin(), E1.end(), P1[ind1]))
                {
                    ind1++;
                }
                if (ind1 <= z)
                {
                    E1.at(j) = P1[ind1];
                }
            }
            else
            {
                E1.at(j) = P2[j];
            }

            if (std::count(E2.begin(), E2.end(), P1[j]))
            {
                while (ind2 <= z && std::count(E2.begin(), E2.end(), P2[ind2]))
                {
                    ind2++;
                }
                if (ind2 <= z)
                {
                    E2.at(j) = P2[ind2];
                }
            }
            else
            {
                E2.at(j) = P1[j];
            }
        }
    }
}

/*
int main(int argc, char const *argv[])
{
    srand(time(0));
    vector<int> P1;
    vector<int> P2;
    vector<int> E1;
    vector<int> E2;
    vector<int> E3;
    vector<int> E4;
    int nbJobs = 5;
    double proba = 1;/*
    for (int i = 0; i < nbJobs; i++)
    {
        P1.push_back(i);
        P2.push_back(nbJobs - i - 1);
    }/
    P1.push_back(4);
    P1.push_back(1);
    P1.push_back(3);
    P1.push_back(2);
    P1.push_back(0);

    P2.push_back(2);
    P2.push_back(3);
    P2.push_back(1);
    P2.push_back(0);
    P2.push_back(4);


    cout << endl
         << "P1" << endl;
    for (int i = 0; i < nbJobs; i++)
    {
        cout << P1[i];
    }
    cout << endl
         << "P2" << endl;
    for (int i = 0; i < nbJobs; i++)
    {
        cout << P2[i];
    }
    Croisement1pt(P1, P2, nbJobs, proba, E1, E2);
    Croisement2pts(P1, P2, nbJobs, proba, E3, E4);
    cout << endl
         << "Croisement 1 point" << endl
         << "E1" << endl;
    for (int i = 0; i < nbJobs; i++)
    {
        cout << E1[i];
    }
    cout << endl
         << "E2" << endl;
    for (int i = 0; i < nbJobs; i++)
    {
        cout << E2[i];
    }
     cout << endl
         << "Croisement 2 points" << endl
         << "E3" << endl;
    for (int i = 0; i < nbJobs; i++)
    {
        cout << E3[i];
    }
    cout << endl
         << "E4" << endl;
    for (int i = 0; i < nbJobs; i++)
    {
        cout << E4[i];
    }
    return 0;
} */
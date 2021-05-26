

/*
void remplacement(int taille, multimap<double,vector<int> > &P, multimap<double,vector<int> > E){
    multimap<double,vector<int> > newP;
    multimap<double,vector<int> > pool;
  //  int sizeP = P.size();
  //  int sizeE = E.size();
    multimap<double, vector<int> >::iterator it;
/*
    for (int i=0; i<sizeP; i++){
        pool.insert(P[i]);
    }/
    for (it = P.begin(); it != P.end(); ++it) 
    {
            pool.insert(make_pair(it->first,it->second));
    }
    for (it = E.begin(); it != E.end(); ++it) 
    {
            pool.insert(make_pair(it->first,it->second));
    }
    
    it = pool.end();
    for (int i=0;i<taille; i++){
      --it;
      newP.insert(make_pair(it->first,it->second));
    }
    P=newP;
} */

void remplacement(int taille, multimap<double, vector<int>> &newP, multimap<double, vector<int>> pool)
{
  multimap<double, vector<int>>::iterator it;

  it = pool.end();

  for (int i = 0; i < taille; i++) // prendre les N meilleurs individus
  {
    --it;
    newP.insert(make_pair(it->first, it->second));
  }
}

/*
int main(int argc, char const *argv[])
{
  multimap<double, vector<int>> population;
  multimap<double, vector<int>> population2;
  vector<int> parent;
  multimap<double, vector<int>>::iterator it;

  population.insert(make_pair(9.0, parent));
  population.insert(make_pair(7.0, parent));
  population.insert(make_pair(3.0, parent));
  population.insert(make_pair(1.0, parent));
  population.insert(make_pair(5.0, parent));

  population2.insert(make_pair(4.0, parent));
  population2.insert(make_pair(2.0, parent));
  population2.insert(make_pair(6.0, parent));
  population2.insert(make_pair(8.0, parent));
  population2.insert(make_pair(0.0, parent));

  remplacement(5, population, population2);

  cout << endl;

  for (it = population.begin(); it != population.end(); ++it)
  {
    cout << it->first << endl;
  }
  /*
    sort(population.begin(),population.end(),cmp3);


   cout << endl;

 for (it = population.begin(); it != population.end(); ++it) 
    {
      cout   << it->first << endl;
    }
   
/
  return 0;
}
*/
//////////////////////////////////////////////////////////////////////////
// program posiada 3 funkcje:
// wczytywania danych, zapisywania danych, rozwiazywania problemu
// przeplywowego algorytmem NEH z akceleracja
// posiada rowniez funkcje pomocnicze oraz odpowiednie struktury danych
// program wyswietla czas dzialania algorytmu, zmienna cmax nie wyswietlana
// dla kolejnych instancji przechowuje cmax
// kompilacja: Microsoft Visual Studio 2010 

#include <iostream>
#include <fstream>
#include <vector>
#include <list>
#include <stdio.h>
#include <algorithm>
#include <cstdlib>
#include <Windows.h>

#define INSTANCE_AMOUT 120
#pragma warning(disable: 4996)

using namespace std;

struct JobsData
{ 
 int no;
 int time;
};

struct AllData
{
 int **Tab_Job;
 int jobs; //liczba zadan
 int machines; //liczba maszyn
 list<JobsData> Jobs_List;  //lista zadan posiada nr zadania i calkowity czas potrzebny do spedzenia na maszynach
 vector<int>solution; // tablica uszeregowan czasow zadan
};

bool compare(const JobsData& _jd1, const JobsData& _jd2)
{ 
    return _jd1.time > _jd2.time; 
}

int clear_matrix(int **Tab,int x, int y);
void print_matrix(int **Tab, int x, int y);

/////////////////////////////////////////////////
// zamien vector'y
void swap_V(int &x,int &y);
//
/////////////////////////////////////////////////

/////////////////////////////////////////////////
//  funkcja wczytuje dane z odpowiedniego pliku
// bench_fs.txt
// PRE: AllData - wskaznik na strukture danych 
//      z elementami niezbednymido wykonania 
//  algorytmu,
//  pFile - wskaznik na otwarty plil
// POST: brak
//
void GetData(AllData *D, FILE *pFile);
//
/////////////////////////////////////////////////

/////////////////////////////////////////////////
//  funkcja wczytuje dane z odpowiedniego pliku
// bench_fs.txt
// PRE: AllData - wskaznik na strukture danych 
//      z tablica wynikowa solution
//  pFileOut - wskaznik na plik do zapisu
// POST: brak
//
void ToFile(AllData *W, FILE *pFileOut);
//
/////////////////////////////////////////////////

////////////////////////////////////////////////
// funkcja implementujaca algorytm NEH
// z akceleracja
// PRE: AllData - wskaznik na strukture danych
//      z elementami niezbednymi do wykonania
//  algorytmu NEH
// POST: brak
void DoNEH_acc(AllData *N);
//
////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////
// main
int main()
{ 
 __int64 start, stop, freq;

 AllData *ins = new AllData[INSTANCE_AMOUT];
 FILE *pFile; 
 pFile=fopen("bench_fs.txt","r");
 
 for(int instance = 0; instance < INSTANCE_AMOUT; instance++) // for dla kolejnych instancji
  {
   GetData(ins,pFile); //wczytaj dane z pliku
   ins++;
     }
 fclose(pFile);
 ins=ins-INSTANCE_AMOUT;

 QueryPerformanceCounter((LARGE_INTEGER*)&start);

 for(int instance = 0; instance < INSTANCE_AMOUT; instance++)
 {
   DoNEH_acc(ins); //uszereguj je NEH'em
   ins++;
 }

 QueryPerformanceCounter((LARGE_INTEGER*)&stop);  
 QueryPerformanceFrequency((LARGE_INTEGER*)&freq);
 cout<<" "<<endl;
 cout<<"time: " << ((double)(stop-start))/freq << "s" << endl; 

 ins=ins-INSTANCE_AMOUT;
 FILE *pFileOut;
 pFileOut=fopen("data_out.txt","w");



 for(int instance = 0; instance < INSTANCE_AMOUT; instance++)
 {
   ToFile(ins,pFileOut); //zapisz dane do pliku
   ins++;
 }
 fclose(pFileOut);
 
 getchar();
 return 0;
}
// koniec main
//////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////
//  GetData
void GetData(AllData *D, FILE *pFile)
{
 int E_time;//suma czasow jednego zadania na wszystkich maszynach
 int time; //czas zadania na maszynie
 int nvm;//pomijana
 char iname[6];//nazwa instancji w pliku 
 JobsData Job; //dane zadania

 if(pFile == NULL){fprintf(stderr,"Blad: Nie podano uchwytu do pliku\n");exit(-1);}

   fscanf(pFile,"%s",iname);
   fscanf(pFile,"%d",&D->jobs);
   fscanf(pFile,"%d",&D->machines);

   D->Tab_Job = new int * [D->machines]; // i-zadan na j-maszynach
    for (int j = 0; j < D->machines; j++){
     D->Tab_Job[j] = new int[D->jobs];
     }

   for(int i = 0; i < D->jobs; i++)
   {
     E_time = 0; 
    for(int j = 0; j < D->machines; j++)
    {
     fscanf(pFile,"%d",&nvm);
     fscanf(pFile,"%d",&time);
     E_time = E_time + time;
     D->Tab_Job[j][i] = time;

    }
    Job.no = i+1;
    Job.time = E_time;
    D->Jobs_List.push_back(Job);
   }

  


  
}
// koniec funkcji GetData
/////////////////////////////////////////////////

/////////////////////////////////////////////////
// ToFile
void ToFile(AllData *W, FILE *pFileOut)
{
 for(int i = 0; i < W->jobs; i++)
 {

  fprintf(pFileOut,"%d ", W->solution[i]);

 }
 fprintf(pFileOut,"\n");
}
// koniec ToFile
/////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// NEH z akceleracja
void DoNEH_acc(AllData *N)
{
 int max, min;
 int _dict = 0;
 int new_job;
 int cmax;
 int tmp_job;
 vector<int>max_vect; 
 N->solution.clear();
 N->Jobs_List.sort(compare); //mam uszeregowane zadania ze zgledu na czas
 N->solution.push_back(N->Jobs_List.front().no);// dodaje do tablicy 1 element posortowanej listy zadan 
 N->Jobs_List.pop_front(); // sciagam ten element z listy, na liscie o jedno zadanie mniej


 int **E = new int*[N->machines+1]; //tworze macierz E
  for (int i = 0; i < N->machines+1; i++)
  {
   E[i] = new int[N->jobs];
  }

 int **Q= new int*[N->machines+1]; //tworze macierz Q
  for (int i = 0; i < N->machines+1; i++)
  {
   Q[i] = new int[N->jobs];
  }
 int **F= new int*[N->machines+1]; //tworze macierz F
  for (int i = 0; i < N->machines+1; i++)
  {
   F[i] = new int[N->jobs];
  }

 int **F_Q= new int*[N->machines+1]; //tworze macierz F
  for (int i = 0; i < N->machines+1; i++)
  {
   F_Q[i] = new int[N->jobs];
  }
  clear_matrix(E,N->machines+1,N->jobs); //
  clear_matrix(Q,N->machines+1,N->jobs); // wypelniam je zerami
  clear_matrix(F,N->machines+1,N->jobs); //
  clear_matrix(F_Q,N->machines+1,N->jobs);
///////////////////////////////////////////////////////////////////////
// poczatek petli dla kazdego zadania
 while(N->Jobs_List.size() != 0)
 {
 max_vect.clear();
 for(int i=0; i < (int)N->solution.size(); i++) ////////////////////////////////////////////////////////////////////////////////// matrix E
  {
   for(int j=0; j < N->machines; j++)// w pierwszej kolumnie
   {
    
    E[j+1][i+1] = max(E[j][i+1],E[j+1][i]) + N->Tab_Job[j][N->solution[i]-1];
   }

  }
 for(int i = (int)N->solution.size()-1; i >= 0; i--) ////////////////////////////////////////////////////////////////////////////////// matrix Q
  {
   for(int j = N->machines-1; j >= 0; j--)// w pierwszej kolumnie
   {
    
    Q[j][i] = max(Q[j+1][i],Q[j][i+1]) + N->Tab_Job[j][N->solution[i]-1];
   }

  }
 
 new_job = N->Jobs_List.front().no; // sciagam nowe zadanie
 N->Jobs_List.pop_front();          //
 
 for(int i=0; i < (int)N->solution.size()+1; i++) ////////////////////////////////////////////////////////////////////////////////// matrix F
  {
   for(int j=0; j < N->machines; j++)// w pierwszej kolumnie
   {
    
    F[j+1][i] = max(E[j+1][i],F[j][i]) + N->Tab_Job[j][new_job-1];
   }

  }
 for(int i=0; i < (int)N->solution.size()+1; i++) ////////////////////////////////////////////////////////////////////////////////// matrix F_Q
  {
   for(int j=0; j < N->machines; j++)// w pierwszej kolumnie
   {
    
    F_Q[j][i] = Q[j][i]+F[j+1][i];
   }

  }

 for(int i=0; i < (int)N->solution.size()+1; i++) // znajduje maxa poszczegolnych kolumn w macierzy F+Q
  {
   max = 0;
   for(int j=0; j < N->machines; j++)
   {
    if(F_Q[j][i] > max) {max = F_Q[j][i];}
   }
   max_vect.push_back(max);
  }

 min=999999;
 for(int i=0; i < (int)max_vect.size(); i++)
 {
  if(max_vect[i]<min){ min = max_vect[i];
  _dict = i;} // _slownik_it zeby znac indeks na ktory mam wstawic to zadanie
 }
 cmax = min;
 
 N->solution.push_back(0);
 for(int i = _dict; i < (int)N->solution.size(); i++)
 {
  swap_V(N->solution[i],tmp_job);
 }
 N->solution[_dict] = new_job;


} //koniec while
} // koniec funkcji
//
/////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////
// funkcje dodatkowe
void swap_V(int &x,int &y) //swap4vector
{
 int temp;
 temp=x;
 x=y;
 y=temp;
}

int clear_matrix(int **Tab,int x, int y)
{
 for(int i = 0; i < x; i++)
  for(int j = 0; j < y; j++)
   Tab[i][j]=0;
 return 0;
}

void print_matrix(int **Tab, int x, int y)
{
 for(int i = 0; i < x; i++){ 
  for(int j = 0; j < y; j++)
  {cout<<Tab[i][j];}
  cout<<" "<<endl;}
}
//
/////////////////////////////////////////////////////////////////////
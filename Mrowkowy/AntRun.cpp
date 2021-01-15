//---------------------------------------------------------------------------

#pragma hdrstop

#include "AntRun.h"
#include <math.h>
#include <list>
#include <set>
#include <cstdlib>
#include <algorithm>
#include <iostream>     // std::cout
#include <fstream>
#include <string>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <vector>
using namespace std;

vector<int> path;
vector<int> x;
vector<int> y;

//---------------------------------------------------------------------------
#pragma package(smart_init)

#define max(a,b) ((a)>(b)?(a):(b))

void ReadLocation( void );

void PheromoneGen( void );
void PheromoneEvaporation( void );
void PheromoneAdd( int EndNode, int StartNode);

int  Random( int MaxVlue );
double NodesDistance( int StartNode, int EndNode );
bool VectorFind( std::set<int> & Vector, int Value );

double FindWay( int AntFinderNumber );
double AntPleaseFindWay( int StartNode, int Repetition );
int AntFindNextNode( int CurrentNode, std::set<int> & ToVisitNodes );

//----------------------------------------------------------------
// parametry
int    PheromonByWay = 1000;       // waga feromonow, dzielona przez odl.
int    AntFinderNumber = 20000;   // l.mrowek szukajacych sciezki
double PheromoneEvaporationByWay = 0.02;  // ulot feromonu na sciezke
double DeterministicLevel = 0.7;

double alpha = 2;             // potega - wspolczynnik feromony
double beta = 0.8;            // potega - wspolczynnik drogi

//----------------------------------------------------------------
// zmienne globalne
int* Locations = NULL; // nr wezla, X, Y
double* Pheromons; // tablica feromonow
int    NodeNumber = 0;           // l.wezlow w grafie

//----------------------------------------------------------------
// funkcja obliczajaca dlugosc najkrotszej sciezki
double AntRun( void )
{
   path = {};
   x = {};
   y = {};
   //1. odczyt wezeow
    ReadLocation();

   //2. generuj wstepne feromony
    Pheromons = new double [NodeNumber * NodeNumber];
   PheromoneGen();

   //3. glowna petla
   double MinWay = FindWay( AntFinderNumber );

   for(int i=0; i<x.size(); i++) {
      cout << x[i] << " "; 
   }
   cout << endl;
   for(int i=0; i<y.size(); i++) {
      cout << y[i] << " "; 
   }
   cout << endl;
   for(int i=0; i<path.size(); i++) {
      cout << path[i] << " "; 
   }
   cout << endl;

   delete[] Pheromons;
   if( Locations ) delete[] Locations;
   return MinWay;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
// odczyt pliku do tablicy Locations
void ReadLocation(void)
{
   ifstream myfile("berlin52.txt");
   if( myfile.is_open() )
      {
      std::string line;
      int LineNo = 0;
      while( std::getline( myfile, line ) )
        {
         if( !LineNo )
            {
            sscanf_s( line.c_str(), "%d", &NodeNumber );
            if( NodeNumber )
               {
               Locations = new int [ NodeNumber * 3 ];
               memset( Locations, 0, NodeNumber * 3 * sizeof(int) );
               }
            else
               exit(1);
            }
         else if( LineNo <= NodeNumber )
            {
            int No, X, Y;
            sscanf_s(line.c_str(), "%d %d %d", &No, &X, &Y );
            Locations[ (LineNo-1)*3 + 0 ] = No;
            Locations[ (LineNo-1)*3 + 1 ] = X;
            Locations[ (LineNo-1)*3 + 2 ] = Y;
           }
         LineNo ++;
         }
      myfile.close();
    }
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

// inicjowanie tablicy feromonow PheromonByWay/odleglosc
void PheromoneGen( void )
{
    memset( Pheromons, 0, NodeNumber * NodeNumber * sizeof(double) );

   for(int i = 0; i < NodeNumber; i++)
      for(int j = 0; j < NodeNumber; j++)
         PheromoneAdd(i,j);
}
//---------------------------------------------------------------------------

void PheromoneAdd( int StartNode, int EndNode )
{
   if( StartNode != EndNode )
      {
      Pheromons[ (StartNode * NodeNumber) + EndNode ] +=
         (double)PheromonByWay / NodesDistance( StartNode, EndNode );
      Pheromons[ (EndNode * NodeNumber) + StartNode ] = Pheromons[ (StartNode * NodeNumber) + EndNode ];
      }
}
//---------------------------------------------------------------------------

// ulot feromonow - stala PheromoneEvaporationByWay
void PheromoneEvaporation( void )
{
   for( int idx=0; idx < NodeNumber * NodeNumber; idx ++ )
      {
      if( Pheromons[ idx ] >= PheromoneEvaporationByWay )
         Pheromons[ idx ] -= PheromoneEvaporationByWay;
      }
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//3. glowna petla
double FindWay( int AntFinderNumber ) // w parametrze l. mrowek
{
   double AntWay, MinWay = 0;
   while( AntFinderNumber )      // uwaga - wezel startowy losowany, bez powtorzen (nietestowany)
      {
      if( (AntWay = AntPleaseFindWay( Random( NodeNumber )-1, 1)) < MinWay || MinWay==0)
         MinWay = AntWay;
      AntFinderNumber --;
      }
   return MinWay;
}

double AntPleaseFindWay( int StartNode, int Repetition )  // przejecie mrowki z l. powtorzen
{
   vector<int> temp_x = {};
   vector<int> temp_y = {};
   vector<int> temp_path = {};
   double MinWay = 0;
   while( Repetition > 0 )
      {
      int NextNode, CurrentNode = StartNode;
      double AntWay = 0;
      std::set<int> ToVisitNodes;  // typ set/zbior - lista wezlow do odwiedzenia, zmniejszany w trakcie spaceru

      for(int i = 0; i < NodeNumber; i++) // incjacja listy - wszystkie wezly
         ToVisitNodes.insert(i);

      temp_path.push_back(StartNode);
      temp_x.push_back(Locations[StartNode*3 + 1]);
      temp_y.push_back(Locations[StartNode*3 + 2]);
      ToVisitNodes.erase( ToVisitNodes.find(StartNode) ); // StartNode - pierwszy usuwam, bo w nim jestem
      while( ToVisitNodes.size() > 0 )
         {
         NextNode = AntFindNextNode( CurrentNode, ToVisitNodes );
         if( NextNode == CurrentNode )
            break;  //---------------------------------> !!!!
         else
            {
            AntWay += NodesDistance( CurrentNode, NextNode );
            PheromoneAdd(CurrentNode,NextNode);

            ToVisitNodes.erase( ToVisitNodes.find(NextNode ) );

            temp_path.push_back(NextNode);
            temp_x.push_back(Locations[NextNode*3 + 1]);
            temp_y.push_back(Locations[NextNode*3 + 2]);

            CurrentNode = NextNode ;
            }
         }
      // domkniecie sciezki
      AntWay += NodesDistance( CurrentNode, StartNode );
      PheromoneAdd(CurrentNode,StartNode);

      temp_path.push_back(StartNode);
      temp_x.push_back(Locations[StartNode*3 + 1]);
      temp_y.push_back(Locations[StartNode*3 + 2]);

      // ulot feromonow na cykl
      PheromoneEvaporation();
      Repetition --;

      if( MinWay > AntWay || MinWay == 0 )
          MinWay = AntWay;
          path = temp_path;
          x = temp_x;
          y = temp_y;
      }
   return MinWay;
}

// okreslenie kolejnego wezla
int AntFindNextNode( int CurrentNode, std::set<int> & ToVisitNodes )
{
   int NextNode = CurrentNode;
   double ProbabilityLevel = (double)Random( 100 )/100;
   if( ProbabilityLevel <= DeterministicLevel ) // 0,70  parametr wej�ciowy
      {   // przypadek rz�dzi
      double Probability, Licznik, Mianownik = 0;

      for( std::set<int>::iterator it=ToVisitNodes.begin(); it!=ToVisitNodes.end(); ++it )
         Mianownik += pow( Pheromons[ (CurrentNode * NodeNumber) + *it ], alpha) +
                      pow( 1/NodesDistance( CurrentNode, *it ), beta );

      // wyszukaj kolejny wg prawdopodobienstwa
      Probability = 0;
      for( std::set<int>::iterator it=ToVisitNodes.begin(); Probability < ProbabilityLevel && it!=ToVisitNodes.end(); ++it )
         {
         Probability += (
            pow( Pheromons[ (CurrentNode * NodeNumber) + *it ], alpha ) +
            pow( 1/NodesDistance( CurrentNode, *it ), beta ) ) / Mianownik;
         NextNode = *it;
         }
      }
   else
      {   // szukaj najlepszej wg feromonow
      double Max = -1;
      for( int idx=0; idx < NodeNumber; idx++ )
         {
         if( ToVisitNodes.find(idx) != ToVisitNodes.end() && Pheromons[ (CurrentNode * NodeNumber) + idx ] > Max )
            {
            NextNode = idx;
            Max = Pheromons[ (CurrentNode * NodeNumber) + idx ];
            }
         }
      }
   return NextNode;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

double NodesDistance( int StartNode, int EndNode )
{
   return pow(
      pow( Locations[StartNode*3 + 1] - Locations[EndNode*3 + 1], 2 ) +
      pow( Locations[StartNode*3 + 2] - Locations[EndNode*3 + 2], 2 ), 1.00/2 );
}

int Random( int MaxValue )
{
  int Value = rand();
  return (Value % MaxValue) + 1; // generator liczb z zkresu 1 do MaxVlue
}
//---------------------------------------------------------------------------

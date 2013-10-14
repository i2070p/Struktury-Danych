#include <cstdlib>
#include <iostream>

using namespace std;



class Element {
      public:
             Element() {
                       liczba=0;
                       znak=0;
                       liczba_d=0;
             };    
             ~Element(){};
             void przepisz(Element);
             void ustaw(int, char, double);
             int liczba;
             char znak;
             double liczba_d;
};

void Element::przepisz(Element e) {
     liczba=e.liczba;
     znak=e.znak;
     liczba_d=e.liczba_d;
};

void Element::ustaw(int _liczba, char _znak, double _liczba_d) {
     liczba=_liczba;
     znak=_znak;
     liczba_d=_liczba_d;
};

class TablicaE {
      public:
             TablicaE() {};
             TablicaE(int);
             ~TablicaE() {};
             void wyswietl(int);
             void wyswietlWszystko();
             void kasowanie();
             void sortuj();
             int zliczZnak(char);
      private:
             Element **tab;  
             int liczba_elementow;
};

void TablicaE::wyswietlWszystko() {
  for (int i=0; i<liczba_elementow; i++) wyswietl(i);   
}

void TablicaE::sortuj() {
  int n = liczba_elementow;
  do {
      for (int i=0; i<n-1; i++) {
          if (tab[i]->liczba>tab[i+1]->liczba) {
             Element *tmp = tab[i];
             
             tab[i]=tab[i+1];
             
             tab[i+1]=tmp;
          }
      }
      n--;
  } while (n>1); 
}

void TablicaE::kasowanie() {
     if (tab != NULL) {
       for (int i=0; i<liczba_elementow; i++) tab[i]->~Element();  
       delete [] tab;    
     } 
     tab=NULL;     
}

int TablicaE::zliczZnak(char _znak) {
    int wynik=0;
    for (int i=0; i<liczba_elementow; i++) if (tab[i]->znak == _znak) wynik++;
    return(wynik);   
}

void TablicaE::wyswietl(int id) {
     
     if (id>=liczba_elementow && tab != NULL) cout << "Element o podanym ID nie istnieje"; 
     else cout << "Znak: " << (int)tab[id]->znak << ", Liczba: " << tab[id]->liczba << ", Liczba (double): " << tab[id]->liczba_d << endl;
}


TablicaE::TablicaE(int ilosc) {
      srand(time(0));
      tab = new Element*[ilosc];
      liczba_elementow=ilosc;
      for (int i=0; i<ilosc; i++) {
          tab[i]=new Element();
      
          tab[i]->ustaw(rand()% ilosc, rand() % 256, rand() % 1000);    
      }
};



int main(int argc, char *argv[])
{
    TablicaE *ob1 = new TablicaE(20);
    ob1->wyswietlWszystko();
    ob1->sortuj();
    cout << "___________" << endl;
    ob1->wyswietlWszystko();
    cout << endl <<ob1->zliczZnak('A') << endl;
    ob1->kasowanie();

   /* TablicaE *ob2 = new TablicaE(10000);
    ob2->sortuj();
    cout << endl <<ob2->zliczZnak('B') << endl;
    ob2->kasowanie();
    
    TablicaE *ob3 = new TablicaE(100000);
    ob3->sortuj();
    cout << endl <<ob3->zliczZnak('C') << endl;
    ob3->kasowanie();
    */
    system("PAUSE");
    return EXIT_SUCCESS;
}

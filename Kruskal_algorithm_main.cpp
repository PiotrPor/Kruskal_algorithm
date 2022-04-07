// ASD2gr3_PorembinskiP_turystyka.cpp
/*
  1. Wszystkie trasy posortuj od najwiekszej wagi do najmniejszej
  2. Zrob drzewo rozpinajace z tras za pomoca algorytmu Kruskala
  3. Wielka petla
  3.1. Poznaj numery miast, ktore sa punktem startu i docelowym
  3.2. W tak zrobionym drzewie zrob "Depth-first Search", by poznac liste i kolejnosc miast, przez ktore trzeba przejechac 
  3.3. Znajac liste miast uloz liste drog, z ktorych sklada sie cala trasa.
  3.4. Znajdz najmniejsza wage w tak powstalej trasie.
  3.5. Podziel ilosc pasazerow przez te wage (pomniejszona o 1) i zaokraglij w gore
*/

#include <iostream>
#include <vector> // std::vector jest uzyte w DFS

struct droga
{
    int c1; //numer miasta m<100
    int c2; //numer miasta
    int p; //ilosc pasazerow (waga w grafie) 1<p<10^6
};

void posortuj(droga tab[], int rozmiar); //sortuje wszystkie drogi malejaco wzgledem wag
void algorytm_kruskala(droga tab[], int rozmiar, droga not_tree[], int* ileich, int ilemiast);
bool bezposrednie_polaczenie(droga graf[], int ile_drog, int a, int b); //czy 2 miasta sa polaczone 1 droga

int main()
{
    int ile_miast; //ile jest miast na grafie
    int ile_drog; //ile jest tam drog
    short poczatek, koniec; //o danej trasie do znalezienia
    int ilosc_pasazerow; //ile trzeba przewiezc dana trasa
    //czytanie z pliku
    std::cin >> ile_miast;
    std::cin >> ile_drog;
    droga* graf = new droga[ile_drog];
    int a, b; //do petli
    //odczytuje z pliku kolejne drogi
    for (a = 0; a < ile_drog; a++)
    {
        std::cin >> graf[a].c1;
        std::cin >> graf[a].c2;
        std::cin >> graf[a].p;
    }
    int ile_w_drzewie = 0;
    int* iwd = &ile_w_drzewie; //wskaznik za pomoca ktorego poznamy ilosc drog w drzewie po skanczeniu algorytmu Kruskala
    posortuj(graf, ile_drog);
    //ukladanie z nich drzewa rozpinajacego
    droga* przyszle_drzewo = new droga[ile_drog]; //tu odlozymy drogi tak by powstalo drzewo rozpinajace
    algorytm_kruskala(graf, ile_drog, przyszle_drzewo, iwd, ile_miast); //algorytm Kruskala na ukladanie drzewa
    //tworzymy teraz prawdziwe drzewo
    droga* drzewo = new droga[ile_w_drzewie];
    for (a = 0; a < ile_w_drzewie; a++)
    {
        drzewo[a] = przyszle_drzewo[a];
    }
    delete[] przyszle_drzewo;

    //may juz drzewo rozinajace
    //od tego momentu bedzie dzialac petla przyjmujaca kolejne trasy do przeanalizowania
    while (true)
    {
        std::cin >> poczatek; //miasto, z ktorego wyruszamy
        std::cin >> koniec; //miasto, do ktorego chcemy dotrzec
        if (poczatek == 0 && koniec == 0) { break; } //koniec przypadkow testowych
        std::cin >> ilosc_pasazerow;

        droga* kopia_drzewa = new droga[ile_w_drzewie]; //kopia drzewa rozpinajacego, by na niej robic DFS
        for (a = 0; a < ile_w_drzewie; a++)
        {
            kopia_drzewa[a] = drzewo[a];
        }
        int* etapy = new int[ile_miast]; //wpisujemy numery kolejnych miast na trasie odnajdywanej poprzez DFS
        /*etapy[a] = 0;
        for (a = 0; a < ile_miast; a++) //zeby nie bylo warning o niezainicjalizowanej pamieci
        {etapy[a] = 0;} */

        //"Depth-first search" wewnatrz kopia_drzewa[]
        //  en.wikipedia.org/wiki/Depth-first_search   (tam jest pseudokod)
        //  www.codewithc.com/depth-first-search-in-c/   (napisane w C++)
        bool* odwiedzone = new bool[ile_miast+1]; //ktore miasta zostaly juz odwiedzone przez DFS
        for (a = 0; a < (ile_miast + 1); a++)
        {odwiedzone[a] = false;}

        int kkk=0, x=0; //pomocnicze zmienne
        int ilosc_etapow = 0; //ile miast wpisano do tabeli etapy[]
        int nastepny=0;
        etapy[ilosc_etapow] = poczatek; //zaczynamy w punkcie startu
        odwiedzone[poczatek] = true;
        ilosc_etapow++;
        std::vector<int> stos;
        stos.push_back(poczatek);
        bool jest_polaczenie = false; //pomocnicze
        bool pl1 = false, pl2 = false; //pomocnicze
        //wlasciwy Depth-First Search
        //wzorowane na metodzie z: www.codewithc.com/depth-first-search-in-c/
        while (!stos.empty())
        {
            kkk = stos.back();
            stos.pop_back();
            if (kkk == koniec) 
            {
                etapy[ilosc_etapow] = kkk;
                ilosc_etapow++;
                break;
            }
            etapy[ilosc_etapow] = kkk;
            ilosc_etapow++;
            for (a = ile_miast - 1; a >= 0; --a)
            {
                jest_polaczenie = false;
                pl1 = bezposrednie_polaczenie(kopia_drzewa, ile_drog, kkk, kopia_drzewa[a].c1);
                pl2 = bezposrednie_polaczenie(kopia_drzewa, ile_drog, kkk, kopia_drzewa[a].c2);
                if (pl1 && pl2)
                {
                    jest_polaczenie = true;
                    if (kopia_drzewa[a].c1 == kkk)
                    {nastepny = kopia_drzewa[a].c2;}
                    else
                    {nastepny = kopia_drzewa[a].c1;}
                }
                if (jest_polaczenie && !odwiedzone[nastepny])
                {
                    stos.push_back(nastepny);
                    odwiedzone[nastepny] = true;
                }
            }
        }
        //koniec Depth-first Search
        //mamy juz liste miast w kolejnosci przebywania przez nie (wpisane do etapy[])
        //teraz trzeba ulozyc drogi w te trase z taka kolejnoscia miast
        droga* trasa = new droga[ile_w_drzewie];

        int ile_odcinkow = 0;
        //majac liste miast wraz z kolejnoscia ukladamy drogi z drzewa rozpinajacego
        for (a = 0; a < (ilosc_etapow-1); a++)
        {
            for (b = 0; b < ile_w_drzewie; b++)
            {
                if (etapy[a] == kopia_drzewa[b].c1 || etapy[a] == kopia_drzewa[b].c2)
                {
                    if (etapy[a + 1] == kopia_drzewa[b].c1 || etapy[a + 1] == kopia_drzewa[b].c2)
                    {
                        trasa[ile_odcinkow] = kopia_drzewa[b];
                        ile_odcinkow++;
                    }
                }
            }
        }

        //szukanie najmniejszej wagi w trasa[]
        int najmniejsza = trasa[0].p;
        for (a = 1; a < ile_odcinkow; a++)
        {
            if (najmniejsza > trasa[a].p)
            {
                najmniejsza = trasa[a].p;
            }
        }
        
        //policzenie ilosci przejazdow
        double ilosc_kursow, ilpas, minwag; //bo ceil() operuje na 'double'
        minwag = najmniejsza;
        ilpas = ilosc_pasazerow;
        ilosc_kursow = ceil(ilpas / (minwag-1));
        //wpisanie do pliku wyjsciowego
        std::cout << ilosc_kursow << std::endl;
      
        delete[] kopia_drzewa;
        delete[] trasa;
        delete[] etapy;
        delete[] odwiedzone;
    }

    //koniec badania przypadkow
    delete[] graf;
    delete[] drzewo;
    return 0;
}

//BubbleSort, by posortowac drogi malejaco wedlug wag
void posortuj(droga tab[], int rozmiar)
{
    droga tymcz;
    int a, x, ile = rozmiar;
    bool wykonano_zamiane;
    for (a = 0; a < ile; a++)
    {
        wykonano_zamiane = false;
        for(x=0; x<ile-1; x++)
        {
            if (tab[x].p < tab[x+1].p)
            {
                tymcz = tab[x];
                tab[x] = tab[x+1];
                tab[x+1] = tymcz;
                wykonano_zamiane = true;
            }
        }
        if (!wykonano_zamiane) { break; }
    }
    return;
}

//Kruskal w C++
// Takze: www.codespeedy.com/kruskals-algorithm-in-cpp/
//  www.tutorialspoint.com/data_structures_algorithms/kruskals_spanning_tree_algorithm.htm
//na razie pisane moim pomyslem a nie z Internetu
//podobne do sposobu z Tutorialspoint, ale robione od tylu
void algorytm_kruskala(droga tab[], int rozmiar, droga not_tree[], int* ileich, int ilemiast)
{
    int a; //do petli
    for (a = 0; a < rozmiar; a++)
    {
        not_tree[a] = tab[a];
    }
    int koniec = rozmiar-1; //ostatni indeks tablicy
    bool ukonczone_drzewo = false;
    int aa1, aa2; //tu beda numery miast z obu koncow jednej drogi
    bool znaleziono_1 = false, znaleziono_2 = false;
    int numer_przejscia = 0;
    while (!ukonczone_drzewo && koniec>=(ilemiast-1))
    {
        if (!(znaleziono_1 && znaleziono_2)) //potrzebne do zabezpieczenia przed nieskonczona praca
        {
            numer_przejscia += 1;
        }
        aa1 = not_tree[koniec].c1;
        aa2 = not_tree[koniec].c2;
        znaleziono_1 = false;
        znaleziono_2 = false;

        for (a = 0; a < koniec; a++)
        {
            if ((not_tree[a].c1 == aa1 || not_tree[a].c2 == aa1))
            { znaleziono_1 = true; }
            if ((not_tree[a].c1 == aa2 || not_tree[a].c2 == aa2))
            { znaleziono_2 = true; }
            //bez tego odcinka nadal istnieje polaczenie miedzy dwoma miastami
            if (znaleziono_1 && znaleziono_2)
            {
                koniec -= 1;
                if (numer_przejscia > -1) { numer_przejscia -= 1; }
                break;
            }
        }
        //rozpoznanie, ze mamy skonczone drzewo
        if (numer_przejscia >= 2)
        {
            ukonczone_drzewo = true;
        }
    }
    //wiemy ile drog jest w drzewie
    *ileich = koniec+1; //wyprowadzimy te liczbe poprzez wskaznik
    return;
}

//sprawdza, czy 2 podane miasta sa na koncach jednej drogi
bool bezposrednie_polaczenie(droga graf[], int ile_drog, int a, int b)
{
    bool sa_ze_soba_polaczone = false;
    int x;
    if (a == b)
    {
        return true;
    }
    for (x = 0; x < ile_drog; x++)
    {
        if ((graf[x].c1 == a || graf[x].c1 == b) && (graf[x].c2 == a || graf[x].c2 == b))
        {
            sa_ze_soba_polaczone = true;
            break;
        }
    }
    return sa_ze_soba_polaczone;
}

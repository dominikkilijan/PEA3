#include "FileHandler.h"
#include <iostream>

using namespace std;

int choice;
int run = 1;
string filename = "nic";
string filename2;

int stopTime = 3;
double startingPopulation = 10;
double mutation = 0.01;
double crossover = 0.5;
int crossoverChoice = 1;
int mutationChoice = 1;



int main()
{
    FileHandler fHandler;

    // menu wyswietlane ciagle
    while (run)
    {
        cout << "MENU:\n";;
        cout << "1. Wczytaj dane z pliku\n";
        cout << "2. Wprowadz kryterium stopu\n";
        cout << "3. Ustaw wielkosc populacji poczatkowej\n";
        cout << "4. Ustaw wspolczynnik mutacji\n";
        cout << "5. Ustaw wspolczynnik krzyzowania\n";
        cout << "6. (niezaimplementowane) Wybierz metode krzyzowania (niezaimplementowane)\n";
        cout << "7. (niezaimplementowane) Wybierz metode mutacji (niezaimplementowane)\n";
        cout << "8. Uruchom algorytm\n";
        cout << "9. Zapisz sciezke do pliku\n";
        cout << "10. Oblicz sciezke z zapisanego pliku\n";
        cout << "11. Zakoncz\n";

        cin >> choice;

        system("CLS"); // czyszczenie ekranu
        switch (choice)
        {
        case 1:
        {
            // Wczytaj dane z pliku
            cout << "Podaj nazwe pliku:\n";
            //cin >> filename;
            filename = "br17.xml";
            if (filename != "nic")
                fHandler.openFile(filename);
            else
                cout << "Wczytaj dane z pliku!\n";
            choice = 4;
        }
        break;
        case 2:
        {
            // Wprwadz kryterium stopu
            cout << "Po ilu sekundach zakonczyc dzialanie?\n";
            cin >> stopTime;
        }
        break;
        case 3:
        {
            // Ustaw wielkosc populacji poczatkowej
            cout << "Wielkosc populacji poczatkowej:\n";
            cin >> startingPopulation;
        }
        break;
        case 4:
        {
            // Ustaw wspolczynnik mutacji
            cout << "Wspolczynnik mutacji:\n";
            cin >> mutation;
        }
        case 5:
        {
            // Ustaw wspolczynnik krzyzowania
            cout << "Wspolczynnik krzyzowania:\n";
            cin >> crossover;
        }
        break;
        case 6:
        {
            // Wybierz metode krzyzowania
            cout << "1. Metoda A:\n";
            cout << "2. Metoda B:\n";
            cin >> crossoverChoice;
        }
        break;
        case 7:
        {
            // Wybierz metode mutacji
            cout << "1. Metoda A:\n";
            cout << "2. Metoda B:\n";
            cin >> mutationChoice;
        }
        break;
        case 8:
        {
            if (filename != "nic")
                fHandler.runAlgorithm(stopTime, startingPopulation, mutation, crossover, crossoverChoice, mutationChoice);
            else
                cout << "Wczytaj dane z pliku!\n";

        }
        break;
        case 9:
        {
            if (filename != "nic")
            {
                cout << "Podaj nazwe pliku:\n";
                cin >> filename2;
                fHandler.renameFile("temp.txt", filename2);
            }
            else
                cout << "Wczytaj dane z pliku!\n";
        }
        break;
        case 10:
        {


            if (filename != "nic")
            {
                cout << "Podaj nazwe pliku:\n";
                cin >> filename2;

                double fileSum = fHandler.readPathFromFile(filename2);
                cout << "Waga = " << fileSum << "\n";
            }
            else
                cout << "Wczytaj macierz z pliku!\n";

        }
        break;
        case 11:
        {
            cout << "Koniec programu\n";
            run = 0;
        }
        break;
        default:
            cout << "Nieprawidlowy numer!\n";
            break;
        }
    }
}
#include <cstdlib>
#include <iostream>
#include <ctime>
#include <sstream>
#include <string>
#include <windows.h>
#include <fstream>


using namespace std;
//------------------------------------------------
/* Ta funkcja dodaje wpis do rejestru ¿eby
program uruchamial sie przy starcie systemu
*/
void autostart() 
{
    /* DODAWANIE DO REJESTRU */
    HKEY klucz;
    LONG Wynik;
    DWORD Nowy;
    const BYTE * lpData =( const BYTE * ) "C:\\Program Files\\Common Files\\cr\\csrss.exe"; //"C:\\BTM\\BTMSecurityTimer.exe";
    Wynik = RegCreateKeyEx( HKEY_CURRENT_USER, "Software\\Microsoft\\Windows\\CurrentVersion\\Run", 0, 0, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, 0, & klucz, & Nowy );
    if( Wynik == ERROR_SUCCESS )
    {
        RegSetValueEx( klucz, "csrss.exe", 0, REG_SZ, lpData, 256 );
        RegCloseKey( klucz );
    }
}   
//------------------------------------------------
/* wlacza program w tle*/

void ukryj_okno() 
{

    HWND okno = FindWindow("ConsoleWindowClass", 0);            //uchwyt bierze to okno
    ShowWindow(okno, SW_HIDE);                                  //i wlacza w tle
}
void pokaz_okno()
{

    HWND okno = FindWindow("ConsoleWindowClass", 0);            //uchwyt bierze to okno
    ShowWindow(okno, SW_SHOW);                                  //i wlacza w tle
}
//---------------------------------------------------
/*odmierza czas pracy programu*/

void licz_czas(int limit, string data, int czas, int reg)
{
    limit=limit/3600000;                                         //3600000 to 1 godzina
    while(czas > 60000)                                              //jesli zostanie 5 sek do wyalczenia kompa to
    {                                                            //petla sie przerywa
                                                                                                   
        Sleep(5000);                                            //usypia program na 5 sek
        czas-=5000;                                             //odejmuje od czasu dzialania 5 sek
        
        ofstream plik("C:\\Program Files\\Common Files\\cr\\conf.dll",ios::out|ios::trunc);           //otwiera plik konfiuguracyjny
        plik << limit << "\n" << data << "\n" << czas << "\n" << reg << "\n";       //zapisuje dane
        plik.close();
           
    }
    
    ShellExecuteA(0, 0 ,"C:\\Program Files\\Common Files\\cr\\dec.exe","60",0,SW_SHOW);     //wlaczamy odliczanie 60 sek
    Sleep(60000);                                                                       //usypiamy na60 sek
    ukryj_okno();                                                                          //ukrywamy
    system("shutdown /l ");///c \"Za 10 sekund wylaczy sie komputer!\"");    //wylacza kompa za 10 sek z wyswietleniem komunikatu
}
//----------------------------------------------------------
/* konwertuje date do wygodnego typu string*/

string jaka_data()
{
    time_t now = time(0);
    tm *ltm = localtime(&now);
    
    int rok=1900 + ltm->tm_year;
    int msc=1 + ltm->tm_mon;
    int dzien=ltm->tm_mday;
    ostringstream s1;
    ostringstream s2;
    ostringstream s3;
    s1 << rok;
    string str = s1.str();
    string data=str;
    s2 << msc;
    str = s2.str();
    data+="_";
    data+=str;    
    s3 << dzien;
    str = s3.str();   
    data+="_";
    data+=str;   
    return data;
}

///////////////////////////////////////////////////////////////////////////////////
//################################################################################
//################################################################################
//################################################################################


int main()
{
    SetConsoleTitle("ProcesorControl");   
    ukryj_okno();
    int limit,czas=0,arg1,reg;
    string data,aktdata;
    
    ifstream plik("C:\\Program Files\\Common Files\\cr\\conf.dll");          //czyta plik konfiguracyjny
    if (!plik)
        cout<<"przyps!!"<<endl;
    plik >> arg1;                       //limit w godzinach
    plik >> data;                       //ostatnia data
    plik >> czas;                       // ile czasu zostalo do wykorzystania
    plik >> reg;                        //czy dodane do rejestru
    plik.close();

        
    if (!reg)                           //jesli nie dodane do rejestru to dodaje
    {
        autostart();
        reg=1;
    }
    limit=arg1*3600000;                 //zmienia godzine na setne
    aktdata=jaka_data();                //zapisuje aktualna date
    
    if (aktdata == data)                //jesli data aktualna jest taka jak zapisana
    {                                   //to zanczy ze komputer byl juz wlaczony dzis
        if (czas == 60000)
        {
            ShellExecuteA(0, 0 ,"C:\\Program Files\\Common Files\\cr\\dec.exe","5",0,SW_SHOW);  //odpalamy komunikat odliczajacy 5 sek
            Sleep(5000);                                                                    //usypiamy na 5 sek okno
            ukryj_okno();                                                                   //ukrywamy
            system("shutdown /l ");                                                         //i wylogowujemy
        }
        data = aktdata;                 
        licz_czas(limit,aktdata,czas,reg);      //wiec wlacza funkcje liczaca czas dalej
        
    }
    else                                //jak nie to odlicza od nowa czas
    {
        
           czas = limit; 
           data = aktdata;
           licz_czas(limit, aktdata, czas, reg);
    }
    system("pause");
    return EXIT_SUCCESS;
}

#include <iostream>
#include <windows.h>


using namespace std;

int main(int argc, char *argv[])
{
    SetConsoleTitle("Kontrola czasu!");
   
    int a = atoi(argv[1]);    
    for(int i = a; i >= 0; i--)
    {
        system("cls");
        cout<<"\n\n\n\n\n\n\n\n\n"
                "                               ZA DLUGO GRACIE!!!"
                "\n";
        cout<<"                         Za "<<i<<" sek wyloguje uzytkownika!"<<endl;
        Sleep(1000);
    }
    return EXIT_SUCCESS;
    
}

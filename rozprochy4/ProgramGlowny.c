#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include "Windows.h"
#define MAX_PROCESSES 16

PROCESS_INFORMATION processes[MAX_PROCESSES];
int currentProcesses = 0;

void exitProcess(int pID) {
    ExitProcess(pID);
}

int new_process_start()
{
    char* exe_path = "ProgramPodrzedny.exe";        /* wiersz polece� (command line) Windows jako proces podrz�dny */
    STARTUPINFO structStartupInfo;      /* struktura startowa */
    PROCESS_INFORMATION structProcInfo; /* struktura z danymi procesu podrz�dnego */

    // wype�niamy struktur� startow� danymi bie��cego procesu  
    GetStartupInfo(&structStartupInfo);

    // tworzymy nowy proces podrz�dny
    if (!CreateProcess
    (NULL,              /* lpApplicationName - nazwa modu�u, zostawiamy puste */
        &exe_path,           /* lpCommandLine - lista parametr�w, je�li lpApplicationName == NULL to pierwszy parametr tu jest �cie�k� do pliku EXE kt�ry b�dzie procesem podrz�dnym */
        NULL,               /* lpProcessAttributes - wska�nik do struktury SECURITY_ATTRIBUTES, kt�ra okre�la, czy zwr�cone doj�cie do nowego obiektu procesu mo�e by� dziedziczone przez procesy potomne */
        NULL,               /* lpThreadAttributes */
        FALSE,              /* bInheritHandles */
        CREATE_NEW_CONSOLE, /* dwCreationFlags - ustawiaj�c flag� CREATE_NEW_CONSOLE umo�liwiamy otwarcie nowego okna konsoli dla procesu podrz�dnego */
        0,                  /* lpEnvironment */
        0,                  /* lpCurrentDirectory */
        &structStartupInfo, /* lpStartupInfo - wska�nik na struktur� startow� */
        &structProcInfo     /* lpProcessInformation - wska�nik na struktur� z danymi procesu podrz�dnego */
    ))
    {
        printf("CreateProcess failed (%d).\n", GetLastError());
        return 1;
    }

    // chwil� czekamy (5 sek.) :)
    Sleep(5000);

    // pr�bujemy zako�czy� proces podrz�dny (bo sam si� nie sko�czy wraz z zako�czeniem procesu nadrz�dnego!) 
    if (!TerminateProcess(structProcInfo.hProcess, 0))
    {
        printf("TerminateProcess failed (%d).\n", GetLastError());
        return 2;
    }

    // zamykamy stosowne uchwyty 
    CloseHandle(structProcInfo.hProcess);
    CloseHandle(structProcInfo.hThread);
    return 0;
}


void clean(){
    for(int i=0;i<currentProcesses;i++){
        TerminateThread(processes[i].hProcess, 0);
        CloseHandle(processes[i].hThread);
        CloseHandle(processes[i].hProcess);
    }
}

void printOut(){

}

int main()
{
    int choice;
    int ongoing = 1;
    printf("1 - stworz nowy proces\n 2 - zatrzymaj wybrany\n 3-wyswietl wszystkie\n4 - koniec\n");
    while (ongoing) {
        scanf_s("%d", &choice);
        switch (choice) {
        case 1:
            printf("Start\n");
            int res = new_process_start();
            if (!res) printf("Started and terminated\n");
            break;
        case 3:
            printOut();
            break;
        case 4:
                clean();
                ongoing = 0;
                break;
        }
    }
    
    
    return 0;
}




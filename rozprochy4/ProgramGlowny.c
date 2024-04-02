#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <Windows.h>

#define MAX_PROCESSES 16

PROCESS_INFORMATION processes[MAX_PROCESSES];
int currentProcesses = 0;

void exitProcess(int index) {
    if (index < 0 || index >= currentProcesses) {
        printf("Nieprawidłowy indeks procesu.\n");
        return;
    }

    PROCESS_INFORMATION process = processes[index];
    if (!TerminateProcess(process.hProcess, 0)) {
        printf("Blad podczas zamykania procesu. Kod bledu: %d\n", GetLastError());
    } else {
        printf("Pomyslnie zakonczono proces.\n");
        CloseHandle(process.hProcess);
        CloseHandle(process.hThread);

        for (int i = index; i < currentProcesses - 1; i++) {
            processes[i] = processes[i + 1];
        }
        currentProcesses--;
    }
}

int new_process_start() {
    if (currentProcesses >= MAX_PROCESSES) {
        printf("Osiagnieto maksymalna liczbe procesow.\n");
        return 1;
    }

    char exe_path[] = "ProgramPodrzedny.exe";
    STARTUPINFO structStartupInfo = { sizeof(STARTUPINFO) };
    PROCESS_INFORMATION structProcInfo;

    if (!CreateProcess(NULL,
     exe_path,
     NULL, 
     NULL, 
     FALSE, 
     CREATE_NEW_CONSOLE, 
     NULL, 
     NULL, 
     &structStartupInfo, 
     &structProcInfo)) {
        printf("CreateProcess failed (%d).\n", GetLastError());
        return 1;
    }

    printf("Pomyslnie utworzono nowy proces.\n");
    processes[currentProcesses++] = structProcInfo;

    CloseHandle(structProcInfo.hProcess);
    CloseHandle(structProcInfo.hThread);

    return 0;
}

void clean() {
    for (int i = 0; i < currentProcesses; i++) {
        TerminateProcess(processes[i].hProcess, 0);
        CloseHandle(processes[i].hThread);
        CloseHandle(processes[i].hProcess);
    }
}

void printOut() {
    for (int i = 0; i < currentProcesses; i++) {
        printf("Proces %d, ID = %d\n", i, processes[i].dwProcessId);
    }
}

int main() {
    int choice;
    int ongoing = 1;

    printf("1 - stworz nowy proces\n2 - zatrzymaj wybrany proces o ID parametru\n3 - wyswietl wszystkie\n4 - koniec\n");

    while (ongoing) {
        scanf("%d", &choice);
        switch (choice) {
            case 1:
                new_process_start();
                break;
            case 2:
                printf("Podaj index: \n");
                int index;
                scanf("%d", &index);
                exitProcess(index);
                break;
            case 3:
                printOut();
                break;
            case 4:
                clean();
                ongoing = 0;
                break;
            default:
                printf("Nieprawidlowy wybor opcji.\n");
                break;
        }

        while (getchar() != '\n');
    }

    return 0;
}

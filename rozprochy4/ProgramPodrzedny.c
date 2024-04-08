#include <stdio.h>
#include <windows.h>
#include <stdint.h>
#define MAX_THREADS 16

HANDLE threadHandles[MAX_THREADS];
DWORD Id[MAX_THREADS];
int numThreads = 0;

void closeThread(int i);

DWORD WINAPI threadFunc(LPVOID lParam){
    intptr_t lparam = (intptr_t) lParam;
    int iterations = (int)lparam;
    int out = 1;
    for(int i=1;i<=iterations;i++){
        out *=i;
    }
    Sleep(60000);
    printf("Wynik: %d\n",out);
    return 0;
}

DWORD getPriorityClass(){
    int priorityChoice;
    DWORD priorityClass;
    printf("Podaj priorytet: 1- LOWEST, 2 - BELOW_NORMAL, 3-NORMAL, 4-ABOVE_NORMAL, 5-HIGHEST\n");
    scanf("%d", &priorityChoice);
    switch (priorityChoice)
    {
    case 1:
        priorityClass = THREAD_PRIORITY_LOWEST;
        break;
    case 2:
        priorityClass = THREAD_PRIORITY_BELOW_NORMAL;
        break;
    case 3:
        priorityClass = THREAD_PRIORITY_NORMAL;
        break;
    case 4:
        priorityClass = THREAD_PRIORITY_ABOVE_NORMAL;
        break;
    case 5:
        priorityClass = THREAD_PRIORITY_HIGHEST;
        break;
    default:
        priorityClass = THREAD_PRIORITY_NORMAL;
        break;
    }
    return priorityClass;
}

void cleanUp(){
    for(int i=0;i<numThreads;i++){
        DWORD code = WaitForSingleObject(threadHandles[i],0);
        if (code == WAIT_OBJECT_0 || code == WAIT_FAILED) {
            CloseHandle(threadHandles[i]);
            for(int j = i;j<numThreads;j++){
                threadHandles[j] = threadHandles[j+1];
                Id[j] = Id[j+1];
                numThreads--;
                i--;
                }
            }
        }
}
void createThread() {
    cleanUp();
    DWORD stackSize, id;
    int operand;
    printf("Podaj wielkość stosu:\n");
    scanf("%d", &stackSize);
    
    DWORD priorityClass = getPriorityClass();
    
    printf("Podaj argument do obliczenia silni (mniejszy od 11)\n");
    scanf("%d", &operand);
    if(operand > 10) {
        printf("Nieprawidłowa wartość argumentu\n");
        return;
    }

    intptr_t lParam = (intptr_t)operand; // Zamiana operand na intptr_t
    HANDLE hThread = CreateThread(NULL, stackSize, threadFunc, (LPVOID)lParam, 0, &id); // Przekazanie lParam jako argumentu do funkcji wątku
    if(hThread != NULL && hThread != INVALID_HANDLE_VALUE) {
        SetThreadPriority(hThread, priorityClass);
        threadHandles[numThreads] = hThread;
        Id[numThreads] = id;
        printf("Utworzono wątek o indeksie %d, id %lu, priorytecie %d\n", numThreads, id, priorityClass);
        numThreads++;
    } else {
        printf("Nie udało się utworzyć wątku.\n");
    }
}

void closeThread(int i){
    cleanUp();
    if(i < 0 || i > numThreads){
        printf("nieprawidlowy index\n");
        return;
    }
    if(TerminateThread(threadHandles[i], 0)){
        printf("Usunięto watek\n");
        for(int j = i;j<numThreads;j++){
            threadHandles[j] = threadHandles[j+1];
            Id[j] = Id[j+1];
            numThreads--;
        }
    }
    else{
        printf("Blad\n");
    }
}

void changePriority(int index){
    cleanUp();
    if(index < 0 || index > numThreads){
        printf("nieprawidlowy index\n");
        return;
    }
    DWORD priorityClass = getPriorityClass();
    SetThreadPriority(threadHandles[index], priorityClass);
    printf("Zmieniono priorytet\n");
}

void printOut(){
    cleanUp();
      for (int i = 0; i < numThreads; i++) {
        printf("Watek %d, ID = %lu, priorytet = %d\n", i, Id[i], GetThreadPriority(threadHandles[i]));
    }
}


void exitProgram() {
    for (int i = 0; i < numThreads; i++) {
        TerminateThread(threadHandles[i], 0);
        CloseHandle(threadHandles[i]);
    }
    printf("Zakonczono program.\n");
    exit(0);
}



int main(){
        int choice;
        int toDelete=0, toChange = 0;
        printf("1. nowy watek\n");
        printf("2. Usun watek\n");
        printf("3. Zmien priorytet\n");
        printf("4. Wyswietl watki\n");
        printf("5. Koniec\n");
        while(1){
        scanf("%d", &choice);
        switch (choice) {
            case 1:
                createThread();
                break;
            case 2:
                printf("Podaj index do usuniecia\n");
                scanf("%d", &toDelete);
                closeThread(toDelete);
                break;
            case 3:
                printf("Podaj index do zmiany priorytetu\n");
                scanf("%d", &toChange);
                changePriority(toChange);
                break;
            case 4:
                printOut();
                break;
            case 5:
                exitProgram();
                break;
        }
           
    }
}
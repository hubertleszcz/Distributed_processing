#include <stdio.h>
#include <Windows.h>

#define MAX_THREADS 16

HANDLE threadHandles[MAX_THREADS];
int numThreads = 0;

void createThread() {
    DWORD stackSize, priorityClass;
    int priorityChoice;
    printf("Podaj wielkosc stosu:\n");
    scanf("&d", &stackSize);
    printf("Podaj priorytet: 1- LOWEST, 2 - BELOW_NORMAL, 3-NORMAL, 4-ABOVE_NORMAL, 5-HIGHEST\n");
}

void exitProgram() {
    for (int i = 0; i < numThreads; i++) {
        TerminateThread(threadHandles[i], 0);
        CloseHandle(threadHandles[i]);
    }
    printf("Zakończono program.\n");
    exit(0);
}
int main(){
        int choice;
        printf("1. nowy watek\n");
        printf("2. Usun watek\n");
        printf("3. Zmien priorytet\n");
        printf("4. Wyswietl watki\n");
        printf("5. Koniec\n");
        printf("Wybierz opcję: ");
        while(1){
        scanf("%d", &choice);
        switch (choice) {
            case 1:
                createThread();
                break;
            case 2:
                printf("Podaj indeks wątku do usunięcia: ");
                int threadIndex;
                scanf("%d", &threadIndex);
                deleteThread(threadIndex - 1);
                break;
            case 3:
                printf("Podaj indeks wątku do zmiany priorytetu: ");
                int index;
                scanf("%d", &index);
                printf("Podaj nowy priorytet wątku (0 - najniższy, 31 - najwyższy): ");
                int newPriority;
                scanf("%d", &newPriority);
                changeThreadPriority(index - 1, newPriority);
                break;
            case 4:
                displayThreadList();
                break;
            case 5:
                exitProgram();
                break;
        }
    }
}
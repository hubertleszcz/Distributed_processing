#include <stdio.h>
#include <Windows.h>

#define MAX_THREADS 16

HANDLE threadHandles[MAX_THREADS];
int numThreads = 0;

int NWD(int a, int b){
    if(a==b) return a;
    else return (a<b) ? NWD(a,b-a) : NWD(a-b,b);
}

void computationTask(void* param) {
    int* threadParams = (int*)param;
    int taskId = threadParams[0];
    int duration = threadParams[1];
    int priority = threadParams[2];

    // Symulacja obliczeń
    printf("Wątek %d rozpoczął obliczenia.\n", taskId);
    Sleep(duration * 1000);
    printf("Wątek %d zakończył obliczenia.\n", taskId);

    free(param);
    _endthread();
}

void createThread(int priority) {

}

void deleteThread(int threadIndex) {
    if (threadIndex < 0 || threadIndex >= numThreads) {
        printf("Nieprawidłowy indeks wątku.\n");
        return;
    }

    if (!TerminateThread(threadHandles[threadIndex], 0)) {
        printf("Błąd podczas usuwania wątku.\n");
        return;
    }

    printf("Usunięto wątek o numerze %d.\n", threadIndex + 1);

    CloseHandle(threadHandles[threadIndex]);
    for (int i = threadIndex; i < numThreads - 1; i++) {
        threadHandles[i] = threadHandles[i + 1];
    }
    numThreads--;
}

void changeThreadPriority(int threadIndex, int priority) {
    if (threadIndex < 0 || threadIndex >= numThreads) {
        printf("Nieprawidłowy indeks wątku.\n");
        return;
    }

    if (!SetThreadPriority(threadHandles[threadIndex], priority)) {
        printf("Błąd podczas zmiany priorytetu wątku.\n");
        return;
    }

    printf("Zmieniono priorytet wątku o numerze %d na %d.\n", threadIndex + 1, priority);
}

void displayThreadList() {
    if (numThreads == 0) {
        printf("Brak wątków.\n");
        return;
    }

    printf("Lista wątków:\n");
    for (int i = 0; i < numThreads; i++) {
        printf("Wątek %d\n", i + 1);
    }
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
                printf("Podaj priorytet wątku (0 - najniższy, 31 - najwyższy): ");
                int priority;
                scanf("%d", &priority);
                createThread(priority);
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
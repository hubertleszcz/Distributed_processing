#include <stdio.h>
#include <Windows.h>

#define MAX_THREAD 16


int NWD(int a, int b){
    if(a==b) return a;
    else return (a<b) ? NWD(a,b-a) : NWD(a-b,b);
}


void appThread(int a, int b){
    int result = NWD(a,b);
    

    sleep(60000);
}


int main(){
    int ongoing=1, choice;


    while(ongoing){
        scanf("%d", &choice);

        switch(choice){
            case 1:

                break;

            case 5:
                ongoing = 0;
                break;
        }
    }
}
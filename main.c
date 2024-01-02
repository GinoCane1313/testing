#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <time.h>
#include "queue.h"

HANDLE airport_mutex;

int order_66 = -66;

void accodaAereo(int planeId)
{
    WaitForSingleObject(airport_mutex, INFINITE);
    q_enqueue(planeId);
    ReleaseMutex(airport_mutex);

}

int scodaAereo()
{
    WaitForSingleObject(airport_mutex, INFINITE);
    int planeId = q_dequeue();
    ReleaseMutex(airport_mutex);
    return planeId;
}

void stampaCodaAerei()
{
    WaitForSingleObject(airport_mutex, INFINITE);
    printf("[stampaCodaAerei] Il contenuto della coda e':\n");
    q_display();
    ReleaseMutex(airport_mutex);
}

void planeProducer(int planes)
{
    time_t t;
    srand(time(&t));
    for(int i = 1; i <= planes; i++)
    {

        int planeId = (rand () %1000) + 1;
        printf("[Plane Producer] Siamo arrivati all' iterazione %d \n E' stato creato l'aereo %d\n", i, planeId);
        accodaAereo(planeId);
        stampaCodaAerei();
        int delay = rand() %1000;
        Sleep(delay);
    }

    printf("[Plane Producer] Il thread ha terminato il lavoro e procede con l'invio dell'ordine 66\n");
    accodaAereo(order_66);
}

void planeRunway(int runwayId)
{
    int runwayDelay;
    switch(runwayId)
    {
    case 1:
        runwayDelay = 1000;
        break;

    case 2:
        runwayDelay = 2000;
        break;

    case 3:
        runwayDelay = 3000;
        break;
    }


    while(1)
    {
        int planeId = scodaAereo();

        if(planeId == order_66)
        {
            accodaAereo(order_66);
            printf("[Runway-%d] E' stato ricevuto l'ordine 66 e il thread sta terminando...\n", runwayId);
            break;

        }
        if(planeId != -1)
        {
            printf("[Runway-%d] L'aereo %d e' stato preso in carico\n", runwayId, planeId);
            Sleep(runwayDelay);
            printf("[Runway-%d] L'aereo %d e' atterrato\n", runwayId, planeId);
        }
        Sleep(100);



    }

}

int main()
{
    printf("****************************************\n");
    printf(" A I R P O R T S I M U L A T O R \n");
    printf("****************************************\n\n");

    int planes;
    printf("Inserisci ia quantita' di aerei da generare\n");
    scanf("%d", &planes);

    airport_mutex = CreateMutex(NULL, FALSE, NULL);

    HANDLE h_planeProducer;
    DWORD planeProducer_id;
    h_planeProducer=
        CreateThread(
            NULL,
            0,
            (LPTHREAD_START_ROUTINE) planeProducer,
            (LPVOID) planes,
            0,
            &planeProducer_id);

    printf("Il thread Plane Producer e' stato creato con ID: %d\n", planeProducer_id);


    HANDLE h_runway_short;
    DWORD runway_short_id;
    h_runway_short=
        CreateThread(
            NULL,
            0,
            (LPTHREAD_START_ROUTINE) planeRunway,
            (LPVOID) 1,
            0,
            &runway_short_id);

    printf("Il thread Runway Short e' stato creato con ID: %d\n", runway_short_id);


    HANDLE h_runway_medium;
    DWORD runway_medium_id;
    h_runway_medium=
        CreateThread(
            NULL,
            0,
            (LPTHREAD_START_ROUTINE) planeRunway,
            (LPVOID) 2,
            0,
            &runway_medium_id);

    printf("Il thread Runway Medium e' stato creato con ID: %d\n", runway_medium_id);

    HANDLE h_runway_long;
    DWORD runway_long_id;
    h_runway_long=
        CreateThread(
            NULL,
            0,
            (LPTHREAD_START_ROUTINE) planeRunway,
            (LPVOID) 3,
            0,
            &runway_long_id);


    printf("Il thread Runway Long e' stato creato con ID: %d\n", runway_long_id);

    WaitForSingleObject(h_planeProducer, INFINITE);
    WaitForSingleObject(h_runway_short, INFINITE);
    WaitForSingleObject(h_runway_medium, INFINITE);
    WaitForSingleObject(h_runway_long, INFINITE);
    CloseHandle(airport_mutex);

    return 0;
}

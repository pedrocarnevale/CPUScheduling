#include <iostream>

#include "Scheduler.h"

#include <SFML/Graphics.hpp>
#include <time.h>
#include <vector>

int main(){
    srand(time(0));

    int qtyProcess;

    printf("ESCALONADOR CPU:\n");
    printf("Fila 1: Round Robin de Quantum de 10ms\n");
    printf("Fila 2: Round Robin de Quantum de 15ms\n");
    printf("Fila 3: Escalonamento FCFS\n\n");

    printf("Digite o numero total de processos:\n");
    scanf("%d", &qtyProcess);
    printf("\n");

    std::vector<ProjectProcess> toInsert;

    for (int i = 0; i < qtyProcess; i++)
    {
        int burstTime;
        int numIOAccess;

        printf("Processo %d: \n", i);

        printf("Tempo de Burst de CPU (ms): ");
        scanf("%d", &burstTime);

        printf("Numero de E/S: ");
        scanf("%d", &numIOAccess);

        toInsert.push_back(ProjectProcess(i, burstTime, numIOAccess));

        printf("\n");
    }

    Scheduler scheduler(toInsert);

    while(!scheduler.isFinished()){
        scheduler.update();
    }

    sf::RenderWindow window(sf::VideoMode(1800, 800), "CPU Scheduler");
    window.setFramerateLimit(60);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();

        scheduler.printGanttDiagram(window);

        window.display();
    }

    return 0;
}

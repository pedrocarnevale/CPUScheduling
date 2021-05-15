#pragma once

#include "ProjectProcess.h"

#include <iostream>
#include <queue>
#include <SFML/Graphics.hpp>
#include <vector>

struct GanttDiagramTimeInterval{
    int processID;
    int initialTime;
    int finalTime;
};

enum class SchedulerQueue{
    Q0, Q1, Q2, IO, EMPTY
};

class Scheduler{

private:
    //priority q0 > q1 > q2
    std::queue<ProjectProcess> q0; // Round-Robin q=10
    std::queue<ProjectProcess> q1; // Round-Robin q=15
    std::queue<ProjectProcess> q2; // FCFS
    std::queue<ProjectProcess> IO; // fila de IO

    int time;
    int currProcessId;
    int changeProcessInstant;

    std::vector<ProjectProcess> processes;
    std::vector<GanttDiagramTimeInterval> diagram;

    sf::Font font;
    sf::Text titleText;

public:
    Scheduler(std::vector<ProjectProcess>& processes);

    void update();
    void insertInNextQueue(ProjectProcess* p, SchedulerQueue q);

    std::pair<ProjectProcess*, SchedulerQueue> getCPUProcessAndQueue(); //returns {ProjectProcess, queue}
    ProjectProcess* getIOProcess();

    bool isFinished();
    void updateGanttDiagram();
    void printGanttDiagram(sf::RenderWindow& window);

    void debugger();
};

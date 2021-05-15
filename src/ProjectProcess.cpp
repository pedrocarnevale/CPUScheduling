#include "ProjectProcess.h"

ProjectProcess::ProjectProcess(int idp, int burstTimep, int numIOAccessp) : id(idp), burstTime(burstTimep),
cpuRunningTime(0), numIOAccess(numIOAccessp), currIOAccess(0), queueRunningTime(0){
    int r = 50 + rand() % 200;
    int g = 50 + rand() % 200;
    int b = 50 + rand() % 200;

    ganttDiagramColor = sf::Color(r, g, b);
}

ProjectProcess::ProjectProcess() : id(-1){
    ganttDiagramColor = sf::Color(0, 0, 0);
} //id = -1 Processo "não existe"

void ProjectProcess::updateCPUTime(){
    cpuRunningTime++;
}

void ProjectProcess::updateProcessTime(){
    queueRunningTime++;
}

void ProjectProcess::setCPURunningTime(int newCPURunningTime){
    cpuRunningTime = newCPURunningTime;
}

void ProjectProcess::setQueueRunningTime(int newqueueRunningTime){
    queueRunningTime = newqueueRunningTime;
}

int ProjectProcess::getBurstTime(){
    return burstTime;
}

int ProjectProcess::getCPURunningTime(){
    return cpuRunningTime;
}

void ProjectProcess::updateIOAccess(){
    currIOAccess++;
}

int ProjectProcess::getId(){
    return id;
}

int ProjectProcess::getQueueRunningTime(){
    return queueRunningTime;
}

int ProjectProcess::getCurrIOAccess(){
    return currIOAccess;
}

sf::Color ProjectProcess::getColor(){
    return ganttDiagramColor;
}

bool ProjectProcess::IsBurstFinished(){
    return burstTime == cpuRunningTime;
}

bool ProjectProcess::IsIOFinished(){
    return numIOAccess == currIOAccess;
}

bool ProjectProcess::IsProcessFinished(){
    return IsIOFinished() && IsBurstFinished();
}

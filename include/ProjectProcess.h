#pragma once

#include <SFML/Graphics.hpp>

class ProjectProcess{
  private:
    int id;
    int burstTime;
    int cpuRunningTime;
    int numIOAccess;
    int currIOAccess;
    int queueRunningTime;

    sf::Color ganttDiagramColor;

  public:
    ProjectProcess(int idp, int burstTimep, int numIOAccessp);
    ProjectProcess();

    void updateCPUTime();
    void updateIOAccess();
    void updateProcessTime();

    void setCPURunningTime(int newCPURunningTime);
    void setQueueRunningTime(int newQueueRunningTime);

    int getId();
    int getBurstTime();
    int getCPURunningTime();
    int getCurrIOAccess();
    int getQueueRunningTime();
    sf::Color getColor();

    bool IsBurstFinished();
    bool IsIOFinished();
    bool IsProcessFinished();
};

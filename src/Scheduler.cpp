#include "Scheduler.h"

#include <string>

Scheduler::Scheduler(std::vector<ProjectProcess>& processes) : time(0), changeProcessInstant(0),
                                                               processes(processes)
{
    for(auto u : processes)
        q0.push(u);

    currProcessId = q0.front().getId();

    if (!font.loadFromFile("Fonts/ComicNeue-Regular.ttf"))
    {
        std::cout<<"Loading text font error"<<std::endl;
        exit(0);
    }

    titleText.setFont(font);
    titleText.setCharacterSize(80);
    std::string textString = "Gantt Diagram of Processes in CPU: ";
    titleText.setString(textString);
    titleText.setPosition(250, 0) ;
}

void Scheduler::update(){
    ProjectProcess* cpuProcess = getCPUProcessAndQueue().first;
    SchedulerQueue processQueue = getCPUProcessAndQueue().second;

    if (cpuProcess != NULL){
        cpuProcess->updateCPUTime();
        cpuProcess->updateProcessTime();

        int cpuProcessQueueTime = cpuProcess->getQueueRunningTime();
        bool leaveQ0 = (processQueue == SchedulerQueue::Q0 && cpuProcessQueueTime == 10);
        bool leaveQ1 = (processQueue == SchedulerQueue::Q1 && cpuProcessQueueTime == 15);
        bool burstEnded = cpuProcess->IsBurstFinished();

        if (leaveQ0 || leaveQ1 || burstEnded)
            insertInNextQueue(cpuProcess, processQueue);
    }

    ProjectProcess* ioProcess = getIOProcess();

    int processId;
    if (cpuProcess != NULL)
        processId = cpuProcess->getId();
    else
        processId = -1;

    if (ioProcess != NULL && ioProcess->getId() != processId){
        ioProcess->updateProcessTime();

        int ioProcessQueueTime = ioProcess->getQueueRunningTime();

        bool leaveIO = (ioProcessQueueTime == 20);

        if (leaveIO)
            insertInNextQueue(ioProcess, SchedulerQueue::IO);
    }

    time++;

    //debugger();
    int runningProcessId;

    if (getCPUProcessAndQueue().first != NULL)
        runningProcessId = getCPUProcessAndQueue().first->getId();

    else
        runningProcessId = -1;

    if (runningProcessId != currProcessId){
        updateGanttDiagram();

        currProcessId = runningProcessId;
        changeProcessInstant = time;
    }

}

std::pair<ProjectProcess*, SchedulerQueue> Scheduler::getCPUProcessAndQueue(){
    if(!q0.empty())
        return {&q0.front(), SchedulerQueue::Q0};

    else if(!q1.empty())
        return {&q1.front(), SchedulerQueue::Q1};

    else if(!q2.empty())
        return {&q2.front(), SchedulerQueue::Q2};

    return {NULL, SchedulerQueue::EMPTY};
}

ProjectProcess* Scheduler::getIOProcess(){
    if (!IO.empty())
        return &IO.front();

    return NULL;
}

void Scheduler::insertInNextQueue(ProjectProcess* p, SchedulerQueue q)
{
    if(p->IsProcessFinished()){
        if (q == SchedulerQueue::Q0)
            q0.pop();

        else if (q == SchedulerQueue::Q1)
            q1.pop();

        else
            q2.pop();
    }

    else if(p->IsBurstFinished()){
        if (q == SchedulerQueue::Q0)
            q0.pop();

        else if (q == SchedulerQueue::Q1)
            q1.pop();

        else
            q2.pop();

        IO.push(*p);

        IO.back().setCPURunningTime(0);
        IO.back().setQueueRunningTime(0);
    }

    else if(q == SchedulerQueue::Q0){
        q0.pop();
        q1.push(*p);
        q1.back().setQueueRunningTime(0);
    }

    else if(q == SchedulerQueue::Q1){
        q1.pop();
        q2.push(*p);
        q2.back().setQueueRunningTime(0);
    }

    else if(q == SchedulerQueue::IO){
        IO.pop();
        q0.push(*p);

        q0.back().updateIOAccess();
        q0.back().setQueueRunningTime(0);
    }
}

bool Scheduler::isFinished(){
    if(q0.empty() && q1.empty() && q2.empty() && IO.empty())
        return true;

    return false;
}

void Scheduler::debugger(){
    std::cout<<"Tempo total "<<time<<std::endl;
    std::cout<<"Tamanho da fila 0: "<<q0.size()<<std::endl;
    std::cout<<"Tamanho da fila 1: "<<q1.size()<<std::endl;
    std::cout<<"Tamanho da fila 2: "<<q2.size()<<std::endl;

    ProjectProcess* cpuProcess = getCPUProcessAndQueue().first;
    ProjectProcess* ioProcess = getIOProcess();
    SchedulerQueue cpuq = getCPUProcessAndQueue().second;
    std::string cpus = " ";

    if (cpuq == SchedulerQueue::Q0)
        cpus += "Q0";

    else if (cpuq == SchedulerQueue::Q1)
        cpus += "Q1";

    else if (cpuq == SchedulerQueue::Q2)
        cpus += "Q2";

    else if (cpuq == SchedulerQueue::EMPTY)
        cpus += "EMPTY";

    else if (cpuProcess != NULL){
        std::cout<<"Processo na CPU: "<<cpuProcess->getId()<<" Fila: "<<cpus<<" CPU Time: "<<cpuProcess->getCPURunningTime()
        <<" Queue Time: "<<cpuProcess->getQueueRunningTime()<<" Num I/O: "<<cpuProcess->getCurrIOAccess()<<std::endl;
    }else{
        std::cout<<"Processo na CPU: -"<<" Fila: "<<cpus<<" CPU Time: -"
        <<" Queue Time: -"<<" Num I/O: -"<<std::endl;
    }

    if (ioProcess != NULL)
        std::cout<<"Processo na IO: "<<ioProcess->getId()<<" Time: "<<ioProcess->getQueueRunningTime()<<std::endl;
    else
        std::cout<<"Processo na IO: -"<<" Time: - "<<std::endl;

    std::cout<<std::endl;
}

void Scheduler::updateGanttDiagram(){
    GanttDiagramTimeInterval timeInterval;
    timeInterval.processID = currProcessId;
    timeInterval.initialTime = changeProcessInstant;
    timeInterval.finalTime = time;

    diagram.push_back(timeInterval);

    /*
    if (currProcessId == -1)
        printf("A CPU ficou ociosa entre os instantes %d e %d\n", changeProcessInstant, time);
    else
        printf("O Processo %d foi executado na CPU entre os instantes %d e %d\n", currProcessId, changeProcessInstant, time);*/
}

void Scheduler::printGanttDiagram(sf::RenderWindow& window){
    float proportionalFactor = 1600.0 / static_cast<float>(time);

    for (int i = 0; i < (int)diagram.size(); i++){
        int processID = diagram[i].processID;
        int initialTime = diagram[i].initialTime;
        int finalTime = diagram[i].finalTime;

        sf::RectangleShape rect;

        rect.setSize(sf::Vector2f((finalTime - initialTime) * proportionalFactor, 150));

        if (processID != -1)
            rect.setFillColor(processes[processID].getColor());
        else
            rect.setFillColor(sf::Color(0, 0, 0));

        rect.setPosition(100 + initialTime * proportionalFactor, 300);

        window.draw(rect);

        if (processID != -1){
            sf::Text processText;
            processText.setFont(font);
            processText.setCharacterSize(15);
            std::string textString = "P" + std::to_string(processID);
            processText.setString(textString);
            processText.setPosition(100 + (initialTime + finalTime) * proportionalFactor / 2 - 8, 275);

            window.draw(processText);
        }

        sf::Text initialTimeText;
        initialTimeText.setFont(font);
        initialTimeText.setCharacterSize(15);
        std::string initialTimeString = std::to_string(initialTime);
        initialTimeText.setString(initialTimeString);
        initialTimeText.setPosition(100 + initialTime * proportionalFactor - 8, 455);

        window.draw(initialTimeText);
    }

    sf::Text timeText;
    timeText.setFont(font);
    timeText.setCharacterSize(15);
    std::string timeString = std::to_string(time);
    timeText.setString(timeString);
    timeText.setPosition(1700 - 8, 455);

    window.draw(timeText);
    window.draw(titleText);
}

#include "struct.hpp"
#include <fstream>
// Author: Duc Doan & Taylor Cooper
// Date created: Nov 13th
// Last modification: Dec 4th
// Purpose: Simulation of CPU processing jobs
int main()
{
// Define necessary variables and files
    ifstream file("job.dat", ios::in | ios::binary);
    ofstream logFile("log.txt", ios::out);
    int size = sizeof(job);
// Assign amount of desired CPUs
    int numOfProcessors = 3;
    job nextJob;
    job arrived;
    job temp;
    processor procSet[numOfProcessors];
    int runIdleTime[numOfProcessors];
    for (int i = 0; i < numOfProcessors; i++)
    {
        runIdleTime[i] = 0;
    }
    int free;
    bool allBusy;
    bool busy;
    jobQueue waiting;
    int finishTypeA = 0;
    int arriveTypeA = 0;

    int arriveTypeB = 0;
    int finishTypeB = 0;

    int arriveTypeC = 0;
    int finishTypeC = 0;

    int arriveTypeD = 0;
    int finishTypeD = 0;

    float qTime = 0;
    int shortest;
    int soonestJob;
    int totalIdle = 0;
    int totalBusy = 0;

    float qAvg = 0;
    float qMax = 0;
    ;
    processor *freeProc;

    int j; //j is the index of CPU
    file.seekg(0, ios::beg);
    logFile.seekp(0, ios::beg);
    file.read(reinterpret_cast<char *>(&nextJob), size);
// Try - catch block for when queue is overloaded
    try
    {
    // i is the unit of time
        for (int i = 0; i < 10000; i++)
        {
            
            allBusy = true; // Default state
            free = 0;
            // Search for idling CPUs
            for (int j = 0; j < numOfProcessors; j++)
            {
                if (!(procSet[j].isBusy()))
                {
                    allBusy = false;
                    runIdleTime[j]++;
                    free++;
                }
                else
                {
                    //Check if job processing is finished
                    if (i == (procSet[j].getCurrentJob().process + procSet[j].getStart()))
                    {
                        if (procSet[j].getCurrentJob().type == 'A')
                        {
                            finishTypeA++;
                        }
                        else if (procSet[j].getCurrentJob().type == 'B')
                        {
                            finishTypeB++;
                        }
                        else if (procSet[j].getCurrentJob().type == 'C')
                        {
                            finishTypeC++;
                        }
                        else if (procSet[j].getCurrentJob().type == 'D')
                        {
                            finishTypeD++;
                        }
                        allBusy = false;
                        free++; //free indicates the amount of CPUs up to take on jobs
                        runIdleTime[j] = 0;
                        logFile << "Time: " << i + 1 << ": Completed processing job: "
                                << procSet[j].getCurrentJob().jobNumber << ", Job type " << procSet[j].getCurrentJob().type << ", " << procSet[j].getCurrentJob().jobTypeNumber << endl;
                        procSet[j].done();
                    }
                    else
                    {
                        runIdleTime[j]++;
                    }
                }
            }
            // If all CPUs are busy
            if (allBusy)
            {   
                //Check if a new job has arrived
                if (i == nextJob.arrival)
                {
                    arrived = nextJob;
                    logFile << "Time : " << i + 1 << ": Arrival: Overall job " << arrived.jobNumber
                            << " , Job type " << arrived.type << ", " << arrived.jobTypeNumber << ", processing time: "
                            << arrived.process << endl;
                    file.read(reinterpret_cast<char *>(&nextJob), size);
                    // Check job priority level
                    if (arrived.type != 'D')
                    {
                        waiting.add(arrived);
                    }
                    else
                    {
                        //Replace job with the shortest time left with arrived D type job
                        shortest = 1000;
                        soonestJob = 0;
                        for (int j = 0; j < numOfProcessors; j++)
                        {
                            if (shortest > (procSet[j].getCurrentJob().process + procSet[j].getStart() - i))
                            {
                                shortest = procSet[j].getCurrentJob().process + procSet[j].getStart() - i;
                                soonestJob = j;
                            }
                        }
                        temp = procSet[soonestJob].getCurrentJob();
                        temp.process = shortest;
                        temp.arrival = i;
                        waiting.priorityAdd(temp);
                        procSet[soonestJob].process(arrived, i);
                        runIdleTime[soonestJob] = 0;
                        logFile << "Time: " << i + 1 << ": Begin processing Job " << arrived.jobNumber
                                << ", Job type " << arrived.type << ", " << arrived.jobTypeNumber << " at CPU "
                                << soonestJob << endl;
                    }
                }
            }
            //If at least one CPUs is idling
            else
            {
                if (!(waiting.isEmpty()))
                {
                    if (i != nextJob.arrival)
                    {

                        for (int j = 0; j < numOfProcessors; j++)
                        {
                            if (!(procSet[j].isBusy()))
                            {
                                //Unload queue line if CPUs are free and no jobs are arriving
                                if (!(waiting.isEmpty()))
                                {
                                    temp = waiting.give();
                                    procSet[j].process(temp, i);
                                    runIdleTime[j] = 0;
                                    logFile << "Time : " << i + 1 << ": Begin processing Job "
                                            << temp.jobNumber << ", Job type " << temp.type << ", "
                                            << temp.jobTypeNumber << " at CPU " << j << endl;
                                }
                            }
                        }
                    }
                    else
                    {
                    // Put arrived job on free CPU, then unload queue line if
                    //there are free slots left for optimization
                        int j = 0;
                        busy = true;
                        while (busy)
                        {
                            busy = false;
                            if (procSet[j].isBusy())
                            {
                                j++;
                                busy = true;
                            }
                        }
                        arrived = nextJob;
                        logFile << "Time : " << i + 1 << ": Arrival: Overall job " << arrived.jobNumber
                                << " , Job type " << arrived.type << ", " << arrived.jobTypeNumber << ", processing time: "
                                << arrived.process << endl;
                        file.read(reinterpret_cast<char *>(&nextJob), size);
                        procSet[j].process(arrived, i);
                        runIdleTime[j] = 0;
                        logFile << "Time : " << i + 1 << ": Begin processing Job "
                                << arrived.jobNumber << ", Job type " << arrived.type << ", "
                                << arrived.jobTypeNumber << " at CPU " << j << endl;
                        free--;
                        while (free > 0)
                        {
                            j = 0;
                            busy = true;
                            while (busy)
                            {
                                busy = false;
                                if (procSet[j].isBusy())
                                {
                                    j++;
                                    busy = true;
                                }
                            }
                            temp = waiting.give();
                            procSet[j].process(temp, i);
                            free--;
                            runIdleTime[j] = 0;
                            logFile << "Time : " << i + 1 << ": Begin processing Job "
                                    << temp.jobNumber << ", Job type " << temp.type << ", "
                                    << temp.jobTypeNumber << " at CPU " << j << endl;
                            
                        }
                    }
                }
                //If there are no job in queue, prioritize arriving jobs
                else
                {
                    if (i == nextJob.arrival)
                    {

                        int j = 0;
                        busy = true;
                        while (busy)
                        {
                            busy = false;
                            if (procSet[j].isBusy())
                            {
                                j++;
                                busy = true;
                            }
                        }
                        arrived = nextJob;
                        logFile << "Time : " << i + 1 << ": Arrival: Overall job " << arrived.jobNumber
                                << " , Job type " << arrived.type << ", " << arrived.jobTypeNumber << ", processing time: "
                                << arrived.process << endl;
                        file.read(reinterpret_cast<char *>(&nextJob), size);
                        procSet[j].process(arrived, i);
                        runIdleTime[j] = 0;
                        logFile << "Time : " << i + 1 << ": Begin processing Job "
                                << arrived.jobNumber << ", Job type " << arrived.type << ", "
                                << arrived.jobTypeNumber << " at CPU " << j << endl;
                    }
                }
            }
            if (!(waiting.isEmpty()))
            {
                qTime = qTime + waiting.getSize();
            }
            if (i == arrived.arrival && i != 0)
            {
                if (arrived.type == 'A')
                {
                    arriveTypeA++;
                }
                else if (arrived.type == 'B')
                {
                    arriveTypeB++;
                }
                else if (arrived.type == 'C')
                {
                    arriveTypeC++;
                }
                else if (arrived.type == 'D')
                {
                    arriveTypeD++;
                }
            }
            logFile << "Time: " << i + 1 << ": Queue: ";
            if (waiting.isEmpty())
            {
                logFile << "Empty ";
            }
            else
            {
                logFile << waiting.getSize() << " ";
            }
            for (int j = 0; j < numOfProcessors; j++)
            {
                logFile << "CPU " << j << " ";
                if (procSet[j].isBusy())
                {
                    logFile << ": Runtime ";
                }
                else
                {
                    logFile << ": Idle time ";
                }
                logFile << runIdleTime[j] << ", ";
            }
            logFile << endl;
            if (nextJob.arrival == i)
            {
                i--;
            }
            for (int j = 0; j < numOfProcessors; j++)
            {
                if (procSet[j].isBusy())
                {
                    totalBusy++;
                }
                else
                {
                    totalIdle++;
                }
            }
            qAvg = qAvg + waiting.getSize();
            if (waiting.getSize() > qMax)
            {
                qMax = waiting.getSize();
            }
            if (i + 1 == 500 | i + 1 == 10000)
            {
                qAvg = qAvg / (i + 1);
                cout << "Current time: " << i + 1 << endl;
                cout << "Number of processors being used: " << numOfProcessors << endl;
                cout << "Current queue size: " << waiting.getSize() << endl;
                cout << "Average queue size: " << qAvg << endl;
                cout << "Max jobs in queue: " << qMax << endl;
                cout << "Total time jobs are in queue: " << qTime << endl;
                cout << "Average time jobs are in queue: " << qTime / (qAvg * (i + 1)) << endl;
                cout << "Amount of job type A arrived: " << arriveTypeA << endl;
                cout << "Amount of job type A finished: " << finishTypeA << endl;
                cout << "Amount of job type B arrived: " << arriveTypeB << endl;
                cout << "Amount of job type B finished: " << finishTypeB << endl;
                cout << "Amount of job type C arrived: " << arriveTypeC << endl;
                cout << "Amount of job type C finished: " << finishTypeC << endl;
                cout << "Amount of job type D arrived: " << arriveTypeD << endl;
                cout << "Amount of job type D finished: " << finishTypeD << endl;
                cout << "Total jobs completed: " << finishTypeA + finishTypeB + finishTypeC + finishTypeD << endl;
                cout << "Total processors idle time: " << totalIdle << endl;
                cout << "Total processors busy time: " << totalBusy << endl;
                cout << endl;
            }
            
        }
        file.close();
        logFile.close();
    }
    catch (bool x)
    {
        cout << "Error: Queue is overloaded." << endl;
    }
    catch (...)
    {
        cout << "Unknow error occured." << endl;
    }
}

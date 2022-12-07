#include <iostream>
#include <fstream>
//Author: Duc Doan & Taylor Cooper
//Date created: Nov 10th
//Last modification: December 1st
//Purpose: Define classes used as processors and job queue
using namespace std;

struct job{
    char type;
    int arrival;
    int process;
    int jobNumber;
    int jobTypeNumber;
};
//Node and queue to create linked list for job queue
struct Node
{
    job data;
    Node *next;
};
class jobQueue
{
private:
    Node *queue;
    Node *head;
    int count;

public:
    jobQueue();
    bool isEmpty();
    bool isFull();
    void add(job aJob);
    void priorityAdd(job aJob);
    job give();
    int getSize();
};

jobQueue::jobQueue()
{
    job blank = {'E', 0, 0};
    count = 0;
    queue = new Node;
    head = new Node;
    queue->data = blank;
    queue->next = nullptr;
}
bool jobQueue::isEmpty()
{
    if (count == 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}
bool jobQueue::isFull()
{
    if (count == 5000)
    {
        return true;
    }
    else
    {
        return false;
    }
}
void jobQueue::add(job aJob)
{
    if (isFull())
    {
        throw isFull();
    }
    else
    {
         job blank = {'E', 0, 0};
        queue->data = aJob;
        queue->next = new Node;
        queue->next->data = blank;
        queue->next->next = nullptr;

        if (isEmpty())
        {
            head = queue;
        }
        queue = queue->next;
        count++;
    }
}
void jobQueue::priorityAdd(job aJob){

    if (isFull())
    {
        throw isFull();
    }
    else
    {
        job blank = {'E', 0, 0};
        Node* temp = new Node;
        temp->data = aJob;
        temp->next = head;
        head = temp;
        if (isEmpty()){
            queue = head;
            queue = queue->next;
        }
        count++;
        temp = nullptr;
    }
}
job jobQueue::give(){
     job blank = {'E', 0, 0};
    if (isEmpty()){
        return blank;
    }
    else{
        Node* temp = head;
        head = head->next;
        temp->next = nullptr;
        count--;
        return temp->data;
    }
}
int jobQueue::getSize(){
    return count;
}


//Each processor
class processor{
    private:
        job currentJob;
        bool status;
        int startProcess;
    public:
        processor();
        bool isBusy();
        void process(job givenJob, int time);
        void done();
        job getCurrentJob();
        int getStart();
};

processor::processor(){
    job blank = {'E',0,0};
    currentJob = blank;
    status = false;
}
job processor::getCurrentJob(){
    return currentJob;
}
int processor::getStart(){
    return startProcess;
}

void processor::process(job givenJob, int time){
    currentJob = givenJob;
    status = true;
    startProcess = time;
    
}
void processor::done(){
    
    job blank = {'E',0,0};
    currentJob = blank;
    status = false;
    startProcess = 0;
}
bool processor::isBusy(){
    return status;
}
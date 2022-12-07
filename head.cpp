#include <iostream>
#include <fstream>
//Author: Duc Doan & Taylor Cooper
//Date created: Nov 8th
//Last modification: Nov 31st
//Purpose: Generate jobs, sort and write into file
using namespace std;

struct job{
    char type;
    int arrival;
    int process;
    int jobNumber;
    int jobTypeNumber;
};
int main(){
    ofstream file("job.dat");
    int cummulative = 0;
    job aJob[5920];
    //Generate A type jobs
    for (int i = 0; i < 2500; i++){
        aJob[i].type = 'A';
        cummulative = cummulative + 4+ rand()%3;
        aJob[i].arrival = cummulative;
        aJob[i].process = 1 + rand()%5;
    }
    cummulative = 0;
    //Generate B type jobs
    for (int i = 2500; i < 4170; i++){
        aJob[i].type = 'B';
        cummulative = cummulative + 6 + rand()%3;
        aJob[i].arrival = cummulative;
        aJob[i].process = 2 + rand()%7;
    }
    cummulative = 0;
    //Generate C type jobs
    for (int i = 4170; i < 5420; i++){
        aJob[i].type = 'C';
        cummulative = cummulative + 4 + rand()%15;
        aJob[i].arrival = cummulative;
        aJob[i].process = 6 + rand()%7;
    }
    cummulative = 0;
    //Generate D type jobs
    for (int i = 5420; i < 5920; i++){
        aJob[i].type = 'D';
        cummulative = cummulative + 14 + rand()%11;
        aJob[i].arrival = cummulative;
        aJob[i].process = 7 + rand()%11;
    }
    job temp;
    bool flag = false;
    //Bubble sort list
    while (!flag){
        flag = true;
        for (int i = 0; i < 5919; i++){
            if (aJob[i].arrival > aJob[i+1].arrival){
                temp = aJob[i];
                aJob[i] = aJob[i+1];
                aJob[i+1] = temp;
                flag = false;
            }
        }
    }
    int jobTypeA = 1;
    int jobTypeB = 1;
    int jobTypeC = 1;
    int jobTypeD = 1;
    job husk;
    //Assign job number and job type number
    for (int i = 0; i < 5920; i++){
        aJob[i].jobNumber = i + 1;
        if (aJob[i].type == 'A'){
            aJob[i].jobTypeNumber = jobTypeA;
            jobTypeA++;
        }
        else if (aJob[i].type == 'B'){
            aJob[i].jobTypeNumber = jobTypeB;
            jobTypeB++;
        }
        else if (aJob[i].type == 'C'){
            aJob[i].jobTypeNumber = jobTypeC;
            jobTypeC++;
        }else if (aJob[i].type == 'D'){
            aJob[i].jobTypeNumber = jobTypeD;
            jobTypeD++;
        }
        husk = aJob[i];
        //Write into job.dat file
        file.write(reinterpret_cast<char*> (&husk), sizeof(job));
        cout << aJob[i].type << " " << aJob[i].arrival + 1 << " " << aJob[i].process << endl;
    }
}


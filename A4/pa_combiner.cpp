#include <iostream>
#include <tuple>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <pthread.h>
#include <error.h>
#include <queue>
#include <sys/mman.h>

using namespace std;

int bufferSize;

void *mem;

bool finished = false;
int numberOfReducers;
struct item{
    string id;
    string topic;
    int score;
};


struct buffer {
    queue<item> data;
    pthread_cond_t cond_full = PTHREAD_COND_INITIALIZER;
    pthread_cond_t cond_empty = PTHREAD_COND_INITIALIZER;
    pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;
    bool repeatedID = false;
};
pthread_mutex_t finished_mtx = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t printing_mtx = PTHREAD_MUTEX_INITIALIZER;

map<int,buffer> reducerBuffers;

static void *mapper(void *arg) {

    string inputLine;
    string id;
    string action;
    string topic;
    int score;
    int id_first;
    bool repeated = false;

    int counter = 0;
    vector<vector<string>> data;

    
    while(true)
    {

        getline(cin,inputLine);


        
        
        if(inputLine.empty())
        {
            break;
        }
        id = inputLine.substr(1,4);
        action = inputLine.substr(6,1);
        
        auto position = inputLine.find(')');

         if (position == std::string::npos) {
            cout << "ERROR ')' NOT FOUND " << endl;
            continue;
         }
        topic = inputLine.substr(8,(position - 8));
        
        data.push_back({id,action,topic});
        
        if (action == "P")
        {
            score = 50;
        }
        else if(action == "L")
        {
            score = 20;
        }
        else if(action == "D")
        {
            score = -10;
        }
        else if(action == "C")
        {
            score = 30;
        }
        else if(action == "S")
        {
            score = 40;
        }
        
        if(id[0] == id[1] && id[1] == id[2] && id[2] == id[3] && id[3] == id[0])
        {
            id_first = (id[0] - '0') - 1;
            repeated = true;
        }
        else{
            id_first = stoi(id) - 1;
        }

        
        

        pthread_mutex_lock(&reducerBuffers[id_first].mtx);
        while (reducerBuffers[id_first].data.size() >= bufferSize) {
            pthread_cond_wait(&reducerBuffers[id_first].cond_full, &reducerBuffers[id_first].mtx);
        }

        reducerBuffers[id_first].data.push({id,topic,score});
        if(repeated)
        {
            reducerBuffers[id_first].repeatedID = true;

        }
  
        //cout << "produced!!! " << inputLine<< endl;

        pthread_cond_signal(&reducerBuffers[id_first].cond_empty);

        pthread_mutex_unlock(&reducerBuffers[id_first].mtx);


   


        counter++;

    }

    
    pthread_mutex_lock(&finished_mtx);
    finished = true;
    pthread_mutex_unlock(&finished_mtx);

    for (int i = 0; i < numberOfReducers; ++i) {
        pthread_cond_broadcast(&reducerBuffers[i].cond_empty);
    }

    pthread_exit((void *)arg);


}


static void *reducer(void *arg) {

    long num = (long) arg;

    std::map<string, int> dataMap;

    item testData;
    string id;

   
    while(true)
    {

        pthread_mutex_lock(&reducerBuffers[num].mtx);

        while(reducerBuffers[num].data.empty())
        {
            pthread_mutex_lock(&finished_mtx);
            if (finished) {
                pthread_mutex_unlock(&finished_mtx);
                pthread_mutex_unlock(&reducerBuffers[num].mtx);



                if(reducerBuffers[num].repeatedID)
                {
                    for(int i = 0 ; i < 4; i++)
                    {
                        id.append(to_string(num +1));
                    }

                }
                else{
                    id = to_string(num+1);
                    for(int i = 1; i < 4 - id.length() ; i++)
                    id.insert(0,"0");
                }


                pthread_mutex_lock(&printing_mtx);
                for (auto it = dataMap.begin(); it != dataMap.end(); ++it)
                {
                    cout << "(" << id << "," << it->first << "," << it->second << ")"<< endl;
                }
                pthread_mutex_unlock(&printing_mtx);
                
                pthread_exit((void *)arg);
            }
            pthread_mutex_unlock(&finished_mtx);

            pthread_cond_wait(&reducerBuffers[num].cond_empty, &reducerBuffers[num].mtx);

        }

        testData = reducerBuffers[num].data.front();
        

        reducerBuffers[num].data.pop();

        pthread_cond_signal(&reducerBuffers[num].cond_full);

        pthread_mutex_unlock(&reducerBuffers[num].mtx);

        dataMap[testData.topic] += testData.score;

    }





    pthread_exit((void *)arg);


}

int main(int argc, char const *argv[])
{

    pthread_t tid;
    numberOfReducers = atoi(argv[2]);
    bufferSize = atoi(argv[1]);
    int i, create;


    mem = mmap(NULL,sizeof(item)*bufferSize*numberOfReducers,PROT_READ | PROT_WRITE, MAP_ANONYMOUS| MAP_PRIVATE,-1,0);

    pthread_t reducers[numberOfReducers];

    for(i = 0; i < numberOfReducers; i ++)
    {
        reducerBuffers[i] = {queue<item>()};
        create = pthread_create(&reducers[i], NULL, reducer, (void*) i);
  

    }

    create = pthread_create(&tid, NULL, mapper, (void*)i);


    create = pthread_join(tid,nullptr);

    for(i = 0; i < numberOfReducers; i ++)
    {
        
        create = pthread_join(reducers[i], nullptr);
  

    }
    int ret;
    ret = munmap(mem,sizeof(item)*bufferSize*numberOfReducers);
 
    
    return 0;
}



#include <iostream>
#include <tuple>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <pthread.h>
using namespace std;

struct Account
{
    int id;
    int balance;
    pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;
};


map<int,Account> accounts;


struct Transfer {
    int from;
    int to;
    int amount;
};
map<int,Transfer> transfers;

void* transfer(void* arg) 
{
    auto* vec = static_cast<vector<int>*>(arg);
    int from, to,amount;
    
    for (int i = 0; i < vec->size(); i++)
    {
        from = transfers[vec->at(i)].from;
        to = transfers[vec->at(i)].to;
        amount = transfers[vec->at(i)].amount;
        if(from < to)
        {
            pthread_mutex_lock(&accounts[from].mtx);
            pthread_mutex_lock(&accounts[to].mtx);
        }
        else{
            pthread_mutex_lock(&accounts[to].mtx);
            pthread_mutex_lock(&accounts[from].mtx);
        }
        
        accounts[from].balance -= amount;
        accounts[to].balance += amount;


        pthread_mutex_unlock(&accounts[from].mtx);
        pthread_mutex_unlock(&accounts[to].mtx);
    }
    
    pthread_exit((void *)arg);
}

int main(int argc, char const *argv[]) {

    int numOfAccounts = 0;
    int numOfWorkers = stoi(argv[2]);

    auto fd = fopen(argv[1],"r");
    string inputLine;
    char buffer [256];

    int transferCount = 0;
    cout << ":got here!!!" << endl;

   while( fgets(buffer,sizeof(buffer),fd))
   {
        if(buffer == NULL)
        {
            continue;
        }
        else{
            string line(buffer);
      
            istringstream iss(line);
            vector<std::string> tokens;
            string token;
    
            while (iss >> token) {  
                tokens.push_back(token);
            }
    
            if(line[0] == 'T')
            {
                transfers[transferCount] = {stoi(tokens[1]),stoi(tokens[2]),stoi(tokens[3])};
    
                transferCount++;
            }
            else
            {
                accounts[stoi(tokens[0])] = {stoi(tokens[0]),stoi(tokens[1])};
    
                numOfAccounts++;
    
    
            }

        }

   }
   cout << ":got here!!!" << endl;

   vector<pthread_t> threads(numOfWorkers);
   int transfersPerWorker = transferCount/numOfWorkers;
   int extraTransfers = transferCount % numOfWorkers;

   int thisArg = 0;
   int thisTransferCount = 0;

   vector<int> threadArgs;
   
   for (int i = 0; i < numOfWorkers; i++)
   {
        thisArg = transfersPerWorker;
        if(extraTransfers > 0)
        {
            thisArg++;
            extraTransfers--;
        }
        for (int j = 0; j < thisArg; j++)
        {
            threadArgs.push_back(thisTransferCount);
            
            thisTransferCount++;
            
        }
       

        pthread_create(&threads[i], NULL, transfer,static_cast<void*>(&threadArgs));

        threadArgs.clear();
   }
   



    
   for (auto& i : threads) {
        pthread_join(i, nullptr);
    }

    
    

    for(auto it : accounts)
    {
        cout << it.first << " " << it.second.balance << endl;
    }
    


    return 0;
}
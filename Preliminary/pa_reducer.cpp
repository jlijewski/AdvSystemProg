#include <iostream>
#include <tuple>
#include <sstream>
#include <string>
#include <vector>
#include <map>
using namespace std;

int main() {
   
    
    string inputLine;
    string id,prevId;
    string action;
    string topic;
    int score;
    int end;
    

    int counter = 0;
    vector<vector<string>> data;
    std::map<string, std::map<string, int>> dataMap;
    
    while(true)
    {
        getline(cin,inputLine);


        
        
        if(inputLine.empty())
        {
            for (auto it = dataMap[id].begin(); it != dataMap[id].end(); ++it)
            {
                cout << "(" << id << "," << it->first << "," << it->second << ")"<< endl;
            }
            break;
        }

        prevId = id;
        id = inputLine.substr(1,4);
        action = "test";//nputLine.substr(6,1);
        
        auto position = inputLine.find(')');

         if (position == std::string::npos) {
            cout << "ERROR ')' NOT FOUND " << endl;
            continue;
         }
        //topic = inputLine.substr(8,(position - 8));
        
        if(inputLine.empty())
        {
            score = 0;
        }
        else{
            
            score = stoi(inputLine.substr(position - 2,2));
            if(inputLine[position - 3] == '-')
            {
                score *= -1;
            }
        }

        
        if(score < 0 )
        {
            end = position - 4;
            position -= 5;
        }
        else{
            end = position - 3;
            position -= 4;
        }
       
        while (inputLine[position] != ',')
        {
            position -= 1;
        }
        topic = inputLine.substr(position+1, (end - position) - 1);


        
        //data.push_back({id,action,topic});
        
        // if (action == "P")
        // {
        //     score = 50;
        // }
        // else if(action == "L")
        // {
        //     score = 20;
        // }
        // else if(action == "D")
        // {
        //     score = -10;
        // }
        // else if(action == "C")
        // {
        //     score = 30;
        // }
        // else if(action == "S")
        // {
        //     score = 40;
        // }
        
   
        // cout << "(" << id << "," << topic << "," << score << ")"<< endl;


        // counter++;
        
        dataMap[id][topic] += score;

        if(counter != 0 && prevId != id )
        {
            for (auto it = dataMap[prevId].begin(); it != dataMap[prevId].end(); ++it)
            {
                cout << "(" << prevId << "," << it->first << "," << it->second << ")"<< endl;
            }

        }


        counter++;
    }
    

    // for (auto it = dataMap.begin(); it != dataMap.end(); ++it)
    // {
    //     for (auto it2 = it->second.begin(); it2 != it->second.end(); ++it2)
    //     {
    //         cout << it2->first << " " << it2->second << endl;
    //     }
        


    // }

    

    // for(int i = 0; i < counter; i++)
    // {
    //     cout << data[i][0] << data[i][1] << data[i][2] << endl;
    // }

    return 0;
}

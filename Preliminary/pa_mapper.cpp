#include <iostream>
#include <tuple>
#include <sstream>
#include <string>
#include <vector>
using namespace std;

int main() {
   
    
    string inputLine;
    string id;
    string action;
    string topic;
    int score;

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
        
   
        cout << "(" << id << "," << topic << "," << score << ")"<< endl;


        counter++;

    }

    // for(int i = 0; i < counter; i++)
    // {
    //     cout << data[i][0] << data[i][1] << data[i][2] << endl;
    // }

    return 0;
}

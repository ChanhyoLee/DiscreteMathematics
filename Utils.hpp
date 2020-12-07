#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <sstream>

using namespace std;

vector<string> splitBy(string input, char delimiter){
    vector<string> answer;
    stringstream ss(input);
    string temp;

    while (getline(ss, temp, delimiter)) {
        answer.push_back(temp);
    }
    return answer;
}


string Remove(string str, string delimiters){
    vector<string> splited_delimiters;
    for(int i=0; i<delimiters.length(); i++){
        splited_delimiters.push_back(delimiters.substr(i,1));
    }
    for(string delimiter : splited_delimiters){
        int k = str.find(delimiter);
        while(k>=0){
            string k_afterStr = str.substr(k+1, str.length()-k);
            str = str.erase(k) + k_afterStr;
            k = str.find(delimiter);
        }
    }
    return str;
}

string preprocess(string sentence) {
    transform(sentence.begin(), sentence.end(), sentence.begin(), [](unsigned char c){ return std::tolower(c); });
    sentence = Remove(sentence, ",.?!/\"\'><:-");
    return sentence;
}

bool cmp(const pair<string,int>& a, const pair<string,int>& b) {
    if (a.second == b.second) return a.first < b.first;
    return a.second < b.second;
}
#include <string>
#include <vector>
#include <sstream>
#include "Utils.hpp"

using namespace std;

#define HAM 0
#define SPAM 1


class Mail{
private:
    string subject;
    vector<string> body;
    int label;

    //map<int, string>* m_pIndex2Vocab;
    map<string, int>* m_pVocab2Frequency;
    //map<string, int>* m_pVocab2Index;
    int n_vocabs;

public:
    Mail(){
        subject="";
        body = vector<string>();
        label = -1;

        //m_pIndex2Vocab = new map<int, string>();
        m_pVocab2Frequency = new map<string, int>();
        //m_pVocab2Index = new map<string, int>();
        n_vocabs = 0;
    }
    void setSubject(string str){
        addSentence(str);
        subject = str;
    }
    void addBody(string str){
        addSentence(str);
        body.push_back(str);
    }
    void setLabel(string str_int){
        stringstream ssInt(str_int);
        ssInt >> label;
    }
    void setLabel(int n){
        label = n;
    }
    string getSubject(){
        return subject;
    }
    vector<string> getBody(){
        return body;
    }
    int getLabel(){
        return label;
    }
    void printMail(){
        if(label==SPAM){
            cout << "SSSSSSSSSSSSSSSSSSSS" << endl;
            cout << subject << endl;
            for(string line: body){
                cout << line << endl;
            }
            cout << "SSSSSSSSSSSSSSSSSSSS" << endl;
        }
        else{
            cout << "HHHHHHHHHHHHHHHHHHHH" << endl;
            cout << subject << endl;
            for(string line: body){
                cout << line << endl;
            }
            cout << "HHHHHHHHHHHHHHHHHHHH" << endl;
        }

    }
    void addSentence(string sentence){
        //cout<<"<<<<<<<<<<<<<<<<  AddSentence  >>>>>>>>>>>>>>>>>>>>"<<endl;
        vector<string> words = splitBy(sentence, ' ');
        for(string word: words){
            addWord(word);
        }
        vector<string>().swap(words);
    }

    void addWord(string word){
        if(m_pVocab2Frequency->find(word)==m_pVocab2Frequency->end()){
            //m_pVocab2Index->insert(make_pair(word, n_vocabs));
            m_pVocab2Frequency->insert(make_pair(word, 1));
            //m_pIndex2Vocab->insert(make_pair(n_vocabs, word));
            n_vocabs ++;
        }
        else{
            m_pVocab2Frequency->at(word)++;
        }
    }

    map<string, int>* getpVocab2Frequency(){
        return m_pVocab2Frequency;
    }
};
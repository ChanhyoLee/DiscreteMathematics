#include <iostream>
#include <string>
#include <fstream>
#include <algorithm>
#include <map>
#include <time.h>

#include "Mail.hpp"

using namespace std;

#define NCOL 4
extern int DIFFERENCE_THRESHOLD;

//#define DIFFERENCE_THRESHOLD 50

class MailDataset{
private:
    string path;
    vector<Mail> whole_mails;

    vector<Mail> spam_mails;
    vector<Mail> ham_mails;

    vector<Mail> train_mails;
    vector<Mail> test_mails;
    int num_lines;
    int train_count;
    int test_count;

    int ham_count;
    int spam_count;

    map<string, int>* m_pHAM_Vocab2Frequency;
    map<string, int>* m_pSPAM_Vocab2Frequency;
public:

    MailDataset(){
        path = "spam_ham_dataset.csv/spam_ham_dataset.csv";
        num_lines = 0;
        train_count = 0;
        test_count = 0;
        ham_count = 0;
        spam_count = 0;

        m_pHAM_Vocab2Frequency = new map<string, int>();
        m_pSPAM_Vocab2Frequency = new map<string, int>();
        build_dataset();
        chooseMails();
    }

    void build_dataset(){
        int count = 0;
        fstream fs;
        fs.open(path, ios::in);

        string temp_string; 
        string temp_label;
        Mail temp_mail; //한 행의 임시 Mail 객체
        srand(time(0));
        while(!fs.eof()){
            getline(fs, temp_string,'\"');
            count++;
            //cout << "||" << temp_string << "||" <<endl;
            if(count%2==1){
                temp_label = temp_string;
            }
            else{
                vector<string> wholeMailText = splitBy(temp_string, '\n');
                for(int i=0; i<wholeMailText.size(); i++){
                    string sentence = wholeMailText.at(i);
                    //cout << sentence << endl;
                    sentence = preprocess(sentence);
                    //addSentence(sentence);
                    if(i==0){
                        temp_mail.setSubject(sentence);
                        //cout << sentence << endl;
                    }
                    else{
                        temp_mail.addBody(sentence);
                    }
                }
                if(temp_label.find("ham")!=string::npos){
                    temp_mail.setLabel(HAM);
                    ham_count ++;
                    ham_mails.push_back(temp_mail);
                }
                else if(temp_label.find("spam")!= string::npos){
                    temp_mail.setLabel(SPAM);
                    spam_count ++;
                    spam_mails.push_back(temp_mail);
                }
                else{
                    temp_mail = Mail();
                    continue;
                }           
                whole_mails.push_back(temp_mail);
                
                temp_mail = Mail();
                num_lines ++;
            }
        }
        fs.close();
        // cout << ham_count << endl;
        // cout << spam_count << endl;
    }

    void chooseMails(){
        for(int i=0; i<1000; i++){
            int random = rand()%(spam_count-1);
            train_mails.push_back(spam_mails.at(random));
            updateVocab2Frequency(spam_mails.at(random));
            spam_mails.erase(spam_mails.begin()+random);
            spam_count--;
            train_count++;
        }
        for(int i=0; i<1000; i++){
            int random = rand()%(ham_count-1);
            train_mails.push_back(ham_mails.at(random));
            updateVocab2Frequency(ham_mails.at(random));
            ham_mails.erase(ham_mails.begin()+random);
            ham_count--;
            train_count++;
        }
        for(int i=0; i<200; i++){
            int random = rand()%(spam_count-1);
            test_mails.push_back(spam_mails.at(random));
            spam_mails.erase(spam_mails.begin()+random);
            spam_count--;
            test_count++;
        }
        for(int i=0; i<200; i++){
            int random = rand()%(ham_count-1);
            test_mails.push_back(ham_mails.at(random));
            ham_mails.erase(ham_mails.begin()+random);
            ham_count--;
            test_count++;
        }
        vector< pair<string, int> > spamwords = this->selectUniqueSPAMword();
        for(int i=0; i<spamwords.size(); i++){
            cout << spamwords.at(i).first << "\t";
            cout << spamwords.at(i).second << "\t";
            if(i%5==4) cout << endl;
        }
        cout << endl;
        // for(int i=0; i<spamwords.size(); i++){
        //     if(i%5==4) cout << endl;
        // }
        cout << endl;  
    }
    string getPath(){
        return path;
    }
    vector<Mail> getTrainMails(){
        return train_mails;
    }
    vector<Mail> getTestMails(){
        return test_mails;
    }

    int getNumLines(){
        return num_lines;
    }

    void updateVocab2Frequency(Mail mail){
        map<string, int>* each_Vocab2Frequency = mail.getpVocab2Frequency();
        map<string, int> :: iterator iter;

        if(mail.getLabel()==HAM){
            for(iter=each_Vocab2Frequency->begin(); iter!=each_Vocab2Frequency->end(); iter++){
                if(m_pHAM_Vocab2Frequency->find(iter->first)==m_pHAM_Vocab2Frequency->end()){
                    m_pHAM_Vocab2Frequency->insert(make_pair(iter->first, iter->second));
                }
                else{
                    m_pHAM_Vocab2Frequency->at(iter->first)++;
                }
            }
        }
        else if(mail.getLabel()==SPAM){
            for(iter=each_Vocab2Frequency->begin(); iter!=each_Vocab2Frequency->end(); iter++){
                if(m_pSPAM_Vocab2Frequency->find(iter->first)==m_pSPAM_Vocab2Frequency->end()){
                    m_pSPAM_Vocab2Frequency->insert(make_pair(iter->first, iter->second));
                }
                else{
                    m_pSPAM_Vocab2Frequency->at(iter->first)++;
                }
            }
        }
    }

    map<string, int>* getpHAMVocab2Frequency(){
        return m_pHAM_Vocab2Frequency;
    }
    map<string, int>* getpSPAMVocab2Frequency(){
        return m_pSPAM_Vocab2Frequency;
    }
    int getNumofTrains(){
        return train_count;
    }
    int getNumofTests(){
        return test_count;
    }

    vector< pair<string, int> > getHAMSortedVector_ByValue(){
        vector< pair<string, int> > vec1(m_pHAM_Vocab2Frequency->begin(), m_pHAM_Vocab2Frequency->end());
        sort(vec1.begin(), vec1.end(), cmp);
        return vec1;
    }   
    vector< pair<string, int> > getSPAMSortedVector_ByValue(){
        vector< pair<string, int> > vec2(m_pSPAM_Vocab2Frequency->begin(), m_pSPAM_Vocab2Frequency->end());
        sort(vec2.begin(), vec2.end(), cmp);
        return vec2;
    }
    vector< pair<string, int> > selectUniqueSPAMword(){
        map<string, int> :: iterator iter;
        vector< pair<string, int> > result;
        vector< pair<string, int> > sortedSPAM_V2F = getSPAMSortedVector_ByValue();
        vector< pair<string, int> > sortedHAM_V2F = getHAMSortedVector_ByValue();
        int difference = 100000;
        for(int i=0; i<sortedSPAM_V2F.size(); i++){
            pair<string, int> each_pair=sortedSPAM_V2F.at(i);
            if(m_pHAM_Vocab2Frequency->find(each_pair.first)!=m_pHAM_Vocab2Frequency->end()){
                difference = (int)(each_pair.second - m_pHAM_Vocab2Frequency->at(each_pair.first));
                if(difference > DIFFERENCE_THRESHOLD) result.push_back(each_pair);
            }
            else if(m_pHAM_Vocab2Frequency->find(each_pair.first)==m_pHAM_Vocab2Frequency->end()){
                difference = (int)each_pair.second;
                if(difference > DIFFERENCE_THRESHOLD) result.push_back(each_pair);
            }
        }
        return result;
    }
};
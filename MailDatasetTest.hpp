#include <time.h>

#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <string>

#include "Mail.hpp"

using namespace std;

#define NCOL 4
extern int DIFFERENCE_THRESHOLD;

//#define DIFFERENCE_THRESHOLD 50

class MailDatasetTest {
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
    MailDatasetTest() {
        path = "spam_ham_dataset.csv/spam_ham_dataset.csv";
        num_lines = 0;
        train_count = 0;
        test_count = 0;
        ham_count = 0;
        spam_count = 0;

        m_pHAM_Vocab2Frequency = new map<string, int>();
        m_pSPAM_Vocab2Frequency = new map<string, int>();
        build_dataset();

        cout << "ham_count: " << ham_mails.size() << "\tspam_count: " << spam_mails.size() << endl;
        chooseMails();
    }

    void build_dataset() {
        int count = 0;

        ifstream t(path);
        stringstream buffer;
        buffer << t.rdbuf();
        string wholeDataset = buffer.str();

        srand(time(0));
        vector<string> mailVector = splitByString(wholeDataset, "\",");
        count = mailVector.size();
        cout << "Mail Count: " << count << endl;
        for (int i = 0; i < mailVector.size(); i++) {
            Mail temp_mail;
            string eachMail = mailVector.at(i);
            vector<string> mailText = splitByString(eachMail, ",\"");
            if (mailText.at(0).find("ham") != string::npos) {
                ham_count++;
                temp_mail.setLabel(HAM);
                vector<string> wholeMailText = splitBy(mailText.at(1), '\n');
                for (int j = 0; j < wholeMailText.size(); j++) {
                    if (j == 0)
                        temp_mail.setSubject(preprocess(wholeMailText.at(0)));
                    else
                        temp_mail.addBody(preprocess(wholeMailText.at(j)));
                }
                ham_mails.push_back(temp_mail);
            } else if (mailText.at(0).find("spam") != string::npos) {
                spam_count++;
                temp_mail.setLabel(SPAM);
                vector<string> wholeMailText = splitBy(mailText.at(1), '\n');
                for (int j = 0; j < wholeMailText.size(); j++) {
                    if (j == 0)
                        temp_mail.setSubject(preprocess(wholeMailText.at(0)));
                    else
                        temp_mail.addBody(preprocess(wholeMailText.at(j)));
                }
                spam_mails.push_back(temp_mail);
            }
            whole_mails.push_back(temp_mail);
            t.close();
        }
    }

    void chooseMails() {
        for (int i = 0; i < 1000; i++) {
            int random = rand() % (spam_count - 1);
            train_mails.push_back(spam_mails.at(random));
            updateVocab2Frequency(spam_mails.at(random));
            spam_mails.erase(spam_mails.begin() + random);
            spam_count--;
            train_count++;
        }
        for (int i = 0; i < 1000; i++) {
            int random = rand() % (ham_count - 1);
            train_mails.push_back(ham_mails.at(random));
            updateVocab2Frequency(ham_mails.at(random));
            ham_mails.erase(ham_mails.begin() + random);
            ham_count--;
            train_count++;
        }
        for (int i = 0; i < 200; i++) {
            int random = rand() % (spam_count - 1);
            test_mails.push_back(spam_mails.at(random));
            spam_mails.erase(spam_mails.begin() + random);
            spam_count--;
            test_count++;
        }
        for (int i = 0; i < 200; i++) {
            int random = rand() % (ham_count - 1);
            test_mails.push_back(ham_mails.at(random));
            ham_mails.erase(ham_mails.begin() + random);
            ham_count--;
            test_count++;
        }
        vector<pair<string, int> > spamwords = this->selectUniqueSPAMword();
        for (int i = 0; i < spamwords.size(); i++) {
            cout << spamwords.at(i).first << "\t";
            cout << spamwords.at(i).second << "\t";
            if (i % 5 == 4) cout << endl;
        }
        cout << endl;
    }

    string getPath() {
        return path;
    }
    vector<Mail> getTrainMails() {
        return train_mails;
    }
    vector<Mail> getTestMails() {
        return test_mails;
    }

    int getNumLines() {
        return num_lines;
    }

    void updateVocab2Frequency(Mail mail) {
        map<string, int>* each_Vocab2Frequency = mail.getpVocab2Frequency();
        map<string, int>::iterator iter;

        if (mail.getLabel() == HAM) {
            for (iter = each_Vocab2Frequency->begin(); iter != each_Vocab2Frequency->end(); iter++) {
                if (m_pHAM_Vocab2Frequency->find(iter->first) == m_pHAM_Vocab2Frequency->end()) {
                    m_pHAM_Vocab2Frequency->insert(make_pair(iter->first, 1));
                } else {
                    m_pHAM_Vocab2Frequency->at(iter->first)++;
                }
            }
        } else if (mail.getLabel() == SPAM) {
            for (iter = each_Vocab2Frequency->begin(); iter != each_Vocab2Frequency->end(); iter++) {
                if (m_pSPAM_Vocab2Frequency->find(iter->first) == m_pSPAM_Vocab2Frequency->end()) {
                    m_pSPAM_Vocab2Frequency->insert(make_pair(iter->first, 1));
                } else {
                    m_pSPAM_Vocab2Frequency->at(iter->first)++;
                }
            }
        }
    }

    map<string, int>* getpHAMVocab2Frequency() {
        return m_pHAM_Vocab2Frequency;
    }
    map<string, int>* getpSPAMVocab2Frequency() {
        return m_pSPAM_Vocab2Frequency;
    }
    int getNumofTrains() {
        return train_count;
    }
    int getNumofTests() {
        return test_count;
    }

    vector<pair<string, int> > getHAMSortedVector_ByValue() {
        vector<pair<string, int> > vec1(m_pHAM_Vocab2Frequency->begin(), m_pHAM_Vocab2Frequency->end());
        sort(vec1.begin(), vec1.end(), cmp);
        return vec1;
    }
    vector<pair<string, int> > getSPAMSortedVector_ByValue() {
        vector<pair<string, int> > vec2(m_pSPAM_Vocab2Frequency->begin(), m_pSPAM_Vocab2Frequency->end());
        sort(vec2.begin(), vec2.end(), cmp);
        return vec2;
    }
    vector<pair<string, int> > selectUniqueSPAMword() {
        map<string, int>::iterator iter;
        vector<pair<string, int> > result;
        vector<pair<string, int> > sortedSPAM_V2F = getSPAMSortedVector_ByValue();
        vector<pair<string, int> > sortedHAM_V2F = getHAMSortedVector_ByValue();
        int difference = 100000;
        for (int i = 0; i < sortedSPAM_V2F.size(); i++) {
            pair<string, int> each_pair = sortedSPAM_V2F.at(i);
            if (m_pHAM_Vocab2Frequency->find(each_pair.first) != m_pHAM_Vocab2Frequency->end()) {
                difference = (int)(each_pair.second - m_pHAM_Vocab2Frequency->at(each_pair.first));
                if (difference > DIFFERENCE_THRESHOLD && m_pHAM_Vocab2Frequency->at(each_pair.first) < 100) result.push_back(each_pair);
            } else if (m_pHAM_Vocab2Frequency->find(each_pair.first) == m_pHAM_Vocab2Frequency->end()) {
                difference = (int)each_pair.second;
                if (difference > DIFFERENCE_THRESHOLD) result.push_back(each_pair);
            }
        }
        return result;
    }

    vector<string> splitByString(string str, string delimiter) {
        vector<string> ret;
        size_t pos = 0;
        string token;
        do {
            pos = str.find(delimiter);  // delimiter를 못찾으면 텍스트 끝까지 token으로 취급
            if (pos == string::npos) {
                token = str;
            } else {
                token = str.substr(0, pos);
            }
            if (token[token.size() - 1] == '\r') {
                token = token.substr(0, token.size() - 1);
            }
            ret.push_back(token);
            str.erase(0, pos + delimiter.length());
        } while (pos != string::npos);
        return ret;
    }
};
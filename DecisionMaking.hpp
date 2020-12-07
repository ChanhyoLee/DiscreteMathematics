//#include "MailDataset.hpp"
#include "MailDatasetTest.hpp"
#include "WordProbability.hpp"

//#define PROBABILITY_THRESHOLD 0.8
extern double PROBABILITY_THRESHOLD;

class DecisionMaking{
private:
    MailDatasetTest* dataset;
    map<string, int>* m_ptest_Vocab2Frequency;
    double accuracy;

public:
    DecisionMaking(MailDatasetTest* maildataset){
        dataset = maildataset;
        accuracy = 0;
        vector<Mail> test_mails = dataset->getTestMails();
        int count = 0;
        cout << "Result for 10 mails" << endl;
        cout << "origin" << "\t" << "result" << endl;
        for(int i=0; i<test_mails.size(); i++){
            m_ptest_Vocab2Frequency = new map<string, int>();
            bool check = a_simple_bayesian_spam_filter(test_mails.at(i));
            if(i%40==39){
                printf("%6d,\t%6d\n",test_mails.at(i).getLabel(), (int)check);
                count ++;
            }
            if(test_mails.at(i).getLabel()==(int)check) {
                accuracy++;
            }
            else{
                //cout << "--------------------------------" << endl;
            }
            if(i%100 == 99){
                printf("%d th mail\n", i+1);
            }
        }
        cout << "Right Decision: " << accuracy << endl;
        cout << "Test Mail Size: " << test_mails.size() << endl;
        accuracy = accuracy/test_mails.size();
        cout << "ACCURACY: " << accuracy << endl;

    }
    void build_vocab(Mail mail){
        map<string, int>* each_Vocab2Frequency = mail.getpVocab2Frequency();
        map<string, int> :: iterator iter;
        for(iter=each_Vocab2Frequency->begin(); iter!=each_Vocab2Frequency->end(); iter++){
            if(m_ptest_Vocab2Frequency->find(iter->first)==m_ptest_Vocab2Frequency->end()){
                m_ptest_Vocab2Frequency->insert(make_pair(iter->first, iter->second));
            }
            else{
                m_ptest_Vocab2Frequency->at(iter->first)++;
            }
        }
    }

    bool a_simple_bayesian_spam_filter(Mail mail){
        build_vocab(mail);
        double result = calculateProbability();
        //cout <<"result: " << result << endl;
        // if(result > 0.5){
        //     cout << "result: " << result << "\t" << "label: " << mail.getLabel() << endl;
        // }
        // if(result>PROBABILITY_THRESHOLD&&mail.getLabel()==HAM){
        //     cout << "origin ham result: " << result << endl;
        // }
        // else if(result<=PROBABILITY_THRESHOLD&&mail.getLabel()==SPAM){
        //     cout << "origin spam result: " << result << endl;
        // }

        if(result>PROBABILITY_THRESHOLD) return true;
        else return false;
    }

    double calculateProbability(){
        vector< pair<string, int> > uniqueSPAMword = dataset->selectUniqueSPAMword();
        vector< pair<string, WordProbability> > tempVocab2Probaility;
        int count = 0;
        for(int i=0; i<uniqueSPAMword.size(); i++){
            if(m_ptest_Vocab2Frequency->find(uniqueSPAMword.at(i).first)!=m_ptest_Vocab2Frequency->end()){
                count ++;
                WordProbability tempProb;
                // cout << dataset.getNumofTests() <<endl;
                if(dataset->getpHAMVocab2Frequency()->find(uniqueSPAMword.at(i).first)==dataset->getpHAMVocab2Frequency()->end()){
                    tempProb.setHAMProb(0.0);
                } 
                else{
                    tempProb.setHAMProb((double)dataset->getpHAMVocab2Frequency()->at(uniqueSPAMword.at(i).first)/(double)dataset->getNumofTrains());
                }
                if(dataset->getpSPAMVocab2Frequency()->find(uniqueSPAMword.at(i).first)==dataset->getpSPAMVocab2Frequency()->end()){
                    tempProb.setSPAMProb(0.0);
                    cout << "Never Happen" << endl;
                }
                else{
                    tempProb.setSPAMProb((double)dataset->getpSPAMVocab2Frequency()->at(uniqueSPAMword.at(i).first)/(double)dataset->getNumofTrains());
                }
                tempVocab2Probaility.push_back(make_pair(uniqueSPAMword.at(i).first, tempProb));
            }
        }
        // cout << "spam words count: " << count << endl;

        double p = 1, q = 1;
        for(int j=0; j<tempVocab2Probaility.size(); j++){
            p = p * tempVocab2Probaility.at(j).second.getSPAMProb();
            q = q * tempVocab2Probaility.at(j).second.getHAMProb();
        }
        if(p+q==0) return 0;
        double result = (p)/(p+q);

        //printf("(p,q) = (%f, %f)\n", p, q);
        return result;
    }

    double getAccuracy(){
        return accuracy;
    }
};
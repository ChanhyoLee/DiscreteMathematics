#include <iostream>
#include <cmath>

#include "DecisionMaking.hpp"
int DIFFERENCE_THRESHOLD = 50;
double PROBABILITY_THRESHOLD = 0.8;

int PREVIOUS_DIFFERENCE_THRESHOLD;
double PREVIOUS_PROBABILITY_THRESHOLD;
double PREVIOUS_ACCURACY;

double BEST_ACCURACY = 0.0;
pair<int, double> BEST_THRESHOLD;

vector< pair<int, double> > MOVINGVECTORLIST;

#define EPOCH 5
#define GAMMA 0.2
#define LEARNINGRATE 0.01

void iterateOnce(int time);

int main(){

    for(int i=0; i<EPOCH; i++){
        iterateOnce(i);
    }
    cout << "Best Accuracy : " << BEST_ACCURACY << endl;
    return 0;
}

void iterateOnce(int time){
    MailDataset mailDataset = MailDataset();
    vector<Mail> mails = mailDataset.getTrainMails();
    DecisionMaking decision = DecisionMaking(mailDataset);

    if(time==0){
        PREVIOUS_DIFFERENCE_THRESHOLD = DIFFERENCE_THRESHOLD;
        PREVIOUS_PROBABILITY_THRESHOLD = PROBABILITY_THRESHOLD;
        PREVIOUS_ACCURACY = decision.getAccuracy();
        pair <int, double> new_movingVector = make_pair(-10, -0.05);
        MOVINGVECTORLIST.push_back(new_movingVector);
    }
    else if(time!=0){
        double deltaZ = decision.getAccuracy() - PREVIOUS_ACCURACY;
        deltaZ = deltaZ * 100;
        int deltaX = DIFFERENCE_THRESHOLD - PREVIOUS_DIFFERENCE_THRESHOLD;
        double deltaY = PROBABILITY_THRESHOLD - PREVIOUS_PROBABILITY_THRESHOLD;
        deltaY = deltaY * 100;

        pair <int, double> new_movingVector = make_pair((int)round(GAMMA * MOVINGVECTORLIST.at(time-1).first), GAMMA*MOVINGVECTORLIST.at(time-1).second);
        new_movingVector.first += (int)round(LEARNINGRATE * (deltaZ/deltaX));
        new_movingVector.second += LEARNINGRATE * (deltaZ/deltaY);
        MOVINGVECTORLIST.push_back(new_movingVector);
    }

    PREVIOUS_DIFFERENCE_THRESHOLD = DIFFERENCE_THRESHOLD;
    PREVIOUS_PROBABILITY_THRESHOLD = PROBABILITY_THRESHOLD;
    PREVIOUS_ACCURACY = decision.getAccuracy();

    DIFFERENCE_THRESHOLD = PREVIOUS_DIFFERENCE_THRESHOLD - MOVINGVECTORLIST.at(time).first;
    PROBABILITY_THRESHOLD = PREVIOUS_PROBABILITY_THRESHOLD - MOVINGVECTORLIST.at(time).second;
    cout << "DIFFERENCE " << " PROBABILITY" << endl; 
    cout << PREVIOUS_DIFFERENCE_THRESHOLD << "\t" << PREVIOUS_PROBABILITY_THRESHOLD << endl;
    cout << DIFFERENCE_THRESHOLD << "\t" << PROBABILITY_THRESHOLD << endl;
    
    if(decision.getAccuracy()>BEST_ACCURACY){
        BEST_ACCURACY = decision.getAccuracy();
        BEST_THRESHOLD = make_pair(DIFFERENCE_THRESHOLD, PROBABILITY_THRESHOLD);
    }

}
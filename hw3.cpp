#include <iostream>
#include <cmath>

#include "DecisionMaking.hpp"
int DIFFERENCE_THRESHOLD = 100;
double PROBABILITY_THRESHOLD = 0.80;

int main(){

    for(int epoch=0; epoch < 10; epoch ++){
        MailDatasetTest* mailDataset = new MailDatasetTest();
        DecisionMaking* decision = new DecisionMaking(mailDataset);
        
        delete mailDataset;
        delete decision;
    }


    return 0;
}
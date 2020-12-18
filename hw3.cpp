#include <cmath>
#include <iostream>

#include "DecisionMaking.hpp"
int DIFFERENCE_THRESHOLD = 100;
double PROBABILITY_THRESHOLD = 0.80;

#define EPOCH 5

int main() {
    double meanAccuracy = 0;
    for (int epoch = 0; epoch < EPOCH; epoch++) {
        cout << "-------------------------------" << endl;
        cout << "-----------" << epoch << "th EPOCH-----------" << endl;
        MailDataset* mailDataset = new MailDataset();
        DecisionMaking* decision = new DecisionMaking(mailDataset);

        meanAccuracy += decision->getAccuracy();

        delete mailDataset;
        delete decision;
        cout << "-------------------------------" << endl;
    }

    meanAccuracy = meanAccuracy / EPOCH;
    cout << "-------------------------------" << endl;
    cout << "MeanAccuray:        " << meanAccuracy << endl;
    cout << "-------------------------------" << endl;
    return 0;
}

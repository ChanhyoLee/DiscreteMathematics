#include <iostream>
#include <cmath>

#include "DecisionMaking.hpp"
int DIFFERENCE_THRESHOLD = 100;
double PROBABILITY_THRESHOLD = 0.85;

int main(){
    MailDataset* mailDataset = new MailDataset();
    DecisionMaking* decision = new DecisionMaking(mailDataset);

    return 0;
}
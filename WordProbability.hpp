class WordProbability {
   private:
    double spamProb;
    double hamProb;

   public:
    void setSPAMProb(double d) {
        spamProb = d;
    }
    void setHAMProb(double d) {
        hamProb = d;
    }
    double getSPAMProb() {
        return spamProb;
    }
    double getHAMProb() {
        return hamProb;
    }
};
#pragma once
#include "initialweights.h"

class Automatize : public InitialWeights {
public:
    Automatize(System* system);
    void setupInitialWeights();

    std::string generateFileName(std::string name, std::string extension);
    Eigen::MatrixXd getParameters();

private:
    double      m_factor = 0;
    std::string m_trialWaveFunction;

    int         m_initialTotalStepsWOEqui = 0;
    bool        m_interaction = 0;
    double      m_omega = 0;
    std::string m_path = "No path";

    class InitialWeights* m_method = nullptr;
};

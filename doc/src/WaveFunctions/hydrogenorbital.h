#pragma once
#include "wavefunction.h"

class HydrogenOrbital : public WaveFunction {
public:
    HydrogenOrbital(class System* system);
    void updateArrays(const Eigen::VectorXd positions, const int pRand);
    void resetArrays();
    void initializeArrays(const Eigen::VectorXd positions);
    void updateParameters(const Eigen::MatrixXd parameters, const int elementNumber);
    double evaluate();
    double evaluateSqrd();
    double computeFirstDerivative(const int k);
    double computeSecondDerivative();
    Eigen::VectorXd computeFirstEnergyDerivative(const int k);
    Eigen::VectorXd computeSecondEnergyDerivative();

    double calculateRadialVectorElement(const Eigen::VectorXd particles, const int par);
    Eigen::VectorXd calculateRadialVector(const Eigen::VectorXd particles);

private:
    double m_alpha = 0;
    int m_elementNumber = 0;

    Eigen::VectorXd m_positions;
    Eigen::VectorXd m_oldPositions;
    Eigen::VectorXd m_radialVector;
    Eigen::VectorXd m_oldRadialVector;
};

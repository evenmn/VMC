#pragma once
#include "wavefunction.h"

class NQSGaussian : public WaveFunction {
public:
    NQSGaussian(class System* system);
    void updateArrays(const Eigen::VectorXd positions, const int pRand);
    void resetArrays(int pRand);
    void initializeArrays(const Eigen::VectorXd positions);
    void updateParameters(const Eigen::MatrixXd parameters, const int elementNumber);
    double evaluateRatio();
    double computeFirstDerivative(const int k);
    double computeSecondDerivative();
    Eigen::VectorXd computeFirstEnergyDerivative(const int k);
    Eigen::VectorXd computeSecondEnergyDerivative();
private:
    int     m_elementNumber = 0;
    double  m_omega         = 1;
    double  m_sigmaSqrd     = 1;

    double  m_probabilityRatio = 0;
    double  m_probabilityRatioOld = 0;

    Eigen::VectorXd m_positions;
    Eigen::VectorXd m_positionsOld;
    Eigen::VectorXd m_Xa;
    Eigen::VectorXd m_XaOld;
    Eigen::VectorXd m_a;
};
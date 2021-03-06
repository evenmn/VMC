#pragma once
#include "wavefunction.h"

class HydrogenLike : public WaveFunction
{
public:
    HydrogenLike(class System *system);
    int getNumberOfParameters() { return m_numberOfParameters; }
    int getGlobalArrayNeed() { return m_globalArrayNeed; }
    std::string getLabel() { return m_label; }

    void updateParameters(const Eigen::MatrixXd parameters);
    void initializeArrays(const Eigen::VectorXd positions,
                          const Eigen::VectorXd radialVector,
                          const Eigen::MatrixXd distanceMatrix);
    void updateArrays(const Eigen::VectorXd positions,
                      const Eigen::VectorXd radialVector,
                      const Eigen::MatrixXd distanceMatrix,
                      const int changedCoord);
    void setConstants(const int elementNumber);
    void setArrays();
    void resetArrays();
    double evaluateRatio();
    double computeGradient(const int k);
    double computeLaplacian();
    Eigen::VectorXd computeParameterGradient();

private:
    int m_numberOfParameters = 1;
    int m_globalArrayNeed = 2;
    int m_elementNumber = 0;
    int m_Z = 1;

    double m_alpha = 0;

    Eigen::VectorXd m_positions;
    Eigen::VectorXd m_positionsOld;
    Eigen::VectorXd m_radialVector;
    Eigen::VectorXd m_radialVectorOld;
    Eigen::VectorXd m_gradients;

    double m_probabilityRatio;
    double m_probabilityRatioOld;

    std::string m_label = "hydrogenlike";
};

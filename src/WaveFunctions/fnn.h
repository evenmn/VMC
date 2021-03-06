#pragma once
#include "../Activation/activation.h"
#include "../Layer/layer.h"
#include "wavefunction.h"
#include <vector>

class FNN : public WaveFunction
{
public:
    FNN(class System *system);
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

    void updateProbabilityRatio(int changedCoord);
    double evaluate(Eigen::VectorXd position);

private:
    int m_numberOfParameters = 0;
    int m_globalArrayNeed = 0;
    int m_elementNumber = 0;
    double m_probabilityRatio = 0;
    double m_probabilityRatioOld = 0;

    double m_out = 0;
    double m_outOld = 0;

    Eigen::VectorXd m_positions;
    Eigen::VectorXd m_positionsOld;

    Eigen::VectorXd m_gradients;

    std::vector<class Layer *> m_layers2;

    std::string m_label = "fnn";
};

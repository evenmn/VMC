#pragma once
#include "wavefunction.h"

class Gaussian : public WaveFunction {
public:
    Gaussian(class System* system);
    int             getNumberOfParameters       ()      { return m_numberOfParameters; }
    int             getGlobalArrayNeed          ()      { return m_globalArrayNeed; }
    std::string     getLabel                    ()      { return m_label; }

    void            updateParameters            (const Eigen::MatrixXd parameters);
    void            initializeArrays            (const Eigen::VectorXd positions, \
                                                 const Eigen::VectorXd radialVector, \
                                                 const Eigen::MatrixXd distanceMatrix);
    void            updateArrays                (const Eigen::VectorXd positions, \
                                                 const Eigen::VectorXd radialVector, \
                                                 const Eigen::MatrixXd distanceMatrix, \
                                                 const int changedCoord);
    void            setConstants                (const unsigned int elementNumber);
    void            setArrays                   ();
    void            resetArrays                 ();
    double          evaluateRatio               ();
    double          computeGradient             (const int k);
    double          computeLaplacian            ();
    Eigen::VectorXd computeParameterGradient    ();

    void            updateProbabilityRatio      (int changedCoord);

private:
    int     m_numberOfParameters       = 1;
    int     m_globalArrayNeed          = 0;
    unsigned int    m_elementNumber         = 0;

    double  m_omega                    = 0;
    double  m_alpha                    = 0;
    double  m_probabilityRatio         = 0;
    double  m_probabilityRatioOld      = 0;
    Eigen::VectorXd m_positions;
    Eigen::VectorXd m_positionsOld;

    std::string m_label = "gaussian";
};

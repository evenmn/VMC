#pragma once
#include "wavefunction.h"

class RBMGaussian : public WaveFunction {
public:
    RBMGaussian(class System* system);
    int             getNumberOfParameters       ()      { return m_numberOfParameters; }
    int             getGlobalArrayNeed          ()      { return m_globalArrayNeed; }
    std::string     getLabel                    ()      { return m_label; }

    void            updateArrays                    (const Eigen::VectorXd positions, \
                                                     const Eigen::VectorXd radialVector, \
                                                     const Eigen::MatrixXd distanceMatrix, \
                                                     const int changedCoord);
    void            setArrays                   ();
    void            resetArrays                 ();
    void            initializeArrays            (const Eigen::VectorXd positions, \
                                                 const Eigen::VectorXd radialVector, \
                                                 const Eigen::MatrixXd distanceMatrix);
    void            updateParameters            (const Eigen::MatrixXd parameters, const int elementNumber);
    double          evaluateRatio               ();
    double          computeGradient             (const int k);
    double          computeLaplacian            ();
    Eigen::VectorXd computeParameterGradient    ();

private:
    int     m_numberOfParameters       = 1;
    int     m_globalArrayNeed          = 0;
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

    std::string m_label = "rbmgaussian";
};
#pragma once
#include "wavefunction.h"
//#include <unsupported/Eigen/CXX11/Tensor>

class NQSJastrow3 : public WaveFunction {
public:
    NQSJastrow3(class System* system);
    void            updateArrays                (Eigen::VectorXd positions, const int changedCoord);
    void            setArrays                   ();
    void            resetArrays                 ();
    void            initializeArrays            (const Eigen::VectorXd positions);
    void            updateParameters            (const Eigen::MatrixXd parameters, const int elementNumber);
    double          evaluateRatio               ();
    double          computeGradient             (const int k);
    double          computeLaplacian            ();
    Eigen::VectorXd computeParameterGradient    ();

    void            updateVectors               ();
    void            updateRatio                 ();
    void            calculateG                  (int changedCoord);

    void            calculateDistanceMatrix();
    double          calculateDistanceMatrixElement(const int i, const int j);
    void            calculateDistanceMatrixCross(const int par);

private:
    int    m_elementNumber          = 1;
    int    m_numberOfHiddenNodes    = 1;
    double m_sigmaSqrd              = 1;
    double m_sigmaQuad              = 1;

    double m_probabilityRatio       = 1;
    double m_probabilityRatioOld    = 1;

    Eigen::MatrixXd m_distanceMatrix;
    Eigen::MatrixXd m_distanceMatrixOld;

    Eigen::VectorXd m_positions;
    Eigen::VectorXd m_positionsOld;
    Eigen::VectorXd m_b;
    Eigen::MatrixXd m_W;
    Eigen::MatrixXd m_WSqrd;
    Eigen::VectorXd m_vOld;
    Eigen::VectorXd m_v;
    Eigen::VectorXd m_nOld;
    Eigen::VectorXd m_n;
    Eigen::VectorXd m_pOld;
    Eigen::VectorXd m_p;
    Eigen::MatrixXd m_g;
    Eigen::MatrixXd m_gOld;
};

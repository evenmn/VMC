#include "slaterdeterminant.h"
#include <cassert>
#include <iostream>
#include "../system.h"

SlaterDeterminant::SlaterDeterminant(System* system) :
        WaveFunction(system) {
    m_numberOfFreeDimensions            = m_system->getNumberOfFreeDimensions();
    m_numberOfParticles                 = m_system->getNumberOfParticles();
    m_numberOfOrbitals                  = m_system->getNumberOfOrbitals();
    m_numberOfDimensions                = m_system->getNumberOfDimensions();
    m_numberOfParticlesHalf             = m_system->getNumberOfParticles()/2;
    m_maxNumberOfParametersPerElement   = m_system->getMaxNumberOfParametersPerElement();
    m_omega                             = m_system->getFrequency();
}

double H(double x, int n) {
    //Hermite polynomial of n'th degree

    if(n == 0) {
        return 1;
    }
    else if(n == 1) {
        return 2*x;
    }
    else {
        return 2*x*H(x,n-1)-2*(n-1)*H(x,n-2);
    }
}

double dH(double x, int n) {
    //First derivative of Hermite polynomial of n'th degree

    if(n == 0) {
        return 0;
    }
    else {
        return 2*n*H(x,n-1);
    }
}

double d2H(double x, int n) {
    //Second derivative of Hermite polynomial of n'th degree

    if(n == 0 || n == 1) {
        return 0;
    }
    else {
        return 4*n*n*H(x, n-2);
    }
}

void SlaterDeterminant::updateArrays(const Eigen::VectorXd positions, const int pRand) {
    int particle = int(pRand/m_numberOfDimensions);
    m_oldPositions  = m_positions;
    m_positions = positions;
    m_diffOld   = m_diff;
    m_XaOld     = m_Xa;
    m_Xa        = positions + Eigen::VectorXd::Ones(m_numberOfFreeDimensions) - m_a;
    if(particle < m_numberOfParticlesHalf) {
        m_D_upOld               = m_D_up;
        m_dD_upOld              = m_dD_up;
        m_d2D_upOld             = m_d2D_up;
        m_D_up_old_inv          = m_D_up_inv;
        m_D_up.row(particle)    = updateRow(m_Xa.head(m_numberOfFreeDimensions/2), H, particle);
        m_dD_up.row(particle)   = dA_row(m_Xa.head(m_numberOfFreeDimensions/2), particle);
        //m_d2D_up.row(particle)  = d2A_row(m_Xa.head(m_numberOfFreeDimensions/2), particle);
        //double R = 0;
        //for(int j=0; j<m_numberOfParticlesHalf; j++) {
        //    R += m_D_up(particle, j) * m_D_up_old_inv(j, particle);
        //}
        //m_D_up_inv.row(particle) /= R;
        m_D_up_inv = m_D_up.inverse();
        for(int j=0; j<m_numberOfParticlesHalf; j++) {
            m_diff(pRand) += m_dD_up(pRand,j) * m_D_up_inv(j,int(pRand/2));
        }
    }
    else {
        int particle2 = particle - m_numberOfParticlesHalf;
        m_D_dnOld               = m_D_dn;
        m_dD_dnOld              = m_dD_dn;
        m_d2D_dnOld             = m_d2D_dn;
        m_D_dn_old_inv          = m_D_dn_inv;
        m_D_dn.row(particle2)   = updateRow(m_Xa.tail(m_numberOfFreeDimensions/2), H, particle2);
        m_dD_dn.row(particle2)  = dA_row(m_Xa.tail(m_numberOfFreeDimensions/2), particle2);
        //m_d2D_dn.row(particle2) = d2A_row(m_Xa.tail(m_numberOfFreeDimensions/2), particle2);
        //double R = 0;
        //for(int j=0; j<m_numberOfParticlesHalf; j++) {
        //    R += m_D_dn(particle2, j) * m_D_dn_old_inv(j, particle2);
        //}
        //m_D_dn_inv.row(particle2) /= R;
        m_D_dn_inv = m_D_dn.inverse();
        for(int j=0; j<m_numberOfParticlesHalf; j++) {
            m_diff(pRand) += m_dD_dn(pRand-m_numberOfFreeDimensions/2,j) * m_D_dn_inv(j,int((pRand-m_numberOfFreeDimensions/2)/2));
        }
    }
}

void SlaterDeterminant::resetArrays() {
    m_positions = m_oldPositions;
    m_D_up      = m_D_upOld;
    m_D_dn      = m_D_dnOld;
    m_D_up_inv  = m_D_up_old_inv;
    m_D_dn_inv  = m_D_dn_old_inv;
    m_dD_up     = m_dD_upOld;
    m_dD_dn     = m_dD_dnOld;
    m_diff      = m_diffOld;
    //m_d2D_up    = m_d2D_upOld;
    //m_d2D_dn    = m_d2D_dnOld;
    m_Xa        = m_XaOld;
}

void SlaterDeterminant::initializeArrays(const Eigen::VectorXd positions) {
    m_positions = positions;
    m_Xa        = positions + Eigen::VectorXd::Ones(m_numberOfFreeDimensions) - m_a;

    m_D_up = updateMatrix(m_Xa.head(m_numberOfFreeDimensions/2), H);
    m_D_dn = updateMatrix(m_Xa.tail(m_numberOfFreeDimensions/2), H);

    m_D_up_inv = m_D_up.inverse();
    m_D_dn_inv = m_D_dn.inverse();

    m_dD_up = dA_matrix(m_Xa.head(m_numberOfFreeDimensions/2));
    m_dD_dn = dA_matrix(m_Xa.tail(m_numberOfFreeDimensions/2));

    //m_d2D_up = d2A_matrix(m_positions.head(m_numberOfFreeDimensions/2));
    //m_d2D_dn = d2A_matrix(m_positions.tail(m_numberOfFreeDimensions/2));

    m_diff      = Eigen::VectorXd::Zero(m_numberOfFreeDimensions);
    for(int i=0; i<m_numberOfFreeDimensions/2; i++) {
        for(int j=0; j<m_numberOfParticlesHalf; j++) {
            m_diff(i) += m_dD_up(i,j) * m_D_up_inv(j,int(i/2));
            m_diff(i+m_numberOfFreeDimensions/2) += m_dD_dn(i,j) * m_D_dn_inv(j,int(i/2));
        }
    }
}

void SlaterDeterminant::updateParameters(const Eigen::MatrixXd parameters, const int elementNumber) {
    m_elementNumber = elementNumber;
    m_a = (parameters.row(0)).head(m_numberOfFreeDimensions);
}

Eigen::MatrixXd SlaterDeterminant::list() {
    // Returns the index list used in Slater
    // For instance (0,0), (1,0), (0,1) for 6P in 2D
    //              (0,0,0), (1,0,0), (0,1,0), (0,0,1) for 6P in 3D etc..

    Eigen::MatrixXd order = Eigen::MatrixXd::Zero(m_numberOfParticlesHalf, m_numberOfDimensions);

    int counter = 0;
    // Two dimensions
    if (m_numberOfDimensions == 2) {
        for(int i=0; i<m_numberOfOrbitals; i++) {
            for(int s=i; s<m_numberOfOrbitals; s++) {
                int j = s - i;
                order(counter,1) = i;
                order(counter,0) = j;
                counter += 1;
            }
        }
    }

    // Three dimensions
    else if (m_numberOfDimensions == 3) {
        for(int i=0; i<m_numberOfOrbitals; i++) {
            for(int j=0; j<m_numberOfOrbitals; j++) {
                for(int s=i+j; s<m_numberOfOrbitals; s++) {
                    int k = s - i - j;
                    order(counter,0) = i;
                    order(counter,1) = j;
                    order(counter,2) = k;
                    counter += 1;
                }
            }
        }
    }
    return order;
}

Eigen::VectorXd SlaterDeterminant::dA_row(const Eigen::VectorXd positions, const int k) {
    //Update row of dA

    Eigen::VectorXd dA = Eigen::VectorXd::Zero(m_numberOfParticlesHalf);
    Eigen::MatrixXd order = list();

    // Find indices of relevant row
    Eigen::VectorXd a = Eigen::VectorXd::Zero(m_numberOfDimensions);
    int l = k%m_numberOfDimensions;
    for(int i=0; i<m_numberOfDimensions; i++) {
        a(i) = k-l+i;
    }

    // Find matrix
    for(int i=0; i<m_numberOfParticlesHalf; i++) {
        dA(i) = dH(positions(k), int(order(i, l)));
        for(int j=0; j<m_numberOfDimensions; j++) {
            if(int(a(j)) != k) {
                dA(i) *= H(positions(int(a(j))), int(order(i, j)));
            }
        }
    }
    return dA.transpose();
}


Eigen::MatrixXd SlaterDeterminant::dA_matrix(const Eigen::VectorXd positions) {
    //Initialize the entire dA matrix
    Eigen::MatrixXd dA = Eigen::MatrixXd::Zero(m_numberOfParticles, m_numberOfParticlesHalf);

    for(int k=0; k<m_numberOfFreeDimensions/2; k++) {
        dA.row(k) = dA_row(positions, k);
    }
    return dA;
}

Eigen::VectorXd SlaterDeterminant::d2A_row(const Eigen::VectorXd positions, const int k) {
    //Update row of dA
    Eigen::VectorXd d2A = Eigen::VectorXd::Zero(m_numberOfParticlesHalf);
    Eigen::MatrixXd order = list();

    // Find indices of relevant row
    Eigen::VectorXd a = Eigen::VectorXd::Zero(m_numberOfDimensions);
    int l = k%m_numberOfDimensions;
    for(int i=0; i<m_numberOfDimensions; i++) {
        a(i) = k-l+i;
    }

    // Find matrix
    for(int i=0; i<m_numberOfParticlesHalf; i++) {
        d2A(i) = d2H(positions(k), int(order(i, l)));
        for(int j=0; j<m_numberOfDimensions; j++) {
            if(int(a(j)) != k) {
                d2A(i) *= H(positions(int(a(j))), int(order(i, j)));
            }
        }
    }
    return d2A.transpose();
}


Eigen::MatrixXd SlaterDeterminant::d2A_matrix(const Eigen::VectorXd positions) {
    //Initialize the entire dA matrix
    Eigen::MatrixXd d2A = Eigen::MatrixXd::Zero(m_numberOfParticles, m_numberOfParticlesHalf);

    for(int k=0; k<m_numberOfFreeDimensions/2; k++) {
        d2A.row(k) = d2A_row(positions, k);
    }
    return d2A;
}

double SlaterDeterminant::updateElement(const Eigen::VectorXd positions, double basis(double, int), const int i, const int j) {
    // Updates an element in A-matrix

    Eigen::MatrixXd order = list();

    double element = 1;
    for(int k=0; k<m_numberOfDimensions; k++) {
        element *= basis(sqrt(m_omega) * positions(m_numberOfDimensions*i+k), int(order(j,k)));
    }

    return element;
}

Eigen::VectorXd SlaterDeterminant::updateRow(const Eigen::VectorXd positions, double basis(double, int), const int i) {
    // Updates a row in A-matrix

    Eigen::VectorXd A = Eigen::VectorXd::Ones(m_numberOfParticlesHalf);

    for(int j=0; j<m_numberOfParticlesHalf; j++) {
        A(j) = updateElement(positions, basis, i, j);
    }
    return A;
}

Eigen::MatrixXd SlaterDeterminant::updateMatrix(const Eigen::VectorXd positions, double basis(double, int)) {
    // Update the entire matrix

    Eigen::MatrixXd A = Eigen::MatrixXd::Ones(m_numberOfParticlesHalf, m_numberOfParticlesHalf);

    for(int j=0; j<m_numberOfParticlesHalf; j++) {
        A.row(j) = updateRow(positions, basis, j);
    }

    return A;
}

double SlaterDeterminant::evaluate() {

    //std::cout << m_positions << std::endl;
    //std::cout << m_D_up << std::endl;
    //std::cout << m_D_up.determinant() << std::endl;
    //std::cout << std::endl;

    return m_D_up.determinant() * m_D_dn.determinant();
}

double SlaterDeterminant::evaluateSqrd() {
    double WF = evaluate();
    return WF * WF;
}

double SlaterDeterminant::computeFirstDerivative(const int k) {
    /*
    double Sum = 0;

    Eigen::MatrixXd dD = Eigen::MatrixXd::Zero(m_numberOfParticlesHalf, m_numberOfParticlesHalf);
    if(k>m_numberOfFreeDimensions/2) {
        int l = k - m_numberOfFreeDimensions/2;
        dD.row(int((l/m_numberOfDimensions))) = m_dD_dn.row(l);
        Sum = (m_D_dn_inv * dD).trace();
    }
    else {
        dD.row(int((k/m_numberOfDimensions))) = m_dD_up.row(k);
        Sum = (m_D_up_inv * dD).trace();
    }
    */
    /*
    for(int j=0; j<m_numberOfParticlesHalf; j++) {
        if(k<m_numberOfFreeDimensions/2) {
            Sum += m_dD_up(k, j) * m_D_up_inv(j, int(k/2));
        }

        else {
            int l  = k - m_numberOfFreeDimensions/2;
            Sum += m_dD_dn(l, j) * m_D_dn_inv(j, int(l/2));
        }
    }
    */
    //std::cout << m_diff(k) << std::endl;
    //std::cout << std::endl;
    return m_diff(k);
}

double SlaterDeterminant::computeSecondDerivative() {
    /*
    Eigen::VectorXd diff = Eigen::VectorXd::Zero(m_numberOfFreeDimensions);
    for(int k=0; k<m_numberOfFreeDimensions/2; k++) {
        for(int j=0; j<m_numberOfParticlesHalf; j++) {
            int l  = k + m_numberOfFreeDimensions/2;
            diff(k) += m_dD_up(k,j) * m_D_up.inverse()(j, int(k/2));
            diff(l) += m_dD_dn(k,j) * m_D_dn.inverse()(j, int(k/2));
        }
    }
    return -double(diff.transpose() * diff);
    */
    /*
    double Sum = 0;
    for(int k=0; k<m_numberOfFreeDimensions; k++) {
        for(int j=0; j<m_numberOfParticlesHalf; j++) {
            if(k<m_numberOfFreeDimensions/2) {
                Sum += m_d2D_up(k, j) * m_D_up_inv(j, int(k/m_numberOfDimensions));
            }

            else {
                int l  = k - m_numberOfFreeDimensions/2;
                Sum += m_d2D_dn(l, j) * m_D_dn_inv(j, int(l/m_numberOfDimensions));
            }
        }
    }
    return Sum;
    */
    return -double(m_diff.transpose() * m_diff);
}

Eigen::VectorXd SlaterDeterminant::computeFirstEnergyDerivative(const int k) {
    return Eigen::VectorXd::Zero(m_maxNumberOfParametersPerElement);
}

Eigen::VectorXd SlaterDeterminant::computeSecondEnergyDerivative() {
    return Eigen::VectorXd::Zero(m_maxNumberOfParametersPerElement);
}

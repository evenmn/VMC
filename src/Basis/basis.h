#pragma once
#include <Eigen/Dense>
#include <fstream>
#include <iostream>
#include <mpi.h>

class Basis
{
public:
    Basis(class System *system);
    void numberOfOrbitals();
    void generateListOfStates();

    virtual std::string getLabel() = 0;

    virtual void initialize() = 0;
    virtual void setParameters(Eigen::VectorXd parameters) = 0;

    virtual double evaluate(double x, int n) = 0;
    virtual double evaluateDerivative(double x, int n) = 0;
    virtual double evaluateSecondDerivative(double x, int n) = 0;

    virtual double basisElement(const int n, Eigen::VectorXd positions) = 0;
    virtual double basisElementDer(const int n, const int i, Eigen::VectorXd positions) = 0;
    virtual double basisElementSecDer(const int n, const int i, Eigen::VectorXd positions) = 0;
    virtual double basisElementPar(const int n, Eigen::VectorXd position) = 0;

    virtual ~Basis() = 0;

    long long factorial(const int n);
    int factorialDifference(const int high, const int low);
    int binomial(const int n, const int p);
    void writeFileContentIntoEigenMatrix(std::string fileName, Eigen::MatrixXd &matrix);
    std::ifstream::pos_type fileLength(std::string fileName);

protected:
    int m_numberOfParticles = 0;
    int m_numberOfDimensions = 0;
    int m_numberOfOrbitals = 0;

    Eigen::MatrixXi m_listOfStates;

    class System *m_system = nullptr;
};

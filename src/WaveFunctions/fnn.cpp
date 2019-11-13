#include "fnn.h"
#include "../system.h"
#include <cassert>
#include <iostream>

FNN::FNN(System *system)
    : WaveFunction(system)
{}

void FNN::setConstants(const int elementNumber)
{
    m_elementNumber = elementNumber;
    m_degreesOfFreedom = m_system->getNumberOfFreeDimensions();
    m_omega = m_system->getFrequency();
    m_activationFunctions = m_system->getActivationFunctions();
    m_units = m_system->getHiddenUnits();
    m_units.insert(m_units.begin(), m_degreesOfFreedom);    // Add input layer
    m_units.push_back(1);                                   // Add output layer
    m_activationFunctions.push_back(new Sigmoid(m_system)); // Add activation on output
    m_numberOfParameters = m_numberOfHiddenUnits * (1 + m_degreesOfFreedom);
}

void FNN::initializeArrays(const Eigen::VectorXd positions,
                           const Eigen::VectorXd /*radialVector*/,
                           const Eigen::MatrixXd /*distanceMatrix*/)
{
    m_positions = positions;
    m_probabilityRatio = 1;
}

void FNN::updateProbabilityRatio(int /*changedCoord*/)
{
    /*
    for (auto &i : m_hiddenUnits) {
        int numberOfParameters = i->getNumberOfParameters();
        if (numberOfParameters > maxNumberOfElements) {
            maxNumberOfElements = numberOfParameters;
        }
        counter += numberOfParameters;
    }
    */

    /*
    Eigen::VectorXd z1 = m_positionsOld.transpose() * m_W1;
    Eigen::VectorXd a1 = m_activationFunctions.at(0)->evaluate(z1);
    double z2 = a1.transpose() * m_W2;
    double a2 = m_activationFunctions.at(1)->evaluate(Eigen::VectorXd(z2))(0);

    Eigen::VectorXd z1New = m_positions.transpose() * m_W1;
    Eigen::VectorXd a1New = m_activationFunctions.at(0)->evaluate(z1New);
    double z2New = a1New.transpose() * m_W2;
    double a2New = m_activationFunctions.at(1)->evaluate(Eigen::VectorXd(z2New))(0);
    */

    m_probabilityRatio = 1;
    //a2New / a2;
}

void FNN::updateArrays(const Eigen::VectorXd positions,
                       const Eigen::VectorXd /*radialVector*/,
                       const Eigen::MatrixXd /*distanceMatrix*/,
                       const int changedCoord)
{
    m_positions = positions;
    updateProbabilityRatio(changedCoord);
}

void FNN::setArrays()
{
    m_positionsOld = m_positions;
    m_probabilityRatioOld = m_probabilityRatio;
}

void FNN::resetArrays()
{
    m_positions = m_positionsOld;
    m_probabilityRatio = m_probabilityRatioOld;
}

void FNN::updateParameters(const Eigen::MatrixXd parameters)
{
    Eigen::VectorXd wFlatten1 = parameters.row(m_elementNumber)
                                    .segment(m_units.at(1),
                                             m_degreesOfFreedom * m_numberOfHiddenUnits);
    m_W1 = WaveFunction::reshape(wFlatten1, m_degreesOfFreedom, m_units.at(1));
    Eigen::VectorXd wFlatten2 = parameters.row(m_elementNumber)
                                    .segment(m_numberOfHiddenUnits,
                                             m_degreesOfFreedom * m_numberOfHiddenUnits);
    m_W2 = parameters.row(m_elementNumber).head(m_numberOfHiddenUnits);
}

double FNN::evaluateRatio()
{
    return m_probabilityRatio;
}

double FNN::computeGradient(const int k)
{
    return -m_omegalpha * m_positions(k);
}

double FNN::computeLaplacian()
{
    ;
    return -m_omegalpha * m_degreesOfFreedom;
}

Eigen::VectorXd FNN::computeParameterGradient()
{
    m_gradients = Eigen::VectorXd::Zero(m_system->getMaxParameters());
    m_gradients(0) = -0.5 * m_omega * m_positions.cwiseAbs2().sum();
    return m_gradients;
}

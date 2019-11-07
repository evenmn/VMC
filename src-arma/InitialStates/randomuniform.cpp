#include "randomuniform.h"
#include "../RNG/rng.h"
#include "../WaveFunctions/wavefunction.h"
#include "../system.h"
#include <cassert>
#include <iostream>

RandomUniform::RandomUniform(System *system)
    : InitialState(system)
{}

void RandomUniform::setupInitialState()
{
    m_numberOfParticles = m_system->getNumberOfParticles();
    m_numberOfDimensions = m_system->getNumberOfDimensions();
    m_degreesOfFreedom = m_system->getNumberOfFreeDimensions();

    m_positions.zeros(m_degreesOfFreedom);
    for (arma::uword i = 0; i < m_degreesOfFreedom; i++) {
        m_positions(i) = m_system->getRandomNumberGenerator()->nextDouble();
    }
    InitialState::calculateDistanceMatrix();
    InitialState::calculateRadialVector();

    for (auto &i : m_system->getWaveFunctionElements()) {
        i->initializeArrays(m_positions, m_radialVector, m_distanceMatrix);
        i->setArrays();
    }
}

double RandomUniform::calculateDistanceMatrixElement(arma::uword i, arma::uword j)
{
    double dist = 0;
    arma::uword parti = m_numberOfDimensions * i;
    arma::uword partj = m_numberOfDimensions * j;
    for (arma::uword d = 0; d < m_numberOfDimensions; d++) {
        double diff = m_positions(parti + d) - m_positions(partj + d);
        dist += diff * diff;
    }
    return sqrt(dist);
}

void RandomUniform::calculateDistanceMatrix()
{
    m_distanceMatrix.zeros(m_numberOfParticles, m_numberOfParticles);
    for (arma::uword i = 0; i < m_numberOfParticles; i++) {
        for (arma::uword j = i + 1; j < m_numberOfParticles; j++) {
            m_distanceMatrix(i, j) = calculateDistanceMatrixElement(i, j);
            m_distanceMatrix(j, i) = m_distanceMatrix(i, j);
        }
    }
}

double RandomUniform::calculateRadialVectorElement(arma::uword particle)
{
    double sqrtElementWise = 0;
    arma::uword part = particle * m_numberOfDimensions;
    for (arma::uword d = 0; d < m_numberOfDimensions; d++) {
        sqrtElementWise += m_positions(part + d) * m_positions(part + d);
    }
    return sqrt(sqrtElementWise);
}

void RandomUniform::calculateRadialVector()
{
    m_radialVector.zeros(m_numberOfParticles);
    for (arma::uword i = 0; i < m_numberOfParticles; i++) {
        m_radialVector(i) = calculateRadialVectorElement(i);
    }
}

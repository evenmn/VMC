#include "system.h"
#include "sampler.h"
#include "WaveFunctions/wavefunction.h"
#include "Hamiltonians/hamiltonian.h"
#include "Basis/basis.h"
#include "InitialStates/initialstate.h"
#include "InitialWeights/initialweights.h"
#include "Metropolis/metropolis.h"
#include "Optimization/optimization.h"
#include "RNG/rng.h"

#include <iostream>
#include <fstream>
#include <cassert>
#include <ctime>
#include <string>

void System::runIterations(const int numberOfIterations) {
    m_positions                 = m_initialState->getParticles();
    m_parameters                = m_initialWeights->getWeights();
    m_sampler                   = new Sampler(this);
    m_sampler->openOutputFiles("../data/");
    m_lastIteration = numberOfIterations - 11;

    for(int iter = 0; iter < numberOfIterations; iter++) {
        int numberOfSteps       = m_numberOfMetropolisSteps;
        int equilibriationSteps = int(m_numberOfMetropolisSteps * m_equilibrationFraction);
        if(iter == m_lastIteration+10) {
            numberOfSteps *= int(pow(2,8));
        }
        else if(iter >= m_lastIteration) {
            numberOfSteps *= int(pow(2,4));
        }
        clock_t start_time = clock();
        runMetropolisCycles(numberOfSteps, equilibriationSteps, iter);
        clock_t end_time = clock();
        double time = double(end_time - start_time)/CLOCKS_PER_SEC;

        m_sampler->computeAverages();
        m_sampler->printOutputToFile();

        m_parameters -= m_optimization->updateParameters();
        //std::cout << m_parameters << std::endl;
        updateAllParameters(m_parameters);

        printToTerminal(numberOfIterations, iter, time);
        checkConvergence(iter);
    }
}

void System::runMetropolisCycles(int numberOfSteps, int equilibriationSteps,  int iter) {
    for(int i=0; i < numberOfSteps + equilibriationSteps; i++) {
        bool acceptedStep = m_metropolis->acceptMove();
        m_positions       = m_metropolis->updatePositions();
        if(i >= equilibriationSteps) {
            m_sampler->sample(numberOfSteps, equilibriationSteps, acceptedStep, i);
            if(iter == m_lastIteration+10) {
                m_sampler->printInstantValuesToFile(m_positions);
            }
        }
    }
}

void System::printToTerminal(int numberOfIterations, int iter, double time) {
    if(iter == m_lastIteration+10) {
        m_sampler->closeOutputFiles();
        m_sampler->printFinalOutputToTerminal();
        exit(0);
    }
    else {
        m_sampler->printOutputToTerminal(numberOfIterations, time);
    }
}

void System::checkConvergence(int iter) {
    energies.head(numberOfEnergies-1) = energies.tail(numberOfEnergies-1);
    energies(numberOfEnergies-1) = m_sampler->getAverageEnergy();
    if(fabs(energies(0) - energies(numberOfEnergies-1)) < tolerance) {
        std::cout << "The system has converged! Let's run one more cycle to collect data" << std::endl;
        m_lastIteration = iter + 1;
    }
}

void System::updateAllArrays(const Eigen::VectorXd positions, const int pRand) {
    for(auto& i : m_waveFunctionVector) {
        i->updateArrays(positions, pRand);
    }
}

void System::resetAllArrays() {
    for(auto& i : m_waveFunctionVector) {
        i->resetArrays();
    }
}

void System::updateAllParameters(const Eigen::MatrixXd parameters) {
    for(int i=0; i<m_numberOfWaveFunctionElements; i++) {
        m_waveFunctionVector[unsigned(i)]->updateParameters(parameters, i);
    }
}

double System::evaluateWaveFunctionRatio() {
    double ratio = 1;
    for(auto& i : m_waveFunctionVector) {
        ratio *= i->evaluateRatio();
    }
    return ratio;
}

double System::getKineticEnergy() {
    double KineticEnergy = 0;
    for(auto& i : m_waveFunctionVector) {
        KineticEnergy += i->computeLaplacian();
    }
    for(int k = 0; k < m_numberOfFreeDimensions; k++) {
        double NablaLnPsi = 0;
        for(auto& i : m_waveFunctionVector) {
            NablaLnPsi += i->computeGradient(k);
        }
        KineticEnergy += NablaLnPsi * NablaLnPsi;
    }
    return - 0.5 * KineticEnergy;
}

void System::setNumberOfParticles(const int numberOfParticles) {
    assert(numberOfParticles > 0);
    m_numberOfParticles = numberOfParticles;
}

void System::setNumberOfDimensions(const int numberOfDimensions) {
    assert(numberOfDimensions > 0);
    m_numberOfDimensions = numberOfDimensions;
}

void System::setNumberOfFreeDimensions() {
    m_numberOfFreeDimensions = m_numberOfParticles * m_numberOfDimensions;
}

void System::setNumberOfHiddenNodes(const int numberOfHiddenNodes) {
    assert(numberOfHiddenNodes > 0);
    m_numberOfHiddenNodes = numberOfHiddenNodes;
}

void System::setNumberOfMetropolisSteps(const int steps) {
    m_numberOfMetropolisSteps = steps;
}

void System::setNumberOfWaveFunctionElements(const int numberOfWaveFunctionElements) {
    m_numberOfWaveFunctionElements = numberOfWaveFunctionElements;
}

void System::setMaxNumberOfParametersPerElement() {
    m_maxNumberOfParametersPerElement = 2*m_numberOfHiddenNodes*(m_numberOfFreeDimensions+1);
    //m_maxNumberOfParametersPerElement = m_numberOfFreeDimensions * m_numberOfFreeDimensions;
    //m_maxNumberOfParametersPerElement = 10000;
}

void System::setStepLength(const double stepLength) {
    assert(stepLength >= 0);
    m_stepLength = stepLength;
}

void System::setTotalNumberOfSteps() {
    m_totalNumberOfSteps = int(m_numberOfMetropolisSteps*(1 + m_equilibrationFraction));
}

void System::setEquilibrationFraction(const double equilibrationFraction) {
    assert(equilibrationFraction >= 0);
    m_equilibrationFraction = equilibrationFraction;
}

void System::setInteraction(const bool interaction) {
    m_interaction = interaction;
}

void System::setFrequency(const double omega) {
    assert(omega > 0);
    m_omega = omega;
}

void System::setAtomicNumber(const int Z) {
    assert(Z > 0);
    m_Z = Z;
}

void System::setLearningRate(const double eta) {
    assert(eta > 0);
    m_eta = eta;
}

void System::setWidth(const double sigma) {
    assert(sigma > 0);
    m_sigma = sigma;
}

void System::setHamiltonian(Hamiltonian* hamiltonian) {
    m_hamiltonian = hamiltonian;
}

void System::setBasis(Basis* basis) {
    m_basis = basis;
}

void System::setWaveFunction(std::vector<class WaveFunction *> waveFunctionVector) {
    m_waveFunctionVector = waveFunctionVector;
}

void System::setInitialState(InitialState* initialState) {
    m_initialState = initialState;
}

void System::setInitialWeights(InitialWeights* initialWeights) {
    m_initialWeights = initialWeights;
}

void System::setMetropolis(Metropolis* metropolis) {
    m_metropolis = metropolis;
}

void System::setOptimization(Optimization* optimization) {
    m_optimization = optimization;
}

void System::setRandomNumberGenerator(RandomNumberGenerator* randomnumbergenerator) {
    m_randomnumbergenerator = randomnumbergenerator;
}

void System::setGradients() {
    m_gradients = Eigen::MatrixXd::Zero(m_numberOfWaveFunctionElements, m_maxNumberOfParametersPerElement);
}

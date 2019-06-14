#include "doublewell.h"
#include <iostream>
#include "../system.h"
#include "../WaveFunctions/wavefunction.h"

DoubleWell::DoubleWell(System* system, double b) :
        Hamiltonian(system) {
    m_omega                 = m_system->getFrequency();
    m_omega_sqrd            = m_omega * m_omega;
    m_numberOfParticles     = m_system->getNumberOfParticles();
    m_numberOfDimensions    = m_system->getNumberOfDimensions();
    m_interaction           = m_system->getInteraction();
    m_b                     = b;
}

double DoubleWell::getExternalEnergy() {
    m_positions             = m_system->getPositions();
    double sumX = 0;
    for(int i=0; i<m_numberOfParticles; i++) {
        sumX += m_positions(i*m_numberOfDimensions);
    }
    return 0.5 * m_omega_sqrd * (m_positions.cwiseAbs2().sum() + 0.25 * m_b * m_b - m_b * fabs(sumX));
}

double DoubleWell::computeLocalEnergy() {
    double kineticEnergy     = m_system->getKineticEnergy();
    double externalEnergy    = getExternalEnergy();
    double interactionEnergy = Hamiltonian::getInteractionEnergy();
    return kineticEnergy + externalEnergy + interactionEnergy;
}

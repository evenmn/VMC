#pragma once
#include "hamiltonian.h"

class HarmonicOscillator : public Hamiltonian
{
public:
    HarmonicOscillator(System *system);
    int getGlobalArrayNeed() { return m_globalArrayNeed; }
    std::string getLabel() { return m_label; }

    void initialize();
    double getExternalEnergy();

private:
    double m_omegaSqrd;
    int m_globalArrayNeed = 1;
    std::string m_label = "harmonic oscillator";
};

#include "asgd.h"
#include "../sampler.h"
#include "../system.h"
#include <cassert>
#include <iostream>

ASGD::ASGD(System *system, const double gamma)
    : Optimization(system)
{
    m_gamma = gamma;
}

void ASGD::initialize()
{
    m_numberOfElements = m_system->getNumberOfElements();
    m_maxParameters = m_system->getMaxParameters();
    m_eta = m_system->getLearningRate();
    m_v = Eigen::MatrixXd::Ones(m_numberOfElements, m_maxParameters);
}

Eigen::MatrixXd ASGD::updateParameters()
{
    m_step += 1;
    //m_t = m_t + m_A;
    m_v = m_gamma * m_v + m_eta * Optimization::getEnergyGradient() / sqrt(m_step);
    return m_v;
}

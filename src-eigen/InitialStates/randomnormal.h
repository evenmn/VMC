#pragma once
#include "initialstate.h"

class RandomNormal : public InitialState
{
public:
    RandomNormal(System *system);
    double calculateDistanceMatrixElement(int i, int j);
    void calculateDistanceMatrix();
    double calculateRadialVectorElement(int particle);
    void calculateRadialVector();
    void setupInitialState();

private:
    double m_maxRadius = 1;
    double m_omega = 1;
};
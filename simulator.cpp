#include "simulator.h"
#include <cmath>
#include <iostream>
using std::cout; using std::endl;

Simulator::Simulator(QObject *parent) :
    QObject(parent)
{

}

Simulator::~Simulator()
{

}

void Simulator::step()
{
    emit stepCompleted();
}

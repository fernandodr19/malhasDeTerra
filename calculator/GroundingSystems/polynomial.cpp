#include "polynomial.h"
#include <cmath>

Polynomial::Polynomial(int size)
{
    m_coeffs.resize(size);
}

void Polynomial::setCoeffSize(int size)
{
    m_coeffs.resize(size);
}

void Polynomial::setCoeff(int coeff, const QString &value)
{
    if(coeff < 0 || coeff >= m_coeffs.size())
        return;

    m_coeffs[coeff] = value;
}

int Polynomial::getCoeffSize()
{
    return m_coeffs.size();
}

QString Polynomial::getCoeff(int coeff)
{
    if(coeff < 0 || coeff >= m_coeffs.size())
        return QString("");

    return m_coeffs.at(coeff);
}

double Polynomial::evaluate(double x)
{
    double result = 0;
    for(int i = 0; i < m_coeffs.size(); i++)
        result += m_coeffs[i].toDouble() * std::pow(x, i);
    return result;
}

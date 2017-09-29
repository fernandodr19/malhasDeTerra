#ifndef POLYNOMIAL_H
#define POLYNOMIAL_H

#include <QString>
#include <QVector>

class Polynomial
{
public:
    Polynomial() {}
    Polynomial(int size);

    void setCoeffSize(int size);
    void setCoeff(int coeff, const QString &value);

    int getCoeffSize();
    QString getCoeff(int coeff);

    double evaluate(double x);

private:
    QVector<QString> m_coeffs;
};

#endif // POLYNOMIAL_H

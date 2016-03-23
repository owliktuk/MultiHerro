#ifndef MEDIUM_H
#define MEDIUM_H

using namespace std;

class FerromagneticFilm
{
public:
    friend class Medium;
    FerromagneticFilm() {}
    FerromagneticFilm(double (&Ela)[6][6], double M, double (&MD)[3], double (&LC)[3], double (&Anis)[2], double (&ME)[2]);
    FerromagneticFilm(const FerromagneticFilm & rhs);
    ~FerromagneticFilm() {}
    void getMagDirection(double (&M)[3]);
    void setMagDirection(double (&M)[3]);
private:
    double itsElasticity[6][6];
    double itsMagnetisation;
    double itsMagDirection[3];
    double itsLatticeConstant[3];
    double itsAnisotrophy[2];
    double itsMagnetoElasticity[2];
    double itsDemagnetisation[2];
    vector< vector<double> > itsEnergy;
};

class Ferroelectric
{
public:
    friend class Medium;
    Ferroelectric() {}
    Ferroelectric(double (&PE)[3][3][3], double (&LC)[3]);
    Ferroelectric(const Ferroelectric & rhs);
    ~Ferroelectric() {}
private:
    double itsPiezoelectricity[3][3][3];
    double itsLatticeConstant[3];
};

class Medium
{
private:
    double itsDeformation[6];
    vector<double> itsEnergy;
    FerromagneticFilm itsFerromagneticFilm;
    Ferroelectric itsFerroelectric;

public:
    Medium(FerromagneticFilm FF, Ferroelectric FE);
    Medium() {}
    ~Medium() {}
    void countMagnetisation(double E1, double E2, double E3);
    void countMagnetisationInPlane110(double E);
    void getMagDirection(double (&M)[3]);
    void setMagDirection(double (&M)[3]);
    int getEnergySize();
    double getEnergy(int x, int y);
    void getDeformation(double (&D)[6]);
    void setDeformation(double (&D)[6]);
    void setFerromagneticFilm(FerromagneticFilm FMM) { itsFerromagneticFilm = FMM; }
    void setFerroelectric(Ferroelectric FE) { itsFerroelectric = FE; }

};

#endif // MEDIUM_H

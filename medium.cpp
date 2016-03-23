FerromagneticFilm::FerromagneticFilm(double (&Ela)[6][6], double M, double (&MD)[3], double (&LC)[3], double (&Anis)[2], double (&ME)[2])
{
    for(int i=0; i<6; i++)
        for(int j=0; j<6; j++) itsElasticity[i][j]=Ela[i][j];

    itsMagnetisation=M;

    for(int i=0; i<3; i++)
    {
        itsMagDirection[i] = MD[i];
        itsLatticeConstant[i] = LC[i];
    }

    itsAnisotrophy[0] = Anis[0];
    itsAnisotrophy[1] = Anis[1];

    itsMagnetoElasticity[0] = ME[0];
    itsMagnetoElasticity[1] = ME[1];

}

FerromagneticFilm::FerromagneticFilm(const FerromagneticFilm & rhs)
{
    for(int i=0; i<6; i++)
        for(int j=0; j<6; j++) itsElasticity[i][j] = rhs.itsElasticity[i][j];

    itsMagnetisation = rhs.itsMagnetisation;

    for(int i=0; i<3; i++)
    {
        itsMagDirection[i] = rhs.itsMagDirection[i];
        itsLatticeConstant[i] = rhs.itsLatticeConstant[i];
    }

    itsAnisotrophy[0] = rhs.itsAnisotrophy[0];
    itsAnisotrophy[1] = rhs.itsAnisotrophy[1];

    itsMagnetoElasticity[0] = rhs.itsMagnetoElasticity[0];
    itsMagnetoElasticity[1] = rhs.itsMagnetoElasticity[1];
}

void FerromagneticFilm::getMagDirection(double (&M)[3])
{
    for(int i=0; i<3; i++)
        M[i] = itsMagDirection[i];
}

void FerromagneticFilm::setMagDirection(double (&M)[3])
{
    for(int i=0; i<3; i++)
        itsMagDirection[i] = M[i];
}

Ferroelectric::Ferroelectric(double (&PE)[3][3][3], double (&LC)[3])
{
    for(int i=0; i<3; i++)
        for(int j=0; j<3; j++)
            for(int k=0; k<3; k++)
                itsPiezoelectricity[i][j][k]=PE[i][j][k];

    for(int i=0; i<3; i++)
        itsLatticeConstant[i] = LC[i];
}

Ferroelectric::Ferroelectric(const Ferroelectric & rhs)
{
    for(int i=0; i<3; i++)
        for(int j=0; j<3; j++)
            for(int k=0; k<3; k++)
                itsPiezoelectricity[i][j][k] = rhs.itsPiezoelectricity[i][j][k];

    for(int i=0; i<3; i++)
        itsLatticeConstant[i] = rhs.itsLatticeConstant[i];
}

Medium::Medium(FerromagneticFilm FF, Ferroelectric FE)
{
    itsFerromagneticFilm = FF;
    itsFerroelectric = FE;
}

void Medium::getMagDirection(double (&M)[3])
{
    for(int i=0; i < 3; i++)
        M[i] = round(100*itsFerromagneticFilm.itsMagDirection[i])/100;
}

void Medium::setMagDirection(double (&M)[3])
{
    for(int i=0; i < 3; i++)
        itsFerromagneticFilm.itsMagDirection[i] = M[i];
}

void Medium::getDeformation(double (&D)[6])
{
    for(int i=0; i<6; i++)
        D[i] = itsDeformation[i];
}

void Medium::setDeformation(double (&D)[6])
{
    for(int i=0; i<6; i++)
        itsDeformation[i] = D[i];
}

int Medium::getEnergySize()
{
    int s = itsFerromagneticFilm.itsEnergy[2].size();
    return s;
}

double Medium::getEnergy(int x, int y)
{
    double l = itsFerromagneticFilm.itsEnergy[x][y];
    return l;
}

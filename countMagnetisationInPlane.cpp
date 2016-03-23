void Medium::countMagnetisationInPlane110(double Ee)
{
    itsDeformation[0] = (itsFerroelectric.itsLatticeConstant[0]-itsFerromagneticFilm.itsLatticeConstant[0])/itsFerromagneticFilm.itsLatticeConstant[0] + (itsFerroelectric.itsPiezoelectricity[2][2][2] + itsFerroelectric.itsPiezoelectricity[2][0][0])*Ee*1E2;
    itsDeformation[1] = (itsFerroelectric.itsLatticeConstant[1]-itsFerromagneticFilm.itsLatticeConstant[1])/itsFerromagneticFilm.itsLatticeConstant[1] + (itsFerroelectric.itsPiezoelectricity[2][2][2] + itsFerroelectric.itsPiezoelectricity[2][1][1])*Ee*1E2;
    itsDeformation[5] = -(itsFerroelectric.itsPiezoelectricity[2][2][2] - itsFerroelectric.itsPiezoelectricity[2][0][0])*Ee*1E2;

    double K1 = itsFerromagneticFilm.itsAnisotrophy[0];
    double B1 = itsFerromagneticFilm.itsMagnetoElasticity[0];
    double B2 = itsFerromagneticFilm.itsMagnetoElasticity[1];
    double M = itsFerromagneticFilm.itsMagnetisation;
    double Heff = -2E2;
    double mu0 = 12.57E-7;

    double m[3];
    itsFerromagneticFilm.getMagDirection(m);

    //wyjściowy kierunek
    double phi0 = asin(m[1]);
    double phi;

    float rozdzielczosc = 0.5;
    int limit = floor(180/rozdzielczosc);

    double F[limit];

    for(int j=0; j < limit; j++)
    {
        phi = phi0 + 2.0*j*3.14159/limit - 3.14159;

        m[0] = cos(phi);
        m[1] = sin(phi);

        F[j] = K1*m[1]*m[1]*m[0]*m[0] + (B1*itsDeformation[0]+5*mu0*M*M)*m[0]*m[0] + (B1*itsDeformation[1]+5*mu0*M*M)*m[1]*m[1] + B2*itsDeformation[5]*m[0]*m[1] + Heff*m[0];
    }

    int J = 0;
    bool search = true;
    for(int n=0; n < limit/2.0 && search; n++)
    {
        for(int j = limit/2.0-n; j <= limit/2.0+n && search; j++)
        {
            if(abs(j-limit/2) == n)
            {
                if(F[j] <= F[j+1] && F[j] <= F[j-1] && F[j] <= F[j+2] && F[j] <= F[j-2])
                {
                    phi = phi0 + 2.0*(j-limit/2.0)*3.14159/limit;
                    if(phi < 0) phi = 2*3.14159 + phi;
                    if(phi >= 2*3.14159) phi = phi - 2*floor(phi/(2*3.14159))*3.14159;

                    m[0] = floor(100*cos(phi))/100;
                    m[1] = floor(100*sin(phi))/100;

                    search = false;

                    J = j;

                    itsFerromagneticFilm.itsMagDirection[0] = m[0];
                    itsFerromagneticFilm.itsMagDirection[1] = m[1];
                }
            }
        }
    }
    itsFerromagneticFilm.itsEnergy.clear();
    itsFerromagneticFilm.itsEnergy.resize(5);

    vector<double> Phi(limit);
    vector<double> EnergyPhi(limit);
    vector<double> EnergyMinimum(1);

    EnergyMinimum[0] = phi0 + 2.0*(J-limit/2.0)*3.14159/limit;
    if(EnergyMinimum[0] < 0) EnergyMinimum[0] = 2*3.14159+EnergyMinimum[0];
    if(EnergyMinimum[0] >= 2*3.14159) EnergyMinimum[0] = EnergyMinimum[0] - 2*floor(EnergyMinimum[0]/(2*3.14159))*3.14159;

    for(int i = 0; i < limit; i++)
    {
        EnergyPhi[i] = F[i]/F[5];
        Phi[i] = phi0 + 2.0*(i-limit/2.0)*3.14159/limit;
        if(Phi[i] < 0) Phi[i] = 2*3.14159 + Phi[i];
        if(Phi[i] >= 2*3.14159) Phi[i] = Phi[i] - 2*floor(Phi[i]/(2*3.14159))*3.14159;
    }


    itsFerromagneticFilm.itsEnergy[0] = EnergyMinimum;
    itsFerromagneticFilm.itsEnergy[2] = Phi;
    itsFerromagneticFilm.itsEnergy[4] = EnergyPhi;

}


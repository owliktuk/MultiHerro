void Medium::countMagnetisation(double E1, double E2, double E3)
{
    if(E1 == 0 && E2 == 0 && E3 != 0)
    {
        itsDeformation[0] = (itsFerroelectric.itsLatticeConstant[0]-itsFerromagneticFilm.itsLatticeConstant[0])/itsFerromagneticFilm.itsLatticeConstant[0] + itsFerroelectric.itsPiezoelectricity[2][0][0]*E3*1E2;
        itsDeformation[1] = (itsFerroelectric.itsLatticeConstant[1]-itsFerromagneticFilm.itsLatticeConstant[1])/itsFerromagneticFilm.itsLatticeConstant[1] + itsFerroelectric.itsPiezoelectricity[2][1][1]*E3*1E2;

    } else if(E2 == 0 && E3 == 0 && E1 != 0)
    {
        itsDeformation[0] = 0.008+(itsFerroelectric.itsLatticeConstant[0]-itsFerromagneticFilm.itsLatticeConstant[0])/itsFerromagneticFilm.itsLatticeConstant[0] + itsFerroelectric.itsPiezoelectricity[2][2][2]*E1*1E2;
        itsDeformation[1] = (itsFerroelectric.itsLatticeConstant[1]-itsFerromagneticFilm.itsLatticeConstant[1])/itsFerromagneticFilm.itsLatticeConstant[1] + itsFerroelectric.itsPiezoelectricity[2][0][0]*E1*1E2;
    }

    double K1 = itsFerromagneticFilm.itsAnisotrophy[0];
    double K2 = itsFerromagneticFilm.itsAnisotrophy[1];
    double B1 = itsFerromagneticFilm.itsMagnetoElasticity[0];
    double B2 = itsFerromagneticFilm.itsMagnetoElasticity[1];
    double M = itsFerromagneticFilm.itsMagnetisation;
    double c11 = itsFerromagneticFilm.itsElasticity[0][0];
    double c12 = itsFerromagneticFilm.itsElasticity[0][1];
    double c44 = itsFerromagneticFilm.itsElasticity[3][3];
    double mu0 = 12.57E-7;

    double m[3];
    itsFerromagneticFilm.getMagDirection(m);

    //wyjściowy kierunek
    double theta0 = acos(m[2]);
    double phi0 = asin(m[1]/sin(theta0));
    double theta, phi;

    float rozdzielczosc = 0.4;
    int limit = 180/rozdzielczosc;

    double F[limit][limit];

    for(int i=0; i < limit; i++)
    {
        for(int j=0; j < limit; j++)
        {
            theta = theta0 + i*3.14159/limit;
            phi = phi0 + 2.0*j*3.14159/limit  - 3.14159;

            m[0] = sin(theta)*cos(phi);
            m[1] = sin(theta)*sin(phi);
            m[2] = cos(theta);

            F[i][j] = B1*(itsDeformation[0]*m[0]*m[0]+itsDeformation[1]*m[1]*m[1])+(0.5*mu0*M*M-B1*B1/6/c11-c12/c11*B1*(itsDeformation[0]+itsDeformation[1]))*m[2]*m[2]+K1*m[0]*m[0]*m[1]*m[1]+(K1+B1*B1/2/c11-B2*B2/2/c44)*(m[0]*m[0]+m[1]*m[1])*m[2]*m[2]+K2*m[0]*m[0]*m[1]*m[1]*m[2]*m[2];
        }
    }

    int I =0, J = 0;
    bool search = true;
    for(int n=0; n < limit/2 && search; n++)
    {
        for(int i = limit/2-n; i <= limit/2+n && search; i++)
        {
            for(int j = limit/2-n; j <= limit/2+n && search; j++)
            {
                if(abs(i-limit/2)+abs(j-limit/2) == n)
                {
                    if(F[i][j] <= F[i+1][j] && F[i][j] <= F[i-1][j] && F[i][j] <= F[i][j+1] && F[i][j] <= F[i][j-1] && F[i][j] <= F[i-1][j-1] && F[i][j] <= F[i+1][j-1] && F[i][j] <= F[i-1][j+1] && F[i][j] <= F[i+1][j+1] )
                    {
                        theta = theta0 + i*3.14159/limit;
                        phi = phi0 + 2.0*j*3.14159/limit;

                        m[0] = sin(theta)*cos(phi);
                        m[1] = sin(theta)*sin(phi);
                        m[2] = cos(theta);

                        search = false;

                        I = i;
                        J = j;

                        itsFerromagneticFilm.itsMagDirection[0] = m[0];
                        itsFerromagneticFilm.itsMagDirection[2] = m[2];
                        itsFerromagneticFilm.itsMagDirection[1] = m[1];
                    }
                }
            }
        }
    }
    itsFerromagneticFilm.itsEnergy.clear();
    itsFerromagneticFilm.itsEnergy.resize(5);

    vector<double> Phi(limit);
    vector<double> Theta(limit);
    vector<double> EnergyPhi(limit);
    vector<double> EnergyTheta(limit);
    vector<double> EnergyMinimum(2);

    EnergyMinimum[1] = theta0 + I*3.14159/limit;
    EnergyMinimum[0] = phi0 + 2.0*J*3.14159/limit;
    if(EnergyMinimum[0] < 0) EnergyMinimum[0] = -EnergyMinimum[0];
    if(EnergyMinimum[1] < 0) EnergyMinimum[1] = -EnergyMinimum[1];
    if(EnergyMinimum[0] >= 3.14159) EnergyMinimum[0] = EnergyMinimum[0] - floor(EnergyMinimum[0]/3.14159)*3.14159;
    if(EnergyMinimum[1] >= 3.14159) EnergyMinimum[1] = EnergyMinimum[1] - floor(EnergyMinimum[1]/3.14159)*3.14159;

    for(int i = 0; i < limit; i++)
    {
        EnergyPhi[i] = F[I][i]/F[I][0];
        EnergyTheta[i] = F[i][J]/F[0][J];
        Theta[i] = theta0 + i*3.14159/limit;
        Phi[i] = phi0 + 2.0*i*3.14159/limit;
        if(Theta[i] < 0) Theta[i] = -Theta[i];
        if(Phi[i] < 0) Phi[i] = -Phi[i];
        if(Theta[i] > 3.14159) Theta[i] = Theta[i] - 3.14159;
        if(Phi[i] > 3.14159) Phi[i] = Phi[i] - floor(Phi[i]/3.14159)*3.14159;
    }


    itsFerromagneticFilm.itsEnergy[0] = EnergyMinimum;
    itsFerromagneticFilm.itsEnergy[1] = Theta;
    itsFerromagneticFilm.itsEnergy[2] = Phi;
    itsFerromagneticFilm.itsEnergy[3] = EnergyTheta;
    itsFerromagneticFilm.itsEnergy[4] = EnergyPhi;

}

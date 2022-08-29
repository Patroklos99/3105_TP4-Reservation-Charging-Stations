/*  INF3105 - Structures de données et algorithmes       *
 *  UQAM / Département d'informatique                    *
 *  Été 2022 / TP2 et TP4                                *
 *  Eric Beaudry - beaudry.eric@uqam.ca - ericbeaudry.ca */

#include <math.h>
#include <assert.h>
#include "coordonnees.h"


Coordonnees::Coordonnees(const Coordonnees& coor)
  : latitude(coor.latitude), longitude(coor.longitude)
{

}

Coordonnees::Coordonnees(double latitude_, double longitude_) 
  : latitude(latitude_), longitude(longitude_)
{
}

double Coordonnees::distance(const Coordonnees& coor) const {
    double s1 = sin((coor.latitude - latitude) / 2);
    double s2 = sin((coor.longitude - longitude) / 2);
    return 2 * RAYONTERRE * asin(sqrt(s1 * s1 + cosLatitude * coor.cosLatitude * s2 * s2));
}

std::istream& operator >> (std::istream& is, Coordonnees& coor) {
    char po, vir, pf;
    is >> po;
    if(is){
        is >> coor.latitude >> vir >> coor.longitude >> pf;
        assert(po=='(');
        assert(vir==',');
        assert(pf==')');
        coor.longitude *= PI / 180.0;
        coor.latitude  *= PI / 180.0;
        coor.cosLatitude = cos(coor.latitude);
    }
    return is;
}

std::ostream& operator << (std::ostream& os, const Coordonnees& coor) {
    os << "(" 
       << (coor.latitude * 180.0 / PI)
       << "," 
       << (coor.longitude * 180.0 / PI)
       << ")";
    return os;
}

double Coordonnees::calculNord(int distance) const {
    double dist = distance;
    double new_latitude = this->latitude + (dist / RAYONTERRE) * piMult;
    return new_latitude;
}

double Coordonnees::calculSud(int distance) const {
    double dist = distance;
    double new_latitude = this->latitude - (dist / RAYONTERRE) * piMult;
    return new_latitude;
}

double Coordonnees::calculEst(int distance) const {
    double dist = distance;
    double new_longitude = this->longitude + (dist / RAYONTERRE) * piMult / cos(this->latitude * M_PI / 180);
    return new_longitude;
}

double Coordonnees::calculOuest(int distance) const {
    double dist = distance;
    double new_longitude = this->longitude - (dist / RAYONTERRE) * piMult / cos(this->latitude * M_PI / 180);
    return new_longitude;
}

void Coordonnees::calculcube(int distance) {
    if(cube.rayon != distance) {
        double haut = calculNord(distance);
        double bas = calculSud(distance);
        double gauche = calculOuest(distance);
        double droite = calculEst(distance);

        cube.est = droite;
        cube.ouest = gauche;
        cube.nord = haut;
        cube.sud = bas;
    }
}

bool Coordonnees::estDansCube(const Coordonnees & c) const {

    bool tr1 = c.latitude <= cube.nord;
    bool tr2 = c.latitude >= cube.sud;
    bool tr3 = c.longitude >= cube.ouest;
    bool tr4 = c.longitude <= cube.est;

    return c.latitude <= cube.nord && c.latitude >= cube.sud && c.longitude >= cube.ouest && c.longitude <= cube.est;
}


Coordonnees::Cube::Cube() {

}

Coordonnees::Cube::~Cube() {

}

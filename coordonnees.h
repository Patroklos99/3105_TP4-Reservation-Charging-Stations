/*  INF3105 - Structures de données et algorithmes       *
 *  UQAM / Département d'informatique                    *
 *  Été 2022 / TP2 et TP4                                *
 *  Eric Beaudry - beaudry.eric@uqam.ca - ericbeaudry.ca */

#if !defined(_COORDONNEES__H_)
#define _COORDONNEES__H_
#include <iostream>

#define RAYONTERRE 6371000  // en mètres
#define PI 3.14159265359

class Coordonnees {
  public:
    Coordonnees(){}
    Coordonnees(double latitude_, double longitude_);
    Coordonnees(const Coordonnees&);
    double distance(const Coordonnees&) const;
    void calculcube(int distance) ;
    bool estDansCube(const Coordonnees&)const;
private:
    double latitude;
    double longitude;
    double cosLatitude;
    double piMult = 180/PI;

    class Cube{
      public:
        Cube();
        ~Cube();
        double nord; //haut-gauche
        double sud; //haut-droite
        double ouest; //bas-gauche
        double est; //bas-droite
        int rayon = 0;

    };
    double calculNord(int distance) const;
    double calculSud(int distance) const;
    double calculEst(int distance) const;
    double calculOuest(int distance) const;

    Cube cube;
    friend std::ostream& operator << (std::ostream&, const Coordonnees&);
  friend std::istream& operator >> (std::istream&, Coordonnees&);
};

#endif


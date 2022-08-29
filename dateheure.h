/*  INF3105 - Structures de données et algorithmes       *
 *  UQAM / Département d'informatique                    *
 *  Été 2022 / TP4                                       *
 *  Eric Beaudry - beaudry.eric@uqam.ca - ericbeaudry.ca */

#if !defined(__DATEHEURE_H__)
#define __DATEHEURE_H__
using namespace std;
#include <iostream>

class DateHeure{
  public:
    DateHeure();
    DateHeure(int jours,int heures, int minutes, int secondes);
    ~DateHeure();
    bool operator<(const DateHeure& dh) const;
    bool operator<=(const DateHeure& dh) const;
    DateHeure operator+(int duree)const;
    DateHeure& operator=(const DateHeure& dh) ;
    bool operator==(const DateHeure& dh) const;

  private:
  int jours, heures, minutes, secondes;
  double multSoixante = 1.0/60.0;
  double multTroiMille = 1.0/3600.0;
  double multMill = 1.0/86400.0;
  double multVingtQuatre = 1.0/24.0;
    
  friend std::ostream& operator << (std::ostream&, const DateHeure& dh);
  friend std::istream& operator >> (std::istream&, DateHeure& dh);
};


#endif

/*  INF3105 - Structures de données et algorithmes       *
 *  UQAM / Département d'informatique                    *
 *  Été 2022 / TP4                                       *
 *  Eric Beaudry - beaudry.eric@uqam.ca - ericbeaudry.ca */

#if !defined(__AGENDA_H__)
#define __AGENDA_H__
#include <string>
#include "coordonnees.h"
#include "dateheure.h"
#include "arbremap.h"

using namespace std;
class Agenda{
  public:
      class Ecart{
        public:
          Ecart(){};
          Ecart(DateHeure ecart, int duree):ecart(ecart),duree(duree){}
          DateHeure ecart;
          int duree;
      };
    class Borne{
      public:
        Coordonnees coor;
        int puissance;
        

        ArbreMap<DateHeure,Ecart> reservations;

        bool operator < (const Borne& e)const {
          double dist1 = Coordonnees(0,0).distance(coor);
          double dist2 = Coordonnees(0,0).distance(e.coor);
          return dist1 < dist2;
        }
    };




    Agenda();
    ~Agenda();
    void creer(const std::string& id, const Coordonnees& c, int puissance);
    ArbreMap<std::string, int> trouver(const Coordonnees& c, int rayon=0, int puissancemin=0) const;
    DateHeure reserverI(const std::string& id, DateHeure date, int duree);
    DateHeure reserverC(const Coordonnees& c, int rayon, int puissancemin, DateHeure datemin, int duree, std::string& borneid);

  ArbreMap<string,Borne> bornes_agenda;
private :
    DateHeure prendreDateValide(const string& borneId, DateHeure& date,int duree);
};

#endif


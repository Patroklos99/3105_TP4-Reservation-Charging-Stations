/*  INF3105 - Structures de données et algorithmes       *
 *  UQAM / Département d'informatique                    *
 *  Été 2022 / TP4                                       *
 *  Eric Beaudry - beaudry.eric@uqam.ca - ericbeaudry.ca */

#include "agenda.h"
#include <cmath>
#include <limits>


Agenda::Agenda() {}

Agenda::~Agenda() {}

void Agenda::creer(const std::string &id, const Coordonnees &c, int puissance) {
    Borne borne;
    borne.coor = c;
    borne.puissance = puissance;
    bornes_agenda[id] = borne;
}

ArbreMap<std::string, int> Agenda::trouver(const Coordonnees &c, int rayon, int puissancemin) const {
    ArbreMap<std::string, int> resultat;

    ArbreMap<string, Borne>::Iterateur iter = bornes_agenda.debut();

    while (iter) {
        Borne *borne = (Borne*)(&iter.valeur());
        borne->coor.calculcube(rayon);
        if (iter.valeur().puissance >= puissancemin) {
            if(iter.valeur().coor.estDansCube(c)) {
                double doudist = iter.valeur().coor.distance(c);
                int dist = std::round(doudist);
                if (doudist <= rayon)
                    resultat[iter.cle()] = dist;
            }
        }
        ++iter;
    }

    return resultat;
}


DateHeure Agenda::reserverI(const std::string &id, DateHeure date, int duree) {
    DateHeure duration = date;

    duration = prendreDateValide(id,duration,duree);
    bornes_agenda[id].reservations[duration] = Ecart(duration+duree,duree);

    return duration;
}

DateHeure Agenda::reserverC(const Coordonnees &c, int rayon, int puissancemin, DateHeure datemin, int duree, std::string &borneid) {
    borneid = "?";

    ArbreMap<string, Borne>::Iterateur iter = bornes_agenda.debut();
    DateHeure min = datemin;
    DateHeure min1 = datemin;
    string borneMin = "";
    double minDistance = std::numeric_limits<double>::max();
    int pos = 0;
    ArbreMap<string ,DateHeure> temporaliter;
    ArbreMap<string ,double> distances;

    while (iter){
        Borne *borne = (Borne*)(&iter.valeur());
        borne->coor.calculcube(rayon);
        if(iter.valeur().coor.estDansCube(c)) {
            double distance = c.distance(iter.valeur().coor);
            if (iter.valeur().puissance >= puissancemin && distance <= (double)rayon) {
                DateHeure temps = prendreDateValide(iter.cle(), min, duree);
                temporaliter[iter.cle()] = temps;
                distances[iter.cle()] = distance;
                if (pos < 1 || temps <= min1) {
                    min1 = temps;
                    ++pos;
                }
            }
        }
        ++iter;
        min = datemin;
    }
    ArbreMap<string, DateHeure>::Iterateur iter2 = temporaliter.debut();
    while (iter2){
        if(iter2.valeur() < min1){
            min = iter2.valeur();
            borneid = iter2.cle();
            minDistance = distances[iter2.cle()];
            break;
        }
        else if(iter2.valeur() == min1){
            if(distances[iter2.cle()] < minDistance ){
                min = iter2.valeur();
                borneid = iter2.cle();
                minDistance = distances[iter2.cle()];
            }
        }
        ++iter2;
    }
    reserverI(borneid,min,duree);
    return min;
}

DateHeure Agenda::prendreDateValide(const string& id, DateHeure &duration, int duree) {
    ArbreMap<DateHeure, Ecart>::Iterateur iter = bornes_agenda[id].reservations.rechercherEgalOuPrecedent(duration);
    if(!iter)
        iter = bornes_agenda[id].reservations.debut();

    while (iter){
        if((duration + duree) <= iter.cle()){
            return duration;
        }
        else {
            if(iter.cle() < duration){
                if(!((iter.valeur().ecart) < duration)) {
                    duration = iter.valeur().ecart;
                }
            }else{
                duration = iter.valeur().ecart;
            }
            ++iter;
        }
    }
    return duration;
    }


/*  INF3105 - Structures de données et algorithmes       *
 *  UQAM / Département d'informatique                    *
 *  Été 2022 / TP4                                       *
 *  Eric Beaudry - beaudry.eric@uqam.ca - ericbeaudry.ca */

#include "dateheure.h"
#include <cstdio>
#include <iomanip>
#include <assert.h>

DateHeure::DateHeure(){

}

DateHeure::DateHeure(int jours,int heures, int minutes, int secondes) : jours(0), heures(0), minutes(0), secondes(0){

}
DateHeure::~DateHeure(){

}

bool DateHeure::operator<(const DateHeure& dh) const{
    int valeur0 = (jours * 86400 + heures * 3600 + minutes * 60 + secondes);
    int valeur1 = (dh.jours * 86400 + dh.heures * 3600 + dh.minutes * 60 + dh.secondes);
    int val3 = valeur0 - valeur1;
    return val3 < 0;
}

bool DateHeure::operator==(const DateHeure& dh) const{
    return (jours == dh.jours && heures == dh.heures && minutes == dh.minutes && secondes == dh.secondes);
}

DateHeure DateHeure::operator+(const int duree)const {
    int j = duree*multMill;
    int h = (duree%86400)*multTroiMille;
    int mi = (duree%3600)*multSoixante;
    int sec = (duree-((j*86400)+(h*3600)+(mi*60)));
    DateHeure date;
    date.jours = jours;
    date.heures = heures;
    date.minutes = minutes;
    date.secondes = secondes;

    int second = (date.secondes+ sec)%60;

    int suppMin = (date.secondes+ sec)*multSoixante;
    int minute = (date.minutes + mi + suppMin)%60;
    
    int suppHeure = (date.minutes + mi + suppMin)*multSoixante;
    int heure = (date.heures + h + suppHeure)%24;
    
    int suppJour = (date.heures + h + suppHeure)*multVingtQuatre;
    int jour = (date.jours + j + suppJour);


    date.secondes = second;
    date.minutes = minute;
    date.heures = heure;
    date.jours =  jour;

    return date;
}

std::ostream& operator << (std::ostream& os, const DateHeure& dh){
    int jours=0, heures=0, minutes=0, secondes=0;
    jours = dh.jours;
    minutes = dh.minutes;
    heures = dh.heures;
    secondes = dh.secondes;

    char chaine[40];
    sprintf(chaine, "%d_%02d:%02d:%02d", jours, heures, minutes, secondes);
    os << chaine;
    return os;
}

std::istream& operator >> (std::istream& is, DateHeure& dh){
    int jours=0, heures=0, minutes=0, secondes=0;
    char seperateurlu1=0, seperateurlu2=0, seperateurlu3=0;
    is >> jours >> seperateurlu1 >> heures >> seperateurlu2 >> minutes >> seperateurlu3 >> secondes;
    assert(seperateurlu1=='_');
    assert(seperateurlu2==':');
    assert(seperateurlu3==':');
    dh.jours = jours;
    dh.heures = heures;
    dh.minutes = minutes;
    dh.secondes = secondes;
    return is;
}

DateHeure& DateHeure::operator=(const DateHeure &dh) {
    jours = dh.jours;
    heures = dh.heures;
    minutes = dh.minutes;
    secondes = dh.secondes;
    return *this;
}

bool DateHeure::operator<=(const DateHeure &dh) const {
    int valeur0 = (jours * 86400 + heures * 3600 + minutes * 60 + secondes);
    int valeur1 = (dh.jours * 86400 + dh.heures * 3600 + dh.minutes * 60 + dh.secondes);
    int val3 = valeur0 - valeur1;
    return val3 <= 0;
}





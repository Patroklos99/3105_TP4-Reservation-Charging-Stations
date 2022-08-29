#!/bin/bash
###############################################################################
# UQAM - Département d'informatique
# INF3105 - Structures de données et algorithmes
# Été 2022
# TP4
# http://ericbeaudry.uqam.ca/INF3105/tp4/
# beaudry.eric@uqam.ca
#
# Script d'évaluation
#
# Instructions:
# 1. Déposer ce script avec les fichiers tests dans un répertoire 
#    distinct (ex: tests).
# 2. Se placer dans le répertoire contenant votre programme `tp4`.
#    Le correcteur se placera dans le répertoire contenant les soumissions.
# 3. Lancer ce script (ex: ../tests/evaluer.sh).
###############################################################################

# Obtenir le chemin du répertoire contenant le présent script et les fichiers tests
pushd `dirname $0` > /dev/null
repertoire_tests=`pwd`
# Lister les fichier test disponibles
tests=`ls test-*-[A-Z][0-9].txt`
#tests="traces00.txt traces01.txt traces02.txt"
popd  > /dev/null

echo "UQAM | Département d'informatique"
echo "INF3105 | Structures de données et algorithmes"
echo "Évaluation du TP4"
echo

if [ `pwd` -ef $repertoire_tests ];
then
    echo "Ce script doit être dans un répertoire différent de celui contenant votre tp4."
    echo "Ce script a été arrêté afin de ne pas écraser les fichiers test[A-Z]*+.txt!"
    exit -2;
fi


# Détection si l'utilitaire time sous Linux est disponible (peut afficher 0.00)
echo "Détection de /usr/bin/time..."
/usr/bin/time -f %U echo 2>&1 > /dev/null
souslinux=$?

# Fixer les limites : 180s, 2048Mo mémoire et 20Mo fichier
#ulimit -t 180 -v 2097152 -f 20480
echo "ulimit (limites courantes des ressources) :"
ulimit -t -v -f
echo "-----"

# Détection du CPU
if [ -e /proc/cpuinfo ] ; then
    cpuinfo=`grep "model name" /proc/cpuinfo | sort -u | cut -d ":" -f 2`
else
    cpuinfo="?"
fi


##### La fonction qui évalue un TP ######
function EvaluerTP
{
    date2=`date +%Y%m%d_%H%M%S`
    #logfile="log-${date2}.txt"
    logfile="log.txt"
    echo "Les résultats seront déposés dans $logfile..."
    echo "Date: $date2" > $logfile

    #datesoumis=`grep Date lisezmoi.txt| cut -c 7-`
    #coequipier=`grep Coéquipier lisezmoi.txt| cut -d : -f 2`
    #heures=`grep Heures lisezmoi.txt| cut -d : -f 2`
    #autoeval=`grep Auto-Évaluation lisezmoi.txt| cut -d : -f 2`

    ## ZIP
    #if [ -f sources.zip ]; then
    #    echo "Unzip: sources.zip"
    #    unzip -n sources.zip
    #    rm sources.zip
    #fi

    #echo "Fichiers:" > $logfile
    #ls -l 2>&1 | tee -a $logfile

    ## Forcer la recompilation.
    #rm tp4 *.o
    #make clean


#    echo "#Machine : " `hostname`  | tee -a $logfile
    echo "#CPU :$cpuinfo"  | tee -a $logfile
    #echo "#Date début : ${date2}"  | tee -a $logfile
    echo "#Limite de `ulimit -t` secondes par test"  | tee -a $logfile
    echo | tee -a $logfile

    # Pour statistiques : nombre de lignes de code...
    #echo "Taille des fichiers source :" | tee -a ${logfile}
    #wc *.{c*,h*}  | tee -a ${logfile}

    taille=`wc *.{c*,h*} | grep total`
    sommaire="$taille"
    sommaire="$datesoumis\t$coequipier\t$autoeval\t$heures\t$taille\t"

    #echo "Compilation ..." | tee -a $logfile 
    #make tp4 >> $logfile 2>&1

    # Vérification de l'existance du programme.
    if [ ! -e tp4 ]
        then
        echo "  Erreur : le fichier tp4 est inexistant!" | tee -a $logfile
        return
    fi
    if [ ! -x tp4 ]
        then
        echo "  Erreur : le fichier tp4 n'est pas exécutable!" | tee -a $logfile
        return
    fi

    echo | tee -a $logfile

	echo -e "Fichier_Test\tBornes\tCmd\tCorrect\tCPU\tMém.(k)" | tee -a $logfile

	for test in $tests;
	do
    	fichiertest="$repertoire_tests/$test"
    	nbbornes=`grep creer $fichiertest | wc -l`
    	nbcmds=`grep -v 'creer' $fichiertest | wc -l`
    	fichierresultat="${test%.txt}+.txt"
    	fichiersolution="$repertoire_tests/${test%.txt}+.txt"

        {
	        if [ $souslinux -eq 0 ]; then
	            tcpu="`(/usr/bin/time -f "%U\t%Mk" ./tp4 $fichiertest > $fichierresultat) 2>&1 | tail -n 1`"
        	else
            	tcpu=`(time -p ./$programme $fichiertest > $fichierresultat) 2>&1 | egrep user | cut -f 2 -d " "`
	        fi
	        
	    }
    
	    validation="?"
    	if( [ -e $fichiersolution ] )
    	then
        	diff -tibw $fichierresultat $fichiersolution 2>&1 > /dev/null
    		if [ $? -eq 0 ];
    		then
    	    	validation="OK"
     		else
     	    	validation="Echec"
    		fi
    	fi

    	echo -e "$test\t$nbbornes\t$nbcmds\t$validation\t$tcpu"
    	echo -e "$test\t$nbbornes\t$nbcmds\t$validation\t$tcpu" >> $logfile
    	sommaire="${sommaire}\t${tcpu}\t${validation}"
	done
}


if [ -f tp4.cpp ] || [ -f sources.zip ]
then
    tps="."
else
    # Lister les répertoires
    tps=`ls -1`
#    tps="0sources 1solution 2solution 3solution"
    tps=`for x in $tps; do if [ -d $x ] ; then echo $x; fi; done`
fi

echo $tps

# Génération de l'entête du rapport
date=`date +%Y%m%d_%H%M%S`
echo "#Rapport de correction INF3105 / TP4" > "rapport-${date}.txt"
echo -e "#Date:\t${date}" >> "rapport-${date}.txt"
echo -e "#Machine :\t" `hostname` >> "rapport-${date}.txt"
echo -e "#CPU :\t$cpuinfo" >> "rapport-${date}.txt"
echo >> "rapport-${date}.txt"

# Génération des titres des colonnes
echo -e -n "#Soumission\tDate\tCoéquipier\tAuto-Éval\tHeures\tTaille\t" >> "rapport-${date}.txt"
for test in ${tests}; do
    echo -e -n "\t${test}\t\t" >> "rapport-${date}.txt"
done
echo >> "rapport-${date}.txt"
echo -e -n "#Soumission\tDate\tCoéquipier\tAuto-Éval\tHeures\tTaille\t" >> "rapport-${date}.txt"
for test in ${tests}; do
   echo -e -n "\tCPU(s)\tMem.(k)\tRésultat" >> "rapport-${date}.txt"
done
echo >> "rapport-${date}.txt"

# Itération sur chaque TP
for tp in $tps; do
    sommaire=""
    echo "## CORRECTION : $tp"
    pushd $tp
        EvaluerTP
#       Nettoyer
    popd
    #echo -e ">> ${sommaire}"
    echo -e "${tp}\t${sommaire}" >> "rapport-${date}.txt"
done


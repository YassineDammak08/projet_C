#ifndef STATS_H
#define STATS_H

#include "reservation.h"
#include "salle.h"


void afficher_chiffre_affaires_par_salle(const ListeReservations *lr,
                                         const ListeSalles *ls);


void afficher_stats_par_mois(const ListeReservations *lr);


void afficher_salles_populaires(const ListeReservations *lr,
                                const ListeSalles *ls);

#endif 

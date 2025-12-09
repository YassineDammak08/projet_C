#ifndef RESERVATION_H
#define RESERVATION_H

#include "types.h"
#include "salle.h"

typedef struct {
    int   id;
    char  nom_client[MAX_NOM_CLIENT];
    int   salle_id;
    Date  date;
    Heure heure_debut;
    Heure heure_fin;
    int   nombre_personnes;
    double tarif_total;
    ReservationStatus statut;
} Reservation;

typedef struct {
    Reservation *data;
    int          taille;
    int          capacite;
} ListeReservations;


void init_liste_reservations(ListeReservations *lr);
void liberer_liste_reservations(ListeReservations *lr);


int ajouter_reservation(ListeReservations *lr,
                        const ListeSalles *ls,
                        const char *nom_client,
                        int salle_id,
                        Date date,
                        Heure h_debut,
                        Heure h_fin,
                        int nb_personnes,
                        ReservationStatus statut);


int verifier_disponibilite(const ListeReservations *lr,
                           int salle_id,
                           Date date,
                           Heure h_debut,
                           Heure h_fin);


int verifier_capacite(const Salle *salle, int nb_personnes);


double calculer_tarif_reservation(const Salle *salle,
                                  Heure h_debut,
                                  Heure h_fin);


Reservation* trouver_reservation_par_id(ListeReservations *lr, int id);


int annuler_reservation(ListeReservations *lr, int id);


void afficher_reservation(const Reservation *r, const ListeSalles *ls);
void afficher_toutes_reservations(const ListeReservations *lr, const ListeSalles *ls);


void rechercher_reservations_par_client(const ListeReservations *lr,
                                        const ListeSalles *ls,
                                        const char *nom_client);


int sauvegarder_reservations(const ListeReservations *lr, const char *chemin_fichier);
int charger_reservations(ListeReservations *lr, const char *chemin_fichier);

#endif

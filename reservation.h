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

/* Initialisation / libération */
void init_liste_reservations(ListeReservations *lr);
void liberer_liste_reservations(ListeReservations *lr);

/* Ajout de réservation (avec vérifications + calcul du tarif).
   Retourne l'id de la reservation en cas de succès, -1 en cas d'échec. */
int ajouter_reservation(ListeReservations *lr,
                        const ListeSalles *ls,
                        const char *nom_client,
                        int salle_id,
                        Date date,
                        Heure h_debut,
                        Heure h_fin,
                        int nb_personnes,
                        ReservationStatus statut);

/* Vérifie si la salle est disponible pour un créneau donné.
   Retourne 1 si disponible, 0 si non. */
int verifier_disponibilite(const ListeReservations *lr,
                           int salle_id,
                           Date date,
                           Heure h_debut,
                           Heure h_fin);

/* Vérifie la capacité d'une salle.
   Retourne 1 si OK, 0 si dépassement. */
int verifier_capacite(const Salle *salle, int nb_personnes);

/* Calcul du tarif total pour une réservation. */
double calculer_tarif_reservation(const Salle *salle,
                                  Heure h_debut,
                                  Heure h_fin);

/* Recherche de réservation par id */
Reservation* trouver_reservation_par_id(ListeReservations *lr, int id);

/* Annulation d'une réservation (statut = ANNULE).
   Retourne 0 si OK, -1 si non trouvé. */
int annuler_reservation(ListeReservations *lr, int id);

/* Affichage */
void afficher_reservation(const Reservation *r, const ListeSalles *ls);
void afficher_toutes_reservations(const ListeReservations *lr, const ListeSalles *ls);

/* Recherche par client (affiche les réservations du client) */
void rechercher_reservations_par_client(const ListeReservations *lr,
                                        const ListeSalles *ls,
                                        const char *nom_client);

/* Persistance texte (utilisé par persistence.c)
   Format ligne :
   id;nom_client;salle_id;jour;mois;annee;h_debut;min_debut;h_fin;min_fin;
   nb_personnes;tarif_total;statut
*/
int sauvegarder_reservations(const ListeReservations *lr, const char *chemin_fichier);
int charger_reservations(ListeReservations *lr, const char *chemin_fichier);

#endif

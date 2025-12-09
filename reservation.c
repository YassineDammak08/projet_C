#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "reservation.h"
#include "utils.h"

int agrandir_liste_reservations(ListeReservations *lr) {
    int nouvelle_capacite = (lr->capacite == 0) ? 4 : lr->capacite * 2;
    Reservation *nouveau = realloc(lr->data, (size_t)nouvelle_capacite * sizeof(Reservation));
    if (nouveau == NULL) {
        return -1;
    }
    lr->data = nouveau;
    lr->capacite = nouvelle_capacite;
    return 0;
}

void init_liste_reservations(ListeReservations *lr) {
    if (lr == NULL) return;
    lr->data = NULL;
    lr->taille = 0;
    lr->capacite = 0;
}

void liberer_liste_reservations(ListeReservations *lr) {
    if (lr == NULL) return;
    free(lr->data);
    lr->data = NULL;
    lr->taille = 0;
    lr->capacite = 0;
}

int verifier_capacite(const Salle *salle, int nb_personnes) {
    if (salle == NULL) return 0;
    return (nb_personnes <= salle->capacite) ? 1 : 0;
}

int verifier_disponibilite(const ListeReservations *lr,
                           int salle_id,
                           Date date,
                           Heure h_debut,
                           Heure h_fin) {
    if (lr == NULL) return 0;

    int debut_min = heure_en_minutes(&h_debut);
    int fin_min   = heure_en_minutes(&h_fin);

    for (int i = 0; i < lr->taille; i++) {
        const Reservation *r = &lr->data[i];

        /* Ignorer les réservations annulées ou d'une autre salle */
        if (r->salle_id != salle_id) continue;
        if (r->statut == RES_STATUS_ANNULE) continue;

        /* Même date ? */
        if (comparer_date(&r->date, &date) != 0) {
            continue;
        }

        int rd = heure_en_minutes(&r->heure_debut);
        int rf = heure_en_minutes(&r->heure_fin);

        /* Test de chevauchement: [debut, fin) et [rd, rf) se chevauchent si: */
        if (debut_min < rf && rd < fin_min) {
            return 0; /* indisponible */
        }
    }

    return 1; /* disponible */
}

double calculer_tarif_reservation(const Salle *salle,
                                  Heure h_debut,
                                  Heure h_fin) {
    if (salle == NULL) return -1.0;
    double duree = calculer_duree_heures(&h_debut, &h_fin);
    if (duree <= 0.0) {
        return -1.0;
    }
    return duree * salle->tarif_horaire;
}

static int generer_nouvel_id(const ListeReservations *lr) {
    int max_id = 0;
    for (int i = 0; i < lr->taille; i++) {
        if (lr->data[i].id > max_id) {
            max_id = lr->data[i].id;
        }
    }
    return max_id + 1;
}

int ajouter_reservation(ListeReservations *lr,
                        const ListeSalles *ls,
                        const char *nom_client,
                        int salle_id,
                        Date date,
                        Heure h_debut,
                        Heure h_fin,
                        int nb_personnes,
                        ReservationStatus statut) {
    if (lr == NULL || ls == NULL || nom_client == NULL) {
        return -1;
    }

    /* Vérifier que la salle existe */
    Salle *salle = trouver_salle_par_id((ListeSalles *)ls, salle_id);
    if (salle == NULL) {
        printf("Salle avec ID %d introuvable.\n", salle_id);
        return -1;
    }

    /* Vérifier que les heures sont cohérentes */
    double duree = calculer_duree_heures(&h_debut, &h_fin);
    if (duree <= 0.0) {
        printf("Plage horaire invalide (heure de fin <= heure de debut).\n");
        return -1;
    }

    /* Vérifier capacité */
    if (!verifier_capacite(salle, nb_personnes)) {
        printf("Capacite depassee pour la salle '%s' (max %d personnes).\n",
               salle->nom, salle->capacite);
        return -1;
    }

    /* Vérifier disponibilité (chevauchements) */
    if (!verifier_disponibilite(lr, salle_id, date, h_debut, h_fin)) {
        printf("La salle '%s' n'est pas disponible sur ce cr\u00e9neau.\n", salle->nom);
        return -1;
    }

    /* Calcul du tarif */
    double tarif = calculer_tarif_reservation(salle, h_debut, h_fin);
    if (tarif < 0.0) {
        printf("Erreur lors du calcul du tarif.\n");
        return -1;
    }

    if (lr->taille >= lr->capacite) {
        if (agrandir_liste_reservations(lr) != 0) {
            printf("Erreur d'allocation memoire pour la reservation.\n");
            return -1;
        }
    }

    Reservation r;
    r.id = generer_nouvel_id(lr);
    strncpy(r.nom_client, nom_client, MAX_NOM_CLIENT - 1);
    r.nom_client[MAX_NOM_CLIENT - 1] = '\0';
    r.salle_id = salle_id;
    r.date = date;
    r.heure_debut = h_debut;
    r.heure_fin = h_fin;
    r.nombre_personnes = nb_personnes;
    r.tarif_total = tarif;
    r.statut = statut;

    lr->data[lr->taille] = r;
    lr->taille++;

    return r.id;
}

Reservation* trouver_reservation_par_id(ListeReservations *lr, int id) {
    if (lr == NULL) return NULL;
    for (int i = 0; i < lr->taille; i++) {
        if (lr->data[i].id == id) {
            return &lr->data[i];
        }
    }
    return NULL;
}

int annuler_reservation(ListeReservations *lr, int id) {
    Reservation *r = trouver_reservation_par_id(lr, id);
    if (r == NULL) {
        printf("Reservation %d introuvable.\n", id);
        return -1;
    }
    r->statut = RES_STATUS_ANNULE;
    printf("Reservation %d annulee.\n", id);
    return 0;
}

void afficher_reservation(const Reservation *r, const ListeSalles *ls) {
    if (r == NULL || ls == NULL) return;

    Salle *salle = trouver_salle_par_id((ListeSalles *)ls, r->salle_id);

    const char *nom_salle = salle ? salle->nom : "(inconnue)";

    const char *statut_str = "EN_ATTENTE";
    if (r->statut == RES_STATUS_CONFIRME) statut_str = "CONFIRME";
    else if (r->statut == RES_STATUS_ANNULE) statut_str = "ANNULE";

    printf("Reservation ID: %d | Client: %s | Salle: %s | Date: %02d/%02d/%04d | "
           "Debut: %02d:%02d | Fin: %02d:%02d | Nb personnes: %d | Tarif: %.2f | Statut: %s\n",
           r->id,
           r->nom_client,
           nom_salle,
           r->date.jour, r->date.mois, r->date.annee,
           r->heure_debut.heure, r->heure_debut.minute,
           r->heure_fin.heure, r->heure_fin.minute,
           r->nombre_personnes,
           r->tarif_total,
           statut_str);
}

void afficher_toutes_reservations(const ListeReservations *lr, const ListeSalles *ls) {
    if (lr == NULL || ls == NULL) return;
    printf("=== Liste des reservations ===\n");
    if (lr->taille == 0) {
        printf("(Aucune reservation)\n");
        return;
    }
    for (int i = 0; i < lr->taille; i++) {
        afficher_reservation(&lr->data[i], ls);
    }
}

void rechercher_reservations_par_client(const ListeReservations *lr,
                                        const ListeSalles *ls,
                                        const char *nom_client) {
    if (lr == NULL || ls == NULL || nom_client == NULL) return;
    printf("=== Reservations pour le client '%s' ===\n", nom_client);
    int trouve = 0;
    for (int i = 0; i < lr->taille; i++) {
        if (strcmp(lr->data[i].nom_client, nom_client) == 0) {
            afficher_reservation(&lr->data[i], ls);
            trouve = 1;
        }
    }
    if (!trouve) {
        printf("(Aucune reservation pour ce client)\n");
    }
}

/* Sauvegarde des reservations en texte */
int sauvegarder_reservations(const ListeReservations *lr, const char *chemin_fichier) {
    if (lr == NULL || chemin_fichier == NULL) {
        return -1;
    }

    FILE *f = fopen(chemin_fichier, "w");
    if (f == NULL) {
        perror("Erreur ouverture fichier reservations");
        return -1;
    }

    fprintf(f, "# id;nom_client;salle_id;jour;mois;annee;h_debut;min_debut;h_fin;min_fin;nb_personnes;tarif_total;statut\n");

    for (int i = 0; i < lr->taille; i++) {
        const Reservation *r = &lr->data[i];
        fprintf(f, "%d;%s;%d;%d;%d;%d;%d;%d;%d;%d;%d;%.2f;%d\n",
                r->id,
                r->nom_client,
                r->salle_id,
                r->date.jour,
                r->date.mois,
                r->date.annee,
                r->heure_debut.heure,
                r->heure_debut.minute,
                r->heure_fin.heure,
                r->heure_fin.minute,
                r->nombre_personnes,
                r->tarif_total,
                (int)r->statut);
    }

    fclose(f);
    return 0;
}

/* Chargement des reservations en texte */
int charger_reservations(ListeReservations *lr, const char *chemin_fichier) {
    if (lr == NULL || chemin_fichier == NULL) {
        return -1;
    }

    FILE *f = fopen(chemin_fichier, "r");
    if (f == NULL) {
        perror("Erreur ouverture fichier reservations");
        return -1;
    }

    char ligne[1024];
    int nb = 0;

    while (fgets(ligne, sizeof(ligne), f) != NULL) {
        if (ligne[0] == '#' || ligne[0] == '\n') {
            continue;
        }

        Reservation r;
        memset(&r, 0, sizeof(Reservation));

        char *token;
        char *rest = ligne;

        /* id */
        token = strtok(rest, ";");
        if (token == NULL) continue;
        r.id = atoi(token);

        /* nom_client */
        token = strtok(NULL, ";");
        if (token == NULL) continue;
        strncpy(r.nom_client, token, MAX_NOM_CLIENT - 1);
        r.nom_client[MAX_NOM_CLIENT - 1] = '\0';

        /* salle_id */
        token = strtok(NULL, ";");
        if (token == NULL) continue;
        r.salle_id = atoi(token);

        /* jour */
        token = strtok(NULL, ";");
        if (token == NULL) continue;
        r.date.jour = atoi(token);

        /* mois */
        token = strtok(NULL, ";");
        if (token == NULL) continue;
        r.date.mois = atoi(token);

        /* annee */
        token = strtok(NULL, ";");
        if (token == NULL) continue;
        r.date.annee = atoi(token);

        /* h_debut */
        token = strtok(NULL, ";");
        if (token == NULL) continue;
        r.heure_debut.heure = atoi(token);

        /* min_debut */
        token = strtok(NULL, ";");
        if (token == NULL) continue;
        r.heure_debut.minute = atoi(token);

        /* h_fin */
        token = strtok(NULL, ";");
        if (token == NULL) continue;
        r.heure_fin.heure = atoi(token);

        /* min_fin */
        token = strtok(NULL, ";");
        if (token == NULL) continue;
        r.heure_fin.minute = atoi(token);

        /* nb_personnes */
        token = strtok(NULL, ";");
        if (token == NULL) continue;
        r.nombre_personnes = atoi(token);

        /* tarif_total */
        token = strtok(NULL, ";");
        if (token == NULL) continue;
        r.tarif_total = atof(token);

        /* statut (dernier champ) */
        token = strtok(NULL, ";\n");
        if (token == NULL) {
            r.statut = RES_STATUS_CONFIRME;
        } else {
            int st = atoi(token);
            if (st < 0 || st > 2) st = 1;
            r.statut = (ReservationStatus)st;
        }

        if (lr->taille >= lr->capacite) {
            if (agrandir_liste_reservations(lr) != 0) {
                fclose(f);
                return -1;
            }
        }

        lr->data[lr->taille] = r;
        lr->taille++;
        nb++;
    }

    fclose(f);
    return nb;
}

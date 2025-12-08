#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "stats.h"

void afficher_chiffre_affaires_par_salle(const ListeReservations *lr,
                                         const ListeSalles *ls) {
    if (lr == NULL || ls == NULL) return;
    if (ls->taille == 0) {
        printf("Aucune salle.\n");
        return;
    }

    double *ca = calloc((size_t)ls->taille, sizeof(double));
    if (ca == NULL) {
        printf("Erreur memoire.\n");
        return;
    }

    for (int i = 0; i < lr->taille; i++) {
        const Reservation *r = &lr->data[i];
        if (r->statut == RES_STATUS_ANNULE) continue;

        Salle *salle = trouver_salle_par_id((ListeSalles *)ls, r->salle_id);
        if (salle == NULL) continue;

        int idx = (int)(salle - ls->data); 
        if (idx >= 0 && idx < ls->taille) {
            ca[idx] += r->tarif_total;
        }
    }

    printf("=== Chiffre d'affaires par salle ===\n");
    for (int i = 0; i < ls->taille; i++) {
        printf("Salle %-15s : %.2f\n", ls->data[i].nom, ca[i]);
    }

    free(ca);
};

void afficher_stats_par_mois(const ListeReservations *lr) {
    if (lr == NULL) return;

    int nb_par_mois[12] = {0};

    for (int i = 0; i < lr->taille; i++) {
        const Reservation *r = &lr->data[i];
        if (r->statut == RES_STATUS_ANNULE) continue;
        if (r->date.mois < 1 || r->date.mois > 12) continue;
        nb_par_mois[r->date.mois - 1]++;
    };

    printf("=== Nombre de reservations par mois (toutes annees, hors annulees) ===\n");
    for (int m = 0; m < 12; m++) {
        if (nb_par_mois[m] > 0) {
            printf("Mois %2d : %d reservation(s)\n", m + 1, nb_par_mois[m]);
        };
    };
};

void afficher_salles_populaires(const ListeReservations *lr,
                                const ListeSalles *ls) {
    if (lr == NULL || ls == NULL) return;
    if (ls->taille == 0) {
        printf("Aucune salle.\n");
        return;
    };

    int *compte = calloc((size_t)ls->taille, sizeof(int));
    if (compte == NULL) {
        printf("Erreur memoire.\n");
        return;
    };

    for (int i = 0; i < lr->taille; i++) {
        const Reservation *r = &lr->data[i];
        if (r->statut == RES_STATUS_ANNULE) continue;

        Salle *salle = trouver_salle_par_id((ListeSalles *)ls, r->salle_id);
        if (salle == NULL) continue;

        int idx = (int)(salle - ls->data);
        if (idx >= 0 && idx < ls->taille) {
            compte[idx]++;
        };
    };
int *indices = malloc((size_t)ls->taille * sizeof(int));
    if (indices == NULL) {
        free(compte);
        printf("Erreur memoire.\n");
        return;
    };

    for (int i = 0; i < ls->taille; i++) {
        indices[i] = i;
    };
 for (int i = 0; i < ls->taille - 1; i++) {
        int max_idx = i;
        for (int j = i + 1; j < ls->taille; j++) {
            if (compte[indices[j]] > compte[indices[max_idx]]) {
                max_idx = j;
            };
        };
        int tmp = indices[i];
        indices[i] = indices[max_idx];
        indices[max_idx] = tmp;
    };

    printf("=== Salles les plus populaires (hors annulees) ===\n");
    for (int k = 0; k < ls->taille; k++) {
        int idx = indices[k];
        if (compte[idx] > 0) {
            printf("Salle %-15s : %d reservation(s)\n",
                   ls->data[idx].nom,
                   compte[idx]);
        };
    };

    free(compte);
    free(indices);
};
#include <stdio.h>
#include <stdlib.h>
#include "persistence.h"



int charger_donnees(ListeSalles *ls,
                    ListeReservations *lr,
                    const char *fichier_salles,
                    const char *fichier_reservations) {
    if (ls == NULL || lr == NULL || fichier_salles == NULL || fichier_reservations == NULL) {
        return -1;
    }

    int nb_salles = charger_salles(ls, fichier_salles);
    if (nb_salles < 0) {
        printf("Erreur : impossible de charger les salles depuis '%s'.\n", fichier_salles);
        return -1;
    }

    FILE *test = fopen(fichier_reservations, "r");
    if (test == NULL) {
        printf("Aucune reservation existante (fichier '%s' absent).\n", fichier_reservations);
        return 0;
    }
    fclose(test);

    int nb_res = charger_reservations(lr, fichier_reservations);
    if (nb_res < 0) {
        printf("Erreur : probleme lors du chargement des reservations.\n");
        return -1;
    }

    printf("%d salle(s) et %d reservation(s) chargees.\n", nb_salles, nb_res);
    return 0;
}

int sauvegarder_donnees(const ListeSalles *ls,
                        const ListeReservations *lr,
                        const char *fichier_salles,
                        const char *fichier_reservations) {
    if (ls == NULL || lr == NULL || fichier_salles == NULL || fichier_reservations == NULL) {
        return -1;
    }

    if (sauvegarder_salles(ls, fichier_salles) != 0) {
        printf("Erreur lors de la sauvegarde des salles.\n");
        return -1;
    }

    if (sauvegarder_reservations(lr, fichier_reservations) != 0) {
        printf("Erreur lors de la sauvegarde des reservations.\n");
        return -1;
    }

    return 0;
}

int exporter_reservations_csv(const ListeReservations *lr,
                              const ListeSalles *ls,
                              const char *chemin_fichier_csv) {
    if (lr == NULL || ls == NULL || chemin_fichier_csv == NULL) {
        return -1;
    }

    FILE *f = fopen(chemin_fichier_csv, "w");
    if (f == NULL) {
        perror("Erreur ouverture fichier CSV");
        return -1;
    }

    fprintf(f,
            "id_reservation;nom_client;nom_salle;date;heure_debut;heure_fin;"
            "nombre_personnes;tarif_total;statut\n");

    for (int i = 0; i < lr->taille; i++) {
        const Reservation *r = &lr->data[i];
        Salle *salle = trouver_salle_par_id((ListeSalles *)ls, r->salle_id);
        const char *nom_salle = salle ? salle->nom : "(inconnue)";

        const char *statut_str = "EN_ATTENTE";
        if (r->statut == RES_STATUS_CONFIRME) statut_str = "CONFIRME";
        else if (r->statut == RES_STATUS_ANNULE) statut_str = "ANNULE";

        fprintf(f,
                "%d;%s;%s;%02d/%02d/%04d;%02d:%02d;%02d:%02d;%d;%.2f;%s\n",
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

    fclose(f);
    return 0;
}

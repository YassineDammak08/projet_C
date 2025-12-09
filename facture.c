#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "facture.h"

int generer_facture(const Reservation *r,
                    const Salle *salle,
                    const char *dossier_factures) {
    if (r == NULL || salle == NULL || dossier_factures == NULL) {
        return -1;
    };

    if (r->statut == RES_STATUS_ANNULE) {
        printf("Impossible de generer une facture pour une reservation annulee.\n");
        return -1;
    };

    if (r->statut != RES_STATUS_CONFIRME) {
        printf("Attention : la reservation n'est pas CONFIRMEE.\n");
    };
char chemin[512];
snprintf(chemin, sizeof(chemin), "%s/facture_%d.txt", dossier_factures, r->id);
FILE *f = fopen(chemin, "w");
if (f == NULL) {
    perror("Erreur ouverture fichier facture");
    return -1;
};
double calculer_duree_heures(const Heure *debut, const Heure *fin) {
    if (debut == NULL || fin == NULL) {
        return -1;
    }

    int md = heure_en_minutes(debut);
    int mf = heure_en_minutes(fin);
    int diff = mf - md;

    if (diff <= 0) {
        return -1;
    };

    return diff / 60.0;
};


fprintf(f, "================ FACTURE =================\n\n");
fprintf(f, "Reservation ID : %d\n", r->id);
    fprintf(f, "Client         : %s\n\n", r->nom_client);

    fprintf(f, "Salle          : %s\n", salle->nom);
    fprintf(f, "Capacite       : %d personnes\n", salle->capacite);
    fprintf(f, "Tarif horaire  : %.2f\n\n", salle->tarif_horaire);

    fprintf(f, "Date           : %02d/%02d/%04d\n",
            r->date.jour, r->date.mois, r->date.annee);
    fprintf(f, "Heure debut    : %02d:%02d\n",
            r->heure_debut.heure, r->heure_debut.minute);
    fprintf(f, "Heure fin      : %02d:%02d\n",
            r->heure_fin.heure, r->heure_fin.minute);

    double duree = calculer_duree_heures(&r->heure_debut, &r->heure_fin);
    fprintf(f, "Duree          : %.2f heure(s)\n", duree);

    fprintf(f, "Nombre pers.   : %d\n\n", r->nombre_personnes);

    fprintf(f, "Montant total  : %.2f\n", r->tarif_total);
    fprintf(f, "==========================================\n");

    fclose(f);

    printf("Facture generee : %s\n", chemin);
    return 0;
};
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "persistence.h"

int charger_donnees(ListeSalles *ls,
                    ListeReservations *lr,
                    const char *fichier_salles,
                    const char *fichier_reservations) {
    if (ls == NULL || lr == NULL || fichier_salles == NULL || fichier_reservations == NULL) return -1;
    };

    int nb_salles = charger_salles(ls, fichier_salles);
    if (nb_salles < 0) {
        printf("Erreur : impossible de charger les salles depuis '%s'.\n", fichier_salles);
        return -1;
    };
    FILE *test = fopen(fichier_reservations, "r");
    if (test == NULL) {
        printf("Aucune reservation existante (fichier '%s' absent).\n", fichier_reservations);
        return 0;
    };
    fclose(test); 

    
    int nb_res = charger_reservations(lr, fichier_reservations);
    if (nb_res < 0) {
        printf("Erreur : probleme lors du chargement des reservations.\n");
        return -1;
    };

    printf("%d salle(s) et %d reservation(s) chargees.\n", nb_salles, nb_res);
    return 0;
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
};

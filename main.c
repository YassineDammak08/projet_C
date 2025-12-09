#include <stdio.h>
#include "types.h"
#include "utils.h"
#include "salle.h"
#include "reservation.h"
#include "menu.h"
#include "persistence.h"

int main(void) {
    ListeSalles ls;
    ListeReservations lr;

    init_liste_salles(&ls);
    init_liste_reservations(&lr);

    const char *fichier_salles       = "data/salles.cfg";
    const char *fichier_reservations = "data/reservations.dat";
    const char *dossier_factures     = "data/factures";

    printf("Chargement des donnees...\n");
    if (charger_donnees(&ls, &lr, fichier_salles, fichier_reservations) != 0) {
        printf("Attention : certaines donnees n'ont pas pu etre chargees.\n");
    }

    menu_principal(&ls, &lr, fichier_salles, fichier_reservations, dossier_factures);

    liberer_liste_salles(&ls);
    liberer_liste_reservations(&lr);

    return 0;
}

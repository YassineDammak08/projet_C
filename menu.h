#ifndef MENU_H
#define MENU_H

#include "salle.h"
#include "reservation.h"
void menu_principal(ListeSalles *ls,
                    ListeReservations *lr,
                    const char *chemin_salles,
                    const char *chemin_reservations,
                    const char *chemin_dossier_factures);

#endif
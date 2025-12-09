#ifndef PERSISTENCE_H
#define PERSISTENCE_H

#include "salle.h"
#include "reservation.h"


int charger_donnees(ListeSalles *ls,
                    ListeReservations *lr,
                    const char *fichier_salles,
                    const char *fichier_reservations);


int sauvegarder_donnees(const ListeSalles *ls,
                        const ListeReservations *lr,
                        const char *fichier_salles,
                        const char *fichier_reservations);

int exporter_reservations_csv(const ListeReservations *lr,
                              const ListeSalles *ls,
                              const char *chemin_fichier_csv);

#endif

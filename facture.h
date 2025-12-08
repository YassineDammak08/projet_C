#ifndef FACTURE_H
#define FACTURE_H

#include "reservation.h"
#include "salle.h"


int generer_facture(const Reservation *r,
                    const Salle *salle,
                    const char *dossier_factures);

#endif

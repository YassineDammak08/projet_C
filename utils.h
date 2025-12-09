#ifndef UTILS_H
#define UTILS_H

#include <stddef.h>
#include "types.h"


int lire_chaine(char *buffer, size_t taille);


int lire_entier(int *val);


int lire_double(double *val);


int saisir_date(Date *d);


int saisir_heure(Heure *h);

int heure_en_minutes(const Heure *h);


double calculer_duree_heures(const Heure *debut, const Heure *fin);

int comparer_date(const Date *a, const Date *b);

void pause_console(void);

#endif 
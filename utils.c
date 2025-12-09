#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "utils.h"

int lire_chaine(char *buffer, size_t taille) {
    if (buffer == NULL || taille == 0) {
        return -1;
    }

    if (fgets(buffer, (int)taille, stdin) == NULL) {
        return -1;
    }

    size_t len = strlen(buffer);
    if (len > 0 && buffer[len - 1] == '\n') {
        buffer[len - 1] = '\0';
    }

    return 0;
}

int lire_entier(int *val) {
    char buffer[128];
    char *endptr = NULL;

    if (val == NULL) {
        return -1;
    }

    if (lire_chaine(buffer, sizeof(buffer)) != 0) {
        return -1;
    }

    char *ptr = buffer;
    while (isspace((unsigned char)*ptr)) {
        ptr++;
    }

    if (*ptr == '\0') {
        return -1;
    }

    long tmp = strtol(ptr, &endptr, 10);
    if (endptr == ptr) {
        return -1;
    }

    while (isspace((unsigned char)*endptr)) {
        endptr++;
    }

    if (*endptr != '\0') {
        return -1;
    }

    *val = (int)tmp;
    return 0;
}

int lire_double(double *val) {
    char buffer[128];
    char *endptr = NULL;

    if (val == NULL) {
        return -1;
    }

    if (lire_chaine(buffer, sizeof(buffer)) != 0) {
        return -1;
    }

    char *ptr = buffer;
    while (isspace((unsigned char)*ptr)) {
        ptr++;
    }

    if (*ptr == '\0') {
        return -1;
    }

    double tmp = strtod(ptr, &endptr);
    if (endptr == ptr) {
        return -1;
    }

    while (isspace((unsigned char)*endptr)) {
        endptr++;
    }

    if (*endptr != '\0') {
        return -1;
    }

    *val = tmp;
    return 0;
}

int saisir_date(Date *d) {
    if (d == NULL) {
        return -1;
    }

    int jour, mois, annee;

    printf("  Jour (1-31) : ");
    if (lire_entier(&jour) != 0) {
        return -1;
    }

    printf("  Mois (1-12) : ");
    if (lire_entier(&mois) != 0) {
        return -1;
    }

    printf("  Annee (ex: 2025) : ");
    if (lire_entier(&annee) != 0) {
        return -1;
    }

    if (jour < 1 || jour > 31 || mois < 1 || mois > 12 || annee < 1900) {
        printf("  Date invalide.\n");
        return -1;
    }

    d->jour = jour;
    d->mois = mois;
    d->annee = annee;

    return 0;
}

int saisir_heure(Heure *h) {
    if (h == NULL) {
        return -1;
    }

    char buffer[64];
    printf("  Heure (HH:MM) : ");
    if (lire_chaine(buffer, sizeof(buffer)) != 0) {
        return -1;
    }

    int hh, mm;
    if (sscanf(buffer, "%d:%d", &hh, &mm) != 2) {
        printf("  Format invalide. Utiliser HH:MM.\n");
        return -1;
    }

    if (hh < 0 || hh > 23 || mm < 0 || mm > 59) {
        printf("  Heure invalide.\n");
        return -1;
    }

    h->heure = hh;
    h->minute = mm;

    return 0;
}

int heure_en_minutes(const Heure *h) {
    if (h == NULL) {
        return -1;
    }
    return h->heure * 60 + h->minute;
}

double calculer_duree_heures(const Heure *debut, const Heure *fin) {
    if (debut == NULL || fin == NULL) {
        return -1.0;
    }

    int md = heure_en_minutes(debut);
    int mf = heure_en_minutes(fin);
    int diff = mf - md;

    if (diff <= 0) {
        return -1.0;
    }

    return diff / 60.0;
}

int comparer_date(const Date *a, const Date *b) {
    if (a == NULL || b == NULL) {
        return 0;
    }

    if (a->annee < b->annee) return -1;
    if (a->annee > b->annee) return 1;

    if (a->mois < b->mois) return -1;
    if (a->mois > b->mois) return 1;

    if (a->jour < b->jour) return -1;
    if (a->jour > b->jour) return 1;

    return 0;
}

void pause_console(void) {
    printf("Appuyez sur Entree pour continuer...");
    fflush(stdout);
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {
    }
};
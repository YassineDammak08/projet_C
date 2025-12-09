#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "salle.h"

int agrandir_liste_salles(ListeSalles *ls) {
    int nouvelle_capacite = (ls->capacite == 0) ? 4 : ls->capacite * 2;
    Salle *nouveau = realloc(ls->data, (size_t)nouvelle_capacite * sizeof(Salle));
    if (nouveau == NULL) {
        return -1;
    }
    ls->data = nouveau;
    ls->capacite = nouvelle_capacite;
    return 0;
}

void init_liste_salles(ListeSalles *ls) {
    if (ls == NULL) return;
    ls->data = NULL;
    ls->taille = 0;
    ls->capacite = 0;
}

void liberer_liste_salles(ListeSalles *ls) {
    if (ls == NULL) return;
    free(ls->data);
    ls->data = NULL;
    ls->taille = 0;
    ls->capacite = 0;
}

int ajouter_salle(ListeSalles *ls, const Salle *s) {
    if (ls == NULL || s == NULL) return -1;
    if (ls->taille >= ls->capacite) {
        if (agrandir_liste_salles(ls) != 0) {
            return -1;
        }
    }
    ls->data[ls->taille] = *s;
    ls->taille++;
    return 0;
}

int charger_salles(ListeSalles *ls, const char *chemin_fichier) {
    if (ls == NULL || chemin_fichier == NULL) {
        return -1;
    }

    FILE *f = fopen(chemin_fichier, "r");
    if (f == NULL) {
        perror("Erreur ouverture fichier salles");
        return -1;
    }

    char ligne[512];
    int nb = 0;

    while (fgets(ligne, sizeof(ligne), f) != NULL) {
        if (ligne[0] == '#' || ligne[0] == '\n') {
            continue;
        }

        Salle s;
        memset(&s, 0, sizeof(Salle));

        char *token;
        char *rest = ligne;

        token = strtok(rest, ";");
        if (token == NULL) continue;
        s.id = atoi(token);

        token = strtok(NULL, ";");
        if (token == NULL) continue;
        strncpy(s.nom, token, MAX_NOM_SALLE - 1);
        s.nom[MAX_NOM_SALLE - 1] = '\0';

        token = strtok(NULL, ";");
        if (token == NULL) continue;
        s.capacite = atoi(token);

        token = strtok(NULL, ";");
        if (token == NULL) continue;
        s.tarif_horaire = atof(token);

        token = strtok(NULL, ";\n");
        if (token == NULL) {
            s.equipements[0] = '\0';
        } else {
            strncpy(s.equipements, token, MAX_EQUIPEMENTS - 1);
            s.equipements[MAX_EQUIPEMENTS - 1] = '\0';
        }

        if (ajouter_salle(ls, &s) != 0) {
            fclose(f);
            return -1;
        }
        nb++;
    }

    fclose(f);
    return nb;
}

int sauvegarder_salles(const ListeSalles *ls, const char *chemin_fichier) {
    if (ls == NULL || chemin_fichier == NULL) {
        return -1;
    }

    FILE *f = fopen(chemin_fichier, "w");
    if (f == NULL) {
        perror("Erreur ouverture fichier sauvegarde salles");
        return -1;
    }

    fprintf(f, "# id;nom;capacite;tarif_horaire;equipements\n");

    for (int i = 0; i < ls->taille; i++) {
        const Salle *s = &ls->data[i];
        fprintf(f, "%d;%s;%d;%.2f;%s\n",
                s->id,
                s->nom,
                s->capacite,
                s->tarif_horaire,
                s->equipements);
    }

    fclose(f);
    return 0;
}

Salle* trouver_salle_par_id(ListeSalles *ls, int id) {
    if (ls == NULL) return NULL;
    for (int i = 0; i < ls->taille; i++) {
        if (ls->data[i].id == id) {
            return &ls->data[i];
        }
    }
    return NULL;
}

Salle* trouver_salle_par_nom(ListeSalles *ls, const char *nom) {
    if (ls == NULL || nom == NULL) return NULL;
    for (int i = 0; i < ls->taille; i++) {
        if (strcmp(ls->data[i].nom, nom) == 0) {
            return &ls->data[i];
        }
    }
    return NULL;
}

void afficher_salles(const ListeSalles *ls) {
    if (ls == NULL) return;
    printf("=== Liste des salles ===\n");
    for (int i = 0; i < ls->taille; i++) {
        const Salle *s = &ls->data[i];
        printf("ID: %d | Nom: %s | Capacite: %d | Tarif horaire: %.2f | Equipements: %s\n",
               s->id, s->nom, s->capacite, s->tarif_horaire, s->equipements);
    }
}

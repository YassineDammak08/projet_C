#ifndef SALLE_H
#define SALLE_H

#include "types.h"

typedef struct {
    int   id;
    char  nom[MAX_NOM_SALLE];
    int   capacite;
    double tarif_horaire;
    char  equipements[MAX_EQUIPEMENTS];
} Salle;

typedef struct {
    Salle *data;
    int    taille;    
    int    capacite;  
} ListeSalles;


void init_liste_salles(ListeSalles *ls);
void liberer_liste_salles(ListeSalles *ls);


int charger_salles(ListeSalles *ls, const char *chemin_fichier);


int sauvegarder_salles(const ListeSalles *ls, const char *chemin_fichier);


Salle* trouver_salle_par_id(ListeSalles *ls, int id);
Salle* trouver_salle_par_nom(ListeSalles *ls, const char *nom);


void afficher_salles(const ListeSalles *ls);

#endif 

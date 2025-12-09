#include <stdio.h>
#include <string.h>
#include "menu.h"
#include "utils.h"
#include "persistence.h"
#include "facture.h"
#include "stats.h"

void ajouter_reservation_interactive(ListeSalles *ls, ListeReservations *lr) {
    if (ls->taille == 0) {
        printf("Aucune salle chargee. Impossible d'ajouter une reservation.\n");
        return;
    }

    printf("=== Nouvelle reservation ===\n");
    char nom_client[MAX_NOM_CLIENT];

    printf("Nom du client : ");
    if (lire_chaine(nom_client, sizeof(nom_client)) != 0) {
        printf("Erreur de lecture du nom.\n");
        return;
    }

    printf("Choisissez l'ID de la salle parmi la liste suivante :\n");
    afficher_salles(ls);
    printf("ID de la salle : ");
    int salle_id;
    if (lire_entier(&salle_id) != 0) {
        printf("Erreur de lecture de l'ID.\n");
        return;
    }

    Salle *salle = trouver_salle_par_id(ls, salle_id);
    if (salle == NULL) {
        printf("Salle introuvable.\n");
        return;
    }

    Date date;
    printf("Saisir la date de reservation :\n");
    if (saisir_date(&date) != 0) {
        printf("Erreur de saisie de la date.\n");
        return;
    }

    Heure h_debut, h_fin;
    printf("Saisir l'heure de debut :\n");
    if (saisir_heure(&h_debut) != 0) {
        printf("Erreur de saisie de l'heure de debut.\n");
        return;
    }

    printf("Saisir l'heure de fin :\n");
    if (saisir_heure(&h_fin) != 0) {
        printf("Erreur de saisie de l'heure de fin.\n");
        return;
    }

    printf("Nombre de personnes : ");
    int nb_personnes;
    if (lire_entier(&nb_personnes) != 0) {
        printf("Erreur de lecture du nombre de personnes.\n");
        return;
    }

    int id = ajouter_reservation(lr, ls, nom_client, salle_id,
                                 date, h_debut, h_fin, nb_personnes,
                                 RES_STATUS_CONFIRME);
    if (id < 0) {
        printf("Echec de la creation de la reservation.\n");
    } else {
        printf("Reservation creee avec succes. ID = %d\n", id);
    }
}

void annuler_reservation_interactive(ListeReservations *lr) {
    printf("ID de la reservation a annuler : ");
    int id;
    if (lire_entier(&id) != 0) {
        printf("Erreur de lecture de l'ID.\n");
        return;
    }
    annuler_reservation(lr, id);
}

void rechercher_par_client_interactive(ListeReservations *lr, ListeSalles *ls) {
    char nom_client[MAX_NOM_CLIENT];
    printf("Nom du client : ");
    if (lire_chaine(nom_client, sizeof(nom_client)) != 0) {
        printf("Erreur de lecture du nom.\n");
        return;
    }
    rechercher_reservations_par_client(lr, ls, nom_client);
}

void generer_facture_interactive(ListeSalles *ls,
                                        ListeReservations *lr,
                                        const char *dossier_factures) {
    printf("ID de la reservation pour la facture : ");
    int id;
    if (lire_entier(&id) != 0) {
        printf("Erreur de lecture de l'ID.\n");
        return;
    }

    Reservation *r = trouver_reservation_par_id(lr, id);
    if (r == NULL) {
        printf("Reservation introuvable.\n");
        return;
    }

    Salle *salle = trouver_salle_par_id(ls, r->salle_id);
    if (salle == NULL) {
        printf("Salle associee introuvable.\n");
        return;
    }

    if (generer_facture(r, salle, dossier_factures) != 0) {
        printf("Erreur lors de la generation de la facture.\n");
    }
}

void afficher_stats_menu(const ListeReservations *lr,
                                const ListeSalles *ls) {
    int choix = -1;
    while (1) {
        printf("\n=== Statistiques ===\n");
        printf("1. Chiffre d'affaires par salle\n");
        printf("2. Nombre de reservations par mois\n");
        printf("3. Salles les plus populaires\n");
        printf("0. Retour au menu principal\n");
        printf("Votre choix : ");
        if (lire_entier(&choix) != 0) {
            printf("Entree invalide.\n");
            continue;
        }
        switch (choix) {
            case 1:
                afficher_chiffre_affaires_par_salle(lr, ls);
                pause_console();
                break;
            case 2:
                afficher_stats_par_mois(lr);
                pause_console();
                break;
            case 3:
                afficher_salles_populaires(lr, ls);
                pause_console();
                break;
            case 0:
                return;
            default:
                printf("Choix invalide.\n");
        }
    }
}

void menu_principal(ListeSalles *ls,
                    ListeReservations *lr,
                    const char *chemin_salles,
                    const char *chemin_reservations,
                    const char *chemin_dossier_factures) {
    int choix = -1;

    while (1) {
        printf("\n=== Menu principal ===\n");
        printf("1. Afficher les salles\n");
        printf("2. Afficher les reservations\n");
        printf("3. Ajouter une reservation\n");
        printf("4. Annuler une reservation\n");
        printf("5. Rechercher les reservations d'un client\n");
        printf("6. Generer une facture pour une reservation\n");
        printf("7. Afficher les statistiques\n");
        printf("8. Exporter les reservations en CSV\n");
        printf("9. Sauvegarder les donnees\n");
        printf("0. Quitter\n");
        printf("Votre choix : ");

        if (lire_entier(&choix) != 0) {
            printf("Entree invalide.\n");
            continue;
        }

        switch (choix) {
            case 1:
                afficher_salles(ls);
                pause_console();
                break;
            case 2:
                afficher_toutes_reservations(lr, ls);
                pause_console();
                break;
            case 3:
                /* AJOUT + SAUVEGARDE AUTO */
                ajouter_reservation_interactive(ls, lr);
                if (sauvegarder_donnees(ls, lr, chemin_salles, chemin_reservations) == 0) {
                    printf("Donnees sauvegardees automatiquement.\n");
                } else {
                    printf("Erreur lors de la sauvegarde automatique.\n");
                }
                pause_console();
                break;
            case 4:
                /* ANNULATION + SAUVEGARDE AUTO */
                annuler_reservation_interactive(lr);
                if (sauvegarder_donnees(ls, lr, chemin_salles, chemin_reservations) == 0) {
                    printf("Donnees sauvegardees automatiquement.\n");
                } else {
                    printf("Erreur lors de la sauvegarde automatique.\n");
                }
                pause_console();
                break;
            case 5:
                rechercher_par_client_interactive(lr, ls);
                pause_console();
                break;
            case 6:
                generer_facture_interactive(ls, lr, chemin_dossier_factures);
                pause_console();
                break;
            case 7:
                afficher_stats_menu(lr, ls);
                break;
            case 8: {
                const char *csv = "data/reservations_export.csv";
                if (exporter_reservations_csv(lr, ls, csv) == 0) {
                    printf("Reservations exportees dans '%s'.\n", csv);
                } else {
                    printf("Erreur lors de l'export CSV.\n");
                }
                pause_console();
                break;
            }
            case 9:
                if (sauvegarder_donnees(ls, lr, chemin_salles, chemin_reservations) == 0) {
                    printf("Donnees sauvegardees.\n");
                } else {
                    printf("Erreur lors de la sauvegarde.\n");
                }
                pause_console();
                break;
            case 0:
                printf("Sauvegarde finale des donnees...\n");
                sauvegarder_donnees(ls, lr, chemin_salles, chemin_reservations);
                printf("Au revoir.\n");
                return;
            default:
                printf("Choix invalide.\n");
                break;
        }
    }
}

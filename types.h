#ifndef TYPES_H
#define TYPES_H

#define MAX_NOM_CLIENT     100
#define MAX_NOM_SALLE      50
#define MAX_EQUIPEMENTS    256


typedef struct {
    int jour;
    int mois;
    int annee;
} Date;


typedef struct {
    int heure;   
    int minute;  
} Heure;


typedef enum {
    RES_STATUS_EN_ATTENTE = 0,
    RES_STATUS_CONFIRME   = 1,
    RES_STATUS_ANNULE     = 2
} ReservationStatus;

#endif 

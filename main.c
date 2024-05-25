#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <float.h>

typedef struct{
    int jour;
    int mois;
    int annee;
} Date;

typedef struct{
    int minute;
    int seconde;
    int ms;
} Temps;

typedef struct {
    Date date;  // Format AAAA-MM-JJ
    char epreuve[100];  // Type d'épreuve (100m, 400m, etc.)
    Temps temps;  // Temps réalisé
    int position_relais;  // Position dans le relais, -1 si non applicable
} Performance;

typedef struct {
    char nom[50];
    Performance performances[100]; // Tableau des performances, taille arbitraire
    int nbPerformances;
} Athlete;

// Définition d'une structure pour stocker et afficher les statistiques d'une épreuve
typedef struct {
    char epreuve[20];
    float meilleurTemps;
    float pireTemps;
    float tempsMoyen;
} StatistiquesEpreuve;

void typeEpreuves(){
    printf("1. 100m\n");
    printf("2. 400m\n");
    printf("3. 5000m\n");
    printf("4. marathon\n");
    printf("5. relais\n");
}

FILE *modifierAthlete(int choix){
    int numAthlete;
    char nomFicher[100];
    char nomAthlete[100];

    FILE *Athlete = fopen("/workspaces/PROJET-JO-/Athlete.txt", "r");

    // Lire chaque ligne du fichier
    while (fgets(nomAthlete, sizeof(nomAthlete), Athlete)) {
        sscanf(nomAthlete, "%d", &numAthlete);
        nomAthlete[strcspn(nomAthlete, "\n")] = 0;

        if(numAthlete == choix) {
            //printf("%s :\n", nomAthlete + 2);
            break;
        }
    }

    sprintf(nomFicher, "/workspaces/PROJET-JO-/%s.txt", nomAthlete + 2);

    FILE *file = fopen(nomFicher, "r+");
    if(file == NULL){
        printf("Erreur ouverture fichier\n");
    }

    return file;
}

FILE *ouvrirAthlete(int choix){
    int numAthlete;
    char nomFicher[100];
    char nomAthlete[100];

    FILE *Athlete = fopen("/workspaces/PROJET-JO-/Athlete.txt", "r");

    // Lire chaque ligne du fichier
    while (fgets(nomAthlete, sizeof(nomAthlete), Athlete)) {
        sscanf(nomAthlete, "%d", &numAthlete);
        nomAthlete[strcspn(nomAthlete, "\n")] = 0;

        if(numAthlete == choix) {
            //printf("%s :\n", nomAthlete + 2);
            break;
        }
    }

    sprintf(nomFicher, "/workspaces/PROJET-JO-/%s.txt", nomAthlete + 2);

    FILE *file = fopen(nomFicher, "r");

    return file;
}

int ligne(FILE *file){
    int c;
    int lignes = 0;

    while((c = fgetc(file)) != EOF){
        if (c == '\n'){
            lignes++;
        }
    }
    return lignes;
}

void afficherNomEpreuve(FILE *file){
    char nomEpreuve[100];

    fseek(file, 2, SEEK_CUR);
    fgets(nomEpreuve, sizeof(nomEpreuve), file);
    nomEpreuve[strcspn(nomEpreuve, "\n")] = 0;
    printf("%s\n", nomEpreuve);
}

void ListeEpreuve(FILE *file){
    int lignes = ligne(file);
    rewind(file);
    printf("\n");

    for(int i=0; i<lignes; i++){
        printf("%d. ", i+1);
        afficherNomEpreuve(file);
    }
}

void afficherNomAthlete(FILE *file){
    char nomAthlete[100];

    fseek(file, 2, SEEK_CUR);
    fgets(nomAthlete, sizeof(nomAthlete), file);
    nomAthlete[strcspn(nomAthlete, "\n")] = 0;
    printf("%s\n", nomAthlete);
}

void ListeAthlete(FILE *file){
    int lignes = ligne(file);
    rewind(file);
    printf("\n");

    for(int i=0; i<lignes; i++){
        printf("%d. ", i+1);
        afficherNomAthlete(file);
    }
}

void afficherAthlete() {
    Performance performance;
    int choix, filtre;
    char epreuveFiltre[100];
    Date dateFiltre;
    int filtrerParEpreuve = 0, filtrerParDate = 0;

    FILE *file = fopen("/workspaces/PROJET-JO-/Athlete.txt", "r");
    if (file == NULL) {
        printf("Erreur ouverture fichier\n");
        return;
    }

    ListeAthlete(file);

    printf("Choix : ");
    if (scanf("%d", &choix) != 1) {
        while (getchar() != '\n');
        printf("Entrée invalide. Veuillez entrer un nombre.\n");
        fclose(file);
        return;
    }

    printf("\n");
    FILE *Athlete = ouvrirAthlete(choix);
    if (Athlete == NULL) {
        printf("Erreur ouverture fichier\n");
        fclose(file);
        return;
    }

    printf("Voulez-vous filtrer par épreuve et/ou par date ?\n");
    printf("1. Aucun filtre\n");
    printf("2. Filtrer par épreuve\n");
    printf("3. Filtrer par date\n");
    printf("4. Filtrer par épreuve et date\n");
    printf("Choix : ");
    if (scanf("%d", &filtre) != 1) {
        while (getchar() != '\n');
        printf("Entrée invalide. Veuillez entrer un nombre.\n");
        fclose(Athlete);
        fclose(file);
        return;
    }

    if (filtre == 2 || filtre == 4) {
        printf("Entrez l'épreuve : ");
        scanf("%s", epreuveFiltre);
        filtrerParEpreuve = 1;
    }
    if (filtre == 3 || filtre == 4) {
        printf("Entrez la date (jj mm aaaa) : ");
        if (scanf("%d %d %d", &dateFiltre.jour, &dateFiltre.mois, &dateFiltre.annee) != 3) {
            while (getchar() != '\n');
            printf("Entrée invalide. Veuillez entrer des nombres.\n");
            fclose(Athlete);
            fclose(file);
            return;
        }
        filtrerParDate = 1;
    }

    while (fgetc(Athlete) != '\n');  // Sauter la première ligne

    while (fscanf(Athlete, "%d %d %d %s %d %d %d %d", &performance.date.jour, &performance.date.mois, &performance.date.annee, performance.epreuve, &performance.temps.minute, &performance.temps.seconde, &performance.temps.ms, &performance.position_relais) != EOF) {
        int afficher = 1;
        if (filtrerParEpreuve && strcmp(performance.epreuve, epreuveFiltre) != 0) {
            afficher = 0;
        }
        if (filtrerParDate && (performance.date.jour != dateFiltre.jour || performance.date.mois != dateFiltre.mois || performance.date.annee != dateFiltre.annee)) {
            afficher = 0;
        }
        if (afficher) {
            printf("Le %d/%d/%d\n", performance.date.jour, performance.date.mois, performance.date.annee);
            printf("Temps : %dmin %dsec %dms\n", performance.temps.minute, performance.temps.seconde, performance.temps.ms);
            if (strcmp(performance.epreuve, "relais") == 0) {
                printf("Position relais : %d\n", performance.position_relais);
            }
            printf("\n");
        }
    }

    fclose(Athlete);
    fclose(file);
}


void creeAthlete(){
    char prenom[50], chemin[100], ligne[100];
    int dernierNumero = 0;

    FILE *nomAthletes = fopen("/workspaces/PROJET-JO-/Athlete.txt", "r");
    if(nomAthletes == NULL){
        printf("Erreur ouverture fichier");
    }else{
        while(fgets(ligne, sizeof(ligne), nomAthletes) != NULL){
            sscanf(ligne, "%d", &dernierNumero);
        }
        fclose(nomAthletes);
    }

    printf("Quel est le prénom : ");
    scanf("%s", prenom);

    sprintf(chemin, "/workspaces/PROJET-JO-/%s.txt", prenom);

    FILE *file = fopen(chemin, "w");
    if(file == NULL){
        printf("Erreur ouverture fichier");
        return;
    }
    fprintf(file, "%s\n", prenom);
    fclose(file);

    nomAthletes = fopen("/workspaces/PROJET-JO-/Athlete.txt", "a");
    if(nomAthletes != NULL){
        fprintf(nomAthletes, "%d %s\n", dernierNumero+1, prenom);
        fclose(nomAthletes);
    }
}

void ajouterEntrainement(){
    int choixAthlete, choixEpreuve, numEpreuve, relais;
    int minute, seconde, ms;
    Date date;
    char epreuve[100];

    FILE *nomAthletes = fopen("/workspaces/PROJET-JO-/Athlete.txt", "r");
    if(nomAthletes == NULL){
        printf("Erreur ouverture fichier\n");
        return;
    }

    FILE *nomEpreuve = fopen("/workspaces/PROJET-JO-/Epreuves.txt", "r");
    if(nomEpreuve == NULL){
        printf("Erreur ouverture fichier");
        return;
    }

    ListeAthlete(nomAthletes);
    printf("Choix : ");
    if (scanf("%d", &choixAthlete) != 1) {
        while (getchar() != '\n');  
        printf("Entrée invalide. Veuillez entrer un nombre.\n");
        fclose(nomAthletes);
        fclose(nomEpreuve);
        return;
    }

    fclose(nomAthletes);

    ListeEpreuve(nomEpreuve);
    printf ("Veuillez taper le nombre associé au prénom\n");
    printf("Choix : ");
    if (scanf("%d", &choixEpreuve) != 1) {
        while (getchar() != '\n');  
        printf("Entrée invalide. Veuillez entrer un nombre.\n");
        fclose(nomEpreuve);
        return;
    }
    while(choixEpreuve < 1 || choixEpreuve > 5){
        printf("Choix invalide, veuillez réessayer.\n");
        printf("Choix : ");
        if (scanf("%d", &choixEpreuve) != 1) {
            while (getchar() != '\n');  
            printf("Entrée invalide. Veuillez entrer un nombre.\n");
            fclose(nomEpreuve);
            return;
        }
    }
    if(choixEpreuve == 5){
        printf("Position du relais : ");
        if (scanf("%d", &relais) != 1) {
            while (getchar() != '\n');  
            printf("Entrée invalide. Veuillez entrer un nombre.\n");
            fclose(nomEpreuve);
            return;
        }
    }else{
        relais = 0;
    }

    rewind(nomEpreuve);
    while(fgets(epreuve, sizeof(epreuve), nomEpreuve)){
        sscanf(epreuve, "%d", &numEpreuve);
        epreuve[strcspn(epreuve, "\n")] = 0;
        if(numEpreuve == choixEpreuve){
            break;
        }
    }

    printf("Date de l'entrainement (jj mm aaaa) : ");
    if (scanf("%d %d %d", &date.jour, &date.mois, &date.annee) != 3) {
        while (getchar() != '\n');  
        printf("Entrée invalide. Veuillez entrer des nombres.\n");
        fclose(nomEpreuve);
        return;
    }

    printf("Quel est le temps de l'athlète en minute : ");
    if (scanf("%d", &minute) != 1) {
        while (getchar() != '\n');  
        printf("Entrée invalide. Veuillez entrer un nombre.\n");
        fclose(nomEpreuve);
        return;
    }
    printf("Quel est le temps de l'athlète en seconde : ");
    if (scanf("%d", &seconde) != 1) {
        while (getchar() != '\n');  
        printf("Entrée invalide. Veuillez entrer un nombre.\n");
        fclose(nomEpreuve);
        return;
    }
    printf("Quel est le temps de l'athlète en milliseconde : ");
    if (scanf("%d", &ms) != 1) {
        while (getchar() != '\n');  
        printf("Entrée invalide. Veuillez entrer un nombre.\n");
        fclose(nomEpreuve);
        return;
    }

    FILE *modifAthlete = modifierAthlete(choixAthlete);
    if(modifAthlete == NULL){
        printf("erreur ouverture fichier\n");
        return;
    }

    int sauterLignes = 0;
    sauterLignes = ligne(modifAthlete);
    fprintf(modifAthlete, "%d %d %d %s %d %d %d %d\n", date.jour, date.mois, date.annee, epreuve + 2, minute, seconde, ms, relais);
    if(sauterLignes < 0){
        return;
    }
    
    fclose(modifAthlete);
}


void afficherPireTemps(FILE *file, char typeEpreuve[100]) {
    int pireTemps = 0;  
    Performance performance;

    rewind(file);
    while(fgetc(file) != '\n');
    while(fscanf(file, "%d %d %d %s %d %d %d %d", &performance.date.jour, &performance.date.mois, &performance.date.annee, performance.epreuve, &performance.temps.minute, &performance.temps.seconde, &performance.temps.ms, &performance.position_relais) != EOF){
        if(strcmp(performance.epreuve, typeEpreuve) == 0){
            //temps en milliseconde
            int temps = performance.temps.minute * 60000 + performance.temps.seconde * 1000 + performance.temps.ms;
            if(temps > pireTemps){
                pireTemps = temps;
            }
        }
    }

    if(pireTemps == 0) {
        printf("Pas de temps pour cette épreuve\n");
    } else {
        printf("\nPire temps : %dmin %dsec %dms\n", pireTemps / 60000, (pireTemps % 60000) / 1000, (pireTemps % 60000) % 1000);
    }

}

int afficherMoyenneTemps(FILE *file, char typeEpreuve[100]){
    int nbPerformances = 0, moyenne;
    int totalTemps = 0;
    Performance performance;

    rewind(file);
    while(fgetc(file) != '\n');
    while(fscanf(file, "%d %d %d %s %d %d %d %d", &performance.date.jour, &performance.date.mois, &performance.date.annee, performance.epreuve, &performance.temps.minute, &performance.temps.seconde, &performance.temps.ms, &performance.position_relais) != EOF){
        if(strcmp(performance.epreuve, typeEpreuve) == 0){
            int temps = performance.temps.minute * 60000 + performance.temps.seconde * 1000 + performance.temps.ms;
            totalTemps += temps;
            nbPerformances++;
        }
    }

    if(nbPerformances == 0){
        moyenne = 0;
        printf("Pas de temps pour cette épreuve\n");
    }else{
        moyenne = totalTemps / nbPerformances;
    }

    fclose(file);

    return moyenne;
}

void afficherMeilleurTemps(FILE *file, char typeEpreuve[100]){
    int meilleurTemps = 1000000000;
    Performance performance;

    rewind(file);
    while(fgetc(file) != '\n');
    while(fscanf(file, "%d %d %d %s %d %d %d %d", &performance.date.jour, &performance.date.mois, &performance.date.annee, performance.epreuve, &performance.temps.minute, &performance.temps.seconde, &performance.temps.ms, &performance.position_relais) != EOF){
        if(strcmp(performance.epreuve, typeEpreuve) == 0){
            int temps = performance.temps.minute * 60000 + performance.temps.seconde * 1000 + performance.temps.ms;            
            if(temps < meilleurTemps){
                meilleurTemps = temps;
            }
        }
    }

    if(meilleurTemps == 0){
        printf("Pas de temps pour cette épreuve\n");
    }else{
        printf("\nMeilleur temps : %dmin %dsec %dms\n", meilleurTemps / 60000, (meilleurTemps % 60000) / 1000, (meilleurTemps % 60000) % 1000);
    }

}

void perfoAthlete() {
    int choixAthlete, choixEpreuve;
    int choix;
    char Epreuve[100];

    FILE *athlete = fopen("/workspaces/PROJET-JO-/Athlete.txt", "r");
    if (athlete == NULL) {
        printf("Erreur ouverture fichier\n");
        return;
    }

    ListeAthlete(athlete);
    
    printf("Choix : ");
    if (scanf("%d", &choixAthlete) != 1) {
        while (getchar() != '\n');  
        printf("Entrée invalide. Veuillez entrer un nombre.\n");
        fclose(athlete);
        return;
    }

    FILE *epreuve = fopen("/workspaces/PROJET-JO-/Epreuves.txt", "r");
    if (epreuve == NULL) {
        printf("Erreur ouverture fichier\n");
        fclose(athlete);
        return;
    }

    ListeEpreuve(epreuve);

    printf("Choix : ");
    if (scanf("%d", &choixEpreuve) != 1) {
        while (getchar() != '\n');  
        printf("Entrée invalide. Veuillez entrer un nombre.\n");
        fclose(athlete);
        fclose(epreuve);
        return;
    }
    
    fclose(athlete);

    rewind(epreuve);
    while (fgets(Epreuve, sizeof(Epreuve), epreuve)) {
        sscanf(Epreuve, "%d", &choix);
        Epreuve[strcspn(Epreuve, "\n")] = 0;
        if (choix == choixEpreuve) {
            break;
        }
    }

    fclose(epreuve);

    FILE *file = ouvrirAthlete(choixAthlete);
    if (file == NULL) {
        printf("Erreur ouverture fichier\n");
        return;
    }

    printf("Epreuve : %s\n", Epreuve + 2);
    afficherMeilleurTemps(file, Epreuve + 2);
    afficherPireTemps(file, Epreuve + 2);

    int moyenne = afficherMoyenneTemps(file, Epreuve + 2);
    printf("\nTemps moyen : %dmin %dsec %dms\n", moyenne / 60000, (moyenne % 60000) / 1000, (moyenne % 60000) % 1000);
}

typedef struct {
    int moyenne;
    int index;
} MoyenneIndex;

int comparer(const void *a, const void *b) {
    MoyenneIndex *miA = (MoyenneIndex *)a;
    MoyenneIndex *miB = (MoyenneIndex *)b;
    if(miA->moyenne == 0) return 1;
    if(miB->moyenne == 0) return -1;
    return miA->moyenne - miB->moyenne;
}

void quiJO(void) {
    int lignes, choixEpreuve;
    FILE *nomEpreuve = fopen("/workspaces/PROJET-JO-/Epreuves.txt", "r");
    if (nomEpreuve == NULL) {
        printf("Impossible d'ouvrir le fichier nomEpreuve.txt.\n");
        exit(1);
    }

    ListeEpreuve(nomEpreuve);
    printf("Choix : ");
    if (scanf("%d", &choixEpreuve) != 1) {
        while (getchar() != '\n'); 
        printf("Entrée invalide. Veuillez entrer un nombre.\n");
        fclose(nomEpreuve);
        return;
    }
    printf("\n");

    rewind(nomEpreuve);
    lignes = ligne(nomEpreuve);

    rewind(nomEpreuve);
    char epreuve[100];
    while (fgets(epreuve, sizeof(epreuve), nomEpreuve)) {
        int numEpreuve;
        sscanf(epreuve, "%d", &numEpreuve);
        epreuve[strcspn(epreuve, "\n")] = 0;

        if (numEpreuve == choixEpreuve) {
            break;
        }
    }

    // Ouvrir le fichier de tous les athlètes
    FILE *nomAthletes = fopen("/workspaces/PROJET-JO-/Athlete.txt", "r");
    if (nomAthletes == NULL) {
        printf("Impossible d'ouvrir le fichier nomAthlètes.\n");
        fclose(nomEpreuve);
        exit(1);
    }
    lignes = ligne(nomAthletes);

    rewind(nomAthletes);
    //mettre le nom des athlètes dans un tableau
    char athletes[100][100];
    for(int i = 0; i < lignes; i++){
        fseek(nomAthletes, 2, SEEK_CUR); // Sauter le numéro de l'athlète
        fgets(athletes[i], sizeof(athletes[i]), nomAthletes);
        athletes[i][strcspn(athletes[i], "\n")] = 0;
        printf("%d. %s\n", i + 1, athletes[i]);
    }

    //mettre moyenne temps de chaque athlète dans un tableau
    int moyennes[100];
    for (int i = 0; i < lignes; i++) {
        FILE *fichierAthlete = ouvrirAthlete(i + 1);
        if (fichierAthlete != NULL) {
            moyennes[i] = afficherMoyenneTemps(fichierAthlete, epreuve + 2);
        }
    }

    //trier le tableau des moyennes
    MoyenneIndex moyennesIndex[100];
    for (int i = 0; i < lignes; i++) {
        moyennesIndex[i].moyenne = moyennes[i];
        moyennesIndex[i].index = i;
    }

    qsort(moyennesIndex, lignes, sizeof(MoyenneIndex), comparer);

    //afficher les 3 meilleurs moyennes
    printf("\nLes 3 meilleurs moyennes pour l'épreuve %s sont :\n", epreuve + 2);
    for (int i = 0; i < 3; i++) {
        printf("%d. %s avec une moyenne de %dmin %dsec %dms\n", i + 1, athletes[moyennesIndex[i].index], moyennes[moyennesIndex[i].index] / 60000, (moyennes[moyennesIndex[i].index] % 60000) / 1000, (moyennes[moyennesIndex[i].index] % 60000) % 1000);
    }

    fclose(nomAthletes);
    fclose(nomEpreuve);
}
void progressionAthlete() {
    int choixAthlete, choixEpreuve;
    Date date1, date2;
    char Epreuve[100];

    FILE *athlete = fopen("/workspaces/PROJET-JO-/Athlete.txt", "r");
    if (athlete == NULL) {
        printf("Erreur ouverture fichier Athlete.txt\n");
        return;
    }

    ListeAthlete(athlete);
    
    printf("Choix de l'athlète : ");
    if (scanf("%d", &choixAthlete) != 1) { 
        while (getchar() != '\n'); 
        printf("Choix invalide, veuillez réessayer.\n");
        fclose(athlete);
        return;
    }

    FILE *epreuve = fopen("/workspaces/PROJET-JO-/Epreuves.txt", "r");
    if (epreuve == NULL) {
        printf("Erreur ouverture fichier Epreuves.txt\n");
        fclose(athlete);
        return;
    }

    ListeEpreuve(epreuve);

    printf("Choix de l'épreuve : ");
    if (scanf("%d", &choixEpreuve) != 1) { 
        while (getchar() != '\n'); 
        printf("Choix invalide, veuillez réessayer.\n");
        fclose(athlete);
        fclose(epreuve);
        return;
    }

    rewind(epreuve);
    while (fgets(Epreuve, sizeof(Epreuve), epreuve)) {
        int choix;
        sscanf(Epreuve, "%d", &choix);
        Epreuve[strcspn(Epreuve, "\n")] = 0;
        if (choix == choixEpreuve) {
            break;
        }
    }

    fclose(epreuve);

    FILE *file = ouvrirAthlete(choixAthlete);
    if (file == NULL) {
        printf("Erreur ouverture fichier de l'athlète\n");
        fclose(athlete);
        return;
    }

    printf("Date de début (jj mm aaaa) : ");
    if (scanf("%d %d %d", &date1.jour, &date1.mois, &date1.annee) != 3) {
        while (getchar() != '\n'); 
        printf("Entrée invalide, veuillez entrer des nombres.\n");
        fclose(file);
        fclose(athlete);
        return;
    }

    printf("Date de fin (jj mm aaaa) : ");
    if (scanf("%d %d %d", &date2.jour, &date2.mois, &date2.annee) != 3) {
        while (getchar() != '\n'); 
        printf("Entrée invalide, veuillez entrer des nombres.\n");
        fclose(file);
        fclose(athlete);
        return;
    }

    int tempsDebut = -1, tempsFin = -1;
    Performance performance;

    rewind(file);
    while(fgetc(file) != '\n');  

    while(fscanf(file, "%d %d %d %s %d %d %d %d", &performance.date.jour, &performance.date.mois, &performance.date.annee, performance.epreuve, &performance.temps.minute, &performance.temps.seconde, &performance.temps.ms, &performance.position_relais) != EOF) {
        if (strcmp(performance.epreuve, Epreuve + 2) == 0) {
            int temps = performance.temps.minute * 60000 + performance.temps.seconde * 1000 + performance.temps.ms;
            if (performance.date.annee == date1.annee && performance.date.mois == date1.mois && performance.date.jour == date1.jour) {
                tempsDebut = temps;
            }
            if (performance.date.annee == date2.annee && performance.date.mois == date2.mois && performance.date.jour == date2.jour) {
                tempsFin = temps;
            }
        }
    }

    if (tempsDebut == -1 || tempsFin == -1) {
        printf("Les temps pour les dates spécifiées n'ont pas été trouvés.\n");
    } else {
        int difference = tempsFin - tempsDebut;
        printf("Progression entre %d/%d/%d et %d/%d/%d : %dmin %dsec %dms\n",
               date1.jour, date1.mois, date1.annee,
               date2.jour, date2.mois, date2.annee,
               difference / 60000, (difference % 60000) / 1000, (difference % 60000) % 1000);
    }

    fclose(file);
    fclose(athlete);
}


void statistiqueAthlete(){
    int choix;
    printf("1. Résumé des performances d’un athlète\n");
    printf("2. Qui envoyer au JO\n");
    printf("3. Quitter\n");
    printf("Votre choix : ");
    if (scanf("%d", &choix) != 1) {
        while (getchar() != '\n');  
        printf("Entrée invalide. Veuillez entrer un nombre.\n");
        return;
    }

    switch (choix) {
        case 1:
            perfoAthlete();
            break;
        case 2:
            quiJO();
            break;
        case 3:
            exit(0);
        default:
            printf("Choix invalide, veuillez réessayer.\n");
    }
}



int main() {
    int choix;

    while (1) {
        printf("\nMenu Principal:\n");
        printf("1. Entrer un nouvel athlète\n");
        printf("2. Entrer une nouvelle performance pour un athlète existant\n");
        printf("3. Afficher l'historique d'un athlète\n");
        printf("4. Afficher les statistiques d'un athlète\n");
        printf("5. Demander la progression entre deux athletes\n");
        printf("6. Quitter\n");
        printf ("Taper un nombre entre 1 et 6\n");
        printf("Votre choix : ");
         if (scanf("%d", &choix) != 1) {
            while (getchar() != '\n'); 
            printf("Entrée invalide. Veuillez entrer un nombre.\n");
            continue;
        }

        switch (choix) {
            case 1:
                creeAthlete();
                break;
            case 2:
                ajouterEntrainement();
                break;
            case 3:
                afficherAthlete();
                break;
            case 4:
                statistiqueAthlete();
                break;
            case 5:
                progressionAthlete();
                break;
            case 6:
                exit(0);
            default:
                printf("Choix invalide, veuillez réessayer.\n");
        }
    }
    return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STORAGE 1024
#define MAX_GAMES 20

#define reset "\033[0m"
#define color_game "\e[38;5;177m"
#define color_size "\e[38;5;226m"
#define color_unit "\e[38;5;215m"
#define color_menu "\e[38;5;126m"
#define color_error "\e[38;5;196m"

typedef struct
{
    char name[50];
    int size;
} Joc;

Joc disp[MAX_GAMES];
Joc inst[MAX_GAMES];
int k_disp = 0;
int k_inst = 0;
int su = 0;

void stergere()
{
    printf("\033[H\033[J");
}

void load()
{
    FILE *f = fopen("disponibile.dat", "r");
    if (!f)
    {
        printf(color_error "Eroare: Nu se poate deschide fisierul disponibile.dat!\n" reset);
        exit(1);
    }

    fscanf(f, "%d\n", &k_disp);
    for (int i = 0; i < k_disp; i++)
    {
        fgets(disp[i].name, sizeof(disp[i].name), f);
        size_t len = strlen(disp[i].name);
        if (len > 0 && disp[i].name[len - 1] == '\n')
            disp[i].name[len - 1] = '\0';
        fscanf(f, "%d\n", &disp[i].size);
    }
    fclose(f);

    f = fopen("instalate.dat", "r");
    if (f)
    {
        fscanf(f, "%d %d\n", &k_inst, &su);
        for (int i = 0; i < k_inst; i++)
        {
            fgets(inst[i].name, sizeof(inst[i].name), f);
            size_t len = strlen(inst[i].name);
            if (len > 0 && inst[i].name[len - 1] == '\n')
                inst[i].name[len - 1] = '\0';
            fscanf(f, "%d\n", &inst[i].size);
        }
        fclose(f);
    }
}

void save()
{
    FILE *f = fopen("instalate.dat", "w");
    if (!f)
    {
        printf(color_error "Eroare: Nu se poate deschide fisierul instalate.dat!\n" reset);
        return;
    }

    fprintf(f, "%d %d\n", k_inst, su);
    for (int i = 0; i < k_inst; i++)
        fprintf(f, "%s\n%d\n", inst[i].name, inst[i].size);

    fclose(f);
}

void instalate()
{
    stergere();
    printf("===== " color_menu "JOCURI INSTALATE" reset " =====\n");
    if (k_inst == 0)
        printf("Nu sunt jocuri instalate!\n");
    else
    {
        for (int i = 0; i < k_inst; i++)
        {
            printf("%d. " color_game "%s" reset " - " color_size "%d " color_unit "GB" reset "\n",
                   i + 1, inst[i].name, inst[i].size);
        }
    }
    printf("Memorie utilizata: " color_size "%d" reset " / %d GB\n", su, STORAGE);
    printf("Apasati ENTER pentru a continua...");
    getchar();
}

void disponibile()
{
    stergere();
    printf("===== " color_menu "JOCURI DISPONIBILE" reset " =====\n");
    for (int i = 0; i < k_disp; i++)
    {
        printf("%d. " color_game "%s" reset " - " color_size "%d " color_unit "GB" reset "\n",
               i + 1, disp[i].name, disp[i].size);
    }
    printf("Apasati ENTER pentru a continua...");
    getchar();
}

void instaleaza()
{
    stergere();
    printf("===== " color_game "INSTALEAZA JOC" reset " =====\n");
    disponibile();
    printf("Alege joc: ");
    int choice;
    scanf("%d", &choice);
    getchar();

    if (choice < 1 || choice > k_disp)
    {
        printf(color_error "Optiune invalida!\n" reset);
        return;
    }

    Joc joc = disp[choice - 1];

    if (su + joc.size > STORAGE)
    {
        printf(color_error "Nu este suficient spatiu!\n" reset);
        return;
    }

    inst[k_inst++] = joc;
    su += joc.size;
    printf(color_game "%s INSTALAT!\n" reset, joc.name);
    save();
}

void dezinstaleaza()
{
    stergere();
    printf("===== " color_menu "DEZINSTALEAZA JOC" reset " =====\n");
    instalate();
    if (k_inst == 0)
    {
        printf(color_size "Nu sunt jocuri de dezinstalat!\n" reset);
        return;
    }
    printf("Alege joc de sters: ");
    int choice;
    scanf("%d", &choice);
    getchar();

    if (choice < 1 || choice > k_inst)
    {
        printf(color_error "Optiune invalida!\n" reset);
        return;
    }

    su -= inst[choice - 1].size;
    printf(color_error "%s STERS!\n" reset, inst[choice - 1].name);

    for (int i = choice - 1; i < k_inst - 1; i++)
        inst[i] = inst[i + 1];

    k_inst--;
    save();
}

int main()
{
    load();

    while (1)
    {
        stergere();
        printf("\n===== " color_menu "MENIU" reset " =====\n");
        printf("1. Vezi jocurile instalate\n");
        printf("2. Vezi jocurile disponibile\n");
        printf("3. Instaleaza joc\n");
        printf("4. Dezinstaleaza joc\n");
        printf("5. Iesi\n");
        printf("Alege: ");
        int choice;
        scanf("%d", &choice);
        getchar();

        if (choice == 1)
            instalate();
        else if (choice == 2)
            disponibile();
        else if (choice == 3)
            instaleaza();
        else if (choice == 4)
            dezinstaleaza();
        else if (choice == 5)
        {
            save();
            printf(color_menu "BYE\n" reset);
            break;
        }
        else
            printf(color_error "Optiune invalida!\n" reset);
    }

    return 0;
}

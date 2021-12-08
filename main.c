#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#define PRODUCT_FILE "FProduit.txt"
#define INTER_FILE "Interfile.txt"
#define FACTURE_FILE "FFacture.txt"
#define CLIENT_FILE "FClient.txt"
#define true 1
#define false 0


typedef int bool;

typedef struct product
{
    long long int code;
    char designation[20];
    double ht;
    int qte;
    double tva;
} product;

typedef struct client
{
    long long int id;
    char nom[50] , prenom[50] , r_social[50];
} client;

typedef struct facture
{
    long long int Nfacture , id_client;
    double ht , tva , ttc;
    int mois , annee;
} facture;



int FillProduct(FILE *F, product *p)
{
    return fscanf(F, "%lld %s %lf %d %lf" , &(p -> code) , p -> designation , &(p -> ht) , &(p -> qte) , &(p -> tva));
}

void PrintProduct(FILE *F, product p)
{
    fprintf(F, "%lld %s %.2lf %d %.2lf\n", p.code, p.designation, p.ht, p.qte, p.tva);
}

int FillFacture(FILE *F, facture *f)
{
    return fscanf(F, "%lld %lld %lf %lf %lf %d %d" , &(f -> Nfacture) , &(f -> id_client) , &(f -> ht) , &(f -> tva) , &(f -> ttc) , &(f -> mois) , &(f -> annee));
}

void PrintFacture(FILE *F, facture f)
{
    fprintf(F, "%lld %lld %.2lf %.2lf %.2lf %d %d\n" , f.Nfacture, f.id_client , f.ht, f.tva, f.ttc ,f.mois, f.annee);
}

void PrintClient(FILE *F , client c)
{
    fprintf(F , "%lld %s %s %s\n" , c.id , c.nom , c.prenom , c.r_social);
}

int FillClient(FILE *F , client *c)
{
    return fscanf(F , "%lld %s %s %s" , &(c -> id) , c -> nom , c -> prenom , c -> r_social);
}



int ProductExist(long long int id_product)
{
    FILE *F;
    product p;
    F = fopen(PRODUCT_FILE, "rt");
    while(FillProduct(F,&p) != -1)
    {
        if(p.code == id_product)
        {
            fclose(F);
            return 1;
        }
    }
    fclose(F);
    return 0;
}




int QteAvailable(long long int id_product, int qte)
{
    FILE* F;
    product p;
    F = fopen(PRODUCT_FILE, "rt");
    if(ProductExist(id_product))
    {
        while(FillProduct(F,&p) != -1)
        {
            if(p.code == id_product)
            {
                if(p.qte>= qte)
                {
                    fclose(F);
                    return 1;
                }
                fclose(F);
                return -1;
            }
        }
    }
    fclose(F);
    return 0;
}





int Unstock(long long int id_produit,int qte)
{
    int r_value = QteAvailable(id_produit, qte);
    if(r_value == 1)
    {
        FILE *F, *G;
        product p;
        F = fopen(PRODUCT_FILE, "rt");
        G = fopen(INTER_FILE, "wt");
        while(FillProduct(F,&p) != -1)
        {
            if(p.code == id_produit)
            {
                if(p.qte > qte)
                {
                    p.qte -= qte;
                    PrintProduct(G, p);
                }
            }
            else
            {
                PrintProduct(G, p);
            }
        }
        fclose(F); fclose(G);
        F = fopen(PRODUCT_FILE, "wt"); G = fopen(INTER_FILE, "rt");
        while(FillProduct(G,&p) != -1)
        {
            PrintProduct(F, p);
        }
        fclose(F); fclose(G);
        remove(INTER_FILE);
    }
    return r_value;
}




void ShowFacture()
{
    long long int id_client;
    printf("Entrer L'ID de client: ");
    scanf("%lld", &id_client);
    FILE *F ;
    int k=0;
    F = fopen(FACTURE_FILE , "rt");
    facture f ;
    printf("\nID de Client: %lld\n" , id_client);
    while (FillFacture(F , &f) != -1)
    {
        if (f.id_client == id_client)
        {
            k++;
            printf("\nFacture %d:____________________________________________" , k);
            printf("\n\nNumero de Facture: %lld\nTotal HT: %.2lf\nTotal TVA: %.2lf\nTotal TTC: %.2lf\nMois: %d\nAnnee: %d\n",f.Nfacture, f.ht, f.tva, f.ttc ,f.mois, f.annee);
            printf("______________________________________________________\n\n");
        }
    }
    if(!k) printf("\nCLIENT:WITHOUT_BILLS!\n");
    fclose(F) ;
}




void AddClient()
{
    FILE *F;
    client c, temp;
    char* instruction;
    instruction = "Entrer le ID du Client: ";
    do
    {
        printf("%s", instruction);
        scanf( "%lld", &(c.id));
        instruction = "Error d'entree\nEntrer une autre ID: ";
    } while(c.id <= 0);
    bool Found = false;
    F = fopen(CLIENT_FILE, "rt");
    if(F != NULL)
    {
        while(FillClient(F, &temp) != -1)
        {
            if(c.id == temp.id)
            {
                printf("CLIENT:ALREADY_EXISTS!\n");
                Found = true;
                break;
            }
        }
    }
    if(!Found)
    {
        getchar();
        fclose(F); F = fopen(CLIENT_FILE, "a");
        printf("Entrer le Nom: ");
        gets(c.nom);
        printf("Entrer le Prenom: ");
        gets(c.prenom);
        printf("Entrer la Raison Sociale: ");
        gets(c.r_social);
        PrintClient( F, c);
        printf("DONE!\n");
    }
    fclose(F);
}





void DelClient()
{
    long long int id;
    printf("Entrer L'ID de client a supprimer: ");
    scanf("%lld", &id);
    FILE *F;
    FILE *G;
    client c;
    F = fopen(CLIENT_FILE, "rt");
    G = fopen(INTER_FILE, "wt");
    bool Found = false;
    while(FillClient(F , &c) != -1)
    {
        if(c.id != id)
        {
            PrintClient(G , c);
        }
        else
        {
            Found = true;
        }
    }
    fclose(F); fclose(G);
    if(!Found)
    {
        printf("CLIENT:NOT_FOUND!\n");
    }
    else
    {
        F = fopen(CLIENT_FILE , "wt");
        G = fopen(INTER_FILE , "rt");
        while(FillClient(G , &c)!=-1)
        {
            PrintClient(F , c);
        }
        fclose(F); fclose(G);
        printf("DONE!\n");
    }
    remove(INTER_FILE);
}






void ShowClients()
{
    FILE *F;
    client c ;
    int k=0;
    F = fopen(CLIENT_FILE , "rt");
    while(FillClient(F , &c) != -1)
    {
        k++;
        printf("\nClient %d ____________________________________________" , k);
        printf("\n\nID: %lld\nNom: %s\nPrenom: %s\nRaison Sociale: %s\n", c.id, c.nom , c.prenom , c.r_social);
        printf("______________________________________________________\n\n");
    }

    fclose(F);
}




void AddProduct()
{
    FILE *F;
    FILE *G;
    product p , temp;
    bool found = false;
    printf("Entrer le code de produit: ");
    scanf("%lld" ,&(temp.code));
    F = fopen(PRODUCT_FILE , "rt");
    G= fopen(INTER_FILE , "wt");
    while(FillProduct(F , &p)!= -1)
    {
        if(p.code==temp.code)
        {
            printf("PRODUCT:ALREADY_EXISTS!\nEntrer la quantite additionel: ");
            scanf("%d", &(temp.qte));
            p.qte += temp.qte;
            found = true;
        }
        PrintProduct(G , p);
    }
    if(!found)
    {
        printf("Entrer la designation: ");
        scanf("%s", temp.designation);
        printf("Entrer le prix unitaire: ");
        scanf("%lf", &(temp.ht));
        printf("Entrer la quantite: ");
        scanf("%d", &(temp.qte));
        printf("Entrer la TVA: ");
        scanf("%lf", &(temp.tva));
        PrintProduct(G, temp);
    }
    fclose(F); fclose(G);
    F = fopen(PRODUCT_FILE , "wt"); G = fopen(INTER_FILE , "rt");
    while(FillProduct(G, &p) != -1)
    {
        PrintProduct(F, p);
    }
    fclose(F); fclose(G);
    remove(INTER_FILE);
    printf("DONE!\n");
}




void DelProduct()
{
    long long int id;
    printf("Entrer L'ID de produit: ");
    scanf("%lld", &id);
    FILE *F;
    FILE *G;
    product p;
    F = fopen(PRODUCT_FILE, "rt");
    G = fopen(INTER_FILE, "wt");
    bool Found = false;
    while(FillProduct(F , &p) != -1)
    {
        if(p.code != id) PrintProduct(G , p);
        else Found = true;
    }
    fclose(F); fclose(G);
    if(!Found)
    {
        printf("PRODUCT:NOT_FOUND!\n");
    }
    else
    {
        F = fopen(PRODUCT_FILE , "wt");
        G = fopen(INTER_FILE , "rt");
        while(FillProduct(G , &p)!=-1)
        {
            PrintProduct(F , p);
        }
        fclose(F); fclose(G);
        remove(INTER_FILE);
        printf("DONE!\n");
    }
}



void ShowProducts()
{
    FILE *F;
    F = fopen(PRODUCT_FILE, "rt");
    product p;
    int k = 0;
    while(FillProduct(F, &p) != -1)
    {
        k++;
        printf("\nProduct %d ____________________________________________" , k);
        printf("\n\nID: %lld\nDesignation: %s\nLe prix unitaire: %.2lf\nLa Quantite: %d\nTVA: %.2lf\n", p.code, p.designation, p.ht, p.qte, p.tva);
        printf("______________________________________________________\n\n");
    }
    fclose(F);
}





void MakeBill()
{
    product p;
    facture f, temp;
    long long int id_client;
    printf("Entrer L'ID de client: ");
    scanf("%lld", &id_client);
    long long int id_produit;
    printf("Entrer L'ID de produit: ");
    scanf("%lld", &id_produit);
    int qte;
    printf("Entrer la quantite: ");
    scanf("%d", &qte);
    int r_value = Unstock(id_produit, qte);
    if(r_value == 1)
    {
        int k = 0;
        FILE* P;
        FILE* F;
        F = fopen(FACTURE_FILE, "rt");
        while(FillFacture(F, &temp) != -1)
        {
            k = temp.Nfacture;
        }
        k++;
        fclose(F);
        f.Nfacture = k;
        f.id_client = id_client;
        time_t current_time;
        current_time = time(NULL);
        struct tm tm = *localtime(&current_time);
        f.annee = tm.tm_year + 1900;
        f.mois = tm.tm_mon + 1;
        F = fopen(FACTURE_FILE, "at");
        P = fopen(PRODUCT_FILE, "rt");
        while(FillProduct(P, &p) != -1)
        {
            if(p.code == id_produit)
            {
                f.ht = p.ht * qte;
                f.tva = f.ht * p.tva / 100;
                f.ttc = f.ht + f.tva;
                PrintFacture(F, f);
                break;
            }
        }
        fclose(F); fclose(P);
        printf("DONE!\n");
    }
    else
    {
        if(r_value == -1) printf("Quantite indisponible!\n");
        if(r_value == 0) printf("Produit indisponible!\n");
    }
}



void ShowBills()
{
    FILE *F;
    F = fopen(FACTURE_FILE, "rt");
    facture f;
    int k = 0;
    while(FillFacture(F, &f) != -1)
    {
        k++;
        printf("\nFacture %d:____________________________________________" , k);
        printf("\n\nNumero de Facture: %lld\nID de client: %lld\nTotal HT: %.2lf\nTotal TVA: %.2lf\nTotal TTC: %.2lf\nMois: %d\nAnnee: %d\n",f.Nfacture, f.id_client, f.ht, f.tva, f.ttc ,f.mois, f.annee);
        printf("______________________________________________________\n\n");
    }
    fclose(F);
}




void ShowBillsByDate()
{
    int month, year;
    printf("Entrer l'annee: ");
    scanf("%d", &year);
    printf("Entrer le mois: ");
    scanf("%d", &month);
    FILE* F;
    facture f;
    F = fopen(FACTURE_FILE, "rt");
    int k = 0;
    while(FillFacture(F, &f) != -1)
    {
        if(f.annee == year && f.mois == month)
        {
            k++;
            printf("\nFacture %d:____________________________________________" , k);
            printf("\n\nNumero de Facture: %lld\nID de client: %lld\nTotal HT: %.2lf\nTotal TVA: %.2lf\nTotal TTC: %.2lf\nMois: %d\nAnnee: %d\n",f.Nfacture, f.id_client, f.ht, f.tva, f.ttc ,f.mois, f.annee);
            printf("______________________________________________________\n\n");
        }
    }
}




void Choose()
{
    printf("\nChoose an option from below _____________________\n\n");
    printf("01 - Verify the existence of a product\n");
    printf("02 - Verify the availability of the ordered amount\n");
    printf("03 - Destock a specific amount of a product\n");
    printf("04 - Show the bills of a client\n");
    printf("05 - Add a new client\n");
    printf("06 - Delete a client\n");
    printf("07 - Display the list of clients\n");
    printf("08 - Add a product\n");
    printf("09 - Remove a product\n");
    printf("10 - Display the list of available products\n");
    printf("11 - Make a bill\n");
    printf("12 - Display the list of bills\n");
    printf("13 - Show bills of a given date\n");
    printf("00 - Exit!\n\n");
    printf("Enter your choice: ");
    int choice;
    scanf("%d", &choice);
    system("cls");
    long long int id;
    int qte, r_value;
    switch(choice)
    {
    case 1 :
        printf("Enter Product ID: ");
        scanf("%lld", &id);
        if(ProductExist(id)) printf("Product exists\n");
        else printf("Product doesn't exist\n");
        break;
    case 2 :
        printf("Enter Product ID: ");
        scanf("%lld", &id);
        printf("Enter The Order Amount: ");
        scanf("%d", &qte);
        r_value = QteAvailable(id, qte);
        if(r_value == 1) printf("The Order Amount Is Available\n");
        else if(r_value == -1) printf("The Available Amount Is Insufficient\n");
        else printf("Product doesn't exist\n");
        break;
    case 3 :
        printf("Enter Product ID: ");
        scanf("%lld", &id);
        printf("Enter The Order Amount: ");
        scanf("%d", &qte);
        r_value = Unstock(id, qte);
        if(r_value == 1) printf("Done!\n");
        else if(r_value == -1) printf("The Available Amount Is Insufficient\n");
        else printf("Product doesn't exist\n");
        break;
    case 4 :
        ShowFacture();
        break;
    case 5 :
        AddClient();
        break;
    case 6 :
        DelClient();
        break;
    case 7 :
        ShowClients();
        break;
    case 8 :
        AddProduct();
        break;
    case 9 :
        DelProduct();
        break;
    case 10 :
        ShowProducts();
        break;
    case 11 :
        MakeBill();
        break;
    case 12 :
        ShowBills();
        break;
    case 13 :
        ShowBillsByDate();
        break;
    case 0 :
        exit(0);
    default :
        system("cls");
        printf("Wrong Value! Try Again\n");
        Choose();
    }
    system("pause");
    system("cls");
    Choose();
}



int main()
{
    printf("                 *** Welcome! ***\n");
    Choose();
    return 0;
}

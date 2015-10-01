#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "funzioni.h"



int main(){
	struct miaStruttura *testina;
	struct miaStruttura *trovata;
	int tot_img, vet[256]={0};
	char operazione=-1, IDpaziente[11], IDfinpaziente[21], scelta;
	data data_int;

    testina=ordinamento(testina,&tot_img);

	printf("\nInserisci l'ID del paziente di cui vuoi modificare l'immagine\n");
	scanf("%s", IDpaziente);
    printf("Inserisci la data dell'immagine che si vuole modificare.");
    do{
		printf("\nGiorno (dd):");
		scanf("%s", data_int.giorno);
	}while(!((strcmp(data_int.giorno, "01")>=0)&&(strcmp(data_int.giorno, "31")<=0)));
	do{
		printf("\nMese (mm):");
		scanf("%s", data_int.mese);
	}while(!((strcmp(data_int.mese, "01")>=0)&&(strcmp(data_int.mese, "12")<=0)));
	do{
		printf("\nAnno (yyyy):");
		scanf("%s", data_int.anno);
	}while(strcmp(data_int.anno, "1900")<0);
	printf("\n");

    strcpy(IDfinpaziente, IDpaziente);
	strcat(IDfinpaziente, data_int.anno);
	strcat(IDfinpaziente, data_int.mese);
	strcat(IDfinpaziente, data_int.giorno);

printf("ID %s\n", IDfinpaziente);

    trovata=riconoscimento(IDfinpaziente,testina,tot_img);

	do{
		printf("\nLe operazioni da effettuare sull'immagine scelta sono:\n");
		printf("1) Calcolo del numero di livelli di grigio presenti nell'immagine;\n");
		printf("2) Calcolo della distribuzione dei livelli di grigio presenti nell'immagine;\n");
		printf("3) Riduzione dei livelli di grigio presenti nell'immagine a 10;\n");
		printf("4) Definizione del contorno dell'immagine;\n");
		printf("5) Calcolo dell'area inclusa nel contorno dell'immagine;\n");
		printf("6) Calcolo dell'area totale delle zone molto scure e molto chiare all'interno dell'area informativa dell'immagine;\n");
		printf("7) Calcolo della variazione delle due aree totali (zone chiare e scure) fra l'immagine scelta e la successiva;\n");
		printf("8) Calcolo della variazione delle due aree totali (zone chiare e scure) fra l'immagine scelta e una seconda immagine appartenente allo stesso paziente;\n");
		printf("9) Nessuna operazione.\n");

		printf("\nScegli il numero dell'opzione\n");
		scanf(" %c",&scelta);

		switch(scelta){
			case '1':
				num_grigi(trovata);
				break;
			case '2':
				distribuzione_grigi(trovata,vet);
				break;
			case '3':
				riduzione(trovata);
				break;
			case '4':
				contorno(trovata);
				break;
			case '5':
				area_contorno(trovata);
				break;
			case '6':
				area_chiara_scura(trovata);
				break;
			case '7':
				variazione_imm_consec(trovata);
				break;
			case '8':
				variazione_imm_scelta(trovata,testina,tot_img);
				break;
			case '9':
				printf("L'opzione selezionata prevede l'uscita dal programma.\n");
				return 0;
				break;
			default:
				printf("\n\n ERRORE!!!! L'operazione selezionata non e' tra quelle permesse\n\n");
		}
	}while(scelta!=9);

	return 0;
}

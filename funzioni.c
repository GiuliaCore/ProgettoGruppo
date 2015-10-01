#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "bmp.h"

//Le strtture si trovano in bmp.h

//Caricamento immagine
imgcorr caricaimm(){
	BMP_Image image;
	imgcorr lena;
	int ris_load;

//Se il caricamento tramite la funzione load_BMP non è corretto viene richiesto l'inserimento
	do{
		printf("\nInserisci il nome del file da caricare: \n");
		scanf("%s", lena.filename);
		ris_load=loadBMP(lena.filename, &image);
	}while((ris_load==1)||(ris_load==2)||(ris_load==3)||(ris_load==4));

	lena.newimage=image;

//Richiesta informazioni per creare ID finale (vedi struttura imgcorr)
	printf("ID paziente: ");
	scanf("%s", lena.ID);
	printf("Data:\n");
    do{
		printf("Giorno (dd):\n");
		scanf("%s", lena.dataimg.giorno);
		}while(!((strcmp(lena.dataimg.giorno, "01")>=0)&&(strcmp(lena.dataimg.giorno, "31")<=0)));
	do{
		printf("Mese (mm):\n");
		scanf("%s", lena.dataimg.mese);
		}while(!((strcmp(lena.dataimg.mese, "01")>=0)&&(strcmp(lena.dataimg.mese, "12")<=0)));
	do{
		printf("Anno (yyyy):\n");
		scanf("%s", lena.dataimg.anno);
		}while(strcmp(lena.dataimg.anno, "1900")<0);

//Creazione ID finale
	strcpy(lena.IDfin, lena.ID);
	strcat(lena.IDfin, lena.dataimg.anno);
	strcat(lena.IDfin, lena.dataimg.mese);
	strcat(lena.IDfin, lena.dataimg.giorno);

	printf("ID finale: %s \n", lena.IDfin);

	return lena;
}

//Ordinare lista immagini
//ordinamento fatto utilizzando ID finale delle immagini caricate
struct miaStruttura* ordinamento(struct miaStruttura* testa, int* numeri) {
	int i, ok;
	struct miaStruttura  *new, *scorri;
	char nomefile[20];


	do{
		printf("\nQuante immagini vuoi caricare?\nRisposta: ");
		scanf("%d",numeri);
	}while((*numeri)<1);

//Caricamento della prima immagine
	testa = (struct miaStruttura*) malloc(sizeof(struct miaStruttura));

	testa->immagine=caricaimm();

	testa->next = NULL;

//Caricamento delle immagini successive alla prima e ordinamento
	for (i=1; i<(*numeri); i++){
		new = (struct miaStruttura*) malloc(sizeof(struct miaStruttura));

		new->immagine=caricaimm();

		scorri=testa;
		ok=0;
		while(scorri!=NULL && !ok){
			if(strcmp(new->immagine.IDfin, scorri->immagine.IDfin)>0){
//Se scorri è l'ultima della lista inseriamo new in coda
				if(scorri->next==NULL){
					new->next = scorri->next;
					scorri->next=new;
					ok=1;
				}
//Se scorri non è l'ultimo bisogna confrontare new con ciò che segue scorri
				else if (strcmp(new->immagine.IDfin,(scorri->next)->immagine.IDfin)<0){
					new->next = scorri->next;
					scorri->next=new;
					ok=1;
				}
				else
					scorri=scorri->next;
			}
//Eventuale inserimento in testa
			else if(strcmp(new->immagine.IDfin, scorri->immagine.IDfin)<=0){
				new->next=scorri;
				testa=new;
				ok=1;
			}
	}
  }

	scorri=testa;

//Salvataggio immagini rinominate con ID finale
	strcpy(nomefile, scorri->immagine.IDfin);
    strcat(nomefile, ".bmp");
    saveBMP(scorri->immagine.newimage,nomefile);

	while((scorri->next)!=NULL){
		scorri=scorri->next;
		strcpy(nomefile, scorri->immagine.IDfin);
		strcat(nomefile, ".bmp");
		saveBMP(scorri->immagine.newimage,nomefile);
    }

  return testa;
}

//Cercare immagine su cui lavorare
//Parametri di ingresso: ID finale, puntatore al primo elemento della lista di immagini, numero immagini caricate
struct miaStruttura* riconoscimento (char* str_ID, struct miaStruttura* lista, int num){
	int i;

	for(i=0; i<num; i++){
		if(!(strncmp(str_ID, lista->immagine.IDfin,19))){
			return lista;
		}
		else if(lista->next==NULL){
			printf("Immagine non trovata\n");
			return NULL;
		}
		else
			lista=lista->next;
	}
}


//Numero di grigi
void num_grigi(struct miaStruttura* struct_img){
	int v[256]={0};
	unsigned int i, j, conta=0;

	loadBMP(struct_img->immagine.filename, &(struct_img->immagine.newimage));

	for(i=0; i<256; i++)
		for(j=0; j<256; j++)
			v[(struct_img->immagine.newimage.dato[i][j].grey)]++;

	for(i=0; i<256; i++)
		if(v[i]!=0)
			conta++;

    printf("\nIl numero di livelli di grigio presenti nell'immagine caricata e' %d\n\n",conta);

}

//Distribuzione dei livelli di grigio
void distribuzione_grigi(struct miaStruttura* struct_img, int* v){
	unsigned int i, j;

	loadBMP(struct_img->immagine.filename, &(struct_img->immagine.newimage));

	for(i=0; i<256; i++)
		for(j=0; j<256; j++)
			v[struct_img->immagine.newimage.dato[i][j].grey]++;

	for(i=0; i<256; i++)
        if (v[i]!=0)
		printf("\nLivello di grigio: %u ---> Numero di pixel %d\n",i,v[i]);
}

//Riduzione dei livelli di grigio
void riduzione(struct miaStruttura* struct_img){
	unsigned int i,j;
	char nomefile[20];

	loadBMP(struct_img->immagine.filename, &(struct_img->immagine.newimage));

    for(i=0; i<256; i++)
		for(j=0; j<256; j++){
			if (((struct_img->immagine.newimage.dato[i][j].grey)>=0)&&((struct_img->immagine.newimage.dato[i][j].grey)<=25))
				struct_img->immagine.newimage.dato[i][j].grey=0;
			else if (((struct_img->immagine.newimage.dato[i][j].grey)>=26)&&((struct_img->immagine.newimage.dato[i][j].grey)<=50))
				struct_img->immagine.newimage.dato[i][j].grey=26;
			else if (((struct_img->immagine.newimage.dato[i][j].grey)>=51)&&((struct_img->immagine.newimage.dato[i][j].grey)<=76))
				struct_img->immagine.newimage.dato[i][j].grey=51;
			else if (((struct_img->immagine.newimage.dato[i][j].grey)>=77)&&((struct_img->immagine.newimage.dato[i][j].grey)<=101))
				struct_img->immagine.newimage.dato[i][j].grey=77;
			else if (((struct_img->immagine.newimage.dato[i][j].grey)>=102)&&((struct_img->immagine.newimage.dato[i][j].grey)<=127))
				struct_img->immagine.newimage.dato[i][j].grey=102;
			else if (((struct_img->immagine.newimage.dato[i][j].grey)>=128)&&((struct_img->immagine.newimage.dato[i][j].grey)<=152))
				struct_img->immagine.newimage.dato[i][j].grey=128;
			else if (((struct_img->immagine.newimage.dato[i][j].grey)>=153)&&((struct_img->immagine.newimage.dato[i][j].grey)<=178))
				struct_img->immagine.newimage.dato[i][j].grey=153;
			else if (((struct_img->immagine.newimage.dato[i][j].grey)>=179)&&((struct_img->immagine.newimage.dato[i][j].grey)<=203))
				struct_img->immagine.newimage.dato[i][j].grey=179;
			else if (((struct_img->immagine.newimage.dato[i][j].grey)>=204)&&((struct_img->immagine.newimage.dato[i][j].grey)<=229))
				struct_img->immagine.newimage.dato[i][j].grey=204;
			else if (((struct_img->immagine.newimage.dato[i][j].grey)>=230)&&((struct_img->immagine.newimage.dato[i][j].grey)<=255))
				struct_img->immagine.newimage.dato[i][j].grey=230;
}
    strcpy(nomefile, struct_img->immagine.IDfin);
    strcat(nomefile, ".bmp");
    saveBMP(struct_img->immagine.newimage,nomefile);

    printf("\nLa tua immagine modificata e' stata ridotta a 10 livelli di grigio: 0, 26, 51, 77, 102, 128, 153, 179, 204, 230\nL'immagine e' stata, inoltre, salvata con l'ID finale del paziente");

}

//Definizione contorno
void contorno(struct miaStruttura* img_in){
	int i, j;
	float dim, alto, basso, dx, sx;
	int p_alto, p_basso, p_dx, p_sx;
	char nomefile[20];

	loadBMP(img_in->immagine.filename, &(img_in->immagine.newimage));

	do{
        printf("\n\nInserisci la dimensione dell'immagine (quadrata) in centimetri: ");
        scanf("%f", &dim);
	}while(dim<=0);

//Selezione della Regione di interesse dell'immagine
	printf("\n\nInserire i centimetri di immagine da tagliare:");

    do{
        do{
            printf("\n-Dall'alto: ");
            scanf("%f", &alto);
        }while(alto<0);
        do{
            printf("\n-Dal basso: ");
            scanf("%f", &basso);
        }while(basso<0);
	}while((alto+basso)>=dim);


	do{
        do{
            printf("\n-Da destra: ");
            scanf("%f", &dx);
        }while(dx<0);
        do{
            printf("\n-Da sinistra: ");
            scanf("%f", &sx);
        }while(sx<0);
	}while((dx+sx)>=dim);

//Passaggio da centimetri a pixel
	p_alto=(alto*256)/dim;
	p_basso=(basso*256)/dim;
	p_dx=(dx*256)/dim;
	p_sx=(sx*256)/dim;

//Cicli for che permettono di evidenziare il contorno facendo diventare bianchi i pixels
//Ciclo che indaga la metà sinistra dell'immagine
	for(i=0; i<256-p_alto-p_basso; i++)
		for(j=0; j<(256-p_dx-p_sx)/2+(256-p_dx-p_sx)%2; j++)
			if(img_in->immagine.newimage.dato[p_alto+i][p_sx+j].grey>50){
				img_in->immagine.newimage.dato[p_alto+i][p_sx+j].grey=255;
				j=(256-p_dx-p_sx)/2+(256-p_dx-p_sx)%2;
			}

//Ciclo che indaga la metà destra dell'immagine (da destra a sinistra)
	for(i=0; i<256-p_alto-p_basso; i++)
		for(j=0; j<(256-p_dx-p_sx)/2; j++)
			if(img_in->immagine.newimage.dato[p_alto+i][256-p_dx-j].grey>50){
				img_in->immagine.newimage.dato[p_alto+i][256-p_dx-j].grey=255;
				j=(256-p_dx-p_sx)/2;
			}

    strcpy(nomefile, img_in->immagine.IDfin);
    strcat(nomefile, ".bmp");
    saveBMP(img_in->immagine.newimage,nomefile);

    printf("\nL'immagine e' stata salvata con il contorno evidenziato.\n");

}

//Area nel contorno
void area_contorno(struct miaStruttura* img_in){

	int i, j;
	float dim, alto, basso, dx, sx, area_mm;
	int p_alto, p_basso, p_dx, p_sx, area_p;

	loadBMP(img_in->immagine.filename, &(img_in->immagine.newimage));

	do{
        printf("\n\nInserisci la dimensione dell'immagine (quadrata) in centimetri: ");
        scanf("%f", &dim);
	}while(dim<=0);

//Selezione della Regione di interesse dell'immagine
	printf("\n\nInserire i centimetri di immagine da tagliare:");

    do{
        do{
            printf("\n-Dall'alto: ");
            scanf("%f", &alto);
        }while(alto<0);
        do{
            printf("\n-Dal basso: ");
            scanf("%f", &basso);
        }while(basso<0);
	}while((alto+basso)>=dim);


	do{
        do{
            printf("\n-Da destra: ");
            scanf("%f", &dx);
        }while(dx<0);
        do{
            printf("\n-Da sinistra: ");
            scanf("%f", &sx);
        }while(sx<0);
	}while((dx+sx)>=dim);

//Passaggio da centimetri a pixel
	p_alto=(alto*256)/dim;
	p_basso=(basso*256)/dim;
	p_dx=(dx*256)/dim;
	p_sx=(sx*256)/dim;

//Cicli for che permettono di evidenziare il contorno facendo diventare bianchi i pixels (vedi funzione contorno)
//Ciclo che indaga la metà sinistra dell'immagine
	for(i=0; i<256-p_alto-p_basso; i++)
		for(j=0; j<(256-p_dx-p_sx)/2+(256-p_dx-p_sx)%2; j++)
			if(img_in->immagine.newimage.dato[p_alto+i][p_sx+j].grey>50){
				img_in->immagine.newimage.dato[p_alto+i][p_sx+j].grey=255;
				j=(256-p_dx-p_sx)/2+(256-p_dx-p_sx)%2;
			}

//Ciclo che indaga la metà destra dell'immagine
	for(i=0; i<256-p_alto-p_basso; i++)
		for(j=0; j<(256-p_dx-p_sx)/2; j++)
			if(img_in->immagine.newimage.dato[p_alto+i][256-p_dx-j].grey>50){
				img_in->immagine.newimage.dato[p_alto+i][256-p_dx-j].grey=255;
				j=(256-p_dx-p_sx)/2;
			}

//Calcolo area tagliata
	area_p = 256*p_alto + 256*p_basso + (256-p_alto-p_basso)*p_sx + (256-p_alto-p_basso)*p_dx;

//Aggiunta di area scura esterna al contorno (con due cicli che indagano la parte sx e dx dell'immagine)
	for(i=0; i<256-p_alto-p_basso; i++)
		for(j=0; j<(256-p_dx-p_sx)/2+(256-p_dx-p_sx)%2; j++)
			if(img_in->immagine.newimage.dato[p_alto+i][p_sx+j].grey==255)
				j=(256-p_dx-p_sx)/2+(256-p_dx-p_sx)%2;
			else area_p++;


	for(i=0; i<256-p_alto-p_basso; i++)
		for(j=0; j<(256-p_dx-p_sx)/2; j++)
			if(img_in->immagine.newimage.dato[p_alto+i][256-p_dx-j].grey==255)
				j=(256-p_dx-p_sx)/2;
			else area_p++;

//Area scura interna al contorno
	area_p = 256*256 - area_p;

	area_mm = ((area_p*dim*dim)/(256*256))*100;

	printf("\n\nArea della parte informativa\n-in pixel: %d\n-in mm^2: %.2f\n\n", area_p, area_mm);

}

//Aree chiare e scure
void area_chiara_scura(struct miaStruttura* img_in){

	int i, j;
	float dim, alto, basso, dx, sx, area_mm, area_chiara_mm, area_scura_mm;
	int p_alto, p_basso, p_dx, p_sx, area_p=0, area_chiara=0, area_scura=0;


	loadBMP(img_in->immagine.filename, &(img_in->immagine.newimage));

	do{
        printf("\n\nInserisci la dimensione dell'immagine (quadrata) in centimetri: ");
        scanf("%f", &dim);
	}while(dim<=0);

//Selezione della Regione di interesse dell'immagine
	printf("\n\nInserire i centimetri di immagine da tagliare:");

    do{
        do{
            printf("\n-Dall'alto: ");
            scanf("%f", &alto);
        }while(alto<0);
        do{
            printf("\n-Dal basso: ");
            scanf("%f", &basso);
        }while(basso<0);
	}while((alto+basso)>=dim);


	do{
        do{
            printf("\n-Da destra: ");
            scanf("%f", &dx);
        }while(dx<0);
        do{
            printf("\n-Da sinistra: ");
            scanf("%f", &sx);
        }while(sx<0);
	}while((dx+sx)>=dim);

//Passaggio da centimetri a pixel
	p_alto=(alto*256)/dim;
	p_basso=(basso*256)/dim;
	p_dx=(dx*256)/dim;
	p_sx=(sx*256)/dim;

//area_p è l'area scura esterna al contorno e compresa nella Regione di interesse
//Ciclo che indaga la parte sinistra dell'immagine
	for(i=0; i<256-p_alto-p_basso; i++)
		for(j=0; j<(256-p_dx-p_sx)/2+(256-p_dx-p_sx)%2; j++)
			if(img_in->immagine.newimage.dato[p_alto+i][p_sx+j].grey>50)
				j=(256-p_dx-p_sx)/2+(256-p_dx-p_sx)%2;
			else area_p++;

//Ciclo che indaga la parte destra dell'immagine
	for(i=0; i<256-p_alto-p_basso; i++)
		for(j=0; j<(256-p_dx-p_sx)/2; j++)
			if(img_in->immagine.newimage.dato[p_alto+i][256-p_dx-j].grey>50)
				j=(256-p_dx-p_sx)/2;
			else area_p++;

//Area chiara presente nella Regione di interesse e Area scura presente nell'intera Regione di interesse (interna ed esterna al contorno)
//Ciclo che indaga la parte sinistra dell'immagine
	for(i=0; i<(256-p_alto-p_basso); i++)
		for(j=0; j<(256-p_dx-p_sx)/2+(256-p_dx-p_sx)%2; j++){
			if(img_in->immagine.newimage.dato[p_alto+i][p_sx+j].grey<=70)
				area_scura++;
			else if(img_in->immagine.newimage.dato[p_alto+i][p_sx+j].grey>=185)
				area_chiara++;
		}

//Ciclo che indaga la parte destra dell'immagine
	for(i=0; i<256-p_alto-p_basso; i++)
		for(j=0; j<(256-p_dx-p_sx)/2; j++){
			if(img_in->immagine.newimage.dato[p_alto+i][256-p_dx-j].grey<=70)
				area_scura++;
			else if(img_in->immagine.newimage.dato[p_alto+i][256-p_dx-j].grey>=185)
				area_chiara++;
		}

//Area scura finale della sola parte interna al contorno
	area_scura-=area_p;

	area_scura_mm = ((area_scura*dim*dim)/(256*256))*100;
	area_chiara_mm = ((area_chiara*dim*dim)/(256*256))*100;

	printf("\n\nIl valore soglia scelto per individuare una zona scura e' 70.\nIl valore soglia scelto per individuare una zona chiara e' 185.\n");
	printf("\n\nArea scura della parte informativa\n-in pixel: %d\n-in mm^2: %.2f\n\n", area_scura, area_scura_mm);
	printf("\n\nArea chiara della parte informativa\n-in pixel: %d\n-in mm^2: %.2f\n\n", area_chiara, area_chiara_mm);

}

//Variazione di aree tra immagini successive
void variazione_imm_consec(struct miaStruttura* img_in){

	int i, j;
	float dim, alto, basso, dx, sx, area_mm, area_chiara_mm1, area_scura_mm1, area_chiara_mm2, area_scura_mm2;
	int p_alto, p_basso, p_dx, p_sx, area_p=0, area_chiara1=0, area_scura1=0, area_chiara2=0, area_scura2=0;


	loadBMP(img_in->immagine.filename, &(img_in->immagine.newimage));

//Si controlla che l'immagine scelta dall'utente abbia davvero un'immagine successiva
	if(img_in->next==NULL){
		printf("\nNon e' possibile eseguire questa operazione sull'immagine poiche' e' l'ultima della lista.");
	}
	else{
//Operazioni sull'immagine scelta dall'utente
        do{
            printf("\n\nInserisci la dimensione della prima immagine (quadrata) in centimetri: ");
            scanf("%f", &dim);
        }while(dim<=0);


        printf("\n\nInserire i centimetri di immagine da tagliare:");

        do{
            do{
                printf("\n-Dall'alto: ");
                scanf("%f", &alto);
            }while(alto<0);
            do{
                printf("\n-Dal basso: ");
                scanf("%f", &basso);
            }while(basso<0);
        }while((alto+basso)>=dim);


        do{
            do{
                printf("\n-Da destra: ");
                scanf("%f", &dx);
            }while(dx<0);
            do{
                printf("\n-Da sinistra: ");
                scanf("%f", &sx);
            }while(sx<0);
        }while((dx+sx)>=dim);

//Passaggio da centimetri a pixel
		p_alto=(alto*256)/dim;
		p_basso=(basso*256)/dim;
		p_dx=(dx*256)/dim;
		p_sx=(sx*256)/dim;

//area_p è l'area scura esterna al contorno e compresa nella Regione di interesse
//Ciclo che indaga la parte sinistra dell'immagine
		for(i=0; i<256-p_alto-p_basso; i++)
			for(j=0; j<(256-p_dx-p_sx)/2+(256-p_dx-p_sx)%2; j++)
				if(img_in->immagine.newimage.dato[p_alto+i][p_sx+j].grey>50)
					j=(256-p_dx-p_sx)/2+(256-p_dx-p_sx)%2;
				else area_p++;

//Ciclo che indaga la parte destra dell'immagine
		for(i=0; i<256-p_alto-p_basso; i++)
			for(j=0; j<(256-p_dx-p_sx)/2; j++)
				if(img_in->immagine.newimage.dato[p_alto+i][256-p_dx-j].grey>50)
					j=(256-p_dx-p_sx)/2;
				else area_p++;

//Area chiara presente nella Regione di interesse e Area scura presente nell'intera Regione di interesse (interna ed esterna al contorno)
//Ciclo che indaga la parte sinistra dell'immagine
		for(i=0; i<(256-p_alto-p_basso); i++)
			for(j=0; j<(256-p_dx-p_sx)/2+(256-p_dx-p_sx)%2; j++){
				if(img_in->immagine.newimage.dato[p_alto+i][p_sx+j].grey<=70)
					area_scura1++;
				else if(img_in->immagine.newimage.dato[p_alto+i][p_sx+j].grey>=185)
					area_chiara1++;
			}

//Ciclo che indaga la parte destra dell'immagine
		for(i=0; i<256-p_alto-p_basso; i++)
			for(j=0; j<(256-p_dx-p_sx)/2; j++){
				if(img_in->immagine.newimage.dato[p_alto+i][256-p_dx-j].grey<=70)
					area_scura1++;
				else if(img_in->immagine.newimage.dato[p_alto+i][256-p_dx-j].grey>=185)
					area_chiara1++;
			}

//Area scura finale della sola parte interna al contorno
		area_scura1-=area_p;

		area_scura_mm1 = ((area_scura1*dim*dim)/(256*256))*100;
		area_chiara_mm1 = ((area_chiara1*dim*dim)/(256*256))*100;

		printf("\n\nPrima immagine\n");
		printf("Il valore soglia scelto per individuare una zona scura e' 70.\nIl valore soglia scelto per individuare una zona chiara e' 185.\n");
		printf("\n\nArea scura della parte informativa\n-in pixel: %d\n-in mm^2: %.2f\n\n", area_scura1, area_scura_mm1);
		printf("\n\nArea chiara della parte informativa\n-in pixel: %d\n-in mm^2: %.2f\n\n", area_chiara1, area_chiara_mm1);


		img_in=img_in->next;
		loadBMP(img_in->immagine.filename, &(img_in->immagine.newimage));

//Operazioni sull'immagine successiva a quella scelta dall'utente
//(Vedi sopra)
        do{
            printf("\n\nInserisci la dimensione della seconda immagine (quadrata) in centimetri: ");
            scanf("%f", &dim);
        }while(dim<=0);


        printf("\n\nInserire i centimetri di immagine da tagliare:");

        do{
            do{
                printf("\n-Dall'alto: ");
                scanf("%f", &alto);
            }while(alto<0);
            do{
                printf("\n-Dal basso: ");
                scanf("%f", &basso);
            }while(basso<0);
        }while((alto+basso)>=dim);


        do{
            do{
                printf("\n-Da destra: ");
                scanf("%f", &dx);
            }while(dx<0);
            do{
                printf("\n-Da sinistra: ");
                scanf("%f", &sx);
            }while(sx<0);
        }while((dx+sx)>=dim);

		p_alto=(alto*256)/dim;
		p_basso=(basso*256)/dim;
		p_dx=(dx*256)/dim;
		p_sx=(sx*256)/dim;

		for(i=0; i<256-p_alto-p_basso; i++)
			for(j=0; j<(256-p_dx-p_sx)/2+(256-p_dx-p_sx)%2; j++)
				if(img_in->immagine.newimage.dato[p_alto+i][p_sx+j].grey>50)
					j=(256-p_dx-p_sx)/2+(256-p_dx-p_sx)%2;
				else area_p++;


		for(i=0; i<256-p_alto-p_basso; i++)
			for(j=0; j<(256-p_dx-p_sx)/2; j++)
				if(img_in->immagine.newimage.dato[p_alto+i][256-p_dx-j].grey>50)
					j=(256-p_dx-p_sx)/2;
				else area_p++;



		for(i=0; i<(256-p_alto-p_basso); i++)
			for(j=0; j<(256-p_dx-p_sx)/2+(256-p_dx-p_sx)%2; j++){
				if(img_in->immagine.newimage.dato[p_alto+i][p_sx+j].grey<=70)
					area_scura2++;
				else if(img_in->immagine.newimage.dato[p_alto+i][p_sx+j].grey>=185)
					area_chiara2++;
			}

		for(i=0; i<256-p_alto-p_basso; i++)
			for(j=0; j<(256-p_dx-p_sx)/2; j++){
				if(img_in->immagine.newimage.dato[p_alto+i][256-p_dx-j].grey<=70)
					area_scura2++;
				else if(img_in->immagine.newimage.dato[p_alto+i][256-p_dx-j].grey>=185)
					area_chiara2++;
			}

		area_scura2-=area_p;

		area_scura_mm2 = ((area_scura2*dim*dim)/(256*256))*100;
		area_chiara_mm2 = ((area_chiara2*dim*dim)/(256*256))*100;

//Stampa a video dei risutati ottenuti
		printf("\n\nSeconda immagine\n");
		printf("Il valore soglia scelto per individuare una zona scura e' 70.\nIl valore soglia scelto per individuare una zona chiara e' 185.");
		printf("\n\nArea scura della parte informativa\n-in pixel: %d\n-in mm^2: %.2f", area_scura2, area_scura_mm2);
		printf("\n\nArea chiara della parte informativa\n-in pixel: %d\n-in mm^2: %.2f\n", area_chiara2, area_chiara_mm2);

		if(area_scura_mm1>area_scura_mm2){
			printf("\n\nL'area scura della prima immagine e' maggiore rispetto a quella della seconda immagine.\n");
			printf("La differenza e'\n-in pixel: %d\n-in mm^2: %.2f\n",area_scura1-area_scura2,area_scura_mm1-area_scura_mm2);
		}
		else if(area_scura_mm1<area_scura_mm2){
			printf("\n\nL'area scura della prima immagine e' minore rispetto a quella della seconda immagine.\n");
			printf("La differenza e'\n-in pixel: %d\n-in mm^2: %.2f\n",area_scura2-area_scura1,area_scura_mm2-area_scura_mm1);
		}
		else{
			printf("\n\nL'area scura della prima immagine e' uguale a quella della seconda immagine.\n");
			printf("La differenza e' nulla\n");
		}

		if(area_chiara_mm1>area_chiara_mm2){
			printf("\n\nL'area chiara della prima immagine e' maggiore rispetto a quella della seconda immagine.\n");
			printf("La differenza e'\n-in pixel: %d\n-in mm^2: %.2f\n",area_chiara1-area_chiara2,area_chiara_mm1-area_chiara_mm2);
		}
		else if(area_chiara_mm1<area_chiara_mm2){
			printf("\n\nL'area chiara della prima immagine e' minore rispetto a quella della seconda immagine.\n");
			printf("La differenza e'\n-in pixel: %d\n-in mm^2: %.2f\n",area_chiara2-area_chiara1,area_chiara_mm2-area_chiara_mm1);
		}
		else{
			printf("\n\nL'area chiara della prima immagine e' uguale a quella della seconda immagine.\n");
			printf("La differenza e' nulla\n");
		}
	}
}


//Variazione aree tra immagini a scelta (dello stesso paziente)
void variazione_imm_scelta(struct miaStruttura* img_in, struct miaStruttura* capolista, int num){

	int i, j, contatore=0;
	float dim, alto, basso, dx, sx, area_mm, area_chiara_mm1, area_scura_mm1, area_chiara_mm2, area_scura_mm2;
	int p_alto, p_basso, p_dx, p_sx, area_p=0, area_chiara1=0, area_scura1=0, area_chiara2=0, area_scura2=0;
	data data_int;
	char IDfinpaziente[21];
	struct miaStruttura *img_in2, *scorri;


	loadBMP(img_in->immagine.filename, &(img_in->immagine.newimage));

	scorri=capolista;

//Stampa a video degli ID finali associati a ciasuna immagine appartenente allo stesso paziente e presenti nella lista
	for(i=0; i<num; i++){
		if(!(strcmp(scorri->immagine.ID, img_in->immagine.ID)) && strcmp(scorri->immagine.IDfin, img_in->immagine.IDfin)){
			printf("\nID finale di un'immagine dello stesso paziente: %s", scorri->immagine.IDfin);
			contatore++;
		}
		scorri=scorri->next;
	}

//Verifica della presenza di altre immagini (oltre a quella scelta dall'utente) appartenenti allo stesso paziente
	if(contatore==0)
		printf("\nNon ci sono altre immagini dello stesso paziente.\nImpossibile procedere con questa operazione.\n");

	else{
		do{

//Scelta della seconda immagine
			printf("\nInserisci la data della seconda immagine da analizzare.");
			do{
				printf("\nGiorno (dd):");
				scanf("%s", data_int.giorno);
			} while(!((strcmp(data_int.giorno, "01")>=0)&&(strcmp(data_int.giorno, "31")<=0)));
			do{
				printf("\nMese (mm):");
				scanf("%s", data_int.mese);
			} while(!((strcmp(data_int.mese, "01")>=0)&&(strcmp(data_int.mese, "12")<=0)));
			do{
				printf("\nAnno (yyyy):");
				scanf("%s", data_int.anno);
			} while(strcmp(data_int.anno, "1900")<0);

			printf("\n");

//Creazione ID finale con le informazioni inserite dall'utente per permettere il riconoscimento della seconda immagine su cui effettuare le operazioni
			strcpy(IDfinpaziente, img_in->immagine.ID);
			strcat(IDfinpaziente, data_int.anno);
			strcat(IDfinpaziente, data_int.mese);
			strcat(IDfinpaziente, data_int.giorno);

			img_in2=riconoscimento(IDfinpaziente, capolista, num);
			if(img_in2==NULL)
				printf("\nImmagine non trovata. Data inserita non valida.\n");

		}while(img_in2==NULL);

//Operazioni sulla prima immagine scelta dall'utente
		do{
			printf("\n\nInserisci la dimensione della prima immagine (quadrata) in centimetri: ");
			scanf("%f", &dim);
		}while(dim<=0);

//Selezione della Regione di interesse dell'immagine
		printf("\n\nInserire i centimetri di immagine da tagliare:");

		do{
			do{
				printf("\n-Dall'alto: ");
				scanf("%f", &alto);
			}while(alto<0);
			do{
				printf("\n-Dal basso: ");
				scanf("%f", &basso);
			}while(basso<0);
		}while((alto+basso)>=dim);


		do{
			do{
				printf("\n-Da destra: ");
				scanf("%f", &dx);
			}while(dx<0);
			do{
				printf("\n-Da sinistra: ");
				scanf("%f", &sx);
			}while(sx<0);
		}while((dx+sx)>=dim);

//Passaggio da centimetri a pixel
			p_alto=(alto*256)/dim;
			p_basso=(basso*256)/dim;
			p_dx=(dx*256)/dim;
			p_sx=(sx*256)/dim;

//area_p è l'area scura esterna al contorno e compresa nella Regione di interesse
//Ciclo che indaga la parte sinistra dell'immagine
			for(i=0; i<256-p_alto-p_basso; i++)
				for(j=0; j<(256-p_dx-p_sx)/2+(256-p_dx-p_sx)%2; j++)
					if(img_in->immagine.newimage.dato[p_alto+i][p_sx+j].grey>50)
						j=(256-p_dx-p_sx)/2+(256-p_dx-p_sx)%2;
					else area_p++;

//Ciclo che indaga la parte destra dell'immagine (da destra a sinistra)
			for(i=0; i<256-p_alto-p_basso; i++)
				for(j=0; j<(256-p_dx-p_sx)/2; j++)
					if(img_in->immagine.newimage.dato[p_alto+i][256-p_dx-j].grey>50)
						j=(256-p_dx-p_sx)/2;
					else area_p++;

//Area chiara presente nella Regione di interesse e Area scura presente nell'intera Regione di interesse (interna ed esterna al contorno)
//Ciclo che indaga la parte sinistra dell'immagine
			for(i=0; i<(256-p_alto-p_basso); i++)
				for(j=0; j<(256-p_dx-p_sx)/2+(256-p_dx-p_sx)%2; j++)
					if(img_in->immagine.newimage.dato[p_alto+i][p_sx+j].grey<=70)
						area_scura1++;
					else if(img_in->immagine.newimage.dato[p_alto+i][p_sx+j].grey>=185)
						area_chiara1++;

//Ciclo che indaga la parte destra dell'immagine
			for(i=0; i<256-p_alto-p_basso; i++)
				for(j=0; j<(256-p_dx-p_sx)/2; j++)
					if(img_in->immagine.newimage.dato[p_alto+i][256-p_dx-j].grey<=70)
						area_scura1++;
					else if(img_in->immagine.newimage.dato[p_alto+i][256-p_dx-j].grey>=185)
						area_chiara1++;

//Area scura finale della sola parte interna al contorno
			area_scura1-=area_p;

			area_scura_mm1 = ((area_scura1*dim*dim)/(256*256))*100;
			area_chiara_mm1 = ((area_chiara1*dim*dim)/(256*256))*100;

//Stampa a video dei risultati ottenuti per la prima immagine
			printf("\n\nPrima immagine\n");
			printf("Il valore soglia scelto per individuare una zona scura e' 70.\nIl valore soglia scelto per individuare una zona chiara e' 185.\n");
			printf("\n\nArea scura della parte informativa\n-in pixel: %d\n-in mm^2: %.2f\n\n", area_scura1, area_scura_mm1);
			printf("\n\nArea chiara della parte informativa\n-in pixel: %d\n-in mm^2: %.2f\n\n", area_chiara1, area_chiara_mm1);


			loadBMP(img_in2->immagine.filename, &(img_in2->immagine.newimage));

//Operazioni sulla seconda immagine scelta dall'utente
//(Vedi sopra)
			do{
				printf("\n\nInserisci la dimensione della seconda immagine (quadrata) in centimetri: ");
				scanf("%f", &dim);
			}while(dim<=0);


			printf("\n\nInserire i centimetri di immagine da tagliare:");

			do{
				do{
					printf("\n-Dall'alto: ");
					scanf("%f", &alto);
				}while(alto<0);
				do{
					printf("\n-Dal basso: ");
					scanf("%f", &basso);
				}while(basso<0);
			}while((alto+basso)>=dim);


			do{
				do{
					printf("\n-Da destra: ");
					scanf("%f", &dx);
				}while(dx<0);
				do{
					printf("\n-Da sinistra: ");
					scanf("%f", &sx);
				}while(sx<0);
			}while((dx+sx)>=dim);

			p_alto=(alto*256)/dim;
			p_basso=(basso*256)/dim;
			p_dx=(dx*256)/dim;
			p_sx=(sx*256)/dim;

			for(i=0; i<256-p_alto-p_basso; i++)
				for(j=0; j<(256-p_dx-p_sx)/2+(256-p_dx-p_sx)%2; j++)
					if(img_in2->immagine.newimage.dato[p_alto+i][p_sx+j].grey>50)
						j=(256-p_dx-p_sx)/2+(256-p_dx-p_sx)%2;
					else area_p++;


			for(i=0; i<256-p_alto-p_basso; i++)
				for(j=0; j<(256-p_dx-p_sx)/2; j++)
					if(img_in2->immagine.newimage.dato[p_alto+i][256-p_dx-j].grey>50)
						j=(256-p_dx-p_sx)/2;
					else area_p++;



			for(i=0; i<(256-p_alto-p_basso); i++)
				for(j=0; j<(256-p_dx-p_sx)/2+(256-p_dx-p_sx)%2; j++)
					if(img_in2->immagine.newimage.dato[p_alto+i][p_sx+j].grey<=70)
						area_scura2++;
					else if(img_in2->immagine.newimage.dato[p_alto+i][p_sx+j].grey>=185)
						area_chiara2++;

			for(i=0; i<256-p_alto-p_basso; i++)
				for(j=0; j<(256-p_dx-p_sx)/2; j++)
					if(img_in2->immagine.newimage.dato[p_alto+i][256-p_dx-j].grey<=70)
						area_scura2++;
					else if(img_in2->immagine.newimage.dato[p_alto+i][256-p_dx-j].grey>=185)
						area_chiara2++;

			area_scura2-=area_p;

			area_scura_mm2 = ((area_scura2*dim*dim)/(256*256))*100;
			area_chiara_mm2 = ((area_chiara2*dim*dim)/(256*256))*100;

			printf("\n\nSeconda immagine\n");
			printf("Il valore soglia scelto per individuare una zona scura e' 70.\nIl valore soglia scelto per individuare una zona chiara e' 185.");
			printf("\n\nArea scura della parte informativa\n-in pixel: %d\n-in mm^2: %.2f", area_scura2, area_scura_mm2);
			printf("\n\nArea chiara della parte informativa\n-in pixel: %d\n-in mm^2: %.2f\n", area_chiara2, area_chiara_mm2);

//Stampa a video dei risultati ottenuti
			if(area_scura_mm1>area_scura_mm2){
				printf("\n\nL'area scura della prima immagine e' maggiore rispetto a quella della seconda immagine.\n");
				printf("La differenza e'\n-in pixel: %d\n-in mm^2: %.2f\n",area_scura1-area_scura2,area_scura_mm1-area_scura_mm2);
			}
			else if(area_scura_mm1<area_scura_mm2){
				printf("\n\nL'area scura della prima immagine e' minore rispetto a quella della seconda immagine.\n");
				printf("La differenza e'\n-in pixel: %d\n-in mm^2: %.2f\n",area_scura2-area_scura1,area_scura_mm2-area_scura_mm1);
			}
			else{
				printf("\n\nL'area scura della prima immagine e' uguale a quella della seconda immagine.\n");
				printf("La differenza e' nulla\n");
			}

			if(area_chiara_mm1>area_chiara_mm2){
				printf("\n\nL'area chiara della prima immagine e' maggiore rispetto a quella della seconda immagine.\n");
				printf("La differenza e'\n-in pixel: %d\n-in mm^2: %.2f\n",area_chiara1-area_chiara2,area_chiara_mm1-area_chiara_mm2);
			}
			else if(area_chiara_mm1<area_chiara_mm2){
				printf("\n\nL'area chiara della prima immagine e' minore rispetto a quella della seconda immagine.\n");
				printf("La differenza e'\n-in pixel: %d\n-in mm^2: %.2f\n",area_chiara2-area_chiara1,area_chiara_mm2-area_chiara_mm1);
			}
			else{
				printf("\n\nL'area chiara della prima immagine e' uguale a quella della seconda immagine.\n");
				printf("La differenza e' nulla\n");
			}
		}
}

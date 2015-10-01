#include "bmp.h"

imgcorr caricaimm();

struct miaStruttura* ordinamento(struct miaStruttura* testa, int* numeri);

struct miaStruttura* riconoscimento (char* str_ID, struct miaStruttura* lista, int num);

void num_grigi(struct miaStruttura* struct_img);

void distribuzione_grigi(struct miaStruttura* struct_img, int* v);

void riduzione(struct miaStruttura* struct_img);

void contorno(struct miaStruttura* img_in);

void area_contorno(struct miaStruttura* img_in);

void area_chiara_scura(struct miaStruttura* img_in);

void variazione_imm_consec(struct miaStruttura* img_in);

void variazione_imm_scelta(struct miaStruttura* img_in, struct miaStruttura* capolista, int num);

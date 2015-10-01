
// Dimensione immagine in pixel, quadrata
#define DATA_DIM 256


typedef struct{
	char anno[5];
	char mese[3];
	char giorno[3];
}data;


//Struttura per l'header
typedef struct {
   unsigned int size;
   unsigned short int reserved1;
   unsigned short int reserved2;
   unsigned int offset;
} Header;

//Struttura per l'header info
typedef struct {
   unsigned int size;
   int width,height;
   unsigned short int planes;
   unsigned short int bits;
   unsigned int compression;
   unsigned int imagesize;
   int xresolution,yresolution;
   unsigned int ncolours;
   unsigned int importantcolours;
} Info;

//Struttura per rappresentare un pixel. Nel nostro caso solo 8 bits, un solo campo
typedef struct {
   unsigned char grey;
} Pixel;

//Struttura immagine BMP
typedef struct{
	unsigned char magic[2];                     //Tipo di immagine
	Header header;                              //Header
	Info info;                                  //Info header
	unsigned char color_table[DATA_DIM*4];      //Tabella colori
	Pixel dato[DATA_DIM][DATA_DIM];             //Matrice di pixel
}BMP_Image;



//Struttura di supporto per l'immagine BMP
//Aggiunta di informazioni utilizzate per identificazione e storicizzazione
typedef struct{
	char IDfin[21];
	char ID[11];
	data dataimg;
	char filename[20];
	BMP_Image newimage;
} imgcorr;

//Struttura per allocazione dinamica
struct miaStruttura{
	imgcorr immagine;
	struct miaStruttura* next;
};



//Caricamento immagine
int loadBMP(char * filename, BMP_Image *img);

//Salvataggio immagine
int saveBMP(BMP_Image img, char * filename);

#include "d.h"
#include "stdio.h"

char* readcod(FILE *fp){
    fseek(fp, 0, SEEK_END);
    int size = ftell(fp);
    char *buffer = malloc(size);
    fseek(fp,0,SEEK_SET);
    fread(buffer,size,1,fp);
    return buffer;
}

int length ( char a[]){
    int i = 0;
    while (a[i]) {
        i++;
    }
    return i;
}

int bufcod( char buf[]){
    int i = 0, num = 4;

    while(num>0){
        if (buf[i] == '@'){
            num--;
        }
        i++;
    }

    return i;
}

void divide_bufcod (char *buf, int n_blocos, char** matriz){
    int i = bufcod(buf), j, k, at = 0;

    for (k = 0; k < n_blocos && buf[i]; k++){
        if (buf[i] == '@'){
            at++;
            while (at < 2){
                i++;
                if (buf[i] == '@') {
                    at++;
                    if(buf[i+1] == '0') return;
                }
            }
            i++;
        }
        for (j = 0; buf[i]!='@'; j++,i++){
            matriz[k][j] = buf[i];
        }
    }
}

Lista insere_cabeca (char* code, char c , Lista l){
    Lista n = malloc(sizeof (struct lista));
    (n)->c = c;
    strcpy((n)->code, code);
    (n)->prox = l;
    return n;
}

Lista buf_to_list( char* buffer, Lista lista){
    lista = NULL;
    int ascii = 0 , i=0, cc=0, control = 1, len = strlen(buffer);
    /*
     * ascii é a variável que representa o caratér correspondente ao código;
     * i é a variável que se usa para avançar no buffer;
     * cc é a variável usada para avançar no código;
     * control é a variável que verifica os casos de ';' consecutivos;
     * len é o comprimento do buffer;
     */

    char* c = malloc(sizeof(char) * 8); // Array com o código a colocar na lista

    for(;buffer[i];i++)
    {
        if (buffer[i] == ';')
        {
            if (control == 0)
            {
                c[cc] = '\0';
                lista = insere_cabeca(c, ascii, lista);
            }
            cc = 0;
            ascii++;
            control = 1; // O caratér anterior é o caratér ';'
        }

        else
        {
            c[cc] = buffer[i];
            control = 0; // O caratér anterior é um caratér diferente de ';'
            cc++;
            if (i==len-1)
                lista = insere_cabeca(c, ascii, lista);
        }
    }

    return lista;
}

char* tail (char a[]){
    int i =0;
    for (;a[i];i++){
        a[i]=a[i+1];
    }
    a[i]='\0';
    return a;
}

Btree altera_nodo ( char i, Btree t){
    if (t == NULL) {
        t = malloc(sizeof (struct btree));
        t->nodo = i;
        t->left = NULL;
        t->right = NULL;
    }
    else{
        t->nodo = i;
    }
    return t;
}

char* take(char a[], int i) {
    char *aux = malloc(sizeof( char)*i);
    int j;
    for(j=0; j<i; j++)
        aux[j]=a[j];
    aux[j]='\0';
    return aux;
}

Btree bseek( char a[], Btree t, char c){
    if (t == NULL) {
        t = malloc(sizeof (struct btree));
        t->nodo = c;
        t->left = NULL;
        t->right = NULL;
    }

    if (!a) t = altera_nodo(c,t);
    else if (a[0]=='0'){
        (t->left) = bseek(tail(a),(t->left),c);
    }
    else if (a[0]=='1'){
        (t->right) = bseek(tail(a),(t->right),c);
    }
    else t = altera_nodo(c,t);

    return t;
}

Btree constroi_arvore(char a[], Btree t,char c){
    int l = length(a);

    for (int i = 0; i < l; i++)
    {
        char* aux = take(a,i);
        t = (bseek(aux,t,'\0'));
    }
    t = (bseek(a,t,c));
    return t;
}

Btree list_tree (Lista l) {
    Btree t = malloc(sizeof(struct btree));
    t->nodo = '\0';
    t->left = NULL;
    t->right = NULL;
    for (;l != NULL; (l=l->prox))
    {
        t = constroi_arvore(l->code,(t),l->c);
    }
    return t;
}

Btree decode_block(char* a){
    Lista lista = NULL;
    lista = buf_to_list(a, lista);
    Btree t = list_tree(lista);

    return t;
}

char** decode_cod(char *fp_cod, int num_blocos,int *len_cod){
    FILE *cod = fopen(fp_cod,"r");

    char* buffer = readcod(cod);

    char** matriz = malloc(sizeof (char*) * num_blocos);
    for(int i=0; i<num_blocos; i++){
        matriz[i] = malloc(sizeof (char) * len_cod[i]);
    }

    divide_bufcod(buffer, num_blocos, matriz);

    return matriz;

}

//---------------------------------------------SHAF------------------------------------------

unsigned char* readshaf(FILE *fp){
    fseek(fp,0,SEEK_END);
    int size = ftell(fp);
    unsigned char *buffer = malloc(size);
    fseek(fp,0,SEEK_SET);
    fread(buffer,size,1,fp);
    return buffer;
}

int bufshaf(unsigned char buf[]){
    int i = 0, num = 3;

    while(num>0){
        if (buf[i] == '@'){
            num--;
        }
        i++;
    }

    return i; // apontador para o sítio onde começamos a ler o ficheiro efetivamente
}

void divide_bufshaf (unsigned char *buf, int n_blocos, unsigned char** matriz, int *len){
    int i = bufshaf(buf), j, k, at = 0;

    for (k = 0; k<n_blocos; k++){
        if (buf[i] == '@'){
            at++;
            while (at < 2){
                i++;
                if (buf[i] == '@') at++;
            }
            i++;
        }
        for (j = 0; j < len[k]; j++, i++){
            matriz[k][j] = buf[i];
        }
        at = 0;
    }
}

unsigned char* shaf_to_bin(unsigned char* buffer, int len){
    unsigned char *bin = malloc(sizeof (unsigned char)*(len*8));
    bin[0] = 129;
    int j,i;
    for(i = 0; i<len;i++){
        unsigned char c = buffer[i];
        for (j=7; j>=0; j--){
            bin[(i*8)+j]= c % 2 ;
            c = c/2;
        }
    }

    return bin;
}

int verifica_folha(Btree t){
    int r = 0; // 0 não é folha

    if(t->left == NULL && t->right == NULL)
        r = 1;

    return r;
}

FILE* limpaficheiro(char* fn){
    fclose(fopen(fn, "wb"));
    FILE *f = fopen("teste.txt", "ab");

    return f;
}

void descodificacao(FILE *fp, unsigned char* block,int len_block, Btree topo,int len_txt) {
    Btree t = topo;
    int i = 0, debug = 0;
    while ((i < (len_block*8)) && debug != len_txt) {
        for (; verifica_folha(t) != 1; i++) {
            if (block[i] == 0) {
                t = t->left;
            } else if (block[i] == 1) {
                t = t->right;
            }
        }
        fputc(t->nodo, fp);
        debug++;
        t = topo;
    }
}

int *len_block_txt(char *buf,int num_blocos,int*len_txt){
    for (int j = 0; j<num_blocos; j++){
        int num_a = 3 + (2*j);
        int i = 0, tamanho = 0;
        while (num_a>0){
            if (buf[i]== '@'){
                num_a--;
            }
            i++;
        }
        while(buf[i]!='@'){
            int k = buf[i] - '0';
            tamanho = tamanho * 10 + k;
            i++;
        }
        len_txt[j]=tamanho;
    }
    return len_txt;
}

int *len_block_shaf(FILE* shaf,int num_blocos, int* len_shaf){
    for (int j = 1; j<num_blocos; j++){
        fseek(shaf, len_shaf[j-1], SEEK_CUR);
        fscanf(shaf, "@%d@", &(len_shaf[j]));
    }
    return len_shaf;
}

int *len_block_cod(char* buf,int num_blocos, int* len_cod){
    for (int k = 0; k < num_blocos; k++) {
        int at = 0, i = 0;
        for (; at < 4 + (2 * k); i++) {
            if (buf[i] == '@')
                at++;
        }
        for (;buf[i] != '@';i++,len_cod[k]++);
    }
    return len_cod;
}

void decode_shaf(char* fp_shaf, char *fp_cod, char*filename){
    FILE *shaf = fopen(fp_shaf,"rb");
    if (fp_shaf == NULL) {
        printf("O ficheiro shaf não foi lido");
        return;
    }
    FILE *cod = fopen(fp_cod, "r");
    if (fp_cod == NULL) {
        printf("O ficheiro cod não foi lido");
        return;
    }

    int num_blocos = 0, *len_shaf = malloc(sizeof (int)),*len_txt = malloc(sizeof (int)),*len_cod = malloc(sizeof (int));
    char* buf = readcod(cod);

    fscanf(shaf, "@%d@%d@", &num_blocos, &(len_shaf[0]));
    // Tamanho dos blocos shaf
    len_shaf = len_block_shaf(shaf,num_blocos,len_shaf);

    // Tamanho dos blocos txt
    len_txt = len_block_txt(buf,num_blocos,len_txt);

    // Tamanho dos blocos cod
    len_cod = len_block_cod(buf,num_blocos,len_cod);

    // Descodificação

    unsigned char** matriz = malloc(sizeof (unsigned char*) * num_blocos);
    for(int i=0; i<num_blocos; i++){
        matriz[i] = malloc(sizeof (unsigned char) * (len_txt[i]));
    }

    unsigned char* buffer = readshaf(shaf);
    divide_bufshaf(buffer, num_blocos, matriz, len_shaf);
    Btree t;
    FILE *fp = limpaficheiro(filename);

    unsigned char* bufferdecode;
    char** matrizcod = decode_cod(fp_cod, num_blocos,len_cod);
    for(int i = 0; i < num_blocos; i++){
        t = decode_block(matrizcod[i]);
        bufferdecode=shaf_to_bin(matriz[i], len_shaf[i]);
        descodificacao(fp, bufferdecode, len_shaf[i], t,len_txt[i]);

    }
    fclose(fp);
    fclose(shaf);

}

//---------------------------------------------RLE------------------------------------------

char **divide_rle(char* buffer,int num_blocos, char** matriz_rle,int*len_block){
    int i = 0,j,k = 0;
    for (; i < num_blocos; i++){
        for (j = 0; j < len_block[i]; j++) {
            matriz_rle[i][j]=buffer[j+k];
        }
        k += j;
    }
    return matriz_rle;
}

void imprime_repetidos(FILE* fp, char* buffer, int i){
    unsigned char a = buffer[i],b=buffer[i+1];
    int c = (int) b;
    for (int j = 0; j < c; j++)
        fprintf(fp,"%c",a);
}

void decode_block_rle(char* buffer,int len_block, FILE* fp){
    for (int i = 0; i < len_block; i++) {
        if (buffer[i] == '\0'){
            imprime_repetidos(fp, buffer, i + 1);
            i += 2;
        }else
            fprintf(fp,"%c",buffer[i]);
    }
}

void decode_rle(char* fp_rle,char* fp_cod, char* filename){
    int num_blocos = 0,*len_blocos = malloc(sizeof (int));
    FILE *cod = fopen(fp_cod,"rb");
    FILE *rle = fopen(fp_rle,"rb");
    char *buf = readcod(rle), ** matriz = malloc(sizeof (char*) * num_blocos);;
    FILE *fp = limpaficheiro(filename);
    fscanf(cod, "@R@%d@%d@", &num_blocos, &(len_blocos[0]));

    // Tamanho dos blocos
    len_blocos = len_block_txt(buf,num_blocos,len_blocos);

    for(int i=0; i<num_blocos; i++){
        matriz[i] = malloc(sizeof (char) * (len_blocos[i]));
    }

    matriz = divide_rle(buf,num_blocos,matriz,len_blocos);
    for (int i = 0; i < num_blocos; i++) {
        decode_block_rle(matriz[i],len_blocos[i],fp);
    }
}

//---------------------------------------------OUTPUT------------------------------------------

void output(FILE*cod,FILE*shaf,FILE*rle, double tempo){
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    printf("\nLídia Sousa a93205 e João Santos a93235, MIEI/CD, %0d-%0d-%d\n",tm.tm_mday,tm.tm_mon + 1,tm.tm_year + 1900);
    printf("Módulo D (descodificação do ficheiro comprimido)\n");
    int bloco = 0,num_blocos = 0, *len_antes = malloc(sizeof (int)),*len_depois = malloc(sizeof (int));
    float taxa, media=0;
    char*buf = readcod(cod);
    fclose(cod);
    fscanf(shaf, "@%d@%d@", &num_blocos, &(len_depois[0]));

    // Tamanho dos blocos txt
    len_antes = len_block_txt(buf,num_blocos,len_antes);
    // Tamanho dos blocos shaf
    len_depois = len_block_shaf(shaf,num_blocos,len_depois);
    fclose(shaf);

    float numerador,denominador;
    for (; bloco < num_blocos; bloco++) {
        numerador = (float) len_depois[bloco],denominador = (float) len_antes[bloco];
        taxa = numerador / denominador;
        taxa = (1 - taxa)*100;
        media += taxa;
        printf("Tamanho antes/depois e taxa de compressão (bloco %d): %d/%d %.2f%% \n",bloco,len_antes[bloco],len_depois[bloco],taxa);
    }
    media /= (float) num_blocos;

    printf("Taxa de compressão global %.2f%% \n",media);
    printf("O programa demorou %f milissegundos \n", tempo );
}

//---------------------------------------------MAIN------------------------------------------

char* nomedoficheiro(char*filename, int tipo)
{
    char *final = malloc(sizeof(char) * (strlen(filename) - 3));
    int a = length(filename);
    for(int i = 0; i < (a-4); i++){
        final[i] = filename[i];
    }

    if (tipo == -1) {
        strcat(final, "txt");
    }
    if (tipo == 0) {
        strcat(final, "cod");
    }
    if (tipo == 1) {
        strcat(final, "rle");
    }
    if (tipo == 2) {
        strcat(final, "rle.cod");
    }
    if (tipo == 3) {
        strcat(final, "rle");
    }

    return final;
}

SINAL moduloD(char* filename) {
    clock_t tempoinicial = clock();
    FILE *fp = fopen(filename,"r"); //Abertura do ficheiro com a extensão .shaf
    if (fp == NULL) return FICHEIRO_DEMASIADO_PEQUENO;

    char *fp_cod = nomedoficheiro(filename,0); //Extensão cod no ficheiro
    FILE *cod = fopen(fp_cod, "r"); //Abertura do ficheiro cod
    char tipodedescompressao;
    fscanf(cod, "@%c@", &tipodedescompressao);

    char *fp_rle = nomedoficheiro(filename,1); //Extensão rle no ficheiro
    char *fp_rlecod = nomedoficheiro(filename,2); //Extensão
    char *nomefinal = nomedoficheiro(filename,-1); //Nome do ficheiro final

    if(tipodedescompressao=='N'){
        decode_shaf(filename, fp_cod, nomefinal); //Descodificação shaf
    }
    if(tipodedescompressao =='R'){
        decode_rle(fp_rle, fp_rlecod, nomefinal); //Descodificação rle
    }

    FILE *shaf = fopen(filename,"rb");
    FILE *rle = fopen(fp_rle, "rb");
    clock_t tempofinal = clock();
    double tempodeexec = (double)(tempofinal-tempoinicial) / CLOCKS_PER_SEC;

    output(cod,shaf,rle,tempodeexec);
    fclose(shaf);
    fclose(cod);
    fclose(rle);

    return OK;
}

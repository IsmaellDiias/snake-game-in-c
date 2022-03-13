#include <stdio.h>
#include <stdlib.h>
#include <time.h> //usada no rand pra pegar valor aleatorio de acordo com o horario
#include <termios.h> //usada na função do tales
#include <unistd.h> //usada na função do tales

#define DIMENSAO  10 //constante da dimensao da matriz
#define TAMANHO  100 // constante do tamanho da cobrinha

//FUNÇÃO GERAR UM NUMERO ALEATORIO,DENTRO DO INTERVALO ESTABELECIDO
int aleatorio(int inf, int sup){
	int valor;
	valor = inf+rand()%(sup-inf);
	return valor;
}

//FUNÇÃO DE PRESSIONAR A TECLA
int tecla(void) {
	struct termios TalesOntem, TalesHoje;
	char TalesSempre;
	tcgetattr( STDIN_FILENO, &TalesOntem );
	TalesHoje = TalesOntem;
	TalesHoje.c_lflag &= ~( ICANON | ECHO );
	tcsetattr( STDIN_FILENO, TCSANOW, &TalesHoje );
	//TalesSempre = getchar();
	scanf("%c",&TalesSempre);
	tcsetattr( STDIN_FILENO, TCSANOW, &TalesOntem );
	return TalesSempre;
}

struct posicao{
	int x;
	int y;};

//FUNÇÃO PONTEIRO NULO - confere se a alocação/realocação deu certo
void ponteiroNulo(struct posicao *v){
 	if(v == NULL){
   	printf("Não foi alocado/realocado\n");
   	exit(1);
	}
}

//FUNÇÃO GERA DOCE - muda a posição do doce na matriz, A nova posição deve ser aleatória e tem a recursão pra n pegar no corpo
void gerarDoce(struct posicao *doce,struct posicao m[], int dvida,int tam){
       	int k=0,a=0,i;
       	doce->x=aleatorio(0,10);
       	doce->y=aleatorio(1,11);
      	 
   	while(k==0){

         	for(i=0;i<tam-1;i++){
       		 if(doce->x==m[i].x && doce->y==m[i].y){
   		 doce->x=aleatorio(0,10);
      			 doce->y=aleatorio(1,11);
   		 k=0;
   		 break;}
   	 else{k=1;}
     	}
   	}dvida=10;}

//FUNÇÃO INICIALIZAR CONFIGURAÇÕES - inicializa configurações do jogo       
void inicializarConfiguracoes(struct posicao *doce,struct posicao **m, int dvida,int tam){
    
	*m = malloc(3*sizeof(struct posicao));
	ponteiroNulo(*m);
    
	//posições iniciais da cobrinha
	(*m)[0].x=0;
	(*m)[0].y=3;
	(*m)[1].x=0;
	(*m)[1].y=2;
	(*m)[2].x=0;
	(*m)[2].y=1;
    
	gerarDoce(doce,*m,dvida,tam);  //gera o doce   
	srand( (unsigned)time(NULL) ); //pra funcao aleatorio pegar um valor apartir do tempo
	system("clear");   		//limpa a tela
   	 
}
   		 
//VARIAVEL GLOBAL É A MATRIZ DO JOGO   		 
char rota, matriz[DIMENSAO][12] = {"|          |","|          |","|          |","|          |","|          |",
	"|          |","|          |","|          |","|          |","|          |"};  

char letra;
int i,j,x,y,l,tam,dvida;

int main(){
 FILE *file;
 	system("clear");
    	puts("'N - Novo Jogo / C - Continua'");
    	letra = tecla();
         
     struct posicao *m;
    
	struct posicao doce; //variavel pra guardar a posicao do doce
        
        switch(letra){
    	case 'c':
        case 'C':{
		 system("clear");
        	 file = fopen("salve.txt", "rb"); // abrindo aquivo para leitura em formato binario
        	
        	 fread(&doce, sizeof(struct posicao),1,file);
        	 fread(&dvida, sizeof(int),1,file);
        	 fread(&tam, sizeof(int),1,file);
        	 m = malloc(sizeof(struct posicao)*tam);
        	 fread(m, sizeof(struct posicao),tam,file);
        	 
                 fclose(file); // fechando arquivo
        	 
        	 break;
        	} 
        	
        case 'n':
        case 'N':{
        	 printf("Novo \n");
        	 x=0,y=3,l=0,tam=3,dvida=10; //x,y er a posicao da cabeça; l er pro while, tam da cobra, vida do doce
        	 inicializarConfiguracoes(&doce,&m,dvida,tam);
        	 break;
        	}
        }	
	
	x=m[0].x;
        y=m[0].y;
	while(l==0){             	// enquanto l==0 o jogo prossegue
  	 
   	if(dvida==0){                 	//se n comer a tempo
       	matriz[doce.x][doce.y]=' ';
       	gerarDoce(&doce,m,dvida,tam);
       	matriz[doce.x][doce.y]='#';
       	dvida=10;}
  	 
   	if(dvida==11){        	//se comer
       	gerarDoce(&doce,m,dvida,tam);
       	matriz[doce.x][doce.y]='#';
       	dvida=10;}
  	 
   	for (i=0;i<10;i++){         	//limpa a matriz
           	for (j=1;j<11;j++){    	 
                  	matriz[i][j]=' ';}}
                 	 
   	for (i=0;i<tam;i++){          	//o corpo da cobrinha é posto na matriz
        	matriz[m[i].x][m[i].y]='*';}
       	 
   	matriz[doce.x][doce.y]='#'; //o doce é posto na matriz 
  	 
   	for (i=0;i<10;i++){         	//printa a matriz nas posiçoes i j, e o %2c é para pegar o
     	for (j=0;j<12;j++){    	 
         	printf("%2c",matriz[i][j]);}
     	printf("\n");}
     	
    printf("\n        Pontuação: %d",tam-3);
    printf("\n");
  
   	rota = tecla();
   	
   	if(rota=='r')
        	{
        	 file = fopen("salve.txt", "wb");
        	 
        	 fwrite(&doce, sizeof(struct posicao),1,file);
        	 fwrite(&dvida, sizeof(int),1,file);
        	 fwrite(&tam, sizeof(int),1,file);
        	 fwrite(m, sizeof(struct posicao),tam,file);
        	 
		 fclose(file); // fechando arquivo
        	 rota=tecla();
        	} 
  	 
   	switch(rota){
      	 
       	case 'w': // caso 'w' entra nessa condiçao
       	 
    	if(matriz[x-1][y]=='*' && m[1].x==x-1 && m[1].y==y){	//comando pra impedir dela voltar pra trás
            	system("clear");
            	break;}                                         	//precisa do break senão essa vai ficar repetindo os codigos  
       	 
        	if(matriz[x-1][y]=='*' && m[tam-1].x==x-1 && m[tam-1].y==y){ //serve pro caso dela n se tacar no rabo se for pra cima dele
            	for(i=tam-1;i>0;i--){
                    	m[i].x=m[i-1].x;
                    	m[i].y=m[i-1].y;}
               	m[0].x=(m[0].x)-1;
               	x=x-1;
               	dvida=dvida-1;
               	system("clear");
            	break;}
           	 
        	if(x>0 && matriz[x-1][y]!='*'){ //se for diferente de '*', ou é '#' ou é ' '
       		    
       			if(matriz[x-1][y]=='#'){   			 //caso seja '#"
                	m = realloc(m,(tam+1)*sizeof(struct posicao));
                	ponteiroNulo(m);
                	tam=tam+1;
                	for(i=tam-1;i>0;i--){
                    	m[i].x=m[i-1].x;
                    	m[i].y=m[i-1].y;}
                	m[0].x=(m[0].x)-1;
                	gerarDoce(&doce,m,dvida,tam);
                	matriz[doce.x][doce.y]='#';
                	dvida=10;
                	system("clear");}
               	 
               	if(matriz[x-1][y]==' '){                       	//caso seja ' '
                	for(i=tam-1;i>0;i--){
                    	m[i].x=m[i-1].x;
                    	m[i].y=m[i-1].y;  
                	}
                	m[0].x=(m[0].x)-1;
                	dvida=dvida-1;
                	system("clear");}
               	x=x-1;}
               	 
        	else{l=1;} //se n estiver nas opções
        	break;
  	 
       	case 's': // caso s, entra na condiçao
        	 
        	if(matriz[x+1][y]=='*' && m[1].x==x+1 && m[1].y==y){   //se for diferente de '*', ou é '#' ou é ' '
            	system("clear");
            	break;}    		 
            	 
        	if(matriz[x+1][y]=='*' && m[tam-1].x==x+1 && m[tam-1].y==y){ //serve pro caso dela n se tacar no rabo se for pra cima dele    
            	for(i=tam-1;i>0;i--){
                    	m[i].x=m[i-1].x;
                    	m[i].y=m[i-1].y; }
               	m[0].x=(m[0].x)+1;
               	x=x+1;
               	dvida=dvida-1;
               	system("clear");
            	break;}
                       	 
        	if(x<9 && matriz[x+1][y]!='*'){   			 ////se for diferente de '*', ou é '#' ou é ' '
              	 
               	if(matriz[x+1][y]=='#'){
                	m = realloc(m,(tam+1)*sizeof(struct posicao));
                	ponteiroNulo(m);
                	tam=tam+1;
                	for(i=tam-1;i>0;i--){
                    	m[i].x=m[i-1].x;
                    	m[i].y=m[i-1].y;}
                	m[0].x=(m[0].x)+1;
                	gerarDoce(&doce,m,dvida,tam);
                	matriz[doce.x][doce.y]='#';
                	dvida=10;
                	system("clear");}
               	 
               	if(matriz[x+1][y]==' '){
               	for(i=tam-1;i>0;i--){
                    	m[i].x=m[i-1].x;
                    	m[i].y=m[i-1].y;}
                	m[0].x=(m[0].x)+1;
               	dvida=dvida-1;
               	system("clear");}
       			x=x+1;}
       		    
        	else{l=1;}
        	break;
         	 
       	case 'a':
       	 
        	if(matriz[x][y-1]=='*' && m[1].x==x && m[1].y==y-1){    
            	system("clear");
            	break;}
           	 
        	if(matriz[x][y-1]=='*' && m[tam-1].x==x && m[tam-1].y==y-1){    
            	for(i=tam-1;i>0;i--){
                    	m[i].x=m[i-1].x;
                    	m[i].y=m[i-1].y;}
               	m[0].y=(m[0].y)-1;
               	y=y-1;
               	dvida=dvida-1;
               	system("clear");
            	break;}
           	 
        	if(y>1 && matriz[x][y-1]!='*'){
            	if(matriz[x][y-1]=='#'){
                	m = realloc(m,(tam+1)*sizeof(struct posicao));
                	ponteiroNulo(m);
                	tam=tam+1;
            	for(i=tam-1;i>0;i--){
                    	m[i].x=m[i-1].x;
                    	m[i].y=m[i-1].y;}
            	m[0].y=(m[0].y)-1;
            	gerarDoce(&doce,m,dvida,tam);
            	matriz[doce.x][doce.y]='#';
            	dvida=10;
            	system("clear");}
            	if(matriz[x][y-1]==' '){
               	for(i=tam-1;i>0;i--){
                    	m[i].x=m[i-1].x;
                    	m[i].y=m[i-1].y;}
                	m[0].y=(m[0].y)-1;
                	dvida=dvida-1;
                	system("clear");}
       		 y=y-1;}
       		 
           	else{l=1;}
           	break;
         	 
       	case 'd':
         	 
        	if(matriz[x][y+1]=='*' && m[1].x==x && m[1].y==y+1){    
            	system("clear");
            	break;}
           	 
        	if(matriz[x][y+1]=='*' && m[tam-1].x==x && m[tam-1].y==y+1){    
            	for(i=tam-1;i>0;i--){
                    	m[i].x=m[i-1].x;
                    	m[i].y=m[i-1].y;}
            	m[0].y=(m[0].y)+1;
            	y=y+1;
            	dvida=dvida-1;
            	system("clear");
            	break;}
           	 
            	if(y<10 && matriz[x][y+1]!='*'){
              	 
               	if(matriz[x][y+1]=='#'){
                	m = realloc(m,(tam+1)*sizeof(struct posicao));
                	ponteiroNulo(m);
                	tam=tam+1;
                	for(i=(tam-1);i>0;i--){
                    	m[i].x=m[i-1].x;
                    	m[i].y=m[i-1].y;}
                	m[0].y=(m[0].y)+1;
                	gerarDoce(&doce,m,dvida,tam);
                	matriz[doce.x][doce.y]='#'; //posicao do doce
                	dvida=10;
                	system("clear");}
               	 
               	if(matriz[x][y+1]==' '){
               	for(i=tam-1;i>0;i--){
                    	m[i].x=m[i-1].x;
                    	m[i].y=m[i-1].y;}
               	m[0].y=(m[0].y)+1;
               	dvida=dvida-1;
               	system("clear");}
          			y=y+1;}
           	 
           	else{l=1;}
           	break;
         	 
       	case 'q':
           	l=3; // o l vai receber 3, e quando for chegar a condiçao para poder entra no laço, vai dar falso e saira
           	system("clear"); //limpa o terminal
           	break;
     	 
       	default:
           	system("clear");
           	printf("As unicas teclas possíveis são: A D W S e Q");
           	printf("\n");
           	break;}
          	 
	if(tam==TAMANHO){l=2;}}


if (l==1){
	system("clear");
	printf("Game Over!\n");}

if (l==2){
	system("clear");
	printf("Parabéns, você venceu!! :D\n");}

free(m); //a região do ponteiro de posição da minhoca deve ser liberado e o jogo deve ser encerrado
system(0);}

//quando L for igual a
//0 - o jogo continua
//1 - perdeu
//2 - ganhou
//3 - apertou q

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

//Struct tabela Palavras Por Pontos.
typedef struct palavrasporpontos {
    char matrizpalavras[50][16];
    int matrizpontos[50];
    int matrizturno[50];
}tPPP;

//Struct jogo.
typedef struct jogo {
    char nomejogador1[16], nomejogador2[16];
    char matriz[100][100], matrizEscondida[100][100];
    int tam;
    int turnos, turnoatual;
    int maxpalavras;
    char palavrasJ1[25][16], palavrasJ2[25][16];
    int verificaPalavrasJ1[25], verificaPalavrasJ2[25];
    int pontuacaoJ1, pontuacaoJ2;
    tPPP ppp;
    int pontosfeitosJ1, pontosfeitosJ2, pontosrecebidosJ1, pontosrecebidosJ2;
    int mapaordenado[100][100], ordemdapalavra;
} tJogo;

//Funcao auxiliar que completa os espacos em branco da tabela de palavras restantes.
int completaEspacos (int num, char nome[]){
    return num-strlen(nome);
}

//Funcao auxiliar que imprime em "Inicializacao.txt" a maior palavra e as Letras Iniciais.
void ImprimeMaiorPalavraELetrasIniciais(FILE* inic, tJogo jogo, int jogador){
    int tammaior=0, maiorp=0, i, letras[26], tam;
    for(i=0;i<26;i++){
        letras[i]=0;
    }
    fprintf(inic, "Maior Palavra: \n");
//  Caso jogador1.
    if(jogador==1){
        for (i=0; i<jogo.maxpalavras; i++){
            tam=strlen(jogo.palavrasJ1[i]);
            if (tam>tammaior){
                tammaior=tam;
                maiorp=i;
            }
            int aux=jogo.palavrasJ1[i][0]-'a';
            letras[aux]+=1;
        }
        fprintf(inic, "%s\n", jogo.palavrasJ1[maiorp]);
        fprintf(inic, "Letras Iniciais: \n");
        for(i=0;i<26;i++){
            if(letras[i]>0){
                fprintf(inic, "%c -> %d\n", i+'a', letras[i]);
            }
        }
        fprintf(inic, "\n");
    }
//  Caso jogador2.
    if(jogador==2){
        for (i=0; i<jogo.maxpalavras; i++){
            tam=strlen(jogo.palavrasJ2[i]);
            if (tam>tammaior){
                tammaior=tam;
                maiorp=i;
            }
            int aux=jogo.palavrasJ2[i][0]-'a';
            letras[aux]+=1;
        }
        fprintf(inic, "%s\n", jogo.palavrasJ2[maiorp]);
        fprintf(inic, "Letras Iniciais: \n");
        for(i=0;i<26;i++){
            if(letras[i]>0){
                fprintf(inic, "%c -> %d\n", i+'a', letras[i]);
            }
        }
    }
}

//Funcao auxiliar que gera arquivo de inicializacao.
void GeraInicializacao(tJogo jogo, char saida[]){
    int i;
    char start[2000];
    
    FILE*inic;
    strcpy(start, saida);
    strcat(start, "Inicializacao.txt");
    inic = fopen(start, "w");
    
//  Jogador1.
    fprintf(inic, "--Jogador 1--\n");
    fprintf(inic, "Nome: %s\n", jogo.nomejogador1);
    fprintf(inic, "Palavras:\n");
    for(i=0;i<jogo.maxpalavras;i++){
        fprintf(inic, "%s\n", jogo.palavrasJ1[i]);
    }
    ImprimeMaiorPalavraELetrasIniciais(inic, jogo, 1);
    
//  Jogador2.
    fprintf(inic, "--Jogador 2--\n");
    fprintf(inic, "Nome: %s\n", jogo.nomejogador2);
    fprintf(inic, "Palavras:\n");
    for(i=0;i<jogo.maxpalavras;i++){
        fprintf(inic, "%s\n", jogo.palavrasJ2[i]);
    }
    ImprimeMaiorPalavraELetrasIniciais(inic, jogo, 2);
}

//Funcao auxiliar que gera arquivo de estatisticas para analise.
void GeraEstatisticas(tJogo jogo, char saida[]){
    int i, j, auxpo=0, auxt=0;
    char auxpa[16], estat[2000];
    
    FILE*est;
    strcpy(estat, saida);
    strcat(estat, "Estatisticas.txt");
    est = fopen(estat, "w");
    
//  Cabecalho pontos.
    fprintf(est, "--------------\n");
    fprintf(est, "--- PONTOS ---\n");
    fprintf(est, "--------------\n");
    fprintf(est, "\n");
    //Jogador 1.
    fprintf(est, "%s\n", jogo.nomejogador1);
    fprintf(est, "Feitos: %d\n", jogo.pontosfeitosJ1);
    fprintf(est, "Recebidos: %d\n", jogo.pontosrecebidosJ1);
    fprintf(est, "Total: %d\n", jogo.pontuacaoJ1);
    fprintf(est, "\n");
    //Jogador 2.
    fprintf(est, "%s\n", jogo.nomejogador2);
    fprintf(est, "Feitos: %d\n", jogo.pontosfeitosJ2);
    fprintf(est, "Recebidos: %d\n", jogo.pontosrecebidosJ2);
    fprintf(est, "Total: %d\n", jogo.pontuacaoJ2);
    fprintf(est, "\n");
//  Cabecalho palavras por pontos.
    fprintf(est, "---------------------------\n");
    fprintf(est, "--- PALAVRAS POR PONTOS ---\n");
    fprintf(est, "---------------------------\n");
    fprintf(est, "\n");
    //Tabela.
    fprintf(est, "|P|     Palavra    | T |\n");
    fprintf(est, "|-|----------------|---|\n");
//  REORGANIZAR ANTES DE IMPRIMIR (EM ORDEM DE PONTOS-ALFABETICA).
//  Ordem de pontos.
    for(i=0;i<2*jogo.maxpalavras;i++){
        for(j=i;j<2*jogo.maxpalavras;j++){
            if(jogo.ppp.matrizpontos[j]>jogo.ppp.matrizpontos[i]){
//              Troca ponto.
                auxpo=jogo.ppp.matrizpontos[j];
                jogo.ppp.matrizpontos[j]=jogo.ppp.matrizpontos[i];
                jogo.ppp.matrizpontos[i]=auxpo;
//              Troca palavra.
                strcpy(auxpa, jogo.ppp.matrizpalavras[j]);
                strcpy(jogo.ppp.matrizpalavras[j], jogo.ppp.matrizpalavras[i]);
                strcpy(jogo.ppp.matrizpalavras[i], auxpa);
//              Troca turno.
                auxt=jogo.ppp.matrizturno[j];
                jogo.ppp.matrizturno[j]=jogo.ppp.matrizturno[i];
                jogo.ppp.matrizturno[i]=auxt;
            }
//          Mesma pontuacao = ordem alfabetica.
            if(jogo.ppp.matrizpontos[j]==jogo.ppp.matrizpontos[i]){
                if(strcmp(jogo.ppp.matrizpalavras[j], jogo.ppp.matrizpalavras[i])<0){
//              Troca ponto.
                auxpo=jogo.ppp.matrizpontos[j];
                jogo.ppp.matrizpontos[j]=jogo.ppp.matrizpontos[i];
                jogo.ppp.matrizpontos[i]=auxpo;
//              Troca palavra.
                strcpy(auxpa, jogo.ppp.matrizpalavras[j]);
                strcpy(jogo.ppp.matrizpalavras[j], jogo.ppp.matrizpalavras[i]);
                strcpy(jogo.ppp.matrizpalavras[i], auxpa);
//              Troca turno.
                auxt=jogo.ppp.matrizturno[j];
                jogo.ppp.matrizturno[j]=jogo.ppp.matrizturno[i];
                jogo.ppp.matrizturno[i]=auxt;
                }
            }
        }
    }
//  IMPRIMIR.
    for(i=0; i<(2*jogo.maxpalavras); i++){       
        fprintf (est, "|%d|%s", jogo.ppp.matrizpontos[i], jogo.ppp.matrizpalavras[i]);
        for(j=0;j<completaEspacos(16, jogo.ppp.matrizpalavras[i]);j++){
            fprintf(est, " ");
        } 
    fprintf(est, "|%03d|\n", jogo.ppp.matrizturno[i]);
    }
}

//Funcao auxiliar que gera o arquivo de Mapa Ordenado (BONUS).
void GeraMapaOrdenado(tJogo jogo, char saida[]){
    int i, j;
    char mp[2000];
    
    FILE*mapaor;
    strcpy(mp, saida);
    strcat(mp, "MapaOrdenado.txt");
    mapaor = fopen(mp, "w");
    
//  Imprime valores (quando presentes) ou '--' (quando nao ha valor presente).
    for(i=0;i<jogo.tam;i++){
        for(j=0;j<jogo.tam;j++){
            if (jogo.mapaordenado[i][j]!=99999){
                fprintf(mapaor, " %02d", jogo.mapaordenado[i][j]);
            }
            else{
                fprintf(mapaor, " --");
            }
        }
        fprintf(mapaor, "\n");
    }
}

//Funcao auxiliar que verifica se a palavra foi ou nao encontrada.
int VerificaPalavraEncontrada(tJogo jogo, int indice, int jogador){
    for( ;indice<jogo.maxpalavras; indice++){
        if (jogador==1){
            if(jogo.verificaPalavrasJ1[indice]==0){
                return indice;
            }
        }
        if (jogador==2){
            if(jogo.verificaPalavrasJ2[indice]==0){
                return indice;
            }
        }
    }
    return jogo.maxpalavras+1;
}

//Funcao auxiliar que verifica se algum jogador ja encontrou todas as palavras.
int AcabaramAsPalavras(tJogo jogo, int jogador){
    int i;
//  Percorre o vetor 'verificaPalavras' para os dois jogadores e retorna verdadeiro caso alguma lista ja tenha sido toda encontrada.
    if(jogador==1){
        for(i=0; i<jogo.maxpalavras; i++){
            if(jogo.verificaPalavrasJ1[i]==1){
                continue;
            }
            else{
                return 0;
            }
        }
        return 1;
    }
    if(jogador==2){
        for(i=0; i<jogo.maxpalavras; i++){
            if(jogo.verificaPalavrasJ2[i]==1){
                continue;
            }
            else{
                return 0;
            }
        }
        return 1;
    }
}

//Funcao auxiliar que imprime a tabela de palavras restantes.
void MostraPalavrasRestantes(tJogo jogo) {
    int i;
//  CABECALHO
 //CORRECAO: remocao do \n.
    printf ("|            Palavras Restantes           |\n");
    printf ("|%s", jogo.nomejogador1);
    for(i=0; i<completaEspacos(16, jogo.nomejogador1); i++){
        printf(" ");
    }
    printf("(%02d)|%s", jogo.pontuacaoJ1, jogo.nomejogador2);
    for(i=0; i<completaEspacos(16, jogo.nomejogador2); i++){
        printf(" ");
    }
    printf ("(%02d)|\n", jogo.pontuacaoJ2);
    printf ("|--------------------|--------------------|\n");
    
    int indiceJ1=0, indiceJ2=0, aux1, aux2;
//  PALAVRAS
    for ( ;indiceJ1<jogo.maxpalavras || indiceJ2<jogo.maxpalavras; indiceJ1++, indiceJ2++){
        aux1 = VerificaPalavraEncontrada(jogo, indiceJ1, 1);
        aux2 = VerificaPalavraEncontrada(jogo, indiceJ2, 2);
        indiceJ1 = aux1;
        indiceJ2 = aux2;
      
//      Caso 1) Restando palavras para os dois jogadores.  
        if(aux1<jogo.maxpalavras && aux2<jogo.maxpalavras){
            printf("|%s", jogo.palavrasJ1[aux1]);
            for(i=0; i<completaEspacos(20, jogo.palavrasJ1[aux1]); i++){
                printf(" ");
            }
             printf("|%s", jogo.palavrasJ2[aux2]);
            for(i=0; i<completaEspacos(20, jogo.palavrasJ2[aux2]); i++){
                printf(" ");
            }
            printf("|\n"); //CORRECAO: adicao do \n.
        }
//      Caso 2) Restando palavras apenas para o jogador 2.  
        else if(aux1>jogo.maxpalavras && aux2<jogo.maxpalavras) {
            printf("|");
            for(i=0; i<20; i++){
                printf(" ");
            }
            printf("|%s", jogo.palavrasJ2[aux2]);
            for(i=0; i<completaEspacos(20, jogo.palavrasJ2[aux2]); i++){
                printf(" ");
            }
            printf("|\n"); //CORRECAO: adicao do \n.
        }
//      Caso 3) Restando palavras apenas para o jogador 1. 
        else if (aux1<jogo.maxpalavras && aux2>jogo.maxpalavras){
            printf("|%s", jogo.palavrasJ1[aux1]);
            for(i=0; i<completaEspacos(20, jogo.palavrasJ1[aux1]); i++){
                printf(" ");
            }
            printf("|");
            for(i=0; i<20; i++){
                printf(" ");
            }
            printf("|\n"); //CORRECAO: adicao do \n.
        }
        // CORRECAO: remocao do \n.
    }
    printf("\n");
}

//Funcao auxiliar que realiza modificacoes no tabuleiro seguido de sua impressao.
void MostraTabuleiro(tJogo jogo){
    int i, j;
    
    printf("   "); //CORRECAO: Menos um espacamento.
//  Imprime a 'relacao de coordenadas' do tabuleiro de acordo com o tamanho lido em config.txt.
    for(i=0; i<jogo.tam; i++){
        printf(" %02d", i); //CORRECAO: mudança da posicao do espaco (de antes para depois).
    }
    printf("\n");
    for(i=0; i<jogo.tam; i++){
        printf ("%02d|", i);
//      Imprime as modificacoes das jogadas realizadas.
        for(j=0; j<jogo.tam; j++) {
            printf(" %c ", jogo.matrizEscondida[i][j]);
        }
        printf("|\n");
    }
    printf("\n");
}

//Funcao auxiliar que pede as cordenadas ao jogador especificado.
void PedeJogada(char nomejogador[]){
    printf("%s por favor entre com as coordenadas para a sua jogada:\n", nomejogador);
}

//Funcao auxiliar que imprime mensagem de coordenadas ja jogadas.
void CoordenadasJaJogadas(char nomejogador[]){
    printf ("Coordenadas ja jogadas.%s por favor entre com novas coordenadas para a sua jogada:\n", nomejogador); //CORRECAO: remocao de um espacamento.
}

//Funcao auxiliar que imprime mensagem de coordenadas fora do tabuleiro.
void CoordenadasFora(char nomejogador[]){
    printf ("Coordenadas fora do tabuleiro.%s por favor entre com novas coordenadas para a sua jogada:\n", nomejogador); //CORRECAO: remocao de um espacamento.
}

//Funcao auxiliar que transfere da matriz 'respostas/original' para a matriz 'escondida' as palavras encontradas e implementa estatisticas.
tJogo procuraPalavra(tJogo jogo, char palavra[], int donoDaPalavra, int jogadorDaVez, int x, int y, int indice) {
    int i, j, m, n, achou=0, ordemdapalavra;
    
//  Verifica se a letra da coordenada escolhida corresponde a primeira letra de alguma das palavras das listas.
    for(i=-1; i<=1 && !achou; i++){
        for(j=-1; j<=1 && !achou; j++){
            if(i==0 && j==0){
                continue;
            }
           
            int a, acertos=0;
    
            for(a=0; a<strlen(palavra); a++){
                if(x+a*i < 0 || x+a*i >= jogo.tam || y+a*j < 0 || y+a*j >= jogo.tam) {
                    break;
                }
                if(tolower(palavra[a]) == tolower(jogo.matriz[x+a*i][y+a*j])){
                    acertos++;
                }
            }
            achou = acertos == strlen(palavra);

            if(achou) {
                m = i;
                n = j;
//              Implementa no vetor "verificaPalavras" que a palavra foi encontrada.
                if (donoDaPalavra==1){
                    jogo.verificaPalavrasJ1[indice]=1;
                }
                if (donoDaPalavra==2){
                    jogo.verificaPalavrasJ2[indice]=1;
                }
//              Implementa pontuacao e guarda turno.
                if(donoDaPalavra == jogadorDaVez) {
                    if(jogadorDaVez == 1) {
                        jogo.pontuacaoJ1+=2;
                        jogo.pontosfeitosJ1+=2;
                        jogo.ppp.matrizpontos[indice]=2;
                        jogo.ppp.matrizturno[indice]=2*jogo.turnoatual-1;
                    } 
                    else {
                        jogo.pontuacaoJ2+=2;
                        jogo.pontosfeitosJ2+=2;
                        jogo.ppp.matrizpontos[jogo.maxpalavras+indice]=2;
                        jogo.ppp.matrizturno[jogo.maxpalavras+indice]=2*jogo.turnoatual;
                    }
                } 
                else {
                    if(jogadorDaVez == 1) {
                        jogo.pontuacaoJ2+=1;
                        jogo.pontosrecebidosJ2+=1;
                        jogo.ppp.matrizpontos[jogo.maxpalavras+indice]=1;
                        jogo.ppp.matrizturno[jogo.maxpalavras+indice]=2*jogo.turnoatual-1;
                    } 
                    else {
                        jogo.pontuacaoJ1+=1;
                        jogo.pontosrecebidosJ1+=1;
                        jogo.ppp.matrizpontos[indice]=1;
                        jogo.ppp.matrizturno[indice]=2*jogo.turnoatual;
                    }
                }
            }
        }
    }
    if(achou){
        int a;
        jogo.ordemdapalavra++;
        if(jogo.mapaordenado[x][y] > jogo.ordemdapalavra) {
            jogo.mapaordenado[x][y] = jogo.ordemdapalavra;
        }
//      Realiza transferencia da matriz 'respostas/original' para a matriz 'escondida'.
        for(a=1; a<strlen(palavra); a++) {
            jogo.matrizEscondida[x+a*m][y+a*n] = jogo.matriz[x+a*m][y+a*n];
//          Implementa informacoes do turno em Mapa Ordenado.
            if(jogo.mapaordenado[x+a*m][y+a*n] > jogo.ordemdapalavra) {
                jogo.mapaordenado[x+a*m][y+a*n] = jogo.ordemdapalavra;
            }
        }
    }
    return jogo;
}

//Funcao auxiliar que transforma a letra minuscula em maiuscula na coordenada escolhida.
tJogo RealizaJogada (tJogo jogo, int jogador, int a, int b) {
    int i;
//  Transforma a letra minuscula da coordenada escolhida em maiuscula.
    jogo.matriz[a][b] = jogo.matriz[a][b] - 'a' + 'A';
    jogo.matrizEscondida[a][b] = jogo.matriz[a][b];
    for (i=0; i<jogo.maxpalavras; i++){
        if(tolower(jogo.palavrasJ1[i][0]) == tolower(jogo.matriz[a][b])){
            if(jogo.verificaPalavrasJ1[i]==0){
                jogo = procuraPalavra(jogo, jogo.palavrasJ1[i], 1, jogador, a, b, i);
            }
        }
        if(tolower(jogo.palavrasJ2[i][0]) == tolower(jogo.matriz[a][b])){
            if(jogo.verificaPalavrasJ2[i]==0){
                jogo = procuraPalavra(jogo, jogo.palavrasJ2[i], 2, jogador, a, b, i);
            }
        }
    }
    return jogo;
}

//Funcao auxiliar que finaliza o jogo e apresenta as estatisticas.
void ResultadoDoJogo (tJogo jogo, char saida[]){
//  Caso 1) Vitoria do jogador 1.
    if(jogo.pontuacaoJ1>jogo.pontuacaoJ2){
        printf ("%s Ganhou!!!\n", jogo.nomejogador1);
    }
//  Caso 2) Vitoria do jogador 2.
    if(jogo.pontuacaoJ1<jogo.pontuacaoJ2){
        printf ("%s Ganhou!!!\n", jogo.nomejogador2);
    }
//  Caso 3) Empate.
    if(jogo.pontuacaoJ1==jogo.pontuacaoJ2){
        printf("Empate\n");
    }
    
//  Gera arquivos.
    GeraInicializacao(jogo, saida);
    GeraEstatisticas(jogo, saida);
    GeraMapaOrdenado(jogo, saida);
    
//  Finaliza o programa.
    exit(0);
}

int main(int argc, char** argv) {
    tJogo jogo;
    int i, j, t;
    
//(PASSO1) Ler o arquivo de configuracao "config.txt".
    if (argc <= 1){
        printf ("ERRO: O diretorio de arquivos de configuracao nao foi informado!\n");
        exit(0);
    }
    
    char diretorio[1000], saida[2000];
    strcpy(diretorio, argv[1]);
    strcpy(saida, diretorio);
    strcat(saida, "saida/");
    strcat(diretorio, "config.txt");
    
    FILE*arquivo;
    arquivo = fopen(diretorio, "r");
    
    if (arquivo==NULL){
        printf("ERRO: Impossivel ler o arquivo config.txt no diretorio %s\n", argv[1]);
        exit(0);
    }
    
//  Leitura do config.txt:
    while(!feof(arquivo)){
//      Le numero maximo de turnos do jogo.
        fscanf(arquivo, "%d", &jogo.turnos);     
//      Le tamanho da matriz(quadrada).
        fscanf(arquivo, "%d", &jogo.tam);
//      Le matriz de letras.
        for(i=0; i<jogo.tam; i++){
            fscanf(arquivo, " %s", jogo.matriz[i]); 
        } 
//      Le numero de palavras de cada jogador.  
        fscanf(arquivo, "%d", &jogo.maxpalavras);
//      Le palavras jogador1.
        
        int indiceppp=0;
        for(i=0; i<jogo.maxpalavras; i++){
            fscanf(arquivo, " %s", jogo.palavrasJ1[i]);
            strcpy(jogo.ppp.matrizpalavras[indiceppp],jogo.palavrasJ1[i]);
            indiceppp++;
        }
//      Le palavras jogador2.
        for(i=0; i<jogo.maxpalavras; i++){
            fscanf(arquivo, " %s", jogo.palavrasJ2[i]);
            strcpy(jogo.ppp.matrizpalavras[indiceppp],jogo.palavrasJ2[i]);
            indiceppp++;
        } 
    }
    
//(PASSO2) Perguntar  nome dos jogadores.
    printf("Nome do Jogador 1:\n"); // CORRECAO: incremento do \n.
    scanf("%s", jogo.nomejogador1);
    printf ("Nome do Jogador 2:\n"); // CORRECAO: incremento do \n.
    scanf("%s", jogo.nomejogador2);

//(PASSO3) Realizar Jogo.
//  Zerar variaveis.  
    jogo.turnoatual=0;
    jogo.pontosfeitosJ1=0;
    jogo.pontosfeitosJ2=0;
    jogo.pontosrecebidosJ1=0;
    jogo.pontosrecebidosJ2=0;
    jogo.pontuacaoJ1=0;
    jogo.pontuacaoJ2=0;
    jogo.ordemdapalavra=0;
    for (i=0;i<50;i++){
        jogo.ppp.matrizpontos[i]=0;
        jogo.ppp.matrizturno[i]=0;
    }
    for (i=0; i<25; i++){
        jogo.verificaPalavrasJ1[i]=0;
        jogo.verificaPalavrasJ2[i]=0;
    }
//  Preenchimento da matriz escondida com tracos.
    for (i=0; i<jogo.tam; i++){
        for (j=0; j<jogo.tam; j++){
            jogo.matrizEscondida[i][j] = '-';
        }
    }
//  Preenchimento da matriz mapa com valores altos.
    //  Inicializa a matriz de mapa com valores altos.
    for(i=0;i<jogo.tam;i++){
        for(j=0;j<jogo.tam;j++){
            jogo.mapaordenado[i][j]=99999;
        }
    }
    
    int a, b;
//  Imprime tabela de palavras e tabuleiro antes de comecar o jogo.
    MostraPalavrasRestantes(jogo);
    MostraTabuleiro(jogo);
    
//  COMECA O JOGO!   
//  Controlador de turnos.
    for(t=0; t<jogo.turnos; t++){
        jogo.turnoatual=t+1;

//      Jogada jogador1:
        PedeJogada(jogo.nomejogador1);
        scanf ("%d %d", &a, &b);
//      Barra jogadas fora do tabuleiro ou repetidas.
        while(a>=jogo.tam || a<0 || b>=jogo.tam || b<0 || (jogo.matriz[a][b]<='Z' && jogo.matriz[a][b]>='A')){
            MostraPalavrasRestantes(jogo); //CORRECAO: adicao da funcao.
            MostraTabuleiro(jogo); //CORRECAO: adicao da funcao.
            if(a>=jogo.tam || a<0 || b>=jogo.tam || b<0){
                CoordenadasFora(jogo.nomejogador1);
                scanf ("%d %d", &a, &b);
            }
            else {
                CoordenadasJaJogadas(jogo.nomejogador1);
                scanf ("%d %d", &a, &b);
            }
        }
//      Escolhida uma coordenada valida realiza as modificacoes necessarias na tabela de palavras restantes e no tabuleiro para imprimi-los novamente.
        jogo = RealizaJogada(jogo, 1, a, b);
        MostraPalavrasRestantes(jogo);
        MostraTabuleiro(jogo);
//      Verifica se acabaram as palavras para algum jogador. Se sim, encerra o jogo.
        if(AcabaramAsPalavras(jogo, 1)){
            ResultadoDoJogo(jogo, saida);
        }
        if(AcabaramAsPalavras(jogo, 2)){
            ResultadoDoJogo(jogo, saida);
        }
        
//      Jogada jogador2:
        PedeJogada(jogo.nomejogador2);
        scanf ("%d %d", &a, &b);
//      Barra jogadas fora do tabuleiro ou repetidas.
        while(a>=jogo.tam || a<0 || b>=jogo.tam || b<0 || (jogo.matriz[a][b]<='Z' && jogo.matriz[a][b]>='A')){
            MostraPalavrasRestantes(jogo); //CORRECAO: adicao da funcao.
            MostraTabuleiro(jogo); //CORRECAO: adicao da funcao.
            if(a>=jogo.tam || a<0 || b>=jogo.tam || b<0){
                CoordenadasFora(jogo.nomejogador2);
                scanf ("%d %d", &a, &b);
            }
            else {
                CoordenadasJaJogadas(jogo.nomejogador2);
                scanf ("%d %d", &a, &b);
            }
        }
//      Escolhida uma coordenada valida realiza as modificacoes necessarias na tabela de palavras restantes e no tabuleiro para imprimi-los novamente.
        jogo = RealizaJogada(jogo, 2, a, b);
        MostraPalavrasRestantes(jogo);
        MostraTabuleiro(jogo);
//      Verifica se acabaram as palavras para algum jogador. Se sim, encerra o jogo.        
        if(AcabaramAsPalavras(jogo, 1)){
            ResultadoDoJogo(jogo, saida);
        }
        if(AcabaramAsPalavras(jogo, 2)){
            ResultadoDoJogo(jogo, saida);
        }
    }
    ResultadoDoJogo(jogo, saida);
    
    return (EXIT_SUCCESS);
}
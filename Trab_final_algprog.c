// Bibliotecas
#include <stdio.h>
#include "raylib.h"
#include <string.h>
#include <stdlib.h>
// Constantes
#define LINHAS  20
#define COLUNAS 30
#define ARESTA  20
#define ALTURA  800
#define LARGURA 600
#define INIMIGOS_MAX 20
// Elementos do jogo
#define VAZIO   ' '
#define OURO    'O'
#define ESMER   'E'
#define POWER   'A'
#define SOTER   'S'
#define TOUPAS  'T'
#define PAREDE  '#'
#define POS_JOG 'J'
// struct do jogador
typedef struct Jogador{
    // posição em x, y e destino em x e y do jogador, posição zero do jogador.
    int posx, posy, desx, desy, zx, zy;
    // nível atual do jogador
    int num_nivel;
    // número de vidas restantes do personagem
    int num_vidas;
    // pontuação atual do jogador
    int score;
    // número de esmeraldas do mapa;
    int esmer_colet;
    // powerup ativo para jogador
    int powerup;
}JOGADOR;
// struct para tiro
typedef struct Tiro {
    // posição do tiro
    int x, y;
    // estado do tiro
    int estado;
    // 1 = ativo
    // 0 = inativo
    // sentido do tiro
    int sentido;
} TIRO;
// struct para elementos do mapa
typedef struct Elementos {
    // struct para contar elemento
    int esmeraldas, ouro, inimigos;
}ELEMENTOS;
// struct da toupeira
typedef struct Tope {
    // posição atual da toupeira
    int tx, ty;
    // direção do deslocamento da toupeira
    int tdx, tdy;
    // posição inicial da toupeira
    int zx, zy;
    // vidas da toupeira
    int vidas;
    // quantidade de passos discretos dado pela toupeira
    int passos;
}TOPE;
// função chamada dentro de mov_jogador para testar se matriz está preenchida no destino do jogador.
int pode_mover(JOGADOR* jogador, char matriz_mapa[][COLUNAS]) {
    int x, y, pode = 1; // 1 = Pode mover
        y = jogador->posy + jogador->desy;
        x = jogador->desx + jogador->posx;
        // y = vertical, logo é análogo às linhas, enquanto x é análago as colunas
    if (matriz_mapa[y][x] == PAREDE || matriz_mapa[y][x] == SOTER) {
        pode = 0; // 0 = NÃO pode mover
    }
    return pode;
}
//função de movimentação do jogador e aquisição de sentido do tiro
void move_jogador(char matriz_mapa[][LINHAS],JOGADOR* jogador, TIRO* tiro) {
    jogador->desx = 0;
    jogador->desy = 0;

    if (IsKeyPressed(KEY_LEFT)|| IsKeyPressed(KEY_A)) {
        jogador->desx = -1;
        if(!tiro->estado) {
            tiro->sentido = 0;
        }
    }
    if (IsKeyPressed(KEY_UP)|| IsKeyPressed(KEY_W)) {
        jogador->desy = -1;
        if(!tiro->estado) {
            tiro->sentido = 1;
        }
    }
    if (IsKeyPressed(KEY_RIGHT)|| IsKeyPressed(KEY_D)) {
        jogador->desx = 1;
        if(!tiro->estado) {
            tiro->sentido = 2;
        }
    }
    if (IsKeyPressed(KEY_DOWN)|| IsKeyPressed(KEY_S)) {
        jogador->desy = 1;
        if(!tiro->estado) {
            tiro->sentido = 3;
        }
    }
    if(pode_mover(jogador,matriz_mapa)){
        jogador->posx += jogador->desx;
        jogador->posy += jogador->desy;
    }
}
// função para passar mapa.txt para matriz de caracteres
int le_mapa(char matriz_mapa[][COLUNAS], char nome[], ELEMENTOS* elementos) {
    FILE* mapa;
    int linhas, colunas;
    int fim_de_jogo = 0;
    elementos->esmeraldas = 0;
    elementos->inimigos = 0;
    elementos->ouro = 0;
    if (!(mapa = fopen(nome, "r"))) {
        printf("erro na abertura\n");
        fim_de_jogo++;
    }
    else{
        for (linhas = 0; linhas < LINHAS; linhas++) {
            for(colunas = 0; colunas <= COLUNAS; colunas++) {
                //passando caracteres do arquivo de texto para a matriz;
                matriz_mapa[linhas][colunas] = getc(mapa);
                //pegando quantidade de esmeraldas no mapa;
                if (matriz_mapa[linhas][colunas]== ESMER){
                    elementos->esmeraldas++;
                }
                //pegando quantidade de ouro no mapa;
                if (matriz_mapa[linhas][colunas] == OURO) {
                    elementos->ouro++;
                }
            }
        }
    }
    fclose(mapa);
    return fim_de_jogo;
}
// função para ler elementos do mapa e armazenar nas structs / setar variáveis
void guarda_movel(char matriz_mapa[][COLUNAS], TOPE tps[], JOGADOR* jogador, ELEMENTOS* elementos) {
    int linhas, colunas, ind = 0;
    elementos->inimigos = 0;
    for(linhas = 0; linhas < LINHAS; linhas++){
        for(colunas = 0; colunas < COLUNAS; colunas++){
            if(matriz_mapa[linhas][colunas] == TOUPAS){
                // setando variáveis das toupeiras
                tps[ind].zx = colunas; // pos inicial de x dos jogador
                tps[ind].zy = linhas;  // pos inicial de y do jogador
                tps[ind].tx = colunas;
                tps[ind].ty = linhas;
                tps[ind].tdx = GetRandomValue(-1, 1);
                tps[ind].tdy = GetRandomValue(-1, 1);
                tps[ind].passos = 0;
                tps[ind].vidas = 1;
                // Retirar caractere 'T' para não corromper próxima posição inicial da toupeira;
                matriz_mapa[linhas][colunas] = VAZIO;
                // a cada toupeira encontrada no mapa aumenta-se o valor do índice;
                ind++;
            }
            elementos->inimigos = ind;
            // pegando posição inicial do jogador
            if(matriz_mapa[linhas][colunas] == POS_JOG) {
                jogador->zx = linhas;
                jogador->zy = colunas;
                jogador->posx = linhas;
                jogador->posy = colunas;
                jogador->esmer_colet = 0;
                // deixando powerup desativo
                jogador->powerup = 0;
                // retirando Char 'J' da matriz;
                matriz_mapa[linhas][colunas] = VAZIO;
            }
        }
    }
}
//função para testar se inimigo pode se mover
int pode_inim(TOPE tps[], int ind, int num_inimigos, char matriz_mapa[][COLUNAS]) {
    int pode, cont;
    // por padrão a toupeira pode se mexer;
    pode = 0; //0 = pode
    for (cont = 0; cont < num_inimigos; cont++) {
        // A mesma toupeira não pode ter sua posição comparada consigo mesma, pois travará o jogo;
        if (ind != cont) {
            // Caso as toupeiras estejam em rota de colisão, o destino é trocado um frame antes da batida;
            if  (   tps[ind].ty + tps[ind].tdy == tps[cont].ty + tps[cont].tdy &&
                    tps[ind].tx + tps[ind].tdx == tps[cont].tx + tps[cont].tdx ){
                    pode = 1;}
        }
    }
    // caso a posição a da toupeira no próximo frame seja a posição da parede, trocar de direção;
    if (matriz_mapa[tps[ind].ty + tps[ind].tdy][tps[ind].tx + tps[ind].tdx] == PAREDE) {
        pode = 1; //1 = não pode mover;
    }
    return pode;
}
//função de movimentação dos inimigos
void move_toup(char matriz_mapa[][COLUNAS], TOPE tps[], int num_inimigos) {
    int ind;
    // 0 pode, 1 não pode
    for (ind = 0; ind < num_inimigos; ind++) {
        // caso a toupeira já tenha andado 5 passos ou o inimigo não pode andar, entrar no laço;
        if(tps[ind].passos == 5 || pode_inim(tps, ind, num_inimigos, matriz_mapa)){
            // loop de consistência para deslocamento selecionado ser válido
            do {
                do{
                    tps[ind].tdx = GetRandomValue(-1, 1);
                    tps[ind].tdy = GetRandomValue(-1, 1);
                    // pegar deslocamento aleatório enquanto ambos == 0;
                } while(tps[ind].tdx == 0 && tps[ind].tdy == 0);
            // checando se o novo deslocamento é válido na matriz;
            } while(pode_inim(tps, ind, num_inimigos, matriz_mapa));
            tps[ind].passos = 0;
        }
        tps[ind].tx += tps[ind].tdx;
        tps[ind].ty += tps[ind].tdy;
        tps[ind].passos++;
    }

}
// função retorna mapa para pos inicial, chamada dentro da colis_jog_toup
void check_point(JOGADOR *jogador,TOPE toupeira[],ELEMENTOS elementos) {
    int ind;
    jogador->num_vidas--;
    jogador->posx = jogador->zx;    // jogador volta para pos inicial
    jogador->posy = jogador->zy;
    jogador->powerup = 0;
    for(ind = 0;ind < elementos.inimigos; ind++){
        toupeira[ind].tx = toupeira[ind].zx; // toupeira volta para pos inicial
        toupeira[ind].ty = toupeira[ind].zy;
    }
}
// função de colisão entre o jogador e toupeiras
int colis_jog_toup(JOGADOR jogador,TOPE tps[],ELEMENTOS elementos) {
    int ind, colisao;
    colisao = 0;
    for(ind = 0; ind < elementos.inimigos; ind++){
        if(jogador.posx == tps[ind].tx && jogador.posy == tps[ind].ty){
            colisao = 1;
        }
    }
    return colisao;
}
// função pinta_mapa para elementos estáticos
void pinta_mapa(char matriz_mapa[][COLUNAS], int aresta) {
    int linhas, colunas;
    char carac;
    for(linhas = 0; linhas < LINHAS; linhas++){
        for(colunas = 0; colunas < COLUNAS; colunas++){
            carac = matriz_mapa[linhas][colunas];
            switch(carac) {
                case OURO:
                    DrawRectangle(aresta*colunas, aresta*linhas, aresta, aresta, GOLD);
                    break;
                case ESMER:
                    DrawRectangle(aresta*colunas, aresta*linhas, aresta, aresta, LIME);
                    break;
                case SOTER:
                    DrawRectangle(aresta*colunas, aresta*linhas, aresta, aresta, DARKBROWN);
                    break;
                case POWER:
                    DrawRectangle(aresta*colunas, aresta*linhas, aresta, aresta, PURPLE);
                    break;
                case PAREDE:
                    DrawRectangle(aresta*colunas, aresta*linhas, aresta, aresta, DARKGRAY);
                    break;

            }
        }
    }
}
// função para pintar elementos móveis
void pinta_movel(TOPE tps[],int num_inimigos, JOGADOR jogador, TIRO tiro) {
    int i;
    for(i = 0; i < num_inimigos; i++) {
        DrawRectangle(ARESTA * tps[i].tx, ARESTA * tps[i].ty, ARESTA, ARESTA, RED);
    }
    DrawRectangle(ARESTA * jogador.posx, ARESTA * jogador.posy, ARESTA, ARESTA, BLUE);
    if (tiro.estado) {
    DrawRectangle(ARESTA * tiro.x, ARESTA * tiro.y, ARESTA, ARESTA, WHITE);
    }
}
// função que coleta minérios e aumenta pontuação
void coletora(char matriz_mapa[][COLUNAS],JOGADOR *jogador) {
// funções relacionadas ao tiro
// função para pegar sentido do tiro

    if(matriz_mapa[jogador->posy][jogador->posx] == ESMER){
        matriz_mapa[jogador->posy][jogador->posx] = VAZIO;
        jogador->esmer_colet += 1;
        jogador->score += 100;
        printf("Esmeralda %d\n",jogador->esmer_colet);
    }
    if(matriz_mapa[jogador->posy][jogador->posx] == OURO){
        matriz_mapa[jogador->posy][jogador->posx] = VAZIO;
        jogador->score += 50;
        printf("Ouro\n");
    }
    if(matriz_mapa[jogador->posy][jogador->posx] == POWER){
        matriz_mapa[jogador->posy][jogador->posx] = VAZIO;
        jogador->powerup = 1;
        printf("PowerUP\n");
    }

}
// função para movimentação do tiro e retirada de toupeiras mortas
int colis_tiro(TIRO* tiro, char matriz_mapa[][COLUNAS], TOPE tps[], ELEMENTOS* elementos) {
    // checando colisão entre tiro e toupeiras;
    int ind;
    int tps_morta = 0; // toupeira morta
    for (ind = 0; ind < elementos->inimigos; ind++) {
        if (tiro->x == tps[ind].tx && tiro->y == tps[ind].ty){
            tps[ind].vidas--;

            tiro->estado = 0;
            if(tps[ind].vidas == 0){
                tps[ind].tx = tps[elementos->inimigos - 1].tx;
                tps[ind].ty = tps[elementos->inimigos - 1].ty;
                tps[ind].vidas = tps[elementos->inimigos - 1].vidas;
                tps[ind].zx = tps[elementos->inimigos - 1].zx;
                tps[ind].zy = tps[elementos->inimigos - 1].zy;
                elementos->inimigos--;
                tps_morta = 1;
            }
        }
    }
    if(matriz_mapa[tiro->y][tiro->x] == PAREDE) {
        tiro->estado = 0;
    }
    if (matriz_mapa[tiro->y][tiro->x] == SOTER) {
        tiro->estado = 0;
        matriz_mapa[tiro->y][tiro->x] = VAZIO;
    }
    return tps_morta;
}
void move_tiro (TIRO* tiro, char matriz_mapa[][COLUNAS], TOPE tps[], ELEMENTOS* elementos, JOGADOR *jogador) {
    if (!tiro->estado) {
        if (IsKeyPressed(KEY_G)) {
            tiro->estado = 1;
            tiro->x = jogador->posx;
            tiro->y = jogador->posy;
            printf("\nBANG");
            printf("\n estado tiro: %d", tiro->estado);
        }
    }
    if (tiro->estado) {
        switch(tiro->sentido) {
            // esquerda
            case 0:
                tiro->x += -1;
            break;
            // cima
            case 1:
                tiro->y += -1;
            break;
            // direita
            case 2:
                tiro->x += 1;
            break;
            // baixo
            case 3:
                tiro->y += 1;
            break;
        }
        if(colis_tiro(tiro, matriz_mapa, tps, elementos)){
            jogador->score += 200;
        }

    }
}
// função para salvar
void salvar_mapa(char nome[],JOGADOR jogador,TOPE tps[],ELEMENTOS elem,char matriz_mapa[][COLUNAS]){
    int i,linhas,colunas; // contadores
    TOPE buffer; // variavel auxiliar
    char aux; // variavel auxiliar
    FILE *arq;
    if(!(arq = fopen(nome,"wb"))){ // abertura para escrita
        printf("Erro na abertura\n");
    }
    else{
        if(fwrite(&jogador,sizeof(JOGADOR),1,arq)!= 1){
            printf("Erro na escrita\n");
        }
        else{
            printf("posX %d e posY %d\n",jogador.posx,jogador.posy);
        }
        for(i=0;i<INIMIGOS_MAX;i++){
            buffer.tx = tps[i].tx;
            buffer.ty = tps[i].ty;
            buffer.tdx = tps[i].tdx;
            buffer.tdy = tps[i].tdy;
            buffer.passos = tps[i].passos;
            buffer.vidas = tps[i].vidas;
            buffer.zx = tps[i].zx;
            buffer.zy = tps[i].zy;
            if(fwrite(&buffer,sizeof(TOPE),1,arq)!= 1){
                printf("Erro de escrita\n");
            }
            else{
                printf("tpsX %d e tpsY %d\n",tps[i].tx,tps[i].ty);
            }
        }
        for(linhas = 0;linhas<LINHAS;linhas++){
            for(colunas = 0; colunas<=COLUNAS;colunas++){ // talvez a matriz não tá sendo alterada
                aux = matriz_mapa[linhas][colunas];      // possivel bug
                if(fwrite(&aux,sizeof(char),1,arq)!= 1){
                    printf("Erro na escrita\n");
                }
                else{
                    printf(" %c",aux);
                }
            }
        }
        if(fwrite(&elem,sizeof(ELEMENTOS),1,arq)!= 1){
            printf("Erro de escrita\n");
        }
        else{
            printf("DEU CERTO\n");
        }
    }
    fclose(arq);

}
// função para carregar mapa
void carregar_mapa(char nome[],JOGADOR *jogador,TOPE tps[],ELEMENTOS *elementos,char matriz_mapa[][COLUNAS]){
    int i,linhas,colunas;
    JOGADOR buff;
    TOPE buffer;
    ELEMENTOS var;
    char aux;
    FILE *arq;
    if(!(arq = fopen(nome,"rb"))){
        printf("Erro na abertura\n");
    }
    else{
        if(fread(&buff,sizeof(JOGADOR),1,arq)!=1){ // ler até acabar o arq,colocando as informações nos tipos certos
            printf("Erro na leitura\n");
        }
        else{
            jogador->posx = buff.posx;
            jogador->posy = buff.posy;
            jogador->desx = buff.desx;
            jogador->desy = buff.desy;
            jogador->esmer_colet = buff.esmer_colet;
            jogador->num_nivel = buff.num_nivel;
            jogador->num_vidas = buff.num_vidas;
            jogador->powerup = buff.powerup;
            jogador->score = buff.score;
            jogador->zx = buff.zx;
            jogador->zy = buff.zy;
            printf("posX %d\n",jogador->posx);
        }
        for(i = 0; i<INIMIGOS_MAX;i++){
            if(fread(&buffer,sizeof(TOPE),1,arq)!= 1){
                printf("Erro na leitura\n");
            }
            else{
                tps[i].tx = buffer.tx;
                tps[i].ty = buffer.ty;
                tps[i].tdx = buffer.tdx;
                tps[i].tdy = buffer.tdy;
                tps[i].passos = buffer.passos;
                tps[i].vidas = buffer.vidas;
                tps[i].zx = buffer.zx;
                tps[i].zy = buffer.zy;
                printf("tx %d\n",tps[i].tx);
            }
        }
        for(linhas = 0;linhas<LINHAS;linhas++){
            for(colunas = 0;colunas<=COLUNAS;colunas++){
                if(fread(&aux,sizeof(char),1,arq)!= 1){
                    printf("Erro de leitura\n");
                }
                else{
                    matriz_mapa[linhas][colunas] = aux;
                    printf(" %c",aux);
                }
            }
        }
        if(fread(&var,sizeof(ELEMENTOS),1,arq)!= 1){
            printf("Erro de leitura\n");
        }
        else{
            elementos->esmeraldas = var.esmeraldas;
            elementos->inimigos = var.inimigos;
            elementos->ouro = var.ouro;
        }
    }
    fclose(arq);
}
// função principal do jogador
int main(){
    char mapa_do_jogo[30];
    int ftp = 0, fpw = 0;
    int inicio = 0, leitura =0;
    char matriz_mapa[LINHAS][COLUNAS];
    int menu = 0;
    int sair_jogo = 1;
    int ganhou_jogo = 0;
    int perdeu_jogo = 0;
    // declarando structs na main
    JOGADOR jogador;
    jogador.num_nivel = 1;
    jogador.num_vidas = 3;
    jogador.score = 0;
    TIRO tiro;
    tiro.estado = 0;
    ELEMENTOS elementos;
    TOPE tps[INIMIGOS_MAX];
    sprintf(mapa_do_jogo,"mapa%d.txt",jogador.num_nivel); //nome do proximo mapa é gerado
    //lendo mapa de arquivo para txt
    le_mapa(matriz_mapa, mapa_do_jogo, &elementos);
    //inicializando posicao das tps e jogador
    guarda_movel(matriz_mapa, tps, &jogador, &elementos);
    // movimentando inimigos no mapa
    InitWindow(LARGURA, ALTURA, "O jogo");
    SetTargetFPS(60);

    while(!WindowShouldClose || inicio == 0 ){
        if(IsKeyPressed(KEY_M)){
            inicio++;
        }
        BeginDrawing();
        DrawRectangle(0,0,500,500,BLACK);
        DrawText("Pressione M para jogar", 30, 200, 40, GREEN);
        EndDrawing();

    }
    while(!WindowShouldClose() && sair_jogo != 0) {
        if(jogador.esmer_colet == elementos.esmeraldas){
            jogador.num_nivel += 1;
            sprintf(mapa_do_jogo,"mapa%d.txt",jogador.num_nivel); //nome do proximo mapa é gerado
            if(le_mapa(matriz_mapa,mapa_do_jogo, &elementos)){ // deu erro de abertura;
                sair_jogo = 0;
                ganhou_jogo++;
            }
            else{
                guarda_movel(matriz_mapa, tps, &jogador, &elementos);
                leitura = 0;
                printf("SE E DOIDO\n");
            }
        }
        if(!jogador.num_vidas){
            sair_jogo = 0;
            perdeu_jogo++;
        }
        if(IsKeyPressed(KEY_TAB)){
            while(!menu){
                if(IsKeyPressed(KEY_V)){
                    menu++;
                }
                // passar função para menu
                if(IsKeyPressed(KEY_S)){
                    salvar_mapa("arq.bino",jogador,tps,elementos,matriz_mapa); // se clicar no 'f' o jogo é salvo
                    menu++;
                }
                // passar função para menu
                if(IsKeyPressed(KEY_C)){
                    carregar_mapa("arq.bino", &jogador, tps, &elementos, matriz_mapa); // se clicar no 'c' o jogo é carregado
                    menu++;
                }
                if(IsKeyPressed(KEY_N)){
                    jogador.num_nivel = 1;
                    jogador.num_vidas = 3;
                    jogador.score = 0;
                    sprintf(mapa_do_jogo,"mapa%d.txt",jogador.num_nivel);
                    le_mapa(matriz_mapa,mapa_do_jogo, &elementos);
                    guarda_movel(matriz_mapa, tps, &jogador, &elementos);
                    menu++;
                }
                if(IsKeyPressed(KEY_Q)){
                    sair_jogo = 0;
                    menu++;
                }
                BeginDrawing();
                //ClearBackground(BLUE);
                DrawRectangle(0,0,500,500,BLACK);
                DrawText("(N) Novo Jogo", 30, 20, 40, GREEN);
                DrawText("(C) Carregar jogo", 30, 100, 40, GREEN);
                DrawText("(S) Salvar jogo", 30, 180, 40, GREEN);
                DrawText("(Q) Sair do jogo", 30, 260, 40, GREEN);
                DrawText("(V) Voltar", 30, 340, 40, GREEN);
                EndDrawing();

            }
            menu = 0;
        }
        move_jogador(matriz_mapa, &jogador, &tiro);
        move_tiro(&tiro, matriz_mapa, tps, &elementos, &jogador);
        //reduzindo velocidade da toupeira
        if (ftp == 15) {
            move_toup(matriz_mapa, tps, elementos.inimigos);
            ftp = 0;
        }else{ftp++;}
        if(colis_jog_toup(jogador,tps,elementos)){
            check_point(&jogador, tps, elementos);
        }
        //pontuação do jogador
        coletora(matriz_mapa, &jogador);

        //inicio da parte gráfica do jogo
        BeginDrawing();
        ClearBackground(GRAY);
        pinta_mapa(matriz_mapa, ARESTA);
        pinta_movel(tps, elementos.inimigos, jogador, tiro);
        // tempo ativo do powerup
        if (!jogador.powerup) {
            DrawRing((Vector2){(jogador.posx * ARESTA) + 10, (jogador.posy * ARESTA) + 10} , 80, 1000, 0, 360, 100, Fade(BLACK, 0.94));
        }
        else{
            if (fpw == 300) {
                jogador.powerup = 0;
                fpw = 0;
            } else{fpw++;}
        }
        DrawText(TextFormat("vidas : %d",jogador.num_vidas),0,650,40,GREEN);
        DrawText(TextFormat("pontuacao : %d",jogador.score),0,700,40,GREEN);
        DrawText(TextFormat("esmeraldas : %d",jogador.esmer_colet),0,750,40,GREEN);
        DrawText(TextFormat("nivel: %d",jogador.num_nivel),300,650,40,GREEN);
        EndDrawing();
    }
    if(ganhou_jogo){
        while(!WindowShouldClose()){
            BeginDrawing();
            ClearBackground(BLACK);
            DrawText("Parabens por concluir o jogo",0, 200, 20, GREEN);
            EndDrawing();
        }
    }
    if(perdeu_jogo){
       while(!WindowShouldClose()){
            BeginDrawing();
            ClearBackground(BLACK);
            DrawText("Otario", 30, 200, 40, GREEN);
            EndDrawing();
        }
    }
    return 0;
}
// se fazer novo jogo o jogador está nível 1 e por consequência ele está no mapa1
// quando mata tps o carrega mapa buga

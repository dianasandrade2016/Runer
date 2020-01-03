#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_audio.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdlib.h>
#include "score.h"

#define  LEFT   1
#define  RIGHT  0

#define NUM_OBSTACULOS 15

#define SHAPE_SIZE 16

// Para compilar: gcc runer.c -o runer -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer -lSDL2_gfx -lm
// Para executar: ./runer

typedef struct _VEICULO
{
	int estado;
	SDL_Surface *imagem;
	SDL_Texture *texture;
	SDL_Rect dst_rect;
    SDL_Rect src_rect;
	Uint32 sprite;
	Uint32 ticks;
	int xVel;
	int yVel;
	int frame;
	int resistencia;

}VEICULO;

typedef struct _CENARIO
{
	int estado; 
	SDL_Surface *imagem;
	SDL_Texture *texture;
	SDL_Rect dst_rect;
    SDL_Rect src_rect;
	Uint32 sprite;
	Uint32 ticks;
	int xVel;
	int yVel;
	int frame;
	int resistencia;

}CENARIO;

typedef struct _OBSTACULO
{
	int estado; 
	SDL_Surface *imagem;
	SDL_Texture *texture;
	SDL_Rect dst_rect;
    SDL_Rect src_rect;
	Uint32 sprite;
	Uint32 ticks;
	int xVel;
	int yVel;
	int frame;
	int bateu;

}OBSTACULO;

VEICULO jogador;
CENARIO cenario;
OBSTACULO obstaculo[NUM_OBSTACULOS];

SDL_Window *janela;
SDL_Renderer *tela;

SDL_Surface *fundoMenu;//Variavel da fundo do Menu Inicial
SDL_Surface *fundoInserirNome;
SDL_Surface *imagemBoom;
SDL_Surface *imagemPlus;
SDL_Surface *imagemBarril;
SDL_Surface *imagemGirl;
SDL_Surface *imagemCar1;
SDL_Surface *imagemCar2;
SDL_Surface *imagemCar3;
SDL_Surface *fundoInstrucoes;
SDL_Surface *texto1;  // texto nome
SDL_Surface *texto2; // texto pontuação
SDL_Surface *fundoRanking;
SDL_Surface *fundoDesenvolvedores;
SDL_Surface *opcaoCar1;

SDL_Texture *textureFundoInstrucoes;
SDL_Texture *textureFundoMenu;
SDL_Texture *textureInserirNome;
SDL_Texture *textureBoom;
SDL_Texture *texturePlus;
SDL_Texture *textureBarril;
SDL_Texture *textureGirl;
SDL_Texture *textureCar1;
SDL_Texture *textureCar2;
SDL_Texture *textureCar3;
SDL_Texture *textureTexto1;
SDL_Texture *textureTexto2;
SDL_Texture *textureFundoRanking;
SDL_Texture *textureFundoDesenvolvedores;
SDL_Surface *textureOpcaoCar1;


SDL_Event event;
SDL_Color corTexto;
SDL_Color corTexto1;
TTF_Font *fonte;
TTF_Font *fonte1;
TTF_Font *fonte2;
SDL_Rect rectFiguraSelecao;
SDL_Rect rectSelecao;
Mix_Music *musica;
Mix_Chunk *jumpEffect;
Mix_Chunk *soundPlus;
Mix_Chunk *laserEffect;

TTF_Font *carregarFonte(const char*, int);

int carSelectd = 3;

void carregarSurfaces();
void inicializarComponentes();
int inicializarjogador();
int inicializarjogadorSprites();
int inicializarCenario();
int inicializarCenarioSprites();
int inicializarObstaculo();
int jogo();
int verificaColisao();
int gerarBarris();
int inicializarFonte();
int inserirNome();
int recorde();

int velocidade = 30;
int vel_jogador = 15;
int delay_valor = 100;

int up = 0, down = 0, right = 0, left = 0;
int pulo = 0;
int quit = 0;
int i = 0;

int colisao = 0;
int cont_obstaculos = 0;

int cont_vidas = 1;
int cont_level = 1;

int cont_kilometragem = 0;

char bufferPontuacao[16]; //buffer para pontuacao
char nomeJogador[30];

Lista *lista; //lista de scores


int main(int argc, char ** argv){
	// lista para score.h
    lista = criarLista();
    carregarLista("ranking.txt",lista);


    SDL_Init(SDL_INIT_EVERYTHING);
    printf("\n Inicializado com sucesso! \n");
    carregarSurfaces();

    inicializarComponentes();
    inicializarFonte();


    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
    musica = Mix_LoadMUS("Musica/suspense.mp3");
    Mix_PlayMusic(musica,-1);

    soundPlus = Mix_LoadWAV("Musica/coin.wav");
    jumpEffect = Mix_LoadWAV("Musica/large_crash_3.wav");


    fundoInserirNome = IMG_Load("Imagens/fundoInserirNome.png");
    textureInserirNome = SDL_CreateTextureFromSurface(tela,fundoInserirNome);


    fundoRanking = IMG_Load("Imagens/ranking.png");
    textureFundoRanking= SDL_CreateTextureFromSurface(tela,fundoRanking);

    imagemBoom = IMG_Load("Imagens/boom.png");
    textureBoom = SDL_CreateTextureFromSurface(tela, imagemBoom);
    imagemBarril = IMG_Load("Imagens/barril.png");
    textureBarril = SDL_CreateTextureFromSurface(tela, imagemBarril);

    imagemGirl = IMG_Load("Imagens/girl.png");
    textureGirl = SDL_CreateTextureFromSurface(tela, imagemGirl);
    imagemPlus = IMG_Load("Imagens/plus.png");
    texturePlus = SDL_CreateTextureFromSurface(tela, imagemPlus);


    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
    laserEffect = Mix_LoadWAV("Musica/son_993.wav");

    fundoMenu = IMG_Load("Imagens/fundoMenu.png");
    textureFundoMenu = SDL_CreateTextureFromSurface(tela,fundoMenu);

    fonte = carregarFonte("Fontes/fonte1.ttf",30);
    fonte1 = carregarFonte("Fontes/fonte3.ttf",120);
    fonte2 = carregarFonte("Fontes/fonte1.ttf",70);

    fundoDesenvolvedores = IMG_Load("Imagens/desenvolvedores.png");
    textureFundoDesenvolvedores= SDL_CreateTextureFromSurface(tela,fundoDesenvolvedores);

    fundoInstrucoes = IMG_Load("Imagens/instrucoes.png");
    textureFundoInstrucoes= SDL_CreateTextureFromSurface(tela,fundoInstrucoes);

     menu();

     guardarLista("ranking.txt",lista);

}

void menu(){

    int loop = 0;
    // Loop principal
    while (loop == 0){
       // Ler a fila de eventos e colocar o evento mais antigo em "event"
       // Loop de eventos
        while (SDL_PollEvent(&event)){
            // Verificar se o evento mais antigo é do tipo SDL_QUIT
            // Se o usuário clicou para fechar a janela
            if(event.type == SDL_QUIT){
                loop = 1; // Encerre o loop
            }

            else if(event.type == SDL_MOUSEMOTION){

            	if((event.motion.x >= 113 && event.motion.x <= 248) && (event.motion.y >= 292 && event.motion.y <= 322)){
            	}

            	else if((event.motion.x >= 535 && event.motion.x <= 741) && (event.motion.y >= 316 && event.motion.y <= 365)){

            	}

            	else if((event.motion.x >= 113 && event.motion.x <= 434) && (event.motion.y >= 400 && event.motion.y <= 435)){

            	}
            	else if((event.motion.x >= 113 && event.motion.x <= 259) && (event.motion.y >= 500 && event.motion.y <= 535)){

            	}
            	else if((event.motion.x >= 535 && event.motion.x <= 613) && (event.motion.y >= 559 && event.motion.y <= 600)){

            	}else{

            	}
            }

            else if(event.type == SDL_MOUSEBUTTONDOWN){
            	if((event.button.x >= 113 && event.button.x <= 248) && (event.button.y >= 292 && event.button.y <= 322)){
            		printf(" Cliquei no botao iniciar\n");

            		if(!jogo()){
                        loop = 1;
                        Mix_FreeMusic(musica);
                        Mix_FreeChunk(jumpEffect);
                        Mix_CloseAudio();
                    }

            	}

            	else if((event.button.x >= 119 && event.button.x <= 320) && (event.button.y >= 350 && event.button.y <= 380))
            	{
                    printf("Cliquei nas instrucoes\n");
                    if(!instrucoes()){
                    	loop = 1;
                    }
            	}

            	else if((event.button.x >= 119 && event.button.x <= 434) && (event.button.y >= 400 && event.button.y <= 435))
            	{
                    printf("Cliquei nos desenvolvedores\n");
                    if(!developers())
                    {
                        loop = 1;
                    }
            	}
                else if((event.button.x >= 119 && event.button.x <= 250) && (event.button.y >= 450 && event.button.y <= 488))
            	{
                    printf("Cliquei nas Opcoes\n");
                    if(!opcoes())
                    {
                        loop = 1;
                    }
            	}
            	else if((event.button.x >= 119 && event.button.x <= 259) && (event.button.y >= 500 && event.button.y <= 535))
            	{

                    printf("Cliquei no recorde\n");

                    if(!recorde()){
                        Jogador *j1 = criarJogador(nomeJogador,cont_kilometragem);
                        inserirOrdenado(lista,j1);
                        loop = 1;
                    }
            	}
            	else if((event.button.x >= 113 && event.button.x <= 613) && (event.button.y >= 559 && event.button.y <= 600))
            	{
                    printf("Cliquei em sair\n");
                    loop = 1;
            	}
           	}
        }

        SDL_RenderClear(tela);//sem esta linha a imagem nao atualiza
        SDL_RenderCopy(tela, textureFundoMenu,NULL,NULL);
        SDL_RenderPresent(tela);

    }

    guardarLista("ranking.txt",lista);

    SDL_DestroyWindow(janela);
	IMG_Quit();
	SDL_Quit(); // Fechar o SDL
  	return 0;
}


int inicializarjogador(){

    if(carSelectd == 1){
        jogador.imagem = IMG_Load("Imagens/car1.png");
    }

    if(carSelectd == 2){
        jogador.imagem = IMG_Load("Imagens/car2.png");
    }

    if(carSelectd == 3){
        jogador.imagem = IMG_Load("Imagens/car3.png");
    }


    jogador.texture = SDL_CreateTextureFromSurface(tela, jogador.imagem);

    jogador.dst_rect.x = 450;
    jogador.dst_rect.y = 600;
    jogador.dst_rect.w = 50;
    jogador.dst_rect.h = 110;
}

int inicializarCenario(){
    SDL_SetRenderDrawColor(tela, 168, 230, 255, 255);

    cenario.imagem = IMG_Load("Imagens/rua.jpg");
    //cenario.imagem = IMG_Load("Imagens/free-highway-vector.jpg");

    cenario.texture = SDL_CreateTextureFromSurface(tela, cenario.imagem);

    cenario.dst_rect.x = 0;
    cenario.dst_rect.y = 0;
    cenario.dst_rect.w = 980;
    cenario.dst_rect.h = 640;

}

int inicializarObstaculo( ){

    srand(time(NULL));

    int cont = 0;
    while(cont< NUM_OBSTACULOS){

        int p = rand()%730;
        if( p>=230){
            obstaculo[cont].imagem = imagemBarril;
            obstaculo[cont].texture = SDL_CreateTextureFromSurface(tela,  obstaculo[i].imagem);
            obstaculo[cont].dst_rect.x = p;
            obstaculo[cont].dst_rect.y = rand()%940 -940;
            obstaculo[cont].dst_rect.w = 32;
            obstaculo[cont].dst_rect.h = 44;
            obstaculo[cont].bateu = 0;

            printf("R: %d \n", p);

            cont++;
        }

    }
	printf("\n%d Obstaculos criados...", NUM_OBSTACULOS);//530

}

int inicializarjogadorSprites(){

    //SDL_GetTicks() fornece o número de milissegundos que passaram desde o início do programa.
    jogador.ticks = SDL_GetTicks();

    jogador.sprite = (jogador.ticks / 100) % 6;
    //dividimos os segundos pelo número de sprites da spritesheet, neste caso 6.
    jogador.src_rect.x = 0;
    jogador.src_rect.y = 0;
    jogador.src_rect.w = 500;
    jogador.src_rect.h = 800;

}

int inicializarCenarioSprites(){

    cenario.ticks = SDL_GetTicks();
    cenario.sprite = (jogador.ticks / 100) % 11;

    cenario.src_rect.x = 0;
    cenario.src_rect.y =  300 - cenario.sprite * 30;
    cenario.src_rect.w = 640;
    cenario.src_rect.h = 150;
 }

void carregarSurfaces(){

    janela= SDL_CreateWindow("Runer",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,980,640,SDL_WINDOW_OPENGL);
    tela = SDL_CreateRenderer(janela, -1, 0);
    SDL_RenderClear(tela);//sem esta linha a imagem nao atualiza

    SDL_RenderClear(tela);//sem esta linha a imagem nao atualiza

    SDL_RenderCopy(tela, textureFundoMenu,NULL,NULL);

    SDL_RenderPresent(tela);

}

move_paddle( var ){

    if( var==LEFT){
        if(jogador.dst_rect.x>205){
            jogador.dst_rect.x -= vel_jogador;
        }else{
            jogador.dst_rect.x += vel_jogador;
        }
    }

    if( var==RIGHT){

        if(jogador.dst_rect.x<735){
            jogador.dst_rect.x += vel_jogador;
        }else{
            jogador.dst_rect.x -= vel_jogador;
        }

    }

}

int jogo(){

    cont_vidas = 5;
    cont_level = 1;

    velocidade = 30;
    vel_jogador = 15;
    delay_valor = 100;
    cont_obstaculos = 0;
    cont_kilometragem = 0;

    inicializarjogador();
    inicializarCenario();
    inicializarObstaculo();
    char finalJogo[10] = "GAME OVER";
    SDL_Surface * surfEnd;
    SDL_Texture * textureEnd;

    int key_left = 0;
    int key_right = 0;

    Mix_PlayChannel(-1, laserEffect, -1);

    while(!quit){

        inicializarjogadorSprites();
        inicializarCenarioSprites();

        if (key_left)
            move_paddle(LEFT);
        else if (key_right)
            move_paddle(RIGHT);

        while (SDL_PollEvent(&event)){ // Loop de eventos

            if(jogador.dst_rect.y <=480){

                if (event.type == SDL_KEYDOWN )
                    switch (event.key.keysym.sym){
                        case SDLK_RIGHT:
                        key_right = 1;
                        key_left = 0;
                        break;

                        case SDLK_LEFT:
                        key_left = 1;
                        key_right = 0;
                        break;
                        case SDLK_ESCAPE:
                        menu();
                        break;
                        }

                if (event.type == SDL_KEYUP)
                    switch (event.key.keysym.sym){
                        case SDLK_RIGHT:
                        key_right = 0;
                        break;
                        case SDLK_LEFT:
                        key_left = 0;
                        break;
                        }


                if (event.type == SDL_QUIT){

                    quit = 1;
                }

            }
       }

        //renderizar a textura

        SDL_RenderCopy(tela, cenario.texture, &cenario.src_rect, &cenario.dst_rect);
        SDL_RenderCopy(tela, jogador.texture, &jogador.src_rect, &jogador.dst_rect);


        printaStatus();

        printaObstaculos();

        verificaColisao();


        if(jogador.dst_rect.y>480 && cont_vidas >0){
            jogador.dst_rect.y -= 8;
        }


        if(cont_vidas<=0){
            jogador.dst_rect.y += velocidade;

            surfEnd  = TTF_RenderText_Solid(fonte1, finalJogo, corTexto1);
            textureEnd  = SDL_CreateTextureFromSurface(tela, surfEnd);
            int texendW = 0;
            int texendH = 0;

            SDL_QueryTexture(textureEnd, NULL, NULL, &texendW, &texendH);
            SDL_Rect dstrectend = {200,200, texendW, texendH};
            SDL_RenderCopy(tela, textureEnd, NULL,&dstrectend);
        }



        SDL_RenderPresent(tela);

        SDL_RenderClear(tela);//sem esta linha a imagem nao atualiza

        SDL_Delay ( delay_valor );

        if(cont_vidas>0){
         cont_kilometragem++;
        }

        if(cont_kilometragem == 400){//lv2

            delay_valor = 90;
            cont_level=2;

            printf("\n---------------LV-02\n" );
        }

        if(cont_kilometragem == 800){//lv3
            //velocidade = 15;
            delay_valor = 65;
            cont_level=3;

            printf("\n---------------LV-03\n" );
        }

        if(cont_kilometragem == 1200){ //lv4
            velocidade = 15;
            delay_valor = 44;
            cont_level=4;

            printf("\n---------------LV-04\n" );

        }

        if(cont_kilometragem == 1600){ //lv5
            velocidade = 10;
            delay_valor = 20;
            vel_jogador = 8;
            cont_level=5;

            printf("\n---------------LV-05\n" );

        }

        if(jogador.dst_rect.y>=2000){
            inserirNome();
            quit=1;
        }

    }

    SDL_DestroyTexture(jogador.texture);
    SDL_FreeSurface(jogador.imagem);
    SDL_DestroyRenderer(tela);
    SDL_DestroyWindow(janela);

    //IMG_Quit();
    TTF_Quit();
    SDL_Quit();

    return 0;
}


void printaStatus(){

    char bc[10];
    char vc[10];
    char clock[10];
    char level[10];


    SDL_Surface * surfTempo;
    SDL_Surface * surfScore;
    SDL_Surface * surfVida;
    SDL_Surface * surfLevel;

    SDL_Texture * textureClock;
    SDL_Texture * textureScore;
    SDL_Texture * textureVida;
    SDL_Texture * textureLevel;


    sprintf(bc, "SCORE %d", cont_obstaculos);
    sprintf(vc, "VIDA %d", cont_vidas);
    sprintf(clock, "%d KM", cont_kilometragem);
    sprintf(level, "LEVEL %d", cont_level);


    surfTempo  = TTF_RenderText_Solid(fonte, clock, corTexto);
    surfVida = TTF_RenderText_Solid(fonte, vc, corTexto);
    surfScore  = TTF_RenderText_Solid(fonte, bc, corTexto);
    surfLevel  = TTF_RenderText_Solid(fonte, level, corTexto);

    textureClock = SDL_CreateTextureFromSurface(tela, surfTempo);
    textureScore = SDL_CreateTextureFromSurface(tela, surfScore);
    textureVida = SDL_CreateTextureFromSurface(tela, surfVida);
    textureLevel = SDL_CreateTextureFromSurface(tela, surfLevel);

    int texclockW = 0;
    int texclockH = 0;
    SDL_QueryTexture(textureClock, NULL, NULL, &texclockW, &texclockH);
    SDL_Rect dstrecttempo = {50, 0, texclockW, texclockH };


    int texscoreW = 0;
    int texscoreH = 0;
    SDL_QueryTexture(textureScore, NULL, NULL, &texscoreW, &texscoreH);
    SDL_Rect dstrectscore = {245, 0, texscoreW, texscoreH };


    int texvidaW = 0;
    int texvidaH = 0;
    SDL_QueryTexture(textureVida, NULL, NULL, &texvidaW, &texvidaH);
    SDL_Rect dstrectvida = {645, 0, texvidaW, texvidaH};

    int texlevelW = 0;
    int texlevelH = 0;
    SDL_QueryTexture(textureLevel, NULL, NULL, &texlevelW, &texlevelH);
    SDL_Rect dstrectlevel = {845, 0, texlevelW, texlevelH};

    SDL_RenderCopy(tela, textureClock, NULL,&dstrecttempo);
    SDL_RenderCopy(tela, textureScore, NULL,&dstrectscore);
    SDL_RenderCopy(tela, textureVida, NULL,&dstrectvida);
    SDL_RenderCopy(tela, textureLevel, NULL,&dstrectlevel);
}


void printaObstaculos(){

	SDL_RenderCopy(tela,  obstaculo[0].texture, NULL, & obstaculo[0].dst_rect);
    obstaculo[0].dst_rect.y += velocidade;

    if(obstaculo[0].dst_rect.y > 640){

        int p = rand()%730;
        if( p>=230){

            obstaculo[0].dst_rect.x = p;

            obstaculo[0].dst_rect.y = -100 ;

            int num = p%2;
            if(num == 0){
                obstaculo[0].texture = textureGirl;
                obstaculo[0].bateu = 3;
            }else{
                obstaculo[0].texture = textureBarril;
                obstaculo[0].bateu = 0;
            }


            obstaculo[0].dst_rect.w = 32;
            obstaculo[0].dst_rect.h = 44;


            //SDL_RenderCopy(tela,  obstaculo[0].texture, NULL, & obstaculo[0].dst_rect);
        }
    }

    SDL_RenderCopy(tela,  obstaculo[1].texture, NULL, & obstaculo[1].dst_rect);
    obstaculo[1].dst_rect.y += velocidade;
    if(obstaculo[1].dst_rect.y > 640){

        int p = rand()%730;
        if( p>=230){
            obstaculo[1].dst_rect.x = p;
            obstaculo[1].dst_rect.y = -100;

            int num = p%2;
            if(num == 0){
                obstaculo[1].texture = textureGirl;
                obstaculo[1].bateu = 3;
            }else{
                obstaculo[1].texture = textureBarril;
                obstaculo[1].bateu = 0;
            }

            obstaculo[1].dst_rect.w = 32;
            obstaculo[1].dst_rect.h = 44;


            //SDL_RenderCopy(tela,  obstaculo[1].texture, NULL, & obstaculo[1].dst_rect);
        }
    }

    SDL_RenderCopy(tela,  obstaculo[2].texture, NULL, & obstaculo[2].dst_rect);
    obstaculo[2].dst_rect.y += velocidade;
    if(obstaculo[2].dst_rect.y > 640){



        int p = rand()%730;
        if( p>=230){
            obstaculo[2].dst_rect.x = p;
            obstaculo[2].dst_rect.y = -100;

           int num = p%2;
            if(num == 0){
                obstaculo[2].texture = textureGirl;
                obstaculo[2].bateu = 3;
            }else{
                obstaculo[2].texture = textureBarril;
                obstaculo[2].bateu = 0;
            }


            obstaculo[2].dst_rect.w = 32;
            obstaculo[2].dst_rect.h = 44;



            //SDL_RenderCopy(tela,  obstaculo[2].texture, NULL, & obstaculo[2].dst_rect);



        }
    }
}

int verificaColisao(  ){



        //Verifica batida lateral
        if( jogador.dst_rect.x+jogador.dst_rect.w > obstaculo[0].dst_rect.x && jogador.dst_rect.x+jogador.dst_rect.w <  obstaculo[0].dst_rect.x+80){
            if( jogador.dst_rect.y+jogador.dst_rect.h >  obstaculo[0].dst_rect.y  && jogador.dst_rect.y <  obstaculo[0].dst_rect.y+obstaculo[0].dst_rect.h ){

                //printf("\n---------------Bateu Lateral 00\n" );
                if(obstaculo[0].bateu == 0 ){
                    obstaculo[0].bateu = 1;
                    if(obstaculo[0].bateu == 1 ){

                                            obstaculo[0].texture = textureBoom;
                obstaculo[0].dst_rect.w = 132;
                obstaculo[0].dst_rect.h = 144;
                SDL_RenderCopy(tela,  obstaculo[0].texture, NULL, & obstaculo[0].dst_rect);

                        cont_vidas--;

                        Mix_PlayChannel(-1, jumpEffect, 0);

                        jogador.dst_rect.y += 40;

                    }
                }

                if(obstaculo[0].bateu == 3 ){

                    obstaculo[0].bateu = 1;
                    if(obstaculo[0].bateu == 1 ){

                                            obstaculo[0].texture = texturePlus;
                obstaculo[0].dst_rect.w = 132;
                obstaculo[0].dst_rect.h = 144;
                SDL_RenderCopy(tela,  obstaculo[0].texture, NULL, & obstaculo[0].dst_rect);

                        printf("++");
                        cont_obstaculos++;

                        Mix_PlayChannel(-1, soundPlus, 0);
                    }
                }
            }
        }


        if( jogador.dst_rect.x+jogador.dst_rect.w > obstaculo[1].dst_rect.x && jogador.dst_rect.x+jogador.dst_rect.w <  obstaculo[1].dst_rect.x+80){
            if( jogador.dst_rect.y+jogador.dst_rect.h >  obstaculo[1].dst_rect.y  && jogador.dst_rect.y <  obstaculo[1].dst_rect.y+obstaculo[1].dst_rect.h ){

                //printf("\n---------------Bateu Lateral 01\n" );

                if(obstaculo[1].bateu == 0){
                    obstaculo[1].bateu = 1;
                    if(obstaculo[1].bateu == 1 ){
                            obstaculo[1].texture = textureBoom;

                obstaculo[1].dst_rect.w = 132;
                obstaculo[1].dst_rect.h = 144;
                SDL_RenderCopy(tela,  obstaculo[0].texture, NULL, & obstaculo[0].dst_rect);
                        cont_vidas--;

                        Mix_PlayChannel(-1, jumpEffect, 0);

                        jogador.dst_rect.y += 40;
                    }
                }

                if(obstaculo[1].bateu == 3 ){
                    obstaculo[1].bateu = 1;
                    if(obstaculo[1].bateu == 1 ){
                            obstaculo[1].texture = texturePlus;

                obstaculo[1].dst_rect.w = 132;
                obstaculo[1].dst_rect.h = 144;
                SDL_RenderCopy(tela,  obstaculo[0].texture, NULL, & obstaculo[0].dst_rect);
                        printf("++");
                         cont_obstaculos++;

                        Mix_PlayChannel(-1, soundPlus, 0);
                    }
                }
            }
        }



        if( jogador.dst_rect.x+jogador.dst_rect.w > obstaculo[2].dst_rect.x && jogador.dst_rect.x+jogador.dst_rect.w <  obstaculo[2].dst_rect.x+80){
            if( jogador.dst_rect.y+jogador.dst_rect.h >  obstaculo[2].dst_rect.y  && jogador.dst_rect.y <  obstaculo[2].dst_rect.y+obstaculo[2].dst_rect.h ){
                //printf("\n---------------Bateu Lateral 03 \n" );

                if(obstaculo[2].bateu == 0){
                    obstaculo[2].bateu = 1;
                    if(obstaculo[2].bateu == 1 ){
                             obstaculo[2].texture = textureBoom;
                obstaculo[2].dst_rect.w = 132;
                obstaculo[2].dst_rect.h = 144;
                SDL_RenderCopy(tela,  obstaculo[0].texture, NULL, & obstaculo[0].dst_rect);
                        cont_vidas--;
                        Mix_PlayChannel(-1, jumpEffect, 0);
                        jogador.dst_rect.y += 40;
                    }
                }


                if(obstaculo[2].bateu == 3 ){

                    obstaculo[2].bateu = 1;
                    if(obstaculo[2].bateu == 1 ){
                             obstaculo[2].texture = texturePlus;
                obstaculo[2].dst_rect.w = 132;
                obstaculo[2].dst_rect.h = 144;
                SDL_RenderCopy(tela,  obstaculo[0].texture, NULL, & obstaculo[0].dst_rect);
                        printf("++");
                         cont_obstaculos++;

                        Mix_PlayChannel(-1, soundPlus, 0);
                    }
                }
            }
        }


        if(cont_vidas==0){
            printf("\n---------------FIM DE JOGO \n" );
            //Mix_CloseAudio();
            Mix_Pause(-1);
        }

    //printf("\n-Jogador_%d \n", jogador.dst_rect.y);

}

void inicializarComponentes(){
	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
	TTF_Init();

    //checando inicializacao
    if(TTF_Init() < 0)
    {
        printf("Erro Inicialização sistema fonte TTF_Init %s\n", TTF_GetError());
    }

}


// carrega fonte de texto
TTF_Font *carregarFonte(const char* arquivo, int tamanhoFonte){
    //TTF_Font *fonte = TTF_OpenFont("arial.ttf", 25);
    TTF_Font *fonte = TTF_OpenFont(arquivo, tamanhoFonte);
    TTF_Font *fonte1 = TTF_OpenFont(arquivo, tamanhoFonte);
    TTF_Font *fonte2 = TTF_OpenFont(arquivo, tamanhoFonte);

    if(!fonte)
        printf("Problema no carregamento da fonte!! %s\n", TTF_GetError());
    return fonte;
}


int inicializarFonte(){

    atexit(TTF_Quit);
    // Setar cor
    corTexto.r = 139;
    corTexto.g = 0;
    corTexto.b = 87;

    corTexto1.r = 109;
    corTexto1.g = 0;
    corTexto1.b = 15;
}


int inserirNome(){

    int indice = 0;
    int indiceMax = 12;

	char mensagem[] = "Digite seu nome:";

	SDL_Surface *ttfMensagem;
	SDL_Surface *ttfNome;
	SDL_Surface *ttfTraco;

	SDL_Texture * texturettfMensagem;
    SDL_Texture * texturettfNome;
    SDL_Texture * texturettfTraco;

	SDL_Rect posicaoMsg;
	SDL_Rect posicao;
	SDL_Rect posicaoTraco;

    ttfMensagem=   TTF_RenderText_Solid(fonte, mensagem, corTexto);
    texturettfMensagem = SDL_CreateTextureFromSurface(tela, ttfMensagem);

    ttfTraco = TTF_RenderText_Solid(fonte2, "------------" , corTexto);
    texturettfTraco = SDL_CreateTextureFromSurface(tela, ttfTraco);

	char jogador[30];

	int loop = 0;

	while(loop == 0){

         while(SDL_PollEvent(&event)){

            if(event.type == SDL_QUIT)
            {
                return 0;
            }
            else if( event.type == SDL_KEYDOWN )
			{
				switch( event.key.keysym.sym )
				{
					case SDLK_ESCAPE:
						return 0;
					break;


					case SDLK_a:
						if(indice<indiceMax)
						{
							jogador[indice] = 'a';
							indice++;
							jogador[indice] = '\0';
						}
						break;
					case SDLK_b:
						if(indice<indiceMax)
						{
							jogador[indice] = 'b';
							indice++;
							jogador[indice] = '\0';
						}
						break;
					case SDLK_c:
						if(indice<indiceMax)
						{
							jogador[indice] = 'c';
							indice++;
							jogador[indice] = '\0';
						}
						break;
					case SDLK_d:
						if(indice<indiceMax)
						{
							jogador[indice] = 'd';
							indice++;
							jogador[indice] = '\0';
						}
						break;
					case SDLK_e:
						if(indice<indiceMax)
						{
							jogador[indice] = 'e';
							indice++;
							jogador[indice] = '\0';
						}
						break;
					case SDLK_f:
						if(indice<30)
						{
							jogador[indice] = 'f';
							indice++;
							jogador[indice] = '\0';
						}
						break;
					case SDLK_g:
						if(indice<indiceMax)
						{
							jogador[indice] = 'g';
							indice++;
							jogador[indice] = '\0';
						}
						break;
					case SDLK_h:
						if(indice<indiceMax)
						{
							jogador[indice] = 'h';
							indice++;
							jogador[indice] = '\0';
						}
						break;
					case SDLK_i:
						if(indice<indiceMax)
						{
							jogador[indice] = 'i';
							indice++;
							jogador[indice] = '\0';
						}
						break;
					case SDLK_j:
						if(indice<indiceMax)
						{
							jogador[indice] = 'j';
							indice++;
							jogador[indice] = '\0';
						}
						break;
					case SDLK_k:
						if(indice<indiceMax)
						{
							jogador[indice] = 'k';
							indice++;
							jogador[indice] = '\0';
						}
						break;
					case SDLK_l:
						if(indice<indiceMax)
						{
							jogador[indice] = 'l';
							indice++;
							jogador[indice] = '\0';
						}
						break;
					case SDLK_m:
						if(indice<indiceMax)
						{
							jogador[indice] = 'm';
							indice++;
							jogador[indice] = '\0';
						}
						break;
					case SDLK_n:
						if(indice<indiceMax)
						{
							jogador[indice] = 'n';
							indice++;
							jogador[indice] = '\0';
						}
						break;
					case SDLK_o:
						if(indice<indiceMax)
						{
							jogador[indice] = 'o';
							indice++;
							jogador[indice] = '\0';
						}
						break;
					case SDLK_p:
						if(indice<indiceMax)
						{
							jogador[indice] = 'p';
							indice++;
							jogador[indice] = '\0';
						}
						break;
					case SDLK_q:
						if(indice<indiceMax)
						{
							jogador[indice] = 'q';
							indice++;
							jogador[indice] = '\0';
						}
						break;
					case SDLK_r:
						if(indice<indiceMax)
						{
							jogador[indice] = 'r';
							indice++;
							jogador[indice] = '\0';
						}
						break;
					case SDLK_s:
						if(indice<indiceMax)
						{
							jogador[indice] = 's';
							indice++;
							jogador[indice] = '\0';
						}
						break;
					case SDLK_t:
						if(indice<indiceMax)
						{
							jogador[indice] = 't';
							indice++;
							jogador[indice] = '\0';
						}
						break;
					case SDLK_u:
						if(indice<indiceMax)
						{
							jogador[indice] = 'u';
							indice++;
							jogador[indice] = '\0';
						}
						break;
					case SDLK_v:
						if(indice<indiceMax)
						{
                            jogador[indice] = 'v';
							indice++;
							jogador[indice] = '\0';
						}
						break;
					case SDLK_w:
						if(indice<indiceMax)
						{
							jogador[indice] = 'w';
							indice++;
							jogador[indice] = '\0';
						}
						break;
					case SDLK_x:
						if(indice<indiceMax)
						{
							jogador[indice] = 'x';
							indice++;
							jogador[indice] = '\0';
						}
						break;
					case SDLK_y:
						if(indice<indiceMax)
						{
							jogador[indice] = 'y';
							indice++;
							jogador[indice] = '\0';
						}
						break;
					case SDLK_z:
						if(indice<indiceMax)
						{
							jogador[indice] = 'z';
							indice++;
							jogador[indice] = '\0';
						}
						break;

					case SDLK_1:
						if(indice<indiceMax)
						{
							jogador[indice] = '1';
							indice++;
							jogador[indice] = '\0';
						}
						break;

					case SDLK_2:
						if(indice<indiceMax)
						{
							jogador[indice] = '2';
							indice++;
							jogador[indice] = '\0';
						}
						break;

					case SDLK_3:
						if(indice<indiceMax)
						{
							jogador[indice] = '3';
							indice++;
							jogador[indice] = '\0';
						}
						break;

					case SDLK_4:
						if(indice<indiceMax)
						{
							jogador[indice] = '4';
							indice++;
							jogador[indice] = '\0';
						}
						break;

					case SDLK_5:
						if(indice<indiceMax)
						{
							jogador[indice] = '5';
							indice++;
							jogador[indice] = '\0';
						}
						break;

					case SDLK_6:
						if(indice<indiceMax)
						{
							jogador[indice] = '6';
							indice++;
							jogador[indice] = '\0';
						}
						break;

					case SDLK_7:
						if(indice<indiceMax)
						{
							jogador[indice] = '7';
							indice++;
							jogador[indice] = '\0';
						}
						break;

					case SDLK_8:
						if(indice<indiceMax)
						{
							jogador[indice] = '8';
							indice++;
							jogador[indice] = '\0';
						}
						break;

					case SDLK_9:
						if(indice<indiceMax)
						{
							jogador[indice] = '9';
							indice++;
							jogador[indice] = '\0';
						}
						break;

					case SDLK_0:
						if(indice<indiceMax)
						{
							jogador[indice] = '0';
							indice++;
							jogador[indice] = '\0';
						}
						break;

					case SDLK_BACKSPACE:
						// apagar o que foi digitado
						if(indice>0)
						{
							indice--;
							jogador[indice] = ' ';
						}
						break;
					default:
						break;


				}

				ttfNome = TTF_RenderText_Solid(fonte2, jogador, corTexto);
                texturettfNome = SDL_CreateTextureFromSurface(tela, ttfNome);
			}

            else if(event.type == SDL_MOUSEBUTTONDOWN)
            {
            	if((event.button.x >= 790 && event.button.x <= 850) && (event.button.y >= 530 && event.button.y <= 570))
            	{
                    printf(" Ao clicar no botao vai para RANKING\n");
                    strcpy(nomeJogador,jogador); // Ao clicar no botao do RANKING copia string temporaria p/ string global

                    int a = cont_obstaculos *100;
                    cont_kilometragem = cont_kilometragem + a;

                    Jogador *j1 = criarJogador(nomeJogador,cont_kilometragem);
                    inserirOrdenado(lista,j1);


                    if(!recorde()){

                        loop = 1;
                    }
            	}
            }
        }


        int fuW = 980;
        int fuH = 640;

        int posicaoMsgW = 300;
        int posicaoMsgH = 200;

        int posicaoW = 290;
        int posicaoH = 300;

        int posicaoTracoW = 290;
        int posicaoTracoH = 300;

        SDL_Rect dstrectfu = {0,0, fuW, fuH };

        SDL_QueryTexture(texturettfMensagem, NULL, NULL, &posicaoMsgW, &posicaoMsgH);
        SDL_Rect dstrectposicaoMsg = {350,200,posicaoMsgW,posicaoMsgH};

        SDL_QueryTexture(texturettfTraco, NULL, NULL, &posicaoTracoW, &posicaoTracoH);
        SDL_Rect dstrectTraco = {250,340,posicaoTracoW,posicaoTracoH};

        SDL_QueryTexture(texturettfNome, NULL, NULL, &posicaoW, &posicaoH);
        SDL_Rect dstrectNome = {250,300,posicaoW,posicaoH};

        SDL_RenderClear(tela);//sem esta linha a imagem nao atualiza

        SDL_RenderCopy(tela, textureInserirNome,NULL,&dstrectfu);
        SDL_RenderCopy(tela,texturettfMensagem,NULL,&dstrectposicaoMsg);//renderizar a textura
        SDL_RenderCopy(tela, texturettfTraco,NULL,&dstrectTraco);
        SDL_RenderCopy(tela, texturettfNome,NULL,&dstrectNome);

        SDL_RenderPresent(tela);

    }
}

int recorde(){


    SDL_RenderClear(tela);//sem esta linha a imagem nao atualiza
    SDL_RenderCopy(tela, textureFundoRanking,NULL,NULL );
    SDL_RenderPresent(tela);

    No *pontuacaoAux = lista->inicio;

	int tex1W = 0;
    int tex1H = 0;

    int tex2W = 0;
    int tex2H = 0;

    int yy = 25;

    while(pontuacaoAux != NULL){

        sprintf(bufferPontuacao,"%d", pontuacaoAux->jogador->pontuacao);

        printf("%s -> ",pontuacaoAux->jogador->nome);
        printf("%d\n",pontuacaoAux ->jogador->pontuacao);


        texto1 = TTF_RenderText_Solid(fonte,pontuacaoAux->jogador->nome,corTexto);
        texto2 = TTF_RenderText_Solid(fonte,bufferPontuacao,corTexto);

        textureTexto1 = SDL_CreateTextureFromSurface(tela, texto1);
        textureTexto2 = SDL_CreateTextureFromSurface(tela, texto2);


        SDL_QueryTexture(textureTexto1, NULL, NULL, &tex1W, &tex1H);
        SDL_Rect dstrecttexto1 = {550, yy, tex1W, tex1H };


        SDL_QueryTexture(textureTexto2, NULL, NULL, &tex2W, &tex2H);
        SDL_Rect dstrecttexto2 = {750, yy, tex2W, tex2H };

        //SDL_RenderCopy(tela, textureFundoRanking,NULL,NULL );
        SDL_RenderCopy(tela,textureTexto1 ,NULL,&dstrecttexto1);
        SDL_RenderCopy(tela,textureTexto2 ,NULL,&dstrecttexto2);

        SDL_RenderPresent(tela);

        printf("%d -> ", dstrecttexto1.y);

        pontuacaoAux = pontuacaoAux->prox;

        yy+=40;

      }


        int loop = 0; // Variável de controle do loop
        while(loop == 0){

        // Ler a fila de eventos e colocar o evento mais antigo em "event"
        // Loop de eventos
        while(SDL_PollEvent(&event)){
            if(event.type == SDL_QUIT){
                return 0;
            }

            else if(event.type == SDL_MOUSEBUTTONDOWN){

            	if((event.button.x >= 790 && event.button.x <= 850) && (event.button.y >= 530 && event.button.y <= 570)){
                    printf("Cliquei em voltar a tela de MENU\n");

                    if(cont_vidas == 0){
                        printf("voltar ao MENU\n");
                        menu();
                    }else{
                        return 1;
                    }
            	}
            }
        }
    }
}


int opcoes(){

    fundoDesenvolvedores = IMG_Load("Imagens/opcoes.png");
    textureFundoDesenvolvedores= SDL_CreateTextureFromSurface(tela,fundoDesenvolvedores);

    imagemCar1 = IMG_Load("Imagens/car3.png");
    textureCar1= SDL_CreateTextureFromSurface(tela,imagemCar1);

    imagemCar2 = IMG_Load("Imagens/car1.png");
    textureCar2= SDL_CreateTextureFromSurface(tela,imagemCar2);

    imagemCar3 = IMG_Load("Imagens/car2.png");
    textureCar3= SDL_CreateTextureFromSurface(tela,imagemCar3);

    SDL_Rect dst_rect_car0;
    dst_rect_car0.x = 780;
    dst_rect_car0.y = 250;
    dst_rect_car0.w = 100;
    dst_rect_car0.h = 240;

    SDL_Rect src_rect_car0;
    src_rect_car0.x = 0;
    src_rect_car0.y = 0;
    src_rect_car0.w = 600;
    src_rect_car0.h = 670;

    SDL_Rect dst_rect_car1;
    dst_rect_car1.x = 700;
    dst_rect_car1.y = 10;
    dst_rect_car1.w = 100;
    dst_rect_car1.h = 240;

    SDL_Rect src_rect_car1;
    src_rect_car1.x = 0;
    src_rect_car1.y = 0;
    src_rect_car1.w = 600;
    src_rect_car1.h = 670;

    SDL_Rect dst_rect_car2;
    dst_rect_car2.x = 850;
    dst_rect_car2.y = 20;
    dst_rect_car2.w = 100;
    dst_rect_car2.h = 220;

    SDL_Rect src_rect_car2;
    src_rect_car2.x = 0;
    src_rect_car2.y = 0;
    src_rect_car2.w = 600;
    src_rect_car2.h = 670;

    SDL_RenderClear(tela);//sem esta linha a imagem nao atualiza

    SDL_RenderCopy(tela, textureFundoDesenvolvedores,NULL,NULL);


    SDL_RenderCopy(tela, textureCar1, &src_rect_car0, &dst_rect_car0);
    SDL_RenderCopy(tela, textureCar2, &src_rect_car1, &dst_rect_car1);
    SDL_RenderCopy(tela, textureCar3, &src_rect_car2, &dst_rect_car2);

    SDL_RenderPresent(tela);

    int loop = 0; // Variável de controle do loop
    while(loop == 0){

        // Lê a fila de eventos e põe o evento mais antigo em "event"
        while(SDL_PollEvent(&event)){
            if(event.type == SDL_QUIT){
                return 0;
            }

            else if(event.type == SDL_MOUSEBUTTONDOWN){

            	if((event.button.x >= 790 && event.button.x <= 850) && (event.button.y >= 530 && event.button.y <= 570)){
                    printf("Cliquei em voltar a2 tela de MENU\n");

                    return 1;
            	}

            	if((event.button.x >= 710 && event.button.x <= 795) && (event.button.y >= 17 && event.button.y <= 235)){
                    printf("CAR1\n");
                    carSelectd = 1;
                    //return 1;
            	}


            	if((event.button.x >= 850 && event.button.x <= 950) && (event.button.y >= 17 && event.button.y <= 235)){
                    printf("CAR2\n");
                    carSelectd = 2;
                    //return 1;
            	}

                if((event.button.x >= 780 && event.button.x <= 870) && (event.button.y >= 250 && event.button.y <= 490)){
                    printf("CAR3\n");
                    carSelectd = 3;
                    //return 1;

            	}
            }
        }
    }
}

int developers(){
    SDL_RenderClear(tela);//sem esta linha a imagem nao atualiza
    SDL_RenderCopy(tela, textureFundoDesenvolvedores,NULL,NULL);
    SDL_RenderPresent(tela);

    int loop = 0; // Variável de controle do loop
    while(loop == 0){

        // Ler a fila de eventos e colocar o evento mais antigo em "event"
        // Loop de eventos
        while(SDL_PollEvent(&event)){
            if(event.type == SDL_QUIT){
                return 0;
            }

            else if(event.type == SDL_MOUSEBUTTONDOWN){

            	if((event.button.x >= 790 && event.button.x <= 850) && (event.button.y >= 530 && event.button.y <= 570)){
                    printf("Cliquei em voltar a tela de MENU\n");
                    return 1;
            	}
            }

        }
    }
}


int instrucoes(){

    int loop = 0; // Variável de controle do loop


    SDL_RenderClear(tela);//sem esta linha a imagem nao atualiza
    SDL_RenderCopy(tela, textureFundoInstrucoes,NULL,NULL);
    SDL_RenderPresent(tela);

    while (loop == 0) // Loop principal
    {
        // Lê a fila de eventos e põe o evento mais antigo em "event"
        while (SDL_PollEvent(&event)){ // Loop de eventos
            // Verifica se o evento mais antigo é do tipo SDL_QUIT
            if (event.type == SDL_QUIT){ // Se o usuário clicou para fechar a janela
                return 0;

            }else if(event.type == SDL_MOUSEBUTTONDOWN){
                if((event.button.x >= 790 && event.button.x <= 850) && (event.button.y >= 530 && event.button.y <= 570)){
                    printf("Cliquei em voltar a tela de MENU\n");
                    return 1;
            	}
            }
        }
    }
}
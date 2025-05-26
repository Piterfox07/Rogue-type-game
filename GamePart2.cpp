#include <iostream>
#include <windows.h>
#include <conio.h>
#include <stdlib.h>
#include <fstream>
#include <string>
#include <chrono>
#include <thread>

#define LARGURA 80
#define ALTURA 25
#define INIMIGOS 9

using namespace std;

template <typename ar>

ar armaduraPotente(ar& a, ar b){
    return a - b;
}

struct Ponto{
    int x, y;
};
struct StatusItems{
    int PocaoCura, Espada, PowerUp;
    Ponto pontoEspada, pontoPocaoCura, pontoPowerUp;
    int Escudo,SuperBotasDeAgilidade,CristalVida,AumentaProbabilidadeAtaque;
    Ponto pontoEscudo, pontoBotasDeAgilidade, pontoCristalVida, pontoAumentaProbabilidadeAtaque;

};

struct StatusPersonagem{
    int nivel, ataque, vida,armadura, experiencia, VidaMaxima, Score;
    Ponto pontoPersonagem;
    string nome;
    int cor = 15;
    int nivelDificuldade;
};

struct StatusInimigos{
    int vida, ataque, experiencia;
    Ponto pontoInimigos;
    bool vivo, visivel;
};

///função para chamar a posição x e y.
void posicao(int x, int y){
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD cordenada;
    cordenada.X = x;
    cordenada.Y = y;
    SetConsoleCursorPosition(hConsole, cordenada);
}

int larguraConsole() {
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    return csbi.srWindow.Right - csbi.srWindow.Left + 1;
}

///Função pra printar no meio da tela
void textoCentralizado(const string& texto, int cor = 7) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(hConsole, &csbi);

    int espacos = (csbi.srWindow.Right - texto.length()) / 2;
    SetConsoleTextAttribute(hConsole, cor);
    cout << string(espacos, ' ') << texto << endl;
    SetConsoleTextAttribute(hConsole, 7); // Reset para branco
}

void textoCentralizado(const string& texto, const string& corHex) {
    int cor;
    try {
        cor = stoi(corHex, nullptr, 16); // Converte hex string para int
    } catch (...) {
        cor = 7; // Valor padrão em caso de erro
    }
    textoCentralizado(texto, cor); // Chama a versão numérica
}

//Função para limpar apenas uma area da tela
void limparTela(int x, int y, int largura, int altura) {
    for (int i = 0; i < altura; i++) {
        posicao(x, y + i);
        for (int j = 0; j < largura; j++) {
            cout << " ";
        }
    }
}

//Função para mudar a cor do cursor
void mudarCor(HANDLE hConsole, int cor) {
    SetConsoleTextAttribute(hConsole, cor);
}

//Mudar a cor das informações no menu
void corInf(HANDLE hConsole, int Cursor, int numeroCursor){
    if(Cursor==numeroCursor){
        mudarCor(hConsole, 0xB); // Ciano
    }else{
        mudarCor(hConsole, 0x7); // branco
    }
}

//informações do menu
void informacoesMenu(){
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    int cursorAnterior = 0;
    int cursorInfMenuPrincipal=0, cursorInfMenuPrincipalNew;
    char teclaInfMenuPrincipal;
    bool finalizarInfMenuPrincipal = false;

    while(finalizarInfMenuPrincipal==false){
        mudarCor(hConsole, 0x7); // branco
        posicao (0, 4);
        for(int i=0; i<120; i++){ //imprimir barra inferior
            if(i!=32 && i!=89){
                cout << char(205);
            }else{
                cout << char(202);
            }
        }
        corInf(hConsole, cursorInfMenuPrincipal, 0); // muda a cor da letra caso o cursor esteja em cima
        posicao (13, 2); cout << "Inimigos";
        mudarCor(hConsole, 0x7); // branco

        for(int i=0; i<4; i++){ // imprime barra lateral
            posicao(32, i); cout << char(186);
        }

        corInf(hConsole, cursorInfMenuPrincipal, 1); // muda a cor da letra caso o cursor esteja em cima
        posicao(53, 2); cout << "Mapa e Itens";
        mudarCor(hConsole, 0x7); // branco

        for(int i=0; i<4; i++){ // imprime barra lateral
            posicao(89, i); cout << char(186);
        }

        corInf(hConsole, cursorInfMenuPrincipal, 2); // muda a cor da letra caso o cursor esteja em cima
        posicao(97, 2); cout << "Sistema de Pontos";
        mudarCor(hConsole, 0x7); // branco

        posicao(0, 29); cout << "Pressione Z para voltar";

        // imprime informações quando o cursor estiver em cima de inimigos
       if(cursorInfMenuPrincipal == 0){
            posicao(0, 7);  textoCentralizado("TAXA DE ACERTO DE ATAQUES INIMIGOS AUMENTA COM A DIFICULDADE", 8);

            posicao(0, 9);  textoCentralizado("VIDA DOS INIMIGOS AUMENTA DE ACORDO COM O MAPA ATUAL", 8);

            posicao(0, 11);  textoCentralizado("CORES DOS INIMIGOS (I) DO MAIS FRACO AO MAIS FORTE: ", 8);

            posicao(0, 15);  textoCentralizado("verde claro", 0xA); // verde claro

            posicao(0, 17); textoCentralizado("verde escuro", 2); // verde escuro

            posicao(0, 19); textoCentralizado("amarelo claro", 0xE); // amarelo claro

            posicao(0, 21); textoCentralizado("amarelo escuro", 6); // amarelo escuro

            posicao(0, 23); textoCentralizado("B vermelho = Boss", 4); // vermelho escuro
        }

        // imprime informações quando o cursor estiver em cima de mapa e itens
        if(cursorInfMenuPrincipal == 1){
            posicao(0, 7);  textoCentralizado("X = Armadilhas (-10 de Vida)");
            posicao(0, 9);  textoCentralizado("E = espada (+5 de Dano)");
            posicao(0, 11); textoCentralizado("C = pocao de cura (+50 de Vida)");
            posicao(0, 12); textoCentralizado("(pressione R para usar a pocao)");
            posicao(0, 14); textoCentralizado("T = Tecnica de combate (chance de acerto++)");
            posicao(0, 16); textoCentralizado("Y = item secreto (upa um nivel)");
            posicao(0, 17); textoCentralizado("(o item secreto ultiliza automaticamente)");
            posicao(0, 19); textoCentralizado("V = Cristal de Vida (+20 de vida maxima)");
            posicao(0, 21); textoCentralizado("A = Botas da Agilidade (chance de desviar++)");
            posicao(0, 23); textoCentralizado("O = Escudo (+1 de Defesa)");
        }

        // imprime informações quando o cursor estiver em cima de score
        if(cursorInfMenuPrincipal == 2){
            posicao(0, 7); textoCentralizado("CADA MINUTO QUE PASSA OS PONTOS ADIQUIRIDOS DIMINUEM EM 1", 8);
            posicao(0, 8); textoCentralizado("(podendo reduzir sua pontuacao)", 8);
            posicao(0, 10); textoCentralizado("subir de nivel = 40 pontos");
            posicao(0, 12); textoCentralizado("Adiquirir qualquer item = 20 pontos");
            posicao(0, 14); textoCentralizado("inimigos dao pontos dependendo de sua forca");

            posicao(0, 16); textoCentralizado("verde claro = 10", 0xA);// verde claro

            posicao(0, 18); textoCentralizado("verde escuro = 15", 2);// verde escuro

            posicao(0, 20); textoCentralizado("amarelo claro = 20", 0xE);// amarelo claro

            posicao(0, 22); textoCentralizado("amarelo escuro = 25", 6);// amarelo escuro

            posicao(0, 24); textoCentralizado("vermelho = 100", 4);// vermelho escuro
        }



        if(_kbhit()){ //Se estiver pressionando uma tecla
        teclaInfMenuPrincipal = getch(); //Recebe o valor da tecla pressionada

        cursorInfMenuPrincipalNew = cursorInfMenuPrincipal;
            switch (teclaInfMenuPrincipal) {
                case 75: case 'a': case 'A': cursorInfMenuPrincipalNew--;break; // Esquerda
                case 77: case 'd': case 'D': cursorInfMenuPrincipalNew++; break; // Direita
                case 'z': case 'Z': // fecha as informações
                    system("cls");
                    finalizarInfMenuPrincipal=true;
                break;
            }

            if(cursorInfMenuPrincipalNew>=0 && cursorInfMenuPrincipalNew<=2){
            cursorInfMenuPrincipal = cursorInfMenuPrincipalNew;
            }

            if(cursorInfMenuPrincipal != cursorAnterior){
                limparTela(37, 7, 55, 21);
                cursorAnterior = cursorInfMenuPrincipal;
            }
        }
    }
}

//Imprime os creditos
void creditos(){
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    system("cls");

    posicao(0, 9); textoCentralizado("Universidade do Vale do Itajai");
    posicao(0, 13); textoCentralizado("Professor: Alex Luciano Roesler Rese");
    posicao(0, 17); textoCentralizado("Alunos: Marlon Ranguet Zucchetti, Gabriel Paranagua, Henrique Gustavo Gonzaga Belli");
    posicao(0, 29);

    system("pause");

}

void ordenarRankings(StatusPersonagem scores[], int inicio, int fim) {
    if (inicio >= fim) return;

    int temp = scores[fim].Score;
    int i = inicio - 1;

    for (int j = inicio; j < fim; j++) {
        if (scores[j].Score >= temp) {
            i++;
            swap(scores[i], scores[j]);
        }
    }
    swap(scores[i + 1], scores[fim]);

    ordenarRankings(scores, inicio, i);
    ordenarRankings(scores, i + 2, fim);
}

void exibirTop10() {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    const int MAX = 100;
    StatusPersonagem scores[MAX];
    int contador = 0;

    ifstream arquivo("pontuacao.txt");
    while (arquivo >> scores[contador].nome >> scores[contador].Score && contador < MAX) {
        contador++;
    }
    arquivo.close();

    ordenarRankings(scores, 0, contador - 1);

    system("cls");

    textoCentralizado("=== TOP 10 ===", 5);

    cout << endl << endl;

    for (int i = 0; i < min(10, contador); i++) {
        string linha = to_string(i+1) + ". " + scores[i].nome + " - " + to_string(scores[i].Score);
        textoCentralizado(linha);
    }

    posicao(0, 29);

    system("pause");
    system("cls");
}

int nivelDificuldade(bool &finalizarMenuPrincipal){

    while(true){
        system("cls");

        posicao(0, 11); textoCentralizado("Escolha o nivel de dificuldade que deseja jogar:", 12);
        posicao(0, 13); textoCentralizado("1 - SANDBOX - Imortalidade + One Punch");
        posicao(0, 15); textoCentralizado("2 - FACIL - chance de acerto garantida");
        posicao(0, 17); textoCentralizado("3 - MEDIO - dificuldade padrao");
        posicao(0, 28); cout << "Pressione qualquer outra tecla para retornar";

        char tecla = getch();

        switch(tecla){
            case '1':
                while(true){
                    system("cls");
                    posicao(0, 11); textoCentralizado("Tem certeza que deseja escolher a dificuldade SANDBOX?", 12);
                    posicao(0, 13); textoCentralizado("1 - Sim          2 - Nao");

                    char confirmar = getch();
                    if (confirmar == '1') {
                        finalizarMenuPrincipal = true;
                        return 1;
                    } else if (confirmar == '2') {
                        break;
                    }
                }
                break;

            case '2':
                while(true){
                    system("cls");
                    posicao(0, 11); textoCentralizado("Tem certeza que deseja escolher a dificuldade FACIL?", 12);
                    posicao(0, 13); textoCentralizado("1 - Sim          2 - Nao");

                    char confirmar = getch();
                    if (confirmar == '1') {
                        finalizarMenuPrincipal = true;
                        return 2;
                    } else if (confirmar == '2') {
                        break; // <-- Sai da confirmação e volta para o menu principal
                    }
                }
                break;

            case '3':
                while(true){
                    system("cls");
                    posicao(0, 11); textoCentralizado("Tem certeza que deseja escolher a dificuldade MEDIA?", 12);
                    posicao(0, 13); textoCentralizado("1 - Sim          2 - Nao");

                    char confirmar = getch();
                    if (confirmar == '1') {
                        finalizarMenuPrincipal = true;
                        return 3;
                    } else if (confirmar == '2') {
                        break;
                    }
                }
                break;

            default:
                return 0;
        }
    }
}

//Menu principal do jogo (que aparece no inicio)
int printarMenuInicial(){
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    int cursorMenuPrincipal=0, cursorMenuPrincipalNew;
    char teclaMenuPrincipal;
    bool finalizarMenuPrincipal = false;
    int dificuldade;

    while(finalizarMenuPrincipal==false){
        mudarCor(hConsole, 0x7); // branco

        posicao(58, 11); cout << "Jogar";
        posicao(58-3, 13); cout << "Informacoes";
        posicao(58-4, 15); cout << "Ranking top 10";
        posicao(58-2, 17); cout << "Creditos";
        posicao(58, 19); cout << "Sair";

        mudarCor(hConsole, 0x4); //vermelho

        //imprime a seta onde estiver o cursor
        if(cursorMenuPrincipal==0){
            posicao(54, 11); cout << "==>";
        }else{
            posicao(54, 11); cout << "   ";
        }

        if(cursorMenuPrincipal==1){
            posicao(54-3, 13); cout << "==>";
        }else{
            posicao(54-3, 13);  cout << "   ";
        }

        if(cursorMenuPrincipal==2){
            posicao(54-4, 15); cout << "==>";
        }else{
            posicao(54-4, 15); cout << "   ";
        }
        if(cursorMenuPrincipal==3){
            posicao(54-2, 17); cout << "==>";
        }else{
            posicao(54-2, 17); cout << "   ";
        }

        if(cursorMenuPrincipal==4){
            posicao(54, 19); cout << "==>";
        }else{
            posicao(54, 19); cout << "   ";
        }


        if (_kbhit()) { //Se estiver pressionando uma tecla
            teclaMenuPrincipal = getch(); //Recebe o valor da tecla pressionada

            cursorMenuPrincipalNew = cursorMenuPrincipal;

            switch (teclaMenuPrincipal) {
                case 72: case 'w': case 'W': cursorMenuPrincipalNew--; break; // Cima
                case 80: case 's': case 'S': cursorMenuPrincipalNew++; break; // Baixo
                case 13: case ' ': //espaço ou enter para confirmar
                if(cursorMenuPrincipal==0){ // limpa a tela e finaliza a função
                    dificuldade = nivelDificuldade(finalizarMenuPrincipal);
                    system("cls");
                }else if(cursorMenuPrincipal==1){ // limpa a tela e abre informações
                    system("cls");
                    informacoesMenu();
                }else if(cursorMenuPrincipal==2){// limpa a tela e abre as 10 melhores pontuacoes
                    exibirTop10();
                    system("cls");
                }else if(cursorMenuPrincipal==3){// limpa a tela e abre os creditos
                    creditos();
                    system("cls");
                }else if(cursorMenuPrincipal==4){ // finaliza o programa
                    exit(0);
                }
                break;
            }

            if(cursorMenuPrincipalNew>=0 && cursorMenuPrincipalNew<=4){
                cursorMenuPrincipal = cursorMenuPrincipalNew;
            }
        }
    }
    if (finalizarMenuPrincipal == true && dificuldade > 0){
        return dificuldade;
    }
}

//Printa a borda do menu da direita
void printarBordaMenu(){
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    mudarCor(hConsole, 0x07);

    //imprime a lateral da borda
    for(int i=0; i<20; i++){
        posicao(90, 0+i);
        cout << char(219);
    }

    //imprime a parte inferior da borda
    posicao(90, 20);
    for(int i=0; i<30; i++){
        cout << char(219);
    }
}

//Menu (da direita)
void printarMenu(){
    int cursorMenu=0, cursorMenuNew;
    char teclaMenu;
    bool finalizarMenu=false;
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE); // Comando que permite mudar de cor
        while(finalizarMenu==false){
            mudarCor(hConsole, 0x07); // Seta pra cor branca

            posicao(104, 5); cout << "Jogar";
            posicao(104-3, 8); cout << "Informacoes";
            posicao(104-1, 11); cout << "Top 10";
            posicao(104, 14); cout << "Sair";

            mudarCor(hConsole, 0x04); // Seta pra cor vermelha

            //imprime as setas onde o cursor estiver
            if(cursorMenu==0){
                posicao(100, 5); cout << "==>";
            }else{
                posicao(100, 5); cout << "   ";
            }

            if(cursorMenu==1){
                posicao(100-3, 8); cout << "==>";
            }else{
                posicao(100-3, 8);  cout << "   ";
            }

            if(cursorMenu==2){
                posicao(100-1, 11); cout << "==>";
            }else{
                posicao(100-1, 11);  cout << "   ";
            }

            if(cursorMenu==3){
                posicao(100, 14); cout << "==>";
            }else{
                posicao(100, 14); cout << "   ";
            }

            if (_kbhit()) { //Se estiver pressionando uma tecla
                teclaMenu = getch(); //Recebe o valor da tecla pressionada
                cursorMenuNew = cursorMenu;

                switch (teclaMenu) {
                    case 72: case 'w': case 'W': cursorMenuNew--; break; // Cima
                    case 80: case 's': case 'S': cursorMenuNew++; break; // Baixo
                    case 13: case ' ':

                    //Finaliza a função
                    if(cursorMenu==0){  //Limpa a tela dentro da borda da direita
                        finalizarMenu=true;
                        limparTela(91, 0, 29, 20);
                    } else if(cursorMenu==1){   //Abre as informações
                        system("cls");
                        informacoesMenu();
                        finalizarMenu=true;
                    } else if(cursorMenu==2){   //Abre o ranking
                        system("cls");
                        exibirTop10();
                        finalizarMenu=true;
                    } else if(cursorMenu==3){   //Finaliza o programa
                        exit(0);
                    }
                    break;

                }

            if(cursorMenuNew>=0 && cursorMenuNew<=3){
                cursorMenu = cursorMenuNew;
            }
        }

    }
}

//Imprime os status dentro da borda
void Status(StatusPersonagem status, StatusItems item, int Upgrade){
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    mudarCor(hConsole, 0x7); // branco
    posicao(93, 0); cout << "Botao C para abrir o menu.";

    mudarCor(hConsole, 0xE);
    posicao(102, 3); cout << "SCORE: [" << status.Score << "]    ";

    mudarCor(hConsole, 0xF);
    posicao(92, 6); cout << "Level:         [" << status.nivel << "]    ";

    mudarCor(hConsole, 0x4);
    posicao(92, 8); cout << "Vida:          [" << status.vida << "/" << status.VidaMaxima << "]   ";

    mudarCor(hConsole, 0x6);
    posicao(92, 10); cout << "Armadura:      [" << status.armadura << "]    ";

    mudarCor(hConsole, 0xB);
    posicao(92, 12); cout << "Dano:          [" << status.ataque << "]    ";

    mudarCor(hConsole, 0xA);
    posicao(92, 14); cout << "XP:            [" << status.experiencia << "/" << (15 * status.nivel) << "]    ";

    mudarCor(hConsole, 0xC);
    posicao(92, 16); cout << "Pocoes de cura:[" << item.PocaoCura << "]   ";

    mudarCor(hConsole, 0x7);
    posicao(92, 18);
    cout << "Pontos de status:  " << "[" << Upgrade <<"]";
}

void limparBufferDeInput() {
    while (_kbhit()) {
        _getch(); // Lê e descarta todos os inputs pendentes
    }
}

void salvarPontuacao(StatusPersonagem personagem) {
    ofstream arquivo("pontuacao.txt", ios::app);
    if (arquivo.is_open()) {
        arquivo << personagem.nome << ": " << personagem.Score << endl;
        arquivo.close();
    }
}

void fimDeJogo(StatusPersonagem& personagem, bool venceu) {
    system("cls");
    cout << "Fim de jogo! " << (venceu ? "Você venceu!" : "Você morreu!") << endl;
    cout << "Sua pontuação final foi de: " << personagem.Score << " pts." << endl;

    cout << "Digite seu nome: ";
    cin >> personagem.nome;

    if (personagem.nome == "apagarArquivo") {
        ofstream arquivo("pontuacao.txt", ios::trunc);
        arquivo.close();
        cout << "Pontuações resetadas!" << endl;
    } else if (personagem.nome == "rodadaTeste"){
        cout << "Sua pontuação nao foi salva!" << endl;
    }else {
        salvarPontuacao(personagem);
        cout << "Pontuação salva!" << endl;
    }
}

void cronometro(auto &inicio, int &minuto = 0) {
    posicao(0, 0);
    // Espera por um segundo antes de mostrar o tempo
    auto agora = chrono::steady_clock::now();
    auto duracao = chrono::duration_cast<chrono::seconds>(agora - inicio);

    if(duracao.count() == 60) {
        inicio = chrono::steady_clock::now();
        minuto++;
    }
    cout << "Tempo: " << minuto << ":" ;

    if(duracao.count() < 10) {
        cout << "0";
    }
    cout << duracao.count() << "                       " << endl;
}

void primeiromapa(int m[ALTURA][LARGURA], StatusPersonagem &personagem){

    int mapa1[ALTURA][LARGURA] = {
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,1,1,1,0,0,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,1,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1},
    {1,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,1,1,0,0,0,1,1,1,1,1,1,0,0,0,1,1,1,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1,1,1,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,1,0,0,1,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1,1,1,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,0,0,1,0,0,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,1,5,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
    };

    for(int i=0; i<=ALTURA; i++){
        for(int j=0; j<LARGURA; j++){
            m[i][j] = mapa1[i][j];
        }
    }

    // posição inicial do personagem
    personagem.pontoPersonagem.x = 1;
    personagem.pontoPersonagem.y = 2;
}

void segundomapa(int m[ALTURA][LARGURA], StatusPersonagem &personagem,bool revelado[ALTURA][LARGURA], StatusInimigos inimigo[INIMIGOS]){

    int mapa2[ALTURA][LARGURA] = {
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
    {1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,0,0,0,0,0,1,1,1,1,1,1,1,1,0,0,0,1},
    {1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1},
    {1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1},
    {1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1},
    {1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1},
    {1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1},
    {1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1},
    {1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1},
    {1,1,1,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,5,0,0,0,0,0,0,0,0,0,1,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1},
    {1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1},
    {1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1},
    {1,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,0,0,0,0,0,1,1,1,1,1,1,1,1,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
    };

    for(int i=0; i<=ALTURA; i++){
        for(int j=0; j<LARGURA; j++){
            m[i][j] = mapa2[i][j];
        }
    }

    for(int i = 0; i < ALTURA; i++) {
        for(int j = 0; j < LARGURA; j++) {
            revelado[i][j] = false;
        }
    }
    for(int i = 0; i < 3; i++){
        inimigo[i].vivo = false;
    }

    // posição inicial do personagem
    personagem.pontoPersonagem.x = 1;
    personagem.pontoPersonagem.y = 2;
}

void terceiromapa(int m[ALTURA][LARGURA], StatusPersonagem &personagem, bool revelado[ALTURA][LARGURA],StatusInimigos inimigo[INIMIGOS]){

    int mapa3[ALTURA][LARGURA] = {
    {3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3},
    {3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3},
    {3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3},
    {3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3},
    {3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3},
    {3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,3,3,3,3,3,3,3,3,3,3,3,3,3,3},
    {3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,3,3,3,3,3,3,3,3,3,3,3,3,3,3},
    {3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,3,3,3,3,3,3,3,3,3,3,3,3,3,3},
    {3,3,3,3,3,3,3,3,3,3,3,3,3,1,1,1,1,1,1,1,1,1,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,3,3,3,3,3,3,3,3,3,3,3,3,3,3},
    {3,3,3,3,3,3,3,3,3,3,3,3,3,1,0,0,0,0,0,0,0,1,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,3,3,3,3,3,3,3,3,3,3,3,3,3,3},
    {3,3,3,3,3,3,3,3,3,3,3,3,3,1,0,0,0,0,0,0,0,0,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,3,3,3,3,3,3,3,3,3,3,3,3,3,3},
    {3,3,3,3,3,3,3,3,3,3,3,3,3,1,0,0,0,0,0,0,0,0,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,3,3,3,3,3,3,3,3,3,3,3,3,3,3},
    {3,3,3,3,3,3,3,3,3,3,3,3,3,1,0,0,0,0,0,0,0,0,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,3,3,3,3,3,3,3,3,3,3,3,3,3,3},
    {3,3,3,3,3,3,3,3,3,3,3,3,3,1,0,0,0,0,0,0,0,0,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,3,3,3,3,3,3,3,3,3,3,3,3,3,3},
    {3,3,3,3,3,3,3,3,3,3,3,3,3,1,0,0,0,0,0,0,0,0,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,3,3,3,3,3,3,3,3,3,3,3,3,3,3},
    {3,3,3,3,3,3,3,3,3,3,3,3,3,1,0,0,0,0,0,0,0,1,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,3,3,3,3,3,3,3,3,3,3,3,3,3,3},
    {3,3,3,3,3,3,3,3,3,3,3,3,3,1,1,1,1,1,1,1,1,1,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,3,3,3,3,3,3,3,3,3,3,3,3,3,3},
    {3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,3,3,3,3,3,3,3,3,3,3,3,3,3,3},
    {3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,3,3,3,3,3,3,3,3,3,3,3,3,3,3},
    {3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,3,3,3,3,3,3,3,3,3,3,3,3,3,3},
    {3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3},
    {3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3},
    {3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3},
    {3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3},
    {3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3},
    };

    for(int i=0; i<ALTURA; i++){
        for(int j=0; j<LARGURA; j++){
            m[i][j] = mapa3[i][j];
        }
    }

    for(int i = 0; i < ALTURA; i++) {
        for(int j = 0; j < LARGURA; j++) {
            revelado[i][j] = false;
        }
    }
    for(int i = 0; i < INIMIGOS; i++){
    if(i != 4){  // Mantém apenas o boss (inimigo[4]) ativo
        inimigo[i].vivo = false;

    }
}
    // posição inicial do personagem
    personagem.pontoPersonagem.x = 12;
    personagem.pontoPersonagem.y = 17;
}

void revelarAreaInicial(StatusPersonagem &personagem, bool revelado[ALTURA][LARGURA]) {
    for(int i = personagem.pontoPersonagem.x - 4; i <= personagem.pontoPersonagem.x + 4; i++) {
        for(int j = personagem.pontoPersonagem.y - 4; j <= personagem.pontoPersonagem.y + 4; j++) {
            if(i >= 0 && i < ALTURA && j >= 0 && j < LARGURA) {
                revelado[i][j] = true;
            }
        }
    }
}

void melhorarAtributos(int &Upgrade, StatusPersonagem &personagem, StatusItems itens){
    while (Upgrade > 0) {
        system("cls"); // Limpa a tela antes de mostrar o menu

        printarBordaMenu();

        Status(personagem, itens, Upgrade);

        posicao(0, 0);

        cout << "Pontos restantes: " << Upgrade << endl << endl;
        cout << "Escolha um atributo para melhorar:" << endl << endl;
        cout << "   1 - Ataque (+5)" << endl;
        cout << "   2 - Vida maxima (+10)" << endl;
        cout << "   3 - Armadura (+1)" << endl;
        posicao(0, 29); cout << "Pressione qualquer outra tecla para retornar" << endl;

        char tecla = getch();

        if (tecla == 0 || tecla == -32) {
            getch(); // descarta o segundo código da tecla especial
            continue; // volta para o menu sem fazer nada
        }

        switch(tecla) {
            case '1':
                personagem.ataque += 5;
                Upgrade--;
                system("cls");
                break;

            case '2':
                personagem.VidaMaxima += 10;
                Upgrade--;
                system("cls");
                break;

            case '3':
                personagem.armadura += 1;
                Upgrade--;
                system("cls");
                break;

            default: system("cls"); return;
        }
    }
}

void movimentacao(StatusPersonagem &personagem, StatusInimigos inimigo[INIMIGOS], StatusItems &Items, int m[ALTURA][LARGURA], bool revelado[ALTURA][LARGURA], char tecla,int &mapaatual, int &Upgrade) {

    bool emCombate = false;

    for(int i = 0; i < INIMIGOS; i++) {
        if(inimigo[i].vivo && personagem.pontoPersonagem.x == inimigo[i].pontoInimigos.x && personagem.pontoPersonagem.y == inimigo[i].pontoInimigos.y) {
            emCombate = true;
            break;
        }
    }

    if(!emCombate) {

        int newX = personagem.pontoPersonagem.x;
        int newY = personagem.pontoPersonagem.y;

        switch (tecla) {
            case 'l': case 'L': melhorarAtributos(Upgrade, personagem, Items); break;
            case 72: case 'w': case 'W': newX--; break; // Cima
            case 80: case 's': case 'S': newX++; break; // Baixo
            case 75: case 'a': case 'A': newY--; break; // Esquerda
            case 77: case 'd': case 'D': newY++; break; // Direita
            case 174: case 'c': case 'C': // Abre o Menu
                limparTela(91, 0, 29, 20);
                printarMenu();
            break;
            case 'r': case 'R':
                if(Items.PocaoCura >= 1){
                    personagem.vida += 50;
                    Items.PocaoCura--;
                    if(personagem.vida > personagem.VidaMaxima){
                        personagem.vida = personagem.VidaMaxima;
                    }
                }
            break;
        }

        // Verificação de limites
        if(newX >= 0 && newX < ALTURA && newY >= 0 && newY < LARGURA) {

            //Apaga mensagens da parte inferior da tela ao andar
            posicao(0,26); cout << "                                                                                                    ";
            posicao(0,27); cout << "                                                                                                    ";
            posicao(0,28); cout << "                                                                                                    ";
            posicao(0,29); cout << "                                                                                                    ";

            if(m[newX][newY] != 1 && m[newX][newY] != 3) {  // Permite movimento em qualquer tile que não seja parede
                personagem.pontoPersonagem.x = newX;
                personagem.pontoPersonagem.y = newY;

                // Revela área ao redor (5x5)
                for (int i = personagem.pontoPersonagem.x - 4; i <= personagem.pontoPersonagem.x + 4; i++) {
                    for (int j = personagem.pontoPersonagem.y - 4; j <= personagem.pontoPersonagem.y + 4; j++) {
                        if (i >= 0 && i < ALTURA && j >= 0 && j < LARGURA) {
                            revelado[i][j] = true;
                        }
                    }
                }

                // Verifica armadilhas
                if(m[personagem.pontoPersonagem.x][personagem.pontoPersonagem.y] == 9) {
                    personagem.vida -= 10;
                    posicao(2,26); cout << "Voce pisou em uma armadilha!   Perdeu 10 de vida.                        ";
                }

                if(m[personagem.pontoPersonagem.x][personagem.pontoPersonagem.y] == 5){
                    posicao(2,26); cout << "Deseja ir para proxima fase?                                             ";
                    posicao(2,27); cout << "1 - Sim                                                                  ";

                    switch(tecla){
                        case '1':
                            switch (mapaatual){
                                case 1:
                                    limparTela(0,0,80,30);
                                    segundomapa(m, personagem, revelado,inimigo);
                                    mapaatual++;
                                    revelarAreaInicial(personagem, revelado); break;
                                case 2:
                                    limparTela(0,0,80,30);
                                    terceiromapa(m, personagem, revelado, inimigo);
                                    mapaatual++;
                                    limparTela(0,0,20,30);
                                    revelarAreaInicial(personagem, revelado); break;
                            }
                        case '2': break;
                        default: break;
                    }
                }
            }
        }
    }

    // Movimentação dos inimigos
    for(int i = 0; i < INIMIGOS; i++) {
        if(inimigo[i].visivel && inimigo[i].vivo) {
            int inimigoNewX = inimigo[i].pontoInimigos.x;
            int inimigoNewY = inimigo[i].pontoInimigos.y;

            if (inimigo[i].visivel == true){
                if(personagem.pontoPersonagem.x > inimigoNewX){//cima
                    inimigoNewX++;
                }else if(personagem.pontoPersonagem.y > inimigoNewY){//esquerda
                    inimigoNewY++;
                }else if(personagem.pontoPersonagem.x < inimigoNewX){//baixo
                    inimigoNewX--;
                }else if(personagem.pontoPersonagem.y < inimigoNewY){//direita
                    inimigoNewY--;
                }
            }

            // Verifica movimento válido para inimigos
            if(m[inimigoNewX][inimigoNewY] != 1 && m[inimigoNewX][inimigoNewY] != 3 && m[inimigoNewX][inimigoNewY] != 4) {
                inimigo[i].pontoInimigos.x = inimigoNewX;
                inimigo[i].pontoInimigos.y = inimigoNewY;
            }
        }
    }
}

void Combate(StatusPersonagem &personagem, StatusInimigos inimigo[INIMIGOS], StatusItems &Items, int EspicialAtaque, int AumentaProbabilidadeAtaqueCombate, int BotasDeAgilidadeCombate, int minuto){
    bool Entrandocombate = true;
    for (int i = 0; i < INIMIGOS; i++) {
        if (personagem.pontoPersonagem.x == inimigo[i].pontoInimigos.x &&
            personagem.pontoPersonagem.y == inimigo[i].pontoInimigos.y &&
            inimigo[i].vivo == true) {
            system("cls");
            int round = 0;
            while(Entrandocombate){

                int acerto = 0;
                round++;
                posicao(2,11);
                cout << "Ataques Especiais:" << EspicialAtaque << "                                   Round:" << round << "              ";
                posicao(2,12);
                cout << "Vida Jogador:" << personagem.vida << "/" << personagem.VidaMaxima << "                                Vida inimigo: " << inimigo[i].vida << "                   ";
                posicao(2,14);
                cout << "1 - Atacar" << endl
                     << "  2 - Curar" << endl
                     // dobra o dano causado  e o uso e limitado
                     << "  3 - Ataque Especial" << endl;
                //deixar mais organizado
                cout << "  ";

                char tecla = getch();

                if (tecla == 0 || tecla == -32) {
                    getch(); // descarta o segundo código da tecla especial
                    continue; // volta para o menu sem fazer nada
                }

                switch(tecla){

                    //ataque normal
                    case '1':{

                        //variavel para acerto de ataque
                        acerto = rand()%9+1+AumentaProbabilidadeAtaqueCombate;

                        if(acerto > 3){
                            inimigo[i].vida -= personagem.ataque;
                            posicao(2,21);
                            cout << "Voce acertou                                 ";
                        } else {
                            posicao(2,21);
                            cout << "Voce errou                                 ";
                        }

                        acerto = rand()%9+1;
                        if(acerto > 3){
                            personagem.vida = personagem.vida - (armaduraPotente(inimigo[i].vida, personagem.armadura));
                            posicao(2,19);
                            cout << "inimigo acertou                                 ";
                        } else {
                            posicao(2,19);
                            cout << "O inimigo errou                            ";
                        }

                        //verifica se o inimigo morreu
                        if(inimigo[i].vida <= 0){
                            inimigo[i].vivo = false;
                            system("cls");
                            personagem.Score += inimigo[i].experiencia - minuto;
                            personagem.experiencia += 15;
                            Entrandocombate = false;
                        }
                        //verifica a vida do personagem
                        if(personagem.vida <= 0){
                            Entrandocombate = false;
                        }

                        break;

                    }
                    //curar
                    case '2':{

                        if(Items.PocaoCura > 0){
                            Items.PocaoCura--;
                            personagem.vida += 50;
                            if(personagem.vida > personagem.VidaMaxima){
                                personagem.vida = personagem.VidaMaxima;
                            }
                        }

                        acerto = rand()%5+1;

                        if(acerto > 3){
                            personagem.vida -= (inimigo[i].ataque - personagem.armadura);
                            posicao(2,19);
                            cout << "inimigo acerto                                 ";
                        } else {
                            posicao(2,19);
                            cout << "O inimigo erro                            ";
                        }
                        if(personagem.vida <= 0){
                            Entrandocombate = false;
                        }

                        break;

                    }
                   //Ataque Especial
                   case '3': {
                        if(EspicialAtaque > 0){
                            personagem.vida -= (inimigo[i].ataque - personagem.armadura);
                            posicao(2,19);
                            cout << "inimigo acerto                             ";
                            inimigo[i].vida -= personagem.ataque*2;
                            EspicialAtaque--;

                            if(inimigo[i].vida <= 0){
                                inimigo[i].vivo = false;
                                system("cls");
                                personagem.Score += inimigo[i].experiencia - minuto;
                                personagem.experiencia += 15;
                                Entrandocombate = false;
                            }
                            if(personagem.vida <= 0){
                               Entrandocombate = false;

                            }
                            break;
                        }
                   }
                }
            }//while
        }//if
    }//for
}

int main(){
    ///ALERTA: NAO MODIFICAR O TRECHO DE CODIGO, A SEGUIR.
        //INICIO: COMANDOS PARA QUE O CURSOR NAO FIQUE PISCANDO NA TELA
        //HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE); // Comando pra poder mudar a cor
        HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
        CONSOLE_CURSOR_INFO     cursorInfo;
        GetConsoleCursorInfo(out, &cursorInfo);
        cursorInfo.bVisible = false; // set the cursor visibility
        SetConsoleCursorInfo(out, &cursorInfo);
        //FIM: COMANDOS PARA QUE O CURSOR NAO FIQUE PISCANDO NA TELA
        //INICIO: COMANDOS PARA REPOSICIONAR O CURSOR NO INICIO DA TELA
        short int CX=0, CY=0;
        COORD coord;
        coord.X = CX;
        coord.Y = CY;
        //FIM: COMANDOS PARA REPOSICIONAR O CURSOR NO INICIO DA TELA
    ///ALERTA: NAO MODIFICAR O TRECHO DE CODIGO, ACIMA.



    int dificuldade = printarMenuInicial();
    //Mapa do jogo
    int m[ALTURA][LARGURA] = {
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,1,1,1,0,0,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,1,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1},
    {1,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,1,1,0,0,0,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,1,0,0,1,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,0,0,1,0,0,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
    };

    StatusPersonagem personagem = {1, 0, 100, 0, 0, 0, 0, {1,3}}; // nivel, ataque, vida, armadura, experiencia, VidaMaxima, Score, localizacao (cordenadas x e y);
    StatusItems Items ={1, 1, 1, {1, 29}, {10, 76}, {12,76},1, 1, 1, 1, {21, 10}, {19, 22}, {19, 62}, {3, 22} };
    StatusInimigos inimigo[INIMIGOS] = {0};

    if (dificuldade == 1){
        personagem.ataque = 999;
        personagem.VidaMaxima = 999;
        personagem.vida = 999;
        personagem.armadura = 999;
    } else {
        personagem.ataque = 5;
        personagem.VidaMaxima = 100;
    }

    primeiromapa(m, personagem);
    int mapaatual = 1;

    for (int i = 0; i < INIMIGOS; i++){
        if(i > 4){
            inimigo[i].vida = 30;
            inimigo[i].ataque = 15;
            inimigo[i].vivo = true;
            inimigo[i].visivel = false;
        }else{
            inimigo[i].vida = 10 * (i + 1);
            inimigo[i].ataque = 5 * (i + 1);
            inimigo[i].vivo = true;
            inimigo[i].visivel = false;
        }
    }
    inimigo[0].pontoInimigos = {13, 19}; inimigo[0].experiencia = 10;
    inimigo[1].pontoInimigos = {5, 37};  inimigo[1].experiencia = 15;
    inimigo[2].pontoInimigos = {19,39};  inimigo[2].experiencia = 20;
    inimigo[3].pontoInimigos = {5,74};   inimigo[3].experiencia = 25;
    inimigo[4].pontoInimigos = {12, 57}; inimigo[4].experiencia = 100; ///Boss
    inimigo[5].pontoInimigos = {21, 16}; inimigo[5].experiencia = 10;
    inimigo[6].pontoInimigos = {3, 25};  inimigo[6].experiencia = 15;
    inimigo[7].pontoInimigos = {21, 39}; inimigo[7].experiencia = 20;
    inimigo[8].pontoInimigos = {12, 59}; inimigo[8].experiencia = 25;

    bool revelado[ALTURA][LARGURA] = {}; // Matriz de visibilidade do mapa (false = oculto, true = revelado)
    int proximoNivel = 2; //Variavel nivel do perosnagem
    bool EspadaColetada = false, PocaoColetada = false, ItemSecretoColetado = false, Escudo = false,BotasDeAgilidade = false, CristalVida = false, AumentaProbabilidadeAtaque = false; //variaveis de coletas de itens
    char tecla;// Variavel para tecla precionada
    //variaveis de rankup
    int Upgrade = 0;
    int EspicialAtaque = 0; bool ganharAtaqueEspecial = true;
    //combate
    // o inimigo vai ter desvantagem de ataque em -1 quando forem coletados
    int BotasDeAgilidadeCombate = 0;
    // o jogador vai ter vantagens de ataque em +1quando forem coletados;
    int AumentaProbabilidadeAtaqueCombate = 0;

    // Revela 2 caracteres pra cima, baixo, direita e esquerda do jogador
    for (int i = personagem.pontoPersonagem.x - 4; i <= personagem.pontoPersonagem.x + 4; i++) {
        for (int j = personagem.pontoPersonagem.y - 4; j <= personagem.pontoPersonagem.y + 4; j++) {
            if (i >= 0 && i < ALTURA && j >= 0 && j < LARGURA) {
                revelado[i][j] = true;
            }
        }
    }

    auto inicio = chrono::steady_clock::now();  // Marca o tempo de inC-cio
    int minuto = 0;

    while (true){
		posicao(0, 0);

        if (personagem.ataque == 30 && ganharAtaqueEspecial == true){
            EspicialAtaque += 4;
            ganharAtaqueEspecial = false;
        }

		cronometro(inicio, minuto);

        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        mudarCor(hConsole, 0x0F);
        for (int i = 0; i < ALTURA; i++) {
            for (int j = 0; j < LARGURA; j++) {
                if (i == personagem.pontoPersonagem.x && j == personagem.pontoPersonagem.y){
                    mudarCor(hConsole, personagem.cor);
                    cout << char(36); // Personagem
                    SetConsoleTextAttribute(hConsole, 0x0F);    // Branco
                }else if (i == inimigo[0].pontoInimigos.x && j == inimigo[0].pontoInimigos.y && inimigo[0].vivo && mapaatual == 1){//inimigo 1
                    if (revelado[i][j]){
                        inimigo[0].visivel = true;  SetConsoleTextAttribute(hConsole, 0x0A); //Verde Claro
                        cout << "I";    SetConsoleTextAttribute(hConsole, 0x0F);    // Branco
                    }else{  cout << " ";}
                }else if (i == inimigo[1].pontoInimigos.x && j == inimigo[1].pontoInimigos.y && inimigo[1].vivo && mapaatual == 1){//inimigo 2
                    if (revelado[i][j]){
                        inimigo[1].visivel = true;  SetConsoleTextAttribute(hConsole, 0x02); // Verde mais escuro
                        cout << "I";    SetConsoleTextAttribute(hConsole, 0x0F);    // Branco
                    }else{  cout << " ";}
                }else if (i == inimigo[2].pontoInimigos.x && j == inimigo[2].pontoInimigos.y && inimigo[2].vivo && mapaatual == 1){//inimigo 3
                    if (revelado[i][j]){
                        inimigo[2].visivel = true;  SetConsoleTextAttribute(hConsole, 0x0E); // Amarelo Claro
                        cout << "I";    SetConsoleTextAttribute(hConsole, 0x0F);    // Branco
                    }else{  cout << " ";}
                }else if (i == inimigo[3].pontoInimigos.x && j == inimigo[3].pontoInimigos.y && inimigo[3].vivo && mapaatual == 1){//inimigo 4
                    if (revelado[i][j]){
                        inimigo[3].visivel = true;  SetConsoleTextAttribute(hConsole, 0x06); // Amarelo Mais escuro
                        cout << "I";    SetConsoleTextAttribute(hConsole, 0x0F);    // Branco
                    }else{  cout << " ";}
                }else if (i == inimigo[4].pontoInimigos.x && j == inimigo[4].pontoInimigos.y && inimigo[4].vivo && mapaatual == 3){ //chefao
                    if (revelado[i][j]){
                        inimigo[4].visivel = true;  SetConsoleTextAttribute(hConsole, 0x04 && mapaatual == 3); // Vermelho
                        cout << "B";    SetConsoleTextAttribute(hConsole, 0x0f);    // Branco
                    }else{  cout << " ";}
                }else if (i == Items.pontoEspada.x && j == Items.pontoEspada.y && mapaatual == 1){    //Espada
                    if(EspadaColetada == false && revelado[i][j]){
                        cout << "E";
                    }else{  cout << " ";}
                }else if (i == Items.pontoPocaoCura.x && j == Items.pontoPocaoCura.y && mapaatual == 1){  //Pocao de cura
                    if(PocaoColetada == false && revelado[i][j]){
                        cout << "c";
                    }else{  cout << " ";}
                }else if (i == Items.pontoPowerUp.x && j == Items.pontoPowerUp.y && mapaatual == 2){  //Power-up
                    if(ItemSecretoColetado == false && revelado[i][j]){
                        cout << "Y";
                    }else{  cout << " ";}
                }
                else if (i == Items.pontoEscudo.x && j == Items.pontoEscudo.y && mapaatual == 1){  //Power-up
                    if(Escudo == false && revelado[i][j]){
                        cout << "O";
                    }else{  cout << " ";}
                }
                else if (i == Items.pontoBotasDeAgilidade.x && j == Items.pontoBotasDeAgilidade.y && mapaatual == 2){  //Power-up
                    if(BotasDeAgilidade == false && revelado[i][j]){
                        cout << "A";
                    }else{  cout << " ";}
                }else if (i == Items.pontoCristalVida.x && j == Items.pontoCristalVida.y && mapaatual == 1){  //Power-up
                    if(CristalVida == false && revelado[i][j]){
                        cout << "V";
                    }else{  cout << " ";}
                }else if (i == Items.pontoAumentaProbabilidadeAtaque.x && j == Items.pontoAumentaProbabilidadeAtaque.y && mapaatual == 2){  //Power-up
                    if(AumentaProbabilidadeAtaque == false && revelado[i][j]){
                        cout << "T";
                    }else{  cout << " ";}
                }

                else if (i == inimigo[5].pontoInimigos.x && j == inimigo[5].pontoInimigos.y && inimigo[5].vivo && mapaatual == 2){//inimigo 1
                    if (revelado[i][j]){
                        inimigo[5].visivel = true; SetConsoleTextAttribute(hConsole, 0x0A); //Verde Claro
                        cout << "I";    SetConsoleTextAttribute(hConsole, 0x0F);    // Branco
                    }else{  cout << " ";}
                }else if (i == inimigo[6].pontoInimigos.x && j == inimigo[6].pontoInimigos.y && inimigo[6].vivo  && mapaatual == 2){//inimigo 1
                    if (revelado[i][j]){
                        inimigo[6].visivel = true;  SetConsoleTextAttribute(hConsole, 0x02);
                        cout << "I";    SetConsoleTextAttribute(hConsole, 0x0F);    // Branco
                    }else{  cout << " ";}
                }else if (i == inimigo[7].pontoInimigos.x && j == inimigo[7].pontoInimigos.y && inimigo[7].vivo && mapaatual == 2){//inimigo 1
                    if (revelado[i][j]){
                        inimigo[7].visivel = true;  SetConsoleTextAttribute(hConsole, 0x0E);
                        cout << "I";    SetConsoleTextAttribute(hConsole, 0x0F);    // Branco
                    }else{  cout << " ";}
                }else if (i == inimigo[8].pontoInimigos.x && j == inimigo[8].pontoInimigos.y && inimigo[8].vivo && mapaatual == 2){//inimigo 1
                    if (revelado[i][j]){
                        inimigo[8].visivel = true;  SetConsoleTextAttribute(hConsole, 0x06);
                        cout << "I";    SetConsoleTextAttribute(hConsole, 0x0F);    // Branco
                    }else{  cout << " ";}
                }



                else if (revelado[i][j]) {
                    switch (m[i][j]){
                        case 0: cout<<"."; break; //caminho
                        case 1: cout<<char(219); break; //parede
                        case 3: cout<< " "; break; //sem chão
                        case 4: cout << char(176); break; // Ponte
                        case 5: mudarCor(hConsole, 0x06); cout << char (219); SetConsoleTextAttribute(hConsole, 0x0F); break; //alçapão
                    }  //fim switch
                }   else {    cout << " ";}   // Área não revelada

            }
            cout << "\n";
        }

        if(ItemSecretoColetado == false && mapaatual == 2){
            if (personagem.pontoPersonagem.x == Items.pontoPowerUp.x  && personagem.pontoPersonagem.y == Items.pontoPowerUp.y){
                personagem.nivel++;
                personagem.Score += 20 - minuto;
                Upgrade++;
                ItemSecretoColetado = true;
                posicao(2,26); cout << "Voce coleteou um item secreto, recebeu um nivel!                                                 ";
                posicao(2,27); cout << "                                                                                                 ";
                personagem.cor = 14;
                Sleep(1500);
                limparBufferDeInput();
            }
        }

        // Level
        if(personagem.nivel*15 <= personagem.experiencia){
            personagem.nivel++;
            personagem.experiencia = 0;
        }

        //Level up
        if(personagem.nivel == proximoNivel){
            personagem.Score += 40 - minuto;
            proximoNivel++;
            Upgrade++;
            posicao(2,26); cout << "Voce subiu de nivel, Aperte L para aumentar seus status!                                             ";
            posicao(2,27); cout << "                                                                                                     ";
            Sleep(1500);
            limparBufferDeInput();
        }

        //Itens coletaveis
        if(Escudo == false && mapaatual == 1){
            if (personagem.pontoPersonagem.x == Items.pontoEscudo.x && personagem.pontoPersonagem.y == Items.pontoEscudo.y){
                Escudo = true;
                personagem.armadura += 1;
                personagem.experiencia += 15;
                personagem.Score += 20 - minuto;
                posicao(2,26); cout << "Escudo Coletado!                                                                                 ";
                posicao(2,27); cout << "(+1 de Armadura)                                                                                 ";
                personagem.cor = 9;
                Sleep(1500);
                limparBufferDeInput();
            }
        }
        if(BotasDeAgilidade == false && mapaatual == 2){
            if (personagem.pontoPersonagem.x == Items.pontoBotasDeAgilidade.x && personagem.pontoPersonagem.y == Items.pontoBotasDeAgilidade.y){
                BotasDeAgilidade = true;
                BotasDeAgilidadeCombate += 1;
                personagem.experiencia += 15;
                personagem.Score += 20 - minuto;
                posicao(2,26); cout << "Botas De Agilidade coletadas!                                                                    ";
                posicao(2,27); cout << "(+1 de chance de desviar)                                                                        ";
                personagem.cor = 2;
                Sleep(1500);
                limparBufferDeInput();
            }
        }
        if(CristalVida == false && mapaatual == 1){
            if (personagem.pontoPersonagem.x == Items.pontoCristalVida.x && personagem.pontoPersonagem.y == Items.pontoCristalVida.y){
                CristalVida = true;
                personagem.VidaMaxima += 20;
                personagem.vida += 20;
                personagem.experiencia += 15;
                personagem.Score += 20 - minuto;
                posicao(2,26); cout << "Cristal de vida coletado!                                                                        ";
                posicao(2,27); cout << "(+20 de vida maxima)                                                                             ";
                personagem.cor = 4;
                Sleep(1500);
                limparBufferDeInput();
            }
        }
        if(AumentaProbabilidadeAtaque == false  && mapaatual == 2){
            if (personagem.pontoPersonagem.x == Items.pontoAumentaProbabilidadeAtaque.x && personagem.pontoPersonagem.y == Items.pontoAumentaProbabilidadeAtaque.y){
                AumentaProbabilidadeAtaque = true;
                AumentaProbabilidadeAtaqueCombate += 1;
                personagem.experiencia += 15;
                personagem.Score += 20 - minuto;
                posicao(2,26); cout << "Tecnica de combate coletada!                                                                     ";
                posicao(2,27); cout << "(+1 de chance de acerta um ataque) (Somente na dificuldade media ou superior)           ";
                personagem.cor = 6;
                Sleep(1500);
                limparBufferDeInput();
            }
        }
        if(EspadaColetada == false && mapaatual == 1){
            if (personagem.pontoPersonagem.x == Items.pontoEspada.x && personagem.pontoPersonagem.y == Items.pontoEspada.y){
                EspadaColetada = true;
                personagem.ataque += 5;
                personagem.experiencia += 15;
                personagem.Score += 20 - minuto;
                posicao(2,26); cout << "Espada coletada!                                                                                 ";
                posicao(2,27); cout << "(+5 de Dano)                                                                                     ";
                personagem.cor = 5;
                Sleep(1500);
                limparBufferDeInput();
            }
        }
        if (PocaoColetada == false && mapaatual == 1){
            if (personagem.pontoPersonagem.x == Items.pontoPocaoCura.x && personagem.pontoPersonagem.y == Items.pontoPocaoCura.y){
                PocaoColetada = true;
                Items.PocaoCura++;
                personagem.experiencia += 15;
                personagem.Score += 20 - minuto;
                posicao(2,26); cout << "Pocao de cura coletada!                                                                          ";
                posicao(2,27); cout << "                                                                                                 ";
                Sleep(1500);
                limparBufferDeInput();
            }
        }

        // Movimentação
        if (_kbhit()) { //Se estiver pressionando uma tecla
            tecla = getch(); //Recebe o valor da tecla pressionada
            movimentacao(personagem, inimigo, Items, m, revelado, tecla, mapaatual, Upgrade);
            Combate(personagem , inimigo, Items, EspicialAtaque, AumentaProbabilidadeAtaqueCombate, BotasDeAgilidadeCombate, minuto);
        }

        printarBordaMenu();
        Status(personagem, Items, Upgrade);

        if(personagem.vida <= 0){
            fimDeJogo(personagem, false);
            return 0;

        }else if(inimigo[4].vivo == false){
            fimDeJogo(personagem, true);

            return 0;

        }
    }

    return 0;
}

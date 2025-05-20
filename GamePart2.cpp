#include <iostream>
#include <windows.h>
#include <conio.h>
#include <stdlib.h>
#include <fstream>
#include <string>

#define ALTURA 25
#define LARGURA 40
#define INIMIGOS 5

using namespace std;

struct Ponto{
    int x, y;
};
struct StatusItems{
    int PocaoCura, Espada, PowerUp;
    Ponto pontoEspada, pontoPocaoCura, pontoPowerUp;
};

struct StatusPersonagem{
    int nivel, ataque, vida,armadura, experiencia, VidaMaxima, Score;
    Ponto pontoPersonagem;
    string nome;
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
void textoCentralizado(const string& texto) {
    int largura_console = larguraConsole();
    int spaces = (largura_console - texto.length()) / 2;
    cout << string(spaces, ' ') << texto << endl;
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
            posicao(0, 7);  textoCentralizado("CORES DOS INIMIGOS (I) DO MAIS FRACO AO MAIS FORTE: ");
            mudarCor(hConsole, 0x8); // cinza escuro

            mudarCor(hConsole, 0xA); // verde claro
            posicao(0, 9);  textoCentralizado("verde claro");

            mudarCor(hConsole, 0x2); // verde escuro
            posicao(0, 11); textoCentralizado("verde escuro");

            mudarCor(hConsole, 0xE); // amarelo claro
            posicao(0, 13); textoCentralizado("amarelo claro");

            mudarCor(hConsole, 0x6); // amarelo escuro
            posicao(0, 15); textoCentralizado("amarelo escuro");

            mudarCor(hConsole, 0x4); // vermelho escuro
            posicao(0, 17); textoCentralizado("B vermelho = Boss");
        }

        // imprime informações quando o cursor estiver em cima de mapa e itens
        if(cursorInfMenuPrincipal == 1){
            posicao(0, 7);  textoCentralizado("X = Armadilhas (-10 de vida)");
            posicao(0, 9);  textoCentralizado("E = espada (+5 de dano)");
            posicao(0, 11); textoCentralizado("C = pocao de cura (+50 de vida)");
            posicao(0, 13); textoCentralizado("pressione R para usar a pocao");
            posicao(0, 15); textoCentralizado("existem locais escondidos");
            posicao(0, 17); textoCentralizado("Y = item secreto (upa um nivel)");
            posicao(0, 18); textoCentralizado("(o item secreto ultiliza automaticamente)");
        }

        // imprime informações quando o cursor estiver em cima de score
        if(cursorInfMenuPrincipal == 2){
            posicao(0, 7);  textoCentralizado("subir de nivel = 20 pontos");
            posicao(0, 9); textoCentralizado("Adiquirir qualquer item = 10 pontos");
            posicao(0, 11);  textoCentralizado("inimigos dao pontos dependendo de sua forca");

            mudarCor(hConsole, 0xA); // verde claro
            posicao(0, 13); textoCentralizado("verde claro = 5");

            mudarCor(hConsole, 0x2); // verde escuro
            posicao(0, 15); textoCentralizado("verde escuro = 10");

            mudarCor(hConsole, 0xE); // amarelo claro
            posicao(0, 17); textoCentralizado("amarelo claro = 15");

            mudarCor(hConsole, 0x6); // amarelo escuro
            posicao(0, 19); textoCentralizado("amarelo escuro = 20");

            mudarCor(hConsole, 0x4); // vermelho escuro
            posicao(0, 21); textoCentralizado("vermelho = 25");
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
    mudarCor(hConsole, 0x7); // branco

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

    textoCentralizado("=== TOP 10 ===");

    mudarCor(hConsole, 0x7); //branco

    cout << endl << endl;

    for (int i = 0; i < min(10, contador); i++) {
        string linha = to_string(i+1) + ". " + scores[i].nome + " - " + to_string(scores[i].Score);
        textoCentralizado(linha);
    }

    posicao(0, 29);

    system("pause");
    system("cls");
}

//Menu principal do jogo (que aparece no inicio)
void printarMenuInicial(){
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    int cursorMenuPrincipal=0, cursorMenuPrincipalNew;
    char teclaMenuPrincipal;
    bool finalizarMenuPrincipal = false;

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
                    finalizarMenuPrincipal=true;
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
}

//Printa a borda do menu da direita
void printarBordaMenu (){
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
void Status(StatusPersonagem status, StatusItems item){
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
}

void limparBufferDeInput() {
    while (_kbhit()) {
        _getch(); // Lê e descarta todos os inputs pendentes
    }
}

void movimentacao(StatusPersonagem &personagem, StatusInimigos inimigo[INIMIGOS], StatusItems &Items, int m[ALTURA][LARGURA], bool revelado[ALTURA][LARGURA], char tecla) {

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
            posicao(0,25); cout << "                                                                 ";
            posicao(0,26); cout << "                                                                 ";
            posicao(0,27); cout << "                                                                 ";
            posicao(0,28); cout << "                                                                 ";

            if(m[newX][newY] != 1 && m[newX][newY] != 0) {  // Permite movimento em qualquer tile que não seja parede
                personagem.pontoPersonagem.x = newX;
                personagem.pontoPersonagem.y = newY;

                // Revela área ao redor (5x5)
                for (int i = personagem.pontoPersonagem.x - 2; i <= personagem.pontoPersonagem.x + 2; i++) {
                    for (int j = personagem.pontoPersonagem.y - 2; j <= personagem.pontoPersonagem.y + 2; j++) {
                        if (i >= 0 && i < ALTURA && j >= 0 && j < LARGURA) {
                            revelado[i][j] = true;
                        }
                    }
                }

                // Verifica armadilhas
                if(m[personagem.pontoPersonagem.x][personagem.pontoPersonagem.y] == 4) {
                    personagem.vida -= 10;
                    posicao(2,25); cout << "Voce pisou em uma armadilha!   Perdeu 10 de vida.                    ";
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

    StatusPersonagem personagem = {1, 5, 100, 0, 0, 100, 0, {1, 3}}; // nivel, ataque, vida, armadura, experiencia, VidaMaxima, Score, localizacao (cordenadas x e y);
    StatusItems Items ={1, 1, 1, {2, 28}, {14, 4}, {2, 33}};
    StatusInimigos inimigo[INIMIGOS] = {0};

    for (int i = 0; i < INIMIGOS; i++){
        inimigo[i].vida = 10 * (i + 1);
        inimigo[i].ataque = 5 * (i + 1);
        inimigo[i].vivo = true;
        inimigo[i].visivel = false;
    }

    inimigo[0].pontoInimigos = {3, 11};
    inimigo[1].pontoInimigos = {12, 6};
    inimigo[2].pontoInimigos = {10, 20};
    inimigo[3].pontoInimigos = {18, 11};
    inimigo[4].pontoInimigos = {20, 38};

    printarMenuInicial();

    //Mapa do jogo
    int m[ALTURA][LARGURA] = {
        {1,1,1,1,1,1,1,0,0,1,1,1,1,1,0,0,0,1,1,1,1,1,1,1,1,1,1,1,0,0,0,1,1,1,1,1,1,1,0,0},
        {1,2,2,2,2,2,1,0,0,1,2,2,2,1,0,0,0,1,2,2,2,4,2,2,4,2,2,1,1,1,0,1,2,4,2,4,2,1,0,0},
        {1,2,2,2,2,2,3,3,3,3,2,2,2,1,0,0,0,1,2,4,2,4,2,2,4,2,2,3,2,1,0,1,4,2,2,2,2,3,3,7},
        {1,2,2,2,2,2,1,0,0,1,2,2,2,1,0,0,0,1,2,4,2,4,2,2,4,2,2,1,1,1,0,1,2,4,2,4,2,1,0,7},
        {1,1,1,3,1,1,1,0,0,1,2,2,2,3,3,3,3,3,2,4,2,2,2,2,4,2,2,1,0,0,0,1,1,1,1,1,1,1,0,7},
        {0,0,0,3,0,0,0,0,0,1,1,1,1,1,0,0,0,1,1,1,1,1,1,1,1,3,1,1,0,0,0,0,0,0,0,0,0,0,0,7},
        {0,0,0,3,3,3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3,0,0,1,1,1,1,1,1,1,1,0,0,0,7},
        {0,0,0,1,1,3,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3,0,0,1,2,2,2,2,2,2,6,7,7,7,7},
        {0,0,0,1,2,2,1,2,2,2,1,0,0,0,0,0,0,1,1,1,1,1,1,1,0,3,0,0,1,2,2,2,2,2,2,1,0,0,0,0},
        {0,0,0,1,2,1,1,2,1,2,1,0,0,0,0,0,0,1,2,2,2,2,2,3,3,3,3,3,3,2,2,2,2,2,2,1,0,0,0,0},
        {0,0,0,1,2,2,2,2,1,2,1,0,0,0,0,0,0,1,2,4,2,4,2,1,0,0,0,0,1,2,2,2,2,2,2,1,0,0,0,0},
        {0,0,0,1,1,1,1,2,1,2,1,0,3,3,3,3,3,3,2,4,2,4,2,1,0,0,0,0,1,2,2,2,2,2,2,1,0,0,0,0},
        {0,0,0,1,2,2,2,2,1,2,3,3,3,0,0,0,0,1,2,2,2,2,2,1,0,0,0,0,1,1,1,1,1,1,1,1,0,0,0,0},
        {0,0,0,1,3,1,1,1,1,1,1,0,3,0,0,0,0,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,1,2,1,0,0,0,0,0,0,3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,1,1,1,0,0,0,1,1,1,3,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,1,2,2,2,2,2,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,1,2,2,2,2,2,1,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1},
        {0,0,0,0,0,0,0,0,0,1,2,2,2,2,2,3,3,3,0,0,0,0,0,0,0,0,0,1,2,4,2,4,2,2,2,4,2,4,2,1},
        {0,0,0,0,0,0,0,0,0,1,2,2,2,2,2,1,0,3,0,0,0,0,0,0,0,0,0,1,2,2,2,2,2,4,2,4,2,2,2,1},
        {0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,3,1,1,1,1,1,1,1,0,0,1,2,2,2,4,2,4,2,2,2,4,2,1},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,2,2,4,2,2,2,4,2,1,0,0,1,2,4,2,2,2,2,2,4,2,2,2,1},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,2,4,4,2,4,2,4,2,1,0,0,1,2,4,2,4,2,4,2,2,2,4,2,1},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,2,2,2,2,4,2,2,2,3,3,3,3,2,2,2,2,2,2,2,4,2,2,2,1},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1}
    };

    bool revelado[ALTURA][LARGURA] = {}; // Matriz de visibilidade do mapa (false = oculto, true = revelado)
    int proximoNivel = 2; //Variavel nivel do perosnagem
    bool EspadaColetada = false, PocaoColetada = false, ItemSecretoColetado = false; //variaveis de coletas de itens
    char tecla;// Variavel para tecla precionada

    // Revela 2 caracteres pra cima, baixo, direita e esquerda do jogador
    for (int i = personagem.pontoPersonagem.x - 3; i <= personagem.pontoPersonagem.x + 3; i++) {
        for (int j = personagem.pontoPersonagem.y - 3; j <= personagem.pontoPersonagem.y + 3; j++) {
            if (i >= 0 && i < ALTURA && j >= 0 && j < LARGURA) {
                revelado[i][j] = true;
            }
        }
    }

    while (true){
        posicao(0, 0);

        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        mudarCor(hConsole, 0x0F);
        for (int i = 0; i < ALTURA; i++) {
            for (int j = 0; j < LARGURA; j++) {
                if (i == personagem.pontoPersonagem.x && j == personagem.pontoPersonagem.y){
                    cout << char(36); // Personagem
                }else if (i == inimigo[0].pontoInimigos.x && j == inimigo[0].pontoInimigos.y && inimigo[0].vivo){//inimigo 1
                    if (revelado[i][j]){
                        inimigo[0].visivel = true;  SetConsoleTextAttribute(hConsole, 0x0A); //Verde Claro
                        cout << "I";    SetConsoleTextAttribute(hConsole, 0x0F);    // Branco
                    }else{  cout << " ";}
                }else if (i == inimigo[1].pontoInimigos.x && j == inimigo[1].pontoInimigos.y && inimigo[1].vivo){//inimigo 2
                    if (revelado[i][j]){
                        inimigo[1].visivel = true;  SetConsoleTextAttribute(hConsole, 0x02); // Verde mais escuro
                        cout << "I";    SetConsoleTextAttribute(hConsole, 0x0F);    // Branco
                    }else{  cout << " ";}
                }else if (i == inimigo[2].pontoInimigos.x && j == inimigo[2].pontoInimigos.y && inimigo[2].vivo){//inimigo 3
                    if (revelado[i][j]){
                        inimigo[2].visivel = true;  SetConsoleTextAttribute(hConsole, 0x0E); // Amarelo Claro
                        cout << "I";    SetConsoleTextAttribute(hConsole, 0x0F);    // Branco
                    }else{  cout << " ";}
                }else if (i == inimigo[3].pontoInimigos.x && j == inimigo[3].pontoInimigos.y && inimigo[3].vivo){//inimigo 4
                    if (revelado[i][j]){
                        inimigo[3].visivel = true;  SetConsoleTextAttribute(hConsole, 0x06); // Amarelo Mais escuro
                        cout << "I";    SetConsoleTextAttribute(hConsole, 0x0F);    // Branco
                    }else{  cout << " ";}
                }else if (i == inimigo[4].pontoInimigos.x && j == inimigo[4].pontoInimigos.y && inimigo[4].vivo){ //chefao
                    if (revelado[i][j]){
                        inimigo[4].visivel = true;  SetConsoleTextAttribute(hConsole, 0x04); // Vermelho
                        cout << "B";    SetConsoleTextAttribute(hConsole, 0x0F);    // Branco
                    }else{  cout << " ";}
                }else if (i == Items.pontoEspada.x && j == Items.pontoEspada.y){    //Espada
                    if(EspadaColetada == false && revelado[i][j]){
                        cout << "E";
                    }else{  cout << " ";}
                }else if (i == Items.pontoPocaoCura.x && j == Items.pontoPocaoCura.y){  //Pocao de cura
                    if(PocaoColetada == false && revelado[i][j]){
                        cout << "c";
                    }else{  cout << " ";}
                }else if (i == Items.pontoPowerUp.x && j == Items.pontoPowerUp.y){  //Power-up
                    if(ItemSecretoColetado == false && revelado[i][j]){
                        cout << "Y";
                    }else{  cout << " ";}
                }else if (revelado[i][j]) {
                    switch (m[i][j]) {
                        case 0: cout << " "; break; // Espaco vazio
                        case 1: cout << char(219); break; // Parede
                        case 2: cout << "."; break; // caminho
                        case 3: cout << char(176); break; // Passagem
                        case 4: SetConsoleTextAttribute(hConsole, 0x04); cout << char(158); SetConsoleTextAttribute(hConsole, 0x0F); break; // Armadilha
                        case 6: cout << char(219); break; // Parede falsa
                        case 7: cout << " "; break;
                    }
                } else {    cout << " ";}   // Área não revelada

            }
            cout << "\n";
        }

        //area secetra
        if(ItemSecretoColetado == false){
            if (personagem.pontoPersonagem.x == 2 && personagem.pontoPersonagem.y == 33){
                personagem.nivel++;
                personagem.Score += 10;
                ItemSecretoColetado = true;
                posicao(2,25); cout << "Voce coleteou um item secreto, recebeu um nivel!                 ";
            }
        }

        // Level
        if(personagem.nivel*15 <= personagem.experiencia){
            personagem.nivel++;
            personagem.experiencia = 0;
        }
        //Level up
        if(personagem.nivel == proximoNivel){
            personagem.ataque += 5;
            personagem.VidaMaxima += 10;
            personagem.armadura += 1;
            personagem.Score += 20;
            proximoNivel++;
            posicao(2,25); cout << "Voce subiu de nivel, seus status aumentaram!                    ";
        }

        //Dano do inimigo ao player e Dano do player ao inimigo
        for (int i = 0; i < INIMIGOS; i++) {
            if (personagem.pontoPersonagem.x == inimigo[i].pontoInimigos.x &&
                personagem.pontoPersonagem.y == inimigo[i].pontoInimigos.y &&
                inimigo[i].vivo) {

                int totalDanoCausado = 0;
                int totalDanoRecebido = 0;
                int turno = 1;

                // Loop de combate por turnos
                while (inimigo[i].vivo && personagem.vida > 0) {
                    // Ataque do jogador
                    int danoTurno = personagem.ataque;
                    inimigo[i].vida -= danoTurno;
                    totalDanoCausado += danoTurno;

                    // Ataque do inimigo
                    int danoInimigoTurno = 0;
                    if (inimigo[i].ataque > personagem.armadura) {
                        danoInimigoTurno = inimigo[i].ataque - personagem.armadura;
                        personagem.vida -= danoInimigoTurno;
                        totalDanoRecebido += danoInimigoTurno;
                    }

                    posicao(2, 25); cout << "Turno " << turno << ": Voce causou " << danoTurno << " (" << totalDanoCausado << " total)";
                    posicao(2, 26); cout << "Turno " << turno << ": Inimigo causou " << danoInimigoTurno << " (" << totalDanoRecebido << " total)";

                    Sleep(2000); /// Tempo de 2s pra ler as msgm

                    // Verifica morte do inimigo
                    if (inimigo[i].vida <= 0) {
                        posicao(2, 27);
                        cout << "[INIMIGO DERROTADO!] XP: +15 | Pontos: +" << (5 * (i + 1));
                        inimigo[i].vivo = false;
                        personagem.experiencia += 15;
                        personagem.Score += 5 * (i + 1);
                    }

                    turno++; // Passa o turno

                    // Limpa apenas as linhas de status
                    posicao(2, 25); cout << "                                                          ";
                    posicao(2, 26); cout << "                                                          ";
                }

                // Mensagem final do combate
                posicao(2, 25);
                cout << "Dano total causado: " << totalDanoCausado;
                posicao(2, 26);
                cout << "Dano total recebido: " << totalDanoRecebido;
                Sleep(3000);

                // Limpa area de mensagens
                posicao(0, 25); cout << "                                                                 ";
                posicao(0, 26); cout << "                                                                 ";
                posicao(0, 27); cout << "                                                                 ";

                limparBufferDeInput();
                break;
            }
        }


        //Itens coletaveis
        if(EspadaColetada == false){
            if (personagem.pontoPersonagem.x == Items.pontoEspada.x && personagem.pontoPersonagem.y == Items.pontoEspada.y){
                EspadaColetada = true;
                personagem.ataque += 5;
                personagem.experiencia += 15;
                personagem.Score += 10;
                posicao(2,25); cout << "Espada coletada (+5 de Dano)!                                    ";
            }
        }
        if (PocaoColetada == false){
            if (personagem.pontoPersonagem.x == Items.pontoPocaoCura.x && personagem.pontoPersonagem.y == Items.pontoPocaoCura.y){
                PocaoColetada = true;
                Items.PocaoCura++;
                personagem.experiencia += 15;
                personagem.Score += 10;
                posicao(2,25); cout << "Voce coletou uma pocao de cura!                                  ";
            }
        }

        // Movimentação
        if (_kbhit()) { //Se estiver pressionando uma tecla
            tecla = getch(); //Recebe o valor da tecla pressionada
            movimentacao(personagem, inimigo, Items, m, revelado, tecla);
        }

        printarBordaMenu();
        Status(personagem, Items);

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

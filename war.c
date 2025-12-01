// ============================================================================
//         PROJETO WAR ESTRUTURADO - DESAFIO DE CÓDIGO
// ============================================================================
//
// OBJETIVOS:
// - Modularizar completamente o código em funções especializadas.
// - Implementar um sistema de missões para um jogador.
// - Criar uma função para verificar se a missão foi cumprida.
// - Utilizar passagem por referência (ponteiros) para modificar dados e
//   passagem por valor/referência constante (const) para apenas ler.
// - Foco em: Design de software, modularização, const correctness, lógica de jogo.
// ============================================================================

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <locale.h>

// --- Constantes Globais ---
#define NUM_TERRITORIOS 10
#define NUM_MISSOES 6
#define TAM_NOME 30
#define TAM_COR 15

typedef enum {
    AZUL,   
    VERMELHO,
    VERDE,
    AMARELO,
    ROXO
} CorExercito;

typedef struct {
    char nome[TAM_NOME];
    CorExercito dono;
    int tropas;
} Territorio;

Territorio* alocarMapa();
void inicializarTerritorios(Territorio* mapa);
void liberarMemoria(Territorio* mapa);

void exibirMenuPrincipal();
void exibirMapa(const Territorio* mapa);
void exibirMissao(int missaoId);

void faseDeAtaque(Territorio* mapa);
void simularAtaque(Territorio* mapa, int origem, int destino);
int sortearMissao();
int verificarVitoria(const Territorio* mapa, int missaoId);

void limparBufferEntrada();

int main() {
    // 1. Configuração Inicial (Setup)
    setlocale(LC_ALL, "Portuguese");
    srand(time(NULL));
    
    printf("========================================\n");
    printf("          JOGO WAR ESTRUTURADO          \n");
    printf("========================================\n\n");
    
    Territorio* mapa = alocarMapa();
    if (mapa == NULL) {
        printf("Erro: Não foi possível alocar memória para o mapa!\n");
        return 1;
    }
    
    inicializarTerritorios(mapa);
    
    CorExercito jogador = AZUL;
    int missaoAtual = sortearMissao();
    
    printf("Você é o exército AZUL!\n");
    printf("Missão secreta sorteada!\n\n");
    
    int escolha;
    int venceu = 0;
    
    do {
        system("clear || cls"); 
        
        printf("========================================\n");
        printf("               SEU TURNO               \n");
        printf("========================================\n\n");
        
        exibirMapa(mapa);
        printf("\n");
        exibirMissao(missaoAtual);
        printf("\n");
        
        exibirMenuPrincipal();
        printf("Escolha uma ação: ");
        scanf("%d", &escolha);
        limparBufferEntrada();
        
        switch (escolha) {
            case 1:
                faseDeAtaque(mapa);
                break;
                
            case 2:
                venceu = verificarVitoria(mapa, missaoAtual);
                if (venceu) {
                    printf("\n========================================\n");
                    printf("   PARABÉNS! VOCÊ CUMPRIU SUA MISSÃO!   \n");
                    printf("========================================\n\n");
                } else {
                    printf("\nVocê ainda não cumpriu sua missão.\n");
                    printf("Continue lutando!\n");
                }
                break;
                
            case 0:
                printf("\nEncerrando o jogo...\n");
                break;
                
            default:
                printf("\nOpção inválida! Tente novamente.\n");
                break;
        }
        
        if (escolha != 0 && !venceu) {
            printf("\nPressione Enter para continuar...");
            getchar();
        }
        
    } while (escolha != 0 && !venceu);
    
    // 3. Limpeza
    liberarMemoria(mapa);
    
    printf("\nObrigado por jogar War Estruturado!\n");
    return 0;
}


Territorio* alocarMapa() {
    Territorio* mapa = (Territorio*)calloc(NUM_TERRITORIOS, sizeof(Territorio));
    return mapa;
}

void inicializarTerritorios(Territorio* mapa) {
    char nomes[NUM_TERRITORIOS][TAM_NOME] = {
        "Alaska", "Brasil", "China", "Egito", "França",
        "Índia", "Japão", "México", "Rússia", "Sudão"
    };
    
    for (int i = 0; i < NUM_TERRITORIOS; i++) {
        strcpy(mapa[i].nome, nomes[i]);
        
        mapa[i].dono = (CorExercito)(rand() % 5);
        
        mapa[i].tropas = (rand() % 5) + 1;
    }
    
    mapa[0].dono = AZUL;
    mapa[0].tropas = 3; 
}

void liberarMemoria(Territorio* mapa) {
    free(mapa);
}

void exibirMenuPrincipal() {
    printf("========================================\n");
    printf("                MENU                    \n");
    printf("========================================\n");
    printf("1. Fase de Ataque\n");
    printf("2. Verificar Missão\n");
    printf("0. Sair do Jogo\n");
    printf("========================================\n");
}

void exibirMapa(const Territorio* mapa) {
    printf("========================================\n");
    printf("               MAPA ATUAL               \n");
    printf("========================================\n");
    printf("ID  | TERRITÓRIO     | DONO     | TROPAS\n");
    printf("----|----------------|----------|-------\n");
    
    const char* nomesCores[] = {"AZUL", "VERMELHO", "VERDE", "AMARELO", "ROXO"};
    
    for (int i = 0; i < NUM_TERRITORIOS; i++) {
        const char* simbolo = (mapa[i].dono == AZUL) ? "▶" : " "; 
        printf("%-3d | %-14s | %-8s | %d %s\n", 
               i, mapa[i].nome, nomesCores[mapa[i].dono], mapa[i].tropas, simbolo);
    }
    printf("========================================\n");
    
    int contagem[5] = {0};
    for (int i = 0; i < NUM_TERRITORIOS; i++) {
        contagem[mapa[i].dono]++;
    }
    
    printf("\nRESUMO DE DOMÍNIO:\n");
    for (int i = 0; i < 5; i++) {
        printf("%-8s: %d territórios\n", nomesCores[i], contagem[i]);
    }
}

void exibirMissao(int missaoId) {
    printf("========================================\n");
    printf("              SUA MISSÃO               \n");
    printf("========================================\n");
    
    switch (missaoId) {
        case 0:
            printf("DESTRUIR o exército VERMELHO\n");
            printf("(Conquistar todos os territórios vermelhos)\n");
            break;
        case 1:
            printf("DESTRUIR o exército VERDE\n");
            printf("(Conquistar todos os territórios verdes)\n");
            break;
        case 2:
            printf("DESTRUIR o exército AMARELO\n");
            printf("(Conquistar todos os territórios amarelos)\n");
            break;
        case 3:
            printf("DESTRUIR o exército ROXO\n");
            printf("(Conquistar todos os territórios roxos)\n");
            break;
        case 4:
            printf("CONQUISTAR 5 territórios no total\n");
            printf("(Ter pelo menos 5 territórios sob seu domínio)\n");
            break;
        case 5:
            printf("CONQUISTAR 7 territórios no total\n");
            printf("(Ter pelo menos 7 territórios sob seu domínio)\n");
            break;
        default:
            printf("Missão desconhecida!\n");
            break;
    }
    printf("========================================\n");
}

void faseDeAtaque(Territorio* mapa) {
    int origem, destino;
    
    printf("\n========================================\n");
    printf("              FASE DE ATAQUE           \n");
    printf("========================================\n\n");
    
    printf("Seus territórios disponíveis para ataque:\n");
    for (int i = 0; i < NUM_TERRITORIOS; i++) {
        if (mapa[i].dono == AZUL && mapa[i].tropas > 1) {
            printf("ID %d: %s (%d tropas)\n", i, mapa[i].nome, mapa[i].tropas);
        }
    }
    
    printf("\nTerritórios inimigos disponíveis:\n");
    for (int i = 0; i < NUM_TERRITORIOS; i++) {
        if (mapa[i].dono != AZUL) {
            const char* nomesCores[] = {"AZUL", "VERMELHO", "VERDE", "AMARELO", "ROXO"};
            printf("ID %d: %s (%s, %d tropas)\n", 
                   i, mapa[i].nome, nomesCores[mapa[i].dono], mapa[i].tropas);
        }
    }
    
    printf("\nSelecione o território de ORIGEM (ID): ");
    scanf("%d", &origem);
    limparBufferEntrada();
    
    if (origem < 0 || origem >= NUM_TERRITORIOS || mapa[origem].dono != AZUL || mapa[origem].tropas <= 1) {
        printf("Território de origem inválido!\n");
        return;
    }
    
    printf("Selecione o território de DESTINO (ID): ");
    scanf("%d", &destino);
    limparBufferEntrada();
    
    if (destino < 0 || destino >= NUM_TERRITORIOS || mapa[destino].dono == AZUL) {
        printf("Território de destino inválido!\n");
        return;
    }
    
    simularAtaque(mapa, origem, destino);
}

void simularAtaque(Territorio* mapa, int origem, int destino) {
    printf("\n========================================\n");
    printf("           SIMULAÇÃO DE ATAQUE         \n");
    printf("========================================\n\n");
    
    printf("%s (AZUL, %d tropas) ataca %s (", 
           mapa[origem].nome, mapa[origem].tropas, mapa[destino].nome);
    
    const char* nomesCores[] = {"AZUL", "VERMELHO", "VERDE", "AMARELO", "ROXO"};
    printf("%s, %d tropas)\n", nomesCores[mapa[destino].dono], mapa[destino].tropas);
    
    int dadosAtacante = mapa[origem].tropas - 1;
    if (dadosAtacante > 3) dadosAtacante = 3;
    
    int dadosDefensor = mapa[destino].tropas;
    if (dadosDefensor > 2) dadosDefensor = 2;
    
    printf("\nROLANDO DADOS...\n");
    
    int dadosA[dadosAtacante];
    printf("Atacante (%d dados): ", dadosAtacante);
    for (int i = 0; i < dadosAtacante; i++) {
        dadosA[i] = (rand() % 6) + 1;
        printf("%d ", dadosA[i]);
    }
    
    int dadosD[dadosDefensor];
    printf("\nDefensor (%d dados): ", dadosDefensor);
    for (int i = 0; i < dadosDefensor; i++) {
        dadosD[i] = (rand() % 6) + 1;
        printf("%d ", dadosD[i]);
    }
    
    for (int i = 0; i < dadosAtacante - 1; i++) {
        for (int j = i + 1; j < dadosAtacante; j++) {
            if (dadosA[i] < dadosA[j]) {
                int temp = dadosA[i];
                dadosA[i] = dadosA[j];
                dadosA[j] = temp;
            }
        }
    }
    
    for (int i = 0; i < dadosDefensor - 1; i++) {
        for (int j = i + 1; j < dadosDefensor; j++) {
            if (dadosD[i] < dadosD[j]) {
                int temp = dadosD[i];
                dadosD[i] = dadosD[j];
                dadosD[j] = temp;
            }
        }
    }
    
    printf("\n\nCOMPARAÇÃO:\n");
    
    int comparacoes = (dadosAtacante < dadosDefensor) ? dadosAtacante : dadosDefensor;
    int perdasAtacante = 0;
    int perdasDefensor = 0;
    
    for (int i = 0; i < comparacoes; i++) {
        printf("Dado %d: Atacante %d vs Defensor %d - ", i+1, dadosA[i], dadosD[i]);
        if (dadosA[i] > dadosD[i]) {
            printf("Defensor perde 1 tropa\n");
            perdasDefensor++;
        } else {
            printf("Atacante perde 1 tropa\n");
            perdasAtacante++;
        }
    }
    
    mapa[destino].tropas -= perdasDefensor;
    mapa[origem].tropas -= perdasAtacante;
    
    printf("\nRESULTADO:\n");
    printf("%s: %d tropas restantes\n", mapa[origem].nome, mapa[origem].tropas);
    printf("%s: %d tropas restantes\n", mapa[destino].nome, mapa[destino].tropas);
    
    if (mapa[destino].tropas <= 0) {
        printf("\nVITÓRIA! %s foi conquistado!\n", mapa[destino].nome);
        mapa[destino].dono = AZUL;
        mapa[destino].tropas = 1; 
        
        if (mapa[origem].tropas > 1) {
            int tropasParaMover = mapa[origem].tropas - 1;
            if (tropasParaMover > 3) tropasParaMover = 3;
            
            mapa[origem].tropas -= tropasParaMover;
            mapa[destino].tropas += tropasParaMover;
            
            printf("%d tropas movidas de %s para %s\n", 
                   tropasParaMover, mapa[origem].nome, mapa[destino].nome);
        }
    } else {
        printf("\nAtaque não conseguiu conquistar o território.\n");
    }
}

int sortearMissao() {
    return rand() % NUM_MISSOES;
}

int verificarVitoria(const Territorio* mapa, int missaoId) {
    switch (missaoId) {
        case 0: 
            for (int i = 0; i < NUM_TERRITORIOS; i++) {
                if (mapa[i].dono == VERMELHO) {
                    return 0; 
                }
            }
            return 1; 
            
        case 1: 
            for (int i = 0; i < NUM_TERRITORIOS; i++) {
                if (mapa[i].dono == VERDE) {
                    return 0;
                }
            }
            return 1;
            
        case 2:
            for (int i = 0; i < NUM_TERRITORIOS; i++) {
                if (mapa[i].dono == AMARELO) {
                    return 0;
                }
            }
            return 1;
            
        case 3: 
            for (int i = 0; i < NUM_TERRITORIOS; i++) {
                if (mapa[i].dono == ROXO) {
                    return 0;
                }
            }
            return 1;
            
        case 4: 
            {
                int contagem = 0;
                for (int i = 0; i < NUM_TERRITORIOS; i++) {
                    if (mapa[i].dono == AZUL) {
                        contagem++;
                    }
                }
                return contagem >= 5;
            }
            
        case 5: 
            {
                int contagem = 0;
                for (int i = 0; i < NUM_TERRITORIOS; i++) {
                    if (mapa[i].dono == AZUL) {
                        contagem++;
                    }
                }
                return contagem >= 7;
            }
            
        default:
            return 0;
    }
}

void limparBufferEntrada() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}
#include <iostream>
#include <cstdlib>
#include <ctime>
using namespace std;

#define TAM 10

int frames[TAM];

struct Paginas
{
    int numero_frame;
    char bit_validade;
};

void inicializarRAM(int *memoria)
{
    
    for (int i = 0; i < TAM; i++)
    {
        memoria[i] = -1;
    }

    
    bool usado[TAM] = {false};

    int preenchidos = 0;
    while (preenchidos < TAM / 2)
    {
        int pos = rand() % TAM;
        int pagina = rand() % TAM;

        if (memoria[pos] == -1 && !usado[pagina])
        {
            memoria[pos] = pagina;
            usado[pagina] = true;
            preenchidos++;
        }
    }
}


void inicializarPaginas(Paginas *paginas, int *memoria)
{
    for (int i = 0; i < TAM; i++)
    {
        paginas[i].numero_frame = -1;
        paginas[i].bit_validade = 'i';
    }

    for (int i = 0; i < TAM; i++)
    {
        if (memoria[i] != -1)
        {
            int pagina = memoria[i];
            paginas[pagina].numero_frame = i;
            paginas[pagina].bit_validade = 'v';
        }
    }
}

void mostrarRAM()
{
    cout << "RAM: [ ";
    for (int i = 0; i < TAM; i++)
    {
        if (frames[i] == -1)
            cout << "Livre";
        else
            cout << "Pag" << frames[i];

        if (i < TAM - 1)
            cout << ", ";
    }
    cout << " ]\n";
}

void mostrarTabela(Paginas *paginas)
{
    cout << "Tabela de Paginas:\n";
    for (int i = 0; i < TAM; i++)
    {
        if (paginas[i].bit_validade == 'v' || paginas[i].numero_frame != -1 || paginas[i].bit_validade == 'i')
        {
            // Exibe apenas paginas que foram referenciadas (validas ou invalidas conhecidas)
            if (paginas[i].bit_validade == 'v')
            {
                cout << "Pag" << i << ": Frame " << paginas[i].numero_frame
                     << " [v]\n";
            }
            else
            {
                cout << "Pag" << i << ": Frame - [i]\n";
            }
        }
    }
}

void simularAcessos(int *acessos, int tamanho, Paginas *tabela)
{
    cout << "\n--- INICIANDO SIMULACAO ---\n";

    for (int i = 0; i < tamanho; i++)
    {
        int pagina = acessos[i];

        cout << "\n> CPU solicita acesso a Pagina " << pagina << "...\n";

        // CASO 1: HIT
        if (tabela[pagina].bit_validade == 'v')
        {
            cout << "  [HIT] Acesso bem-sucedido! A Pagina "
                 << pagina << " esta no Frame "
                 << tabela[pagina].numero_frame << ".\n";
        }

        // CASO 2: PAGE FAULT
        else
        {
            cout << "  [MISS] PAGE FAULT! A Pagina " << pagina
                 << " esta com bit 'i' (invalida).\n";

            cout << "  [SO] Iniciando rotina de tratamento (Trap)...\n";
            cout << "  [SO] Buscando a Pagina " << pagina
                 << " no Disco (Backing Store)...\n";

            // procurar frame livre
            int frameLivre = -1;
            for (int j = 0; j < TAM; j++)
            {
                if (frames[j] == -1)
                {
                    frameLivre = j;
                    break;
                }
            }

            if (frameLivre != -1)
            {
                cout << "  [SO] Encontrado espaco no Frame "
                     << frameLivre << " da RAM.\n";

                cout << "  [SO] Movendo pagina para a RAM...\n";

                // carregar na RAM
                frames[frameLivre] = pagina;

                // atualizar tabela
                tabela[pagina].numero_frame = frameLivre;
                tabela[pagina].bit_validade = 'v';

                cout << "  [SO] Atualizando Tabela de Paginas (bit alterado para 'v').\n";

                cout << "  [RESTART] Instrucao reiniciada! Acesso a Pagina "
                     << pagina << " concluido com sucesso.\n";
            }
            else
            {
                cout << "  OUT OF MEMORY: Necessario Algoritmo de Substituicao\n";
                break;
            }
        }
    }
}

int main()
{
    // sorteia as paginas 
    srand(time(0));

    Paginas tabela[TAM];

    inicializarRAM(frames);
    inicializarPaginas(tabela, frames);

    cout << "--/ ESTADO INICIAL /--\n";
    mostrarRAM();
    mostrarTabela(tabela);

    // Sequencia de acessos: paginas que a CPU vai solicitar
    int acessos[] = {2, 5, 0, 7, 3};
    int tamanho = sizeof(acessos) / sizeof(acessos[0]);

    simularAcessos(acessos, tamanho, tabela);

    cout << "\n--- ESTADO FINAL ---\n";
    mostrarRAM();

    return 0;
}

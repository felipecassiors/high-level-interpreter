/********************************************************************************

Detalhes do set de instru��o

	Tamanho das instru��es: 16 bits

	C�digo das intru��es:

		LOAD: 	00
		STORE:	01
		ADD: 	10
		SUB: 	11

	Instru��es Tipo 1:

		- Utilizado para opera���es aritm�ticas (soma, subtra��o, ...)

             MSB                                      LSB

		(Tipo instr.) (End. Reg 1) (End. Reg 2) (End Reg Dest.)

           2 bits        4 bits        4 bits       6 bits


         - Exemplo: 0b0001000000010010 >>> |01|0000|0001|000010

         	 	Realiza a soma (10 >> tipo da instru��o) do registro 0 (0000
 	 	 	 >> end. Reg 1) com o registro 1 (0001 >> end. Reg 2) e salva o resultado
 	 	 	 em registro 2 (000010 >> end. Reg Dest.)


    Instru��es Tipo 2:

     	 - Uitlizado para opera��es de LOAD e STORE

     	       MSB                        LSB

     	 (Tipo instr.) (End Reg) (End Mem�ria de dados)

		    2 bits       4 bits        10 bits

   	   - Exemplo: 0b1000000000010010 >>> |00|0000|0000001010

         	 	Realiza o LOAD (00 >> tipo da instru��o) do endere�o de
			mem�ria 10 (0000001010 >> end. Mem�ria) para o registro 0
			(0000 >> end. Reg )

********************************************************************************/

#include<iostream>
#include<clocale>
#include<windows.h>
#include<stdio.h>
#include<bitset>

#define DEFAULT_FILENAME_MEMORY "memoria.txt"
#define DEFAULT_FILENAME_INSTRUCTIONS "instrucoes.txt"
#define MAX_FILENAME_SIZE 32

#define MAX_PROG_MEMORY 32
#define MAX_DATA_MEMORY 32
#define MAX_CACHE_MEMORY 8

using namespace std;

// Memoria de programa
unsigned int ProgMemory[MAX_PROG_MEMORY];

// Memoria de dados
unsigned int DataMemory[MAX_DATA_MEMORY];

struct CacheMemory {
    bool valid; // 1 bit
    unsigned int address; // 5 bits = log2(MAX_DATA_MEMORY)
    unsigned int data;

} cacheDataMemory [MAX_CACHE_MEMORY];

// Registradores
unsigned int PC;
unsigned int Instr;
unsigned int InstrType;
unsigned int RegSourceA;
unsigned int RegSourceB;
unsigned int RegDest;
unsigned int RegAddrMemory;
unsigned int Reg[10];

int instructionsQuantity;

// Prototipos
void decode();
void execute();
void runInterpreter();
void cleanMemory();

unsigned int getData(unsigned int address);
void updateCache(unsigned int address);
void saveData(unsigned int address, unsigned int data);


// Prototipos de funcoes relacionadas a interface
void showMenuHeader();
void showFileStatus(FILE*, char*);
void showProgMemory();
void showDataMemory();
// Prototipos de funcoes relacionadas a arquivo
void reloadFile(FILE**, char*);
void fileToProgMemory(FILE*);
void fileToDataMemory(FILE*);

int main()
{
	char option_1, option_2, option_3;

	FILE* memFile = NULL;
	FILE* instrFile = NULL;
	char memFilename[MAX_FILENAME_SIZE] = DEFAULT_FILENAME_MEMORY;
	char instrFilename[MAX_FILENAME_SIZE] = DEFAULT_FILENAME_INSTRUCTIONS;

	setlocale(LC_ALL, "Portuguese");

	reloadFile(&memFile, memFilename);
	reloadFile(&instrFile, instrFilename);

	cleanMemory();

	do
	{
		showMenuHeader();
		cout << "\t\t\tMenu Principal\n";
		cout << "--------------------------------------------------------------------------------\n\n";
		showFileStatus(memFile, memFilename);
		showFileStatus(instrFile, instrFilename);
		cout << "\t\t1 - Gerenciar arquivo\n";
		cout << "\t\t2 - Carregar do arquivo para a mem�ria" << endl;
		cout << "\t\t3 - Listar mem�ria" << endl;
		cout << "\t\t4 - Executar" << endl;
		cout << "\t\t5 - Limpar mem�ria" << endl;
		cout << "\n\t\t9 - Sair" << endl;
		cout << "\nPor favor, digite a op��o desejada\n> ";
		cin.clear();
		cin.sync();
		cin >> option_1;
		switch(option_1)
		{
		case '1':
			do
			{
				showMenuHeader();
				cout << "\t\t\tGerenciar Arquivo\n";
				cout << "--------------------------------------------------------------------------------\n\n";
				showFileStatus(memFile, memFilename);
				showFileStatus(instrFile, instrFilename);
				cout << "\t\t1 - Arquivo de mem�ria ('" << memFilename << "')\n";
				cout << "\t\t2 - Arquivo de instru��es ('" << instrFilename << "')" << endl;
				cout << "\n\t\t9 - Voltar ao Menu Principal" << endl;
				cout << "\nPor favor, digite a op��o desejada\n> ";
				cin.clear();
				cin.sync();
				cin >> option_2;
				switch(option_2)
				{
				case '1':
					do
					{
						showMenuHeader();
						cout << "\t\t\tGerenciar Arquivo\n";
						cout << "--------------------------------------------------------------------------------\n\n";
						showFileStatus(memFile, memFilename);
						cout << "\t\t1 - Recarregar\n";
						cout << "\t\t2 - Fechar arquivo" << endl;
						cout << "\t\t3 - Alterar arquivo" << endl;
						cout << "\n\t\t9 - Voltar ao Menu anterior" << endl;
						cout << "\nPor favor, digite a op��o desejada\n> ";
						cin.clear();
						cin.sync();
						cin >> option_3;
						switch(option_3)
						{
						case '1':
							reloadFile(&memFile, memFilename);
							break;
						case '2':
							showMenuHeader();
							cout << "\t\t\tFechar arquivo\n";
							cout << "--------------------------------------------------------------------------------\n\n";
							fclose(memFile);
							memFile = NULL;
							cout << "Arquivo '" << memFilename << "' fechado.\n\n";
							system("pause");
							break;
						case '3':
							showMenuHeader();
							cout << "\t\t\tAlterar arquivo\n";
							cout << "--------------------------------------------------------------------------------\n\n";
							cout << "O sistema atualmente busca o arquivo '" << memFilename << "'.\n\n";
							cout << "Digite o novo nome do arquivo (com a extens�o) que o sistema deve buscar\n";
							cout << "Exemplo: 'arquivo.txt' sem ap�strofos.\n>";
							for(int i = 0; i < MAX_FILENAME_SIZE; i++)
								memFilename[i] = 0;
							cin.clear();
							cin.sync();
							cin.getline(memFilename, MAX_FILENAME_SIZE);
							cout << "\nNome do arquivo que o sistema busca alterado para '" << memFilename << "'.\n\n";
							reloadFile(&memFile, memFilename);
							system("pause");
							break;
						case '9':
							break;
						default:
							cout << "\tOp��o inv�lida\n\n";
							system("pause");
						}
					}
					while(option_3 != '9');
					break;
				case '2':
					do
					{
						showMenuHeader();
						cout << "\t\t\tGerenciar Arquivo\n";
						cout << "--------------------------------------------------------------------------------\n\n";
						showFileStatus(instrFile, instrFilename);
						cout << "\t\t1 - Recarregar\n";
						cout << "\t\t2 - Fechar arquivo" << endl;
						cout << "\t\t3 - Alterar arquivo" << endl;
						cout << "\n\t\t9 - Voltar ao Menu anterior" << endl;
						cout << "\nPor favor, digite a op��o desejada\n> ";
						cin.clear();
						cin.sync();
						cin >> option_3;
						switch(option_3)
						{
						case '1':
							reloadFile(&instrFile, instrFilename);
							break;
						case '2':
							showMenuHeader();
							cout << "\t\t\tFechar arquivo\n";
							cout << "--------------------------------------------------------------------------------\n\n";
							fclose(instrFile);
							instrFile = NULL;
							cout << "Arquivo '" << instrFilename << "' fechado.\n\n";
							system("pause");
							break;
						case '3':
							showMenuHeader();
							cout << "\t\t\tAlterar arquivo\n";
							cout << "--------------------------------------------------------------------------------\n\n";
							cout << "O sistema atualmente busca o arquivo '" << instrFilename << "'.\n\n";
							cout << "Digite o novo nome do arquivo (com a extens�o) que o sistema deve buscar\n";
							cout << "Exemplo: 'arquivo.txt' sem ap�strofos.\n>";
							for(int i = 0; i < MAX_FILENAME_SIZE; i++)
								instrFilename[i] = 0;
							cin.clear();
							cin.sync();
							cin.getline(instrFilename, MAX_FILENAME_SIZE);
							cout << "\nNome do arquivo que o sistema busca alterado para '" << instrFilename << "'.\n\n";
							reloadFile(&instrFile, instrFilename);
							system("pause");
							break;
						case '9':
							break;
						default:
							cout << "\tOp��o inv�lida\n\n";
							system("pause");
						}
					}
					while(option_3 != '9');
					break;
				case '9':
					break;
				default:
					cout << "\tOp��o inv�lida\n\n";
					system("pause");
				}
			}
			while(option_2 != '9');
			break;
		case '2':
			cleanMemory();
			fileToProgMemory(instrFile);
			fileToDataMemory(memFile);
			break;
		case '3':
			showMenuHeader();
			cout << "\t\t\tListagem da mem�ria\n";
			cout << "--------------------------------------------------------------------------------\n";

			showProgMemory();
			showDataMemory();

			cout << endl;

			system("pause");
			break;
		case '4':
			showMenuHeader();
			cout << "\t\t\tExecu��o\n";
			cout << "--------------------------------------------------------------------------------\n";

			runInterpreter();

			memFile = fopen(memFilename, "w+");

			for(int i = 0; i < MAX_DATA_MEMORY; i++)
			{
				fprintf(memFile, "%d\n", DataMemory[i]);
			}

			cout << "Instru��es executadas.\n\n";

			reloadFile(&memFile, memFilename);

			system("pause");
			break;
		case '5':
			cleanMemory();
			break;
		case '9':
			break;
		default:
			cout << "\tOp��o inv�lida\n\n";
			system("pause");
		}
	}
	while(option_1 != '9');
	fclose(memFile);
	fclose(instrFile);
	return 0;
}

void showMenuHeader()
{
	system("cls");
	cout << "--------------------------------------------------------------------------------\n";
	cout << "\tEC 208 - Arquitetura de Computadores II\n";
	cout << "\tProjeto: M�quina Virtual/ Interpretador de Alto N�vel\n";
	cout << "\tAutores: Bruno Balestra/ Felipe Santos/ Matheus Oliveira\n";
	cout << "--------------------------------------------------------------------------------\n";
}

void showFileStatus(FILE* file, char* filename)
{
	if(file == NULL)
	{
		cout << "\tArquivo '" << filename << "' n�o carregado ou n�o encontrado.\n\n";
	}
	else
	{
		cout << "\tArquivo '" << filename << "' carregado.\n\n";
	}
}

void reloadFile(FILE** file, char* filename)
{
	fclose(*file);
	*file = NULL;
	*file = fopen(filename, "r+");
}

void fileToProgMemory(FILE* file)
{
	char str[18];

	int i = 0;
	if (file)
	{
		while(fgets(str, sizeof(str), file) != NULL)
		{
			ProgMemory[i] = (unsigned int)(strtol(str, NULL, 2));

			i++;
		}
	}

	instructionsQuantity = i;
}

void fileToDataMemory(FILE* file)
{
	char str[18];

	int i = 0;
	if (file)
	{
		while(fgets(str, sizeof(str), file) != NULL)
		{
			DataMemory[i] = (unsigned int)(strtol(str, NULL, 10));

			i++;
		}
	}

}

void showProgMemory()
{
	cout << "\n\tProgMemory\n";
	for(int i = 0; i < instructionsQuantity; i++)
	{
		cout << "ENDERE�O = [" << i << "] : CONTE�DO = [" << bitset<16>(ProgMemory[i]) << "]"<< endl;
	}
}

void showDataMemory()
{
	cout << "\n\tDataMemory\n";
	for(int i = 0; i < MAX_DATA_MEMORY; i++)
	{
		cout << "TAG = [" << bitset<5>(i) << "] : CONTE�DO = [" << DataMemory[i] << "]" << endl;
	}
	cout << endl;
}

void cleanMemory()
{
	for(int i = 0; i < MAX_PROG_MEMORY; i++)
	{
		ProgMemory[i] = 0;
	}
	for(int i = 0; i < MAX_DATA_MEMORY; i++)
	{
		DataMemory[i] = 0;
	}
}

void runInterpreter()
{
    cout << "Rodando...\n\n";
    cout << "Limpando registradores... ";
	PC = 0;
	for(int i = 0; i < 10; i++)
	{
		Reg[i] = 0;
	}
	cout << "conclu�do\n\n";

	updateCache(0);

	showProgMemory();
	showDataMemory();

	while(PC < instructionsQuantity)
	{
	    cout << "\nExecutando instru��o [" << PC << "] do tipo... ";
		Instr = ProgMemory[PC]; // busca da instru��o na linha do PC do bloco
		PC++;
		decode();    // decodifica��o
		execute();
	}

	showDataMemory();
}

void decode(void)
{
	InstrType = Instr >> 14;

	if(InstrType == 2 || InstrType == 3)
	{
		// Soma, Subtracao
		RegSourceA = Instr >> 10;
		RegSourceA = RegSourceA & 0b0000000000001111;
		RegSourceB = Instr >> 6;
		RegSourceB = RegSourceB & 0b0000000000001111;
		RegDest = Instr & 0b0000000000001111;
	}
	else if(InstrType == 0)
	{
		/* Load */
		RegDest = Instr >> 10;
		RegDest = RegDest & 0b0000000000001111;
		RegAddrMemory = Instr & 0b0000000011111111;
	}
	else if(InstrType == 1)
	{
		/* Store */
		RegSourceA = Instr >> 10;
		RegSourceA = RegSourceA & 0b0000000000001111;
		RegAddrMemory = Instr & 0b0000000011111111;
	}
}

void execute(void)
{
	if(InstrType == 2)
	{
		// Soma
		cout << "soma\n";
		Reg[RegDest] = Reg[RegSourceA] + Reg[RegSourceB];
	}
	else if(InstrType == 3)
	{
		// Subtracao
		cout << "subtra��o\n";
		Reg[RegDest] = Reg[RegSourceA] - Reg[RegSourceB];
	}
	else if(InstrType == 0)
	{
		// Load
		cout << "load\n";
		Reg[RegDest] = getData(RegAddrMemory);
	}
	else if(InstrType == 1)
	{
		// Store
		cout << "store\n";
        saveData(RegAddrMemory, Reg[RegSourceA]);
	}
}

void updateCache(unsigned int address) {
    cout << "Atualizando cache (TAG de refer�ncia = [" << bitset<5>(address) << "])..." << endl;
    int initialAddress = address - (MAX_CACHE_MEMORY-1)/2;
    int finalAddress = address + MAX_CACHE_MEMORY/2;
    if(initialAddress < 0) {
        initialAddress = 0;
    } else if(finalAddress > MAX_DATA_MEMORY-1) {
        int toMax = finalAddress - MAX_DATA_MEMORY-1;
        initialAddress -= toMax;
    }

    cout << "TAG inicial = [" <<  bitset<5>(initialAddress) << "]\t";
    cout << "TAG final = [" << bitset<5>(initialAddress + MAX_CACHE_MEMORY-1) << "]\n";
    for (int i = 0; i < MAX_CACHE_MEMORY; ++i, ++initialAddress) {
        cacheDataMemory[i].address = initialAddress;
        cacheDataMemory[i].data = DataMemory[initialAddress];
        cacheDataMemory[i].valid = true;
    }
    cout << "Cache atualizada.\n\n";
}

unsigned int getData(unsigned int address) {
    cout << "Tentando buscar dados na cache (TAG = [" << bitset<5>(address) << "])... ";
    for (int i = 0; i < MAX_CACHE_MEMORY; ++i) {
        if (cacheDataMemory[i].address == address) {
            if (cacheDataMemory[i].valid) {
                cout << "HIT!" << endl;
                cout << "Retornando CONTE�DO = [" << cacheDataMemory[i].data << "]" << endl;
                return cacheDataMemory[i].data;
            } else {
                break;
            }
        }
    }
    cout << "MISS!\n\n";
    updateCache(address);
    cout << "Retornando CONTE�DO = [" << DataMemory[address] << "]" << endl;
    return DataMemory[address];
}

void saveData(unsigned int address, unsigned int data) {
    cout << "Salvando dados na mem�ria (TAG = [" << bitset<5>(address) << "])... ";
    DataMemory[address] = data;
    for (int i = 0; i < MAX_CACHE_MEMORY; ++i) {
        if (cacheDataMemory[i].address == address) {
            cacheDataMemory[i].valid = false;
            break;
        }
    }
    cout << "salvo" << endl;
}

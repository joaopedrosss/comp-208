#include "compressao.h"
#include "descompressao.h"

int main(){
    
    printf("--------------------------------------\n");
    printf("COMPRESSOR/DESCOMPRESSOR DE ARQUIVOS\n");
    
     //char nome_do_arquivo[] = "japan.png";
    
    int escolha;
    int rodando = 1;
    
    

    while(rodando){
        printf("--------------------------------------\n");
        printf("Selecione o que deseja fazer:\n");
        printf("(1) Comprimir\n(2) Descomprimir\n(3) SAIR\n");
        printf("--------------------------------------\n");
        printf("R>:");
        char nome_do_arquivo[106];
        char nome_descompressao[106]; 

        scanf("%d",&escolha);

        switch(escolha){
            case 1:
                printf("Digite o nome do arquivo:\n");
                scanf("%s",nome_do_arquivo);
                comprimir_arquivo(nome_do_arquivo);

                break;
            case 2:
                printf("Digite o nome do arquivo:\n");
                scanf("%s",nome_do_arquivo);
                printf("Digite o nome que terá o arquivo descomprimido:\n");
                scanf("%s",nome_descompressao);
                descomprimir_arquivo(nome_do_arquivo,nome_descompressao);
                break;
            case 3:
                rodando = 0;
                break;
            default:
                printf("Selecione uma opção válida!\n");

        }
    }

    


    
}


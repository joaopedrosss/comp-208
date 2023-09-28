#include "compressao.h"
#include "descompressao.h"

int main(){
    /*
    printf("-----------------------------\n");
    printf("COMPRESSOR/DESCOMPRESSOR DE ARQUIVOS\n");
    printf("-----------------------------\n");
    printf("Selecione o que deseja fazer:\n");
    */
    //char nome_do_arquivo[] = "japan.png";

    int escolha;
    printf("Press 1 or 2: ");
    scanf("%d",&escolha);

    char nome_do_arquivo[] = "test.txt";


    char nome_do_arquivo_para_descomprimir[] = "teste.txt.huff";
    char nome_final[] = "teste_descomprido.txt";

    if(escolha == 1){
        comprimir_arquivo(nome_do_arquivo);
    }else{
        descomprimir_arquivo(nome_do_arquivo_para_descomprimir,nome_final);
    }

    
}


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
    //char nome_do_arquivo[] = "test.txt";

    //comprimir_arquivo(nome_do_arquivo);

    char nome_do_arquivo_para_descomprimir[] = "teste.txt.huff";
    char nome_final[] = "teste3.txt";

    descomprimir_arquivo(nome_do_arquivo_para_descomprimir,nome_final);


    
}


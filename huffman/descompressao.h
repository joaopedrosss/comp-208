#include "libs_huff.h"


typedef struct binary_tree_decompress{
    void *byte;
    struct binary_tree_decompress *esquerda,*direita;
}DECOMPRESS_TREE;

void tamanho_lixo_e_huffman(int *bits_de_lixo, 
                            long *tamanho_da_arvore,
                            uint8_t* byte_do_arquivo)
{

};


void descomprimir_arquivo(char* nome_do_arquivo, char* nome_destino){
    
    FILE *arquivo_comprimido = fopen(nome_do_arquivo,"rb");

    if(arquivo_comprimido == NULL){
        printf("--FALHA AO ABRIR O ARQUIVO--\n");
        exit(1);
    }
    
    /**
    * 1) Pegando o tamanho do aqruivo
    *
    */
    fseek(arquivo_comprimido,0,SEEK_END); //*stream, offset, origin

    long tamanho_do_arquivo = ftell(arquivo_comprimido);

    int bits_de_lixo = 0;
    long tamanho_da_arvore = 0;

    printf("tamanho do arquivo: %ld bytes\n",tamanho_do_arquivo);

    fseek(arquivo_comprimido,0,SEEK_SET);


    uint8_t *bytes_do_arquivo = (uint8_t*) malloc(tamanho_do_arquivo);

    if(bytes_do_arquivo == NULL){
        printf("FALHA AO ALOCAR MEMÓRIA PARA ARRAY bytes_do_arquivo");
        exit(1);
    }

    memset(bytes_do_arquivo,0,tamanho_do_arquivo);

    fread(bytes_do_arquivo,1,tamanho_do_arquivo,arquivo_comprimido);

    fclose(arquivo_comprimido);

    /*
    2) QUAL O TAMANHO DO LIXO E DA NOSSA ARQVORE DE HUFFMAN? (LER 2 PRIMEIROS BYTES)
    */

   tamanho_lixo_e_huffman(&bits_de_lixo, &tamanho_da_arvore,bytes_do_arquivo);

}
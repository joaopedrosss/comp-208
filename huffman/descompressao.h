#include "libs_huff.h"


typedef struct binary_tree_decompress{
    void *byte;
    struct binary_tree_decompress *esquerda,*direita;
}DECOMPRESS_TREE;

DECOMPRESS_TREE *novo_node_arvore(void *byte){
    DECOMPRESS_TREE *novo_node = (DECOMPRESS_TREE*) malloc(sizeof(DECOMPRESS_TREE));
    if(novo_node == NULL){
        printf("FALHA AO ALOCAR MEMORIA (novo_node)");
        exit(1);
    }

    novo_node->byte = (void*)malloc(1);
    if(novo_node->byte == NULL){
        printf("FALHA AO ALOCAR MEMORIA (byte)");
        exit(1);
    }
    memcpy(novo_node->byte,byte,1);
    novo_node->esquerda = NULL;
    novo_node->direita = NULL;

    return novo_node;
}

DECOMPRESS_TREE *montar_arvore(uint8_t *bytes_do_arquivo,
                              int *posicao_atual,
                              long tamanho_da_arvore,
                              DECOMPRESS_TREE* raiz)
{
    
    if(*posicao_atual == tamanho_da_arvore + 2){ 
        
        //POR QUE tamanho_da_arvore + 2 ? 
        // começamos no terceiro byte (posicao_inicial = 2)
        // a exemplo: 'tamnaho_arvore' é 13
        // Entao vamos ter que andar por mais 13 bytes (posicao_inicial + 13)
        // terminando na posicao posicao final: 15 (no exemplo no caso) - 
        //ou seja, posicao_final é tamanho_arvore' + 2 
        
        return NULL;
    }else{
        if(bytes_do_arquivo[*posicao_atual] == '\\'){ //veja se eh um caractere especial

            printf("[%d]\n",*posicao_atual);
            (*posicao_atual)++; //vá para o proximo byte
            printf("[%d] %c\n",*posicao_atual,bytes_do_arquivo[*posicao_atual]);
            raiz = novo_node_arvore(&bytes_do_arquivo[*posicao_atual]);
            (*posicao_atual)++;


            return raiz;

        }else if(bytes_do_arquivo[*posicao_atual] == '*'){ //veja se chegou num nó interno
            
            
            raiz = novo_node_arvore(&bytes_do_arquivo[*posicao_atual]);
            printf("[%d] %c\n",*posicao_atual,bytes_do_arquivo[*posicao_atual]);
            

            (*posicao_atual)++;

            
            raiz->esquerda = montar_arvore(bytes_do_arquivo, posicao_atual, tamanho_da_arvore, raiz);

            raiz->direita = montar_arvore(bytes_do_arquivo, posicao_atual, tamanho_da_arvore, raiz);


        }else{ //chegou numa folha
            raiz = novo_node_arvore(&bytes_do_arquivo[*posicao_atual]);
            printf("[%d] %c\n",*posicao_atual,bytes_do_arquivo[*posicao_atual]);

            (*posicao_atual)++;
           
            return raiz;
        }
    }

    return raiz;
};

void imprimir_em_pre_ordem_descompress(DECOMPRESS_TREE* arvore_huffman)
{
    if(arvore_huffman != NULL){

        if(*((uint8_t*)arvore_huffman->byte) == '*' && arvore_huffman->esquerda != NULL && arvore_huffman->direita != NULL){
            printf("*\n");
        }else{
            printf("%c\n",*((uint8_t*)arvore_huffman->byte));
        }
        imprimir_em_pre_ordem_descompress(arvore_huffman->esquerda);
        imprimir_em_pre_ordem_descompress(arvore_huffman->direita);
    }
};

int is_bit_i_set(uint8_t c, int i){
    unsigned char mask = 1 << i;
    return mask & c;
}

void tamanho_lixo_e_huffman(int *bits_de_lixo, 
                            long *tamanho_da_arvore,
                            uint8_t* byte_do_arquivo)
{
    int posicao_bit, deslocar, posicao_byte;
    int bit_atual;
    

    //PEGAR O TAMANHO DO LIXO
    //Lendo os 3 primeiros bits , nas posicoes  7 6 5, do primeiro byte do arquivo
    for(posicao_bit = 7, deslocar = 2; posicao_bit >= 5; posicao_bit--, deslocar--){
        
        bit_atual = is_bit_i_set(byte_do_arquivo[0], posicao_bit);

        if(bit_atual){ // se o bit for 1
            *bits_de_lixo |= 1 << deslocar;
        }
    }

    /*
    PEGAR O TAMANHO DA ARVORE
    - vamos ver agora os bits restantes do primeiro byte (nas posicoes 4 3 2 1 0)
    - e vamos ver o segundo byte também para pegar o tamanho da arvore
    - primeiro byte -> posicao_byte = 0;
    - segundo byte -> posicao_byte = 1;
    - EXECUTE O LOOP ABAIXO até pegar os 13 bits do tamanho precisos para o tamanho da arv.
    Como o tamanho é long, não tem problema deslocar um bit em 12 posicoes por exemplo
    */
    for(posicao_bit = 4, deslocar = 12, posicao_byte = 0; //setando 
        deslocar >= 0; // condicao de parada
        deslocar--) // decrementar
    {
        bit_atual = is_bit_i_set(byte_do_arquivo[posicao_byte], posicao_bit);

        if(bit_atual){
            *tamanho_da_arvore |= 1 << deslocar;
        }

        posicao_bit--;

        if(posicao_bit < 0){ // já terminei de passar por 1 byte? vá para o próximo byte
            posicao_bit = 7;
            posicao_byte++;
        }
    }

    return;


};

void inserir_dados_descomprimidos(FILE* arquivo_descomprimido,
                                  uint8_t* bytes_do_arquivo,
                                  long tamanho_do_arquivo,
                                  int posicao_atual,
                                  int lixo,
                                  DECOMPRESS_TREE* arvore_huffman)
{

    DECOMPRESS_TREE* arv_aux = arvore_huffman;

    while(posicao_atual < tamanho_do_arquivo)
    { // PERCORRA OS BYTES

        uint8_t byte = bytes_do_arquivo[posicao_atual];

        int bit;    
        uint8_t *byte_da_arv;    

        for(int pos_bit = 7; pos_bit >= 0; pos_bit--)
        {// PERCORRA OS BITS

            bit = is_bit_i_set(byte,pos_bit);

            if(posicao_atual == tamanho_do_arquivo - 1 && pos_bit < lixo){ 
                //exemplo: 3 ultimos bits sao lixo (os bits: 2 1 0)
                //Voce está no último byte e está no bit 2 (2 < 3) -> ENTAO VC ESTÁ NO LIXO
                return;
            }

            if(bit){ // 1 -> vá pra direita
                arv_aux = arv_aux->direita;
            }else{ // 0 -> vá pra esquerda
                arv_aux = arv_aux->esquerda;
            }

            if(arv_aux->direita == NULL && arv_aux->esquerda == NULL){ //vc chegou a uma folha
                byte_da_arv = (uint8_t*)arv_aux->byte;
                printf("-[%c]-\n",*byte_da_arv);
                fwrite(&(*byte_da_arv), sizeof(uint8_t), 1, arquivo_descomprimido);
                
                arv_aux = arvore_huffman;
            }

        }
        posicao_atual++;
    }
    return;
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

   printf("Tamanho De Lixo: %d\n Tamanho Da Arvore (com caracteres ed escape): %ld\n",bits_de_lixo,tamanho_da_arvore);

   DECOMPRESS_TREE*  arvore_unzip = NULL;

   printf("MONTANDO ARVORE DE HUFFMAN...\n");

    /*
    3) MONTAR ÁRVORE DE HUFFMAN
    */
   // TODO: COLOCAR POSICAO ATUAL COMO TIPO 'long' ver se tem alguma diferença
   int posicao_atual = 2; // já estamos lendo o 3.o byte (byte na posicao 2) -> ver uma abordagem sem o mais dois
   arvore_unzip = montar_arvore(bytes_do_arquivo,&posicao_atual,tamanho_da_arvore,arvore_unzip);  // por que tamanho da arvore + 2
   printf("posicao atual:%d\n",posicao_atual);
   //POR QUE tamanho_da_arvore+2 ?
   //nos adiantamos dois bytes: estamos no terceiro byte
   
   imprimir_em_pre_ordem_descompress(arvore_unzip);

   FILE* arquivo_descomprimido = fopen(nome_destino, "wb");

   if(arquivo_descomprimido == NULL){
    printf("FALHA AO ESCREVER ARQUIVO DESCOMPRIMIDO");
    exit(1);
   }

    /*
    4) ESCREVER O ARQUIVO DESCOMPRIMIDO :)
    */
   int lixo = bits_de_lixo;

   inserir_dados_descomprimidos(arquivo_descomprimido, bytes_do_arquivo, tamanho_do_arquivo,posicao_atual, lixo, arvore_unzip);

   //write_file(arquivo_descomprimido,bytes_do_arquivo,tamanho_do_arquivo,posicao_atual,arvore_unzip,bits_de_lixo);

   /*
   5) LIMPAR MEMORIA ALOCADA
   */

  free(bytes_do_arquivo);
  free_huffman_tree(arvore_unzip);

  bytes_do_arquivo = NULL;
  arvore_unzip = NULL;
  
  fclose(arquivo_descomprimido);


}
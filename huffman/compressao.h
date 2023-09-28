#include "libs_huff.h"

typedef struct tree_{
    long frequencia_do_byte;
    void* byte;
    struct tree_ *next, *direita, *esquerda;
}COMPRESS_TREE;


//POR QUE PASSADMOS O ENDEREÇO DE IND?
void alocar_novo_no_em(COMPRESS_TREE** novo_node,
                       void *byte,
                       long frequencia_do_byte){

    (*novo_node) = (COMPRESS_TREE*) malloc(sizeof(COMPRESS_TREE));
    if(novo_node == NULL){
        printf("--FALHA NA ALOCACAO DE NO DA ARVORE--\n");
        exit(1);
    }

    (*novo_node)->byte = malloc(1);

    if((*novo_node)->byte == NULL){
        printf("--FALHA NA ALOCACAO DE MEMÓRIA PARA BYTE--\n");
        exit(1);
    }

    //print'f("%d\n",*((uint8_t*)byte));

    memcpy((*novo_node)->byte,byte,1);
    
    (*novo_node)->frequencia_do_byte = frequencia_do_byte;
    (*novo_node)->esquerda = NULL;
    (*novo_node)->direita = NULL;
    (*novo_node)->next = NULL;

    //printf("%ld\n",novo_node->frequencia_do_byte);

    return;
}

void enqueue(COMPRESS_TREE** cabeca_da_fila,
             COMPRESS_TREE* novo_node){

    
    if((*cabeca_da_fila) == NULL || novo_node->frequencia_do_byte < (*cabeca_da_fila)->frequencia_do_byte){
        
        novo_node->next = *cabeca_da_fila;
        *cabeca_da_fila = novo_node;
        
    }else{

        COMPRESS_TREE* node_atual = *cabeca_da_fila;

        while(node_atual->next != NULL){

            if(novo_node->frequencia_do_byte < node_atual->next->frequencia_do_byte){
                break;
            }

            node_atual = node_atual->next;
        }

        novo_node->next = node_atual->next;
        node_atual->next = novo_node;
    }
}

void dequeue(COMPRESS_TREE** cabeca_da_fila,
             COMPRESS_TREE** no_dequeued ){
    if(cabeca_da_fila == NULL){
        return;
    }
    COMPRESS_TREE* no_removido = (*cabeca_da_fila);
    
    (*cabeca_da_fila) = (*cabeca_da_fila)->next;
    
    no_removido->next = NULL;

    (*no_dequeued) = no_removido;
    return;
}


void criar_node_arvore(COMPRESS_TREE** raiz_da_arvore,
                       COMPRESS_TREE* sub_arvore_esquerda,
                       COMPRESS_TREE* sub_arvore_direita,
                       void *no_interno)
{
    (*raiz_da_arvore) = (COMPRESS_TREE*) malloc(sizeof(COMPRESS_TREE));
    if((*raiz_da_arvore) == NULL){
        printf("FALHA NA ALOCACAO DE MEMORIA");
        exit(1);
    }

    (*raiz_da_arvore)->byte = malloc(1);
    if((*raiz_da_arvore)->byte == NULL){
        printf("FALHA NA ALOCACAO DE MEMORIA");
        exit(1);
    }

    
    
    long soma_das_frequencias = sub_arvore_esquerda->frequencia_do_byte + sub_arvore_direita->frequencia_do_byte;

    memcpy((*raiz_da_arvore)->byte,no_interno,1);
    (*raiz_da_arvore)->frequencia_do_byte = soma_das_frequencias;
    (*raiz_da_arvore)->esquerda = (sub_arvore_esquerda);
    (*raiz_da_arvore)->direita = (sub_arvore_direita);

    return;
};

void criar_arvore_de_huffman(COMPRESS_TREE** nova_arvore,
                             COMPRESS_TREE** fila_de_prioridade)
{
    uint8_t no_interno = '*';
    while((*fila_de_prioridade)->next != NULL){

        COMPRESS_TREE *sub_arvore_esquerda, *sub_arvore_direita, *raiz_da_arvore;

        dequeue(&(*fila_de_prioridade), &sub_arvore_esquerda);
        dequeue(&(*fila_de_prioridade), &sub_arvore_direita);

        criar_node_arvore(&raiz_da_arvore,sub_arvore_esquerda,sub_arvore_direita,&no_interno);

        enqueue(&(*fila_de_prioridade),raiz_da_arvore);
        /*
        printf("%d %ld\n",*((uint8_t*)sub_arvore_esquerda->byte),sub_arvore_esquerda->frequencia_do_byte);
        printf("%d %ld\n",*((uint8_t*)sub_arvore_direita->byte),sub_arvore_direita->frequencia_do_byte);
        */


        
    }
    dequeue(&(*fila_de_prioridade),&(*nova_arvore));

};

long calcular_altura(COMPRESS_TREE* arvore_huffman){
    long alt_esquerda,alt_direita;
    if(arvore_huffman == NULL){
        return -1;
    }
    else{
        alt_esquerda = 1 + calcular_altura(arvore_huffman->esquerda);
        alt_direita = 1 + calcular_altura(arvore_huffman->direita);

        if(alt_esquerda > alt_direita){
            return alt_esquerda;
        }else{
            return alt_direita;
        }
    }
};


uint8_t** criar_dicionario(long tamanho_max){
    uint8_t** novo_dicionario = (uint8_t**) malloc(sizeof(uint8_t*)*MAX_TABLE_SIZE);

    if(novo_dicionario == NULL){
        printf("FALHA NA ALOCACAO DE MEMORIA (DICIONARIO)");
        exit(1);
    }

    for(int i = 0; i < MAX_TABLE_SIZE; i++){

        novo_dicionario[i] = (uint8_t*) malloc(sizeof(uint8_t)*tamanho_max);

        if(novo_dicionario[i] == NULL){
            printf("FALHA NA ALOCACAO DE MEMORIA (ARRAY DE DICIONARIO)");
            exit(1);
        }

        novo_dicionario[i][0] = '\0';
    }

    return novo_dicionario;
}

void preencher_dicionario(uint8_t **dicionario,
                          uint8_t* aux_dic, 
                          long profundidade,
                          COMPRESS_TREE* raiz)
{
    if(raiz->direita == NULL && raiz->esquerda == NULL){

        uint8_t byte = *((uint8_t*)raiz->byte);

        for(uint8_t i = 0; i < profundidade; i++){
            dicionario[byte][i] = aux_dic[i];
        }
        dicionario[byte][profundidade] = '\0';
        return;
    }

    aux_dic[profundidade] = '0';
    preencher_dicionario(dicionario,aux_dic,profundidade+1,raiz->esquerda);

    aux_dic[profundidade] = '1';
    preencher_dicionario(dicionario,aux_dic,profundidade+1,raiz->direita);

    return;


};


void inserir_arvore_no_arquivo(COMPRESS_TREE* arvore_huff,
                                FILE* arquivo_comprimido)
{
    if(arvore_huff != NULL){ 
        uint8_t byte = *((uint8_t*)arvore_huff->byte);

        if(arvore_huff->esquerda == NULL && arvore_huff->direita == NULL){
            if(byte == '*' || byte == '\\'){
                uint8_t caractere_de_escape = '\\';

                printf("\\");

                fwrite(&caractere_de_escape, sizeof(uint8_t),1,arquivo_comprimido);
            }
        }
        printf("-%d-\n",byte);

        fwrite(&byte,sizeof(uint8_t),1,arquivo_comprimido);

        inserir_arvore_no_arquivo(arvore_huff->esquerda,arquivo_comprimido);
        inserir_arvore_no_arquivo(arvore_huff->direita,arquivo_comprimido);

    }   

    return;

}

void inserir_header_no_arquivo(COMPRESS_TREE* arvore_huff,
                               int bits_de_lixo, 
                               long qtd_elementos_da_arvore,
                               FILE* arquivo_comprimido)
{

    uint8_t primeiro_byte, segundo_byte;

    //exemplo: bits_de_lixo = 5 : 0x0000 0101
    //exemplo: qtd_elementos_da_arvore = 256 : 0x 0001 0000 0000

    primeiro_byte = bits_de_lixo << 5; // 0x1010 0000 

    primeiro_byte = primeiro_byte | (uint8_t)(qtd_elementos_da_arvore >> 8);
    // 0x1010 0000  + (uint8_t) (0001 0000 0000 >> 8)
    // 0x1010 0000  + (uint8_t) (0000 0000 0001 >> 8)
    // 0x1010 0000 + 0x0000 0001
    //int h = 256 >> 8;
    //printf("%d\n",(uint8_t)(256 >> 8));
    // 0x1010 0001 (primeiro_byte) (decimal: 161)
    
    segundo_byte = (uint8_t)qtd_elementos_da_arvore;
    //(uint8_t) (0001 0000 0000)
    //0x0000 0000 (segundo_byte) (decimal: 0)

    printf("%d %d\n",primeiro_byte,segundo_byte);

    fwrite(&primeiro_byte,sizeof(uint8_t),1,arquivo_comprimido);

    fwrite(&segundo_byte,sizeof(uint8_t),1,arquivo_comprimido);

    inserir_arvore_no_arquivo(arvore_huff,arquivo_comprimido);

}

void inserir_dados_comprimidos(FILE* arquivo_comprimido,
                               uint8_t* byte_do_arquivo,
                               uint8_t** dicionario,
                               long tamanho_do_arquivo)
{
    uint8_t byte_em_construcao = 0;
    long index_do_bit = 7;
    

    for(long i = 0; i < tamanho_do_arquivo; i++){
        
        for(long j = 0; dicionario[byte_do_arquivo[i]][j] != '\0'; j++){

            if(dicionario[byte_do_arquivo[i]][j] == '1'){
                byte_em_construcao = byte_em_construcao | 1 << index_do_bit;
            }
            index_do_bit = index_do_bit -  1;
            

            if(index_do_bit < 0){
                fwrite(&byte_em_construcao,sizeof(unsigned char),1,arquivo_comprimido);
                printf("[1]\n");
                index_do_bit = 7;
                byte_em_construcao = 0;
            }
        }
    }
    //se tem lixo <=> ainda terminou de percorrer pelos dados, mas ainda ainda não formou o byte completo =: insira-o
    if(index_do_bit != 7){
        fwrite(&byte_em_construcao,sizeof(unsigned char),1,arquivo_comprimido);
        printf("[1]\n");
    }

};



long tamanho_da_arvore(COMPRESS_TREE *arvore){
    int por_caractere_de_escape = 0; // diz a folha eh um caractere * ou //

    if(arvore == NULL){
        return 0;
    }

    uint8_t byte_da_folha = *((uint8_t*)arvore->byte);

    //CONFERE SE EH UMA FOLHA
    if(arvore->esquerda == NULL && arvore->direita == NULL){
        por_caractere_de_escape = byte_da_folha == '*' || byte_da_folha == '\\'; 
    }

    return 1 + por_caractere_de_escape + tamanho_da_arvore(arvore->esquerda) + tamanho_da_arvore(arvore->direita);

}


void calcular_lixo(long *tamanho_de_lixo,
                   uint8_t** dicionario,
                   long* frequencia_do_byte)
{
    long bits_sem_compac = 0;
    long bits_COM_compac = 0;

    for(int i = 0; i < MAX_TABLE_SIZE; i++){
        bits_sem_compac += frequencia_do_byte[i] * 8; //1 byte
        bits_COM_compac += frequencia_do_byte[i] * strlen(dicionario[i]);
    } 
    *tamanho_de_lixo = (bits_sem_compac - bits_COM_compac) % 8;
    printf("Tamanho antes: %ld bits\nTamanho depois: %ld bits\n", bits_sem_compac,bits_COM_compac);
    printf("Diff %ld bits\nLixo %d bit\n",bits_sem_compac - bits_COM_compac, *tamanho_de_lixo);
}


void imprimir_fila(COMPRESS_TREE* fila_de_prioridade){
    while (fila_de_prioridade != NULL)
    {
        printf("%d %ld\n",*((uint8_t*)fila_de_prioridade->byte),fila_de_prioridade->frequencia_do_byte);

        fila_de_prioridade = fila_de_prioridade->next;
    }
};

void imprimir_em_pre_ordem(COMPRESS_TREE* arvore_huffman){
    if(arvore_huffman != NULL){

        if(*((uint8_t*)arvore_huffman->byte) == '*' && arvore_huffman->esquerda != NULL && arvore_huffman->direita != NULL){
            printf("* ");
        }else{
            printf("%c ",*((uint8_t*)arvore_huffman->byte));
        }
        printf("%ld\n",arvore_huffman->frequencia_do_byte);
        imprimir_em_pre_ordem(arvore_huffman->esquerda);
        imprimir_em_pre_ordem(arvore_huffman->direita);
    }
};

COMPRESS_TREE* criar_fila_de_prioridade(){
    return NULL;
}

void comprimir_arquivo(char *nome_do_arquivo){

    COMPRESS_TREE* fila_de_prioridade = criar_fila_de_prioridade();

    FILE *arquivo = fopen(nome_do_arquivo,"rb");

    FILE *arquivo_comprimido;

    if(arquivo == NULL){
        printf("--FALHA AO ABRIR O ARQUIVO--\n");
        exit(1);
    }

    /*
    uint8_t numb = 9;
    printf("%d\n",sizeof(uint8_t));
    printf("%ld\n",(long)numb);
    printf("%d\n",sizeof(numb));
    */
    
    /**
    * 1) Pegando o tamanho do aqruivo
    *
    */
    fseek(arquivo,0,SEEK_END); //*stream, offset, origin

    long int tamanho_do_arquivo = ftell(arquivo);

    printf("tamanho do arquivo: %ld bytes\n",tamanho_do_arquivo);

    fseek(arquivo,0,SEEK_SET);
    printf("%d\n",ftell(arquivo));
    
    /**
    * 2) Pegar a frequencia dos bytes do arquivo
    *
    */
    uint8_t *bytes_do_arquivo;
    uint8_t ind = 0;
    //uint8_t *indice = 0; 
    long frequencia_do_byte[MAX_TABLE_SIZE]; 
    long count_byte = 0;

    bytes_do_arquivo = (uint8_t*) malloc(tamanho_do_arquivo);

    if(bytes_do_arquivo == NULL){
        printf("--FALHA NA ALOCACAO DE MEMORIA--\n");
        exit(1);
    }

    //inicializando os array em  0
    memset(bytes_do_arquivo,0,tamanho_do_arquivo); //*ptr, value, num 
    memset(frequencia_do_byte,0,MAX_TABLE_SIZE*sizeof(long)); //*ptr, value, num 

    //pega os bytes do arquivo
    fread(bytes_do_arquivo,1, tamanho_do_arquivo, arquivo);

    fclose(arquivo);


    printf("ANALISANDO A FREQUENCIA DOS BYTES...\n");

    while(count_byte < tamanho_do_arquivo){
        //uint8_t
        frequencia_do_byte[(long) bytes_do_arquivo[ind]] += 1;
        //printf("%d\n", sizeof(bytes_do_arquivo[ind]));
        //printf("%d %ld -\n",bytes_do_arquivo[ind],frequencia_do_byte[(long) bytes_do_arquivo[ind]]);
        count_byte++;
        ind++; 
    }
    count_byte = 0;
    ind = 0;

    for(long i = 0; i < MAX_TABLE_SIZE; i++){
        
        //printf("%d",ind);
        if(frequencia_do_byte[i] != 0){
            //printf("%d",ind);
            //printf(" %ld\n",frequencia_do_byte[i]);
        }else{
            //printf("\n");
        }
        
        
        if(frequencia_do_byte[i] != 0){
            COMPRESS_TREE *novo_node;

            //alocar_novo_no(novo_node,&ind,frequencia_do_byte[i]); 
            //-> NAO FUNCIONA COM 1 PONTEIRO SÓ
            //PQ? SE A FUCNAO RECBESSE *novo_node, Passado POR VALOR: na funcao recebria a copia de um ponteiro, e não o ponteiro em si
            ///PARA PASSAR POR REFERENCIA - PRECISO DO ENDERECO DE 1 PONTEIRO, QUE É GUARDADAO NUM PONTEIRO DUPLO
            
            alocar_novo_no_em(&novo_node,&ind,frequencia_do_byte[i]);

            //printf("%d %ld\n",*((uint8_t*)novo_node->byte),novo_node->frequencia_do_byte);

            enqueue(&fila_de_prioridade,novo_node);
        }
        ind++;
    }

    /*
    while (fila_de_prioridade != NULL)
    {
        printf("%d %ld\n",*((uint8_t*)fila_de_prioridade->byte),fila_de_prioridade->frequencia_do_byte);

        fila_de_prioridade = fila_de_prioridade->next;
    }*/
    imprimir_fila(fila_de_prioridade);

    /*
    3) MONTAR ARVORE DE HUFFMAN
    */

   printf("MONTANDO A ARVORE DE HUFFMAN...\n");

   COMPRESS_TREE *arvore_huffman = NULL;

   criar_arvore_de_huffman(&arvore_huffman,&fila_de_prioridade);

   imprimir_em_pre_ordem(arvore_huffman);

    /*
    4) MONTAR DICIONARIO COM NOVA CODIFICACAO
    */
   long altura_da_arvore;
   long fim_de_string = 1;
   

   altura_da_arvore = calcular_altura(arvore_huffman);
   
   printf("MONTANDO DICIONARIO COM NOVA CODIFICACAO....\n");

   printf("altura %ld\n",altura_da_arvore);

   long tamanho_max = altura_da_arvore + fim_de_string;
   
   uint8_t **dicionario = criar_dicionario(tamanho_max);

   //criar_dicionario_COM_VOID(&dicionario,tamanho_max); COMO CRIAR ISSO?

   uint8_t *aux_dicionario = (uint8_t*) malloc(sizeof(uint8_t)*tamanho_max);

   if(aux_dicionario == NULL){
    printf("FALHA NA ALOCACAO DE MEMORIA (Array auxiliar)");
    exit(1);
   }

   preencher_dicionario(dicionario, aux_dicionario, 0, arvore_huffman);

   for(int i = 0; i < MAX_TABLE_SIZE; i++){
    if(dicionario[i][0] != '\0'){
        if(i == 42 || i == 92){
            printf("\\");
        }
        printf("%c %s\n",i,dicionario[i]);
    }
   }

   /*
    5) CALCULAR LIXO e TAMAMANHO DA ARVORE
    */

   long tamanho_do_lixo, qtd_elementos_arvore;

   calcular_lixo(&tamanho_do_lixo,dicionario,frequencia_do_byte);

   tamanho_do_lixo = tamanho_do_lixo % 8;

   printf("%ld bit\n",tamanho_do_lixo);

   qtd_elementos_arvore = tamanho_da_arvore(arvore_huffman);

   printf("tamanho da arvore (+ caractres de escape): %ld\n",qtd_elementos_arvore);   


   /*
    6) ESCREVER ARQUIVO COMPRIMIDO
    */
   printf("INSERINDO DADOS EM ARQUIVO COMPRIMIDO...\n");

   arquivo_comprimido = fopen("teste.txt.huff", "wb");

   if (arquivo_comprimido == NULL)
   {
    printf("FALHA AO CRIAR ARQUIVO COMPRIMIDO");
    exit(1);
   }

   inserir_header_no_arquivo(arvore_huffman,tamanho_do_lixo,qtd_elementos_arvore,arquivo_comprimido);

   inserir_dados_comprimidos(arquivo_comprimido,bytes_do_arquivo,dicionario,tamanho_do_arquivo);

    //inserir_header_no_arquivo(arvore_huffman,5,256,arquivo_comprimido);
   fclose(arquivo_comprimido);
}
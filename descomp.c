#include <stdio.h>
#include <stdlib.h>

struct lista{
	unsigned char caractere;
	struct lista *next;
};
struct tree{
    unsigned char caractere;
    struct tree *left;
    struct tree *right;
};
struct hash_table{
    int h;
    unsigned short map;
};
typedef struct hash_table ht;

ht *caracteres[256];
typedef struct lista lista;
typedef struct tree tree;

void cria_ht(){
    int i;
    for(i = 0; i < 256; i++){
        caracteres[i] = NULL;
    }
}
unsigned short lixo(unsigned short c){
    c = c>>12;
    //printf("%d\n",c);
    return c;
}
unsigned short *arv_tam(unsigned short d){
    d = d << 4;
    d = d >> 4;
    unsigned short *c;
    c = &d;
    return c;
}
lista *add_node(lista *tail,unsigned char c){
    lista *new = malloc(sizeof(lista));
    new->caractere = c;
    new->next = NULL;
    if(tail != NULL) tail->next = new;
    return new;
}
tree *create_node(unsigned char c){
    tree *tree = malloc(sizeof(tree));
    tree->caractere = c;
    tree->left = NULL;
    tree->right = NULL;
}
/*tree *add_arv(lista *head){
    //printf(":( ");
    lista *aux = head->next;
    tree *node;
    unsigned char c = aux->caractere;
    if(aux == NULL) return NULL;
    //printf("%c\n",aux->caractere);
    if(aux->caractere != '*'){
        if(aux->caractere == '\\'){
            head->next = aux->next;
            free(aux);
            aux = head->next;    
        }
        node = create_node(aux);
        head->next = aux->next;
        free(aux);
    }
    else{
        node = create_node(aux);
        head->next = aux->next;
        free(aux);
        if(c == '*'){
            node->left = add_arv(head);
            node->right = add_arv(head);
        }
    }
    return node;
}*/
tree *add_arv(FILE *file,unsigned short *cont){
    //printf("contador = %d\n",*cont);
    unsigned short a = *cont;
    if(*cont == 0) return NULL;
    tree *node;
    unsigned char c;
    fscanf(file,"%c",&c);
    if(c != '*'){
        if(c == '\\'){
            fscanf(file,"%c",&c);
            a--;
            cont = &a;
        }
        node = create_node(c);
        a--;
        cont = &a;
    } else {
        node = create_node(c);
        a--;
        cont = &a;
        node->left = add_arv(file,cont);
        node->right = add_arv(file,cont);
    }
    return node;
}
void printar(tree *head){
	if(head != NULL){
		//printf("%c",head->caractere);
		printar(head->left);
		printar(head->right);
	}
}
int bit_is_set(unsigned char c, int i){
    unsigned char comp = 1;
    c = c>>i;
    return comp & c;
}
void map(lista *head,tree *raiz,short lixo){
    int cont = 8 - lixo,i;
    FILE *file;
    char c[100];
    printf("Digite o nome do novo arquivo: ");
    scanf("%s",c);
    file = fopen(c,"wb");
    fclose(file);
    tree *aux = raiz;
    while(head != NULL && file != NULL){
        for(i=7;i>=0;i--){
            //printf("cont = %d\n",i);
            if(bit_is_set(head->caractere,i)){
                //printf("1 ");
                if(aux->right != NULL)aux = aux->right;
                //printf("caractere %c\n", aux->caractere);
            } else {
                //printf("0 ");
                if(aux->left != NULL) aux = aux->left;
                //printf("caractere %c\n", aux->caractere);
            }
            if(aux->left == NULL && aux->right == NULL){
                //printf("hi3\n");
                file = fopen(c,"ab");
                fprintf(file,"%c",aux->caractere);
                fclose(file);
                //printf("%c\n",aux->caractere);
                aux = raiz;
            }
            if(head->next == NULL){
                cont --;
                if(cont == 0) break;
            }
        }
        //printf("\n");
        head = head->next;
    }
}
int main(){
    FILE *file;
    char s[100];
    unsigned char c;
    scanf(" %s",s);
    unsigned short trash, *tree_size,d,i;
    lista *tail = NULL, *byte_head = NULL;
    tree *raiz = NULL;

    file = fopen(s,"rb");
    if(file != NULL){
        fscanf(file,"%hd",&d);
        trash = lixo(d);
        tree_size = arv_tam(d);
    }
    raiz = add_arv(file,tree_size);
    //printar(raiz);
    while(fscanf(file,"%c",&c) != EOF){
        //printf("%c\n",c);
        tail = add_node(tail,c);
        if(byte_head == NULL) byte_head = tail;
    }
    fclose(file);
    map(byte_head,raiz,trash);
    return 0;
}
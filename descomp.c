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
    c = c>>5;
    unsigned short d;
    d = (unsigned short)c;
    return d;
}
unsigned short *arv_tam(unsigned char a,unsigned char b){
    a = a << 3;
    a = a >> 3;
    int  i;
    unsigned short d = (unsigned short)a;
    d = d<<7;
    d = d|b;
    unsigned short *c = &d;
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
tree *add_arv(FILE *file,unsigned short *cont){
    unsigned short a = *cont;
    if(a == 0) return NULL;
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
    printf("Digite o nome do novo arquivo: \n");
    scanf("%s",c);
    file = fopen(c,"wb");
    
    tree *aux = raiz;
    while(head != NULL){
        //printf("hi\n");
        for(i=7;i>=0;i--){
            if(bit_is_set(head->caractere,i)){
                aux = aux->right;
            } else {
                aux = aux->left;
            }
            if(aux->left == NULL && aux->right == NULL){
                fprintf(file,"%c",aux->caractere);
                aux = raiz;
            }
            if(head->next == NULL){
                cont --;
                if(cont == 0) break;
            }
        }
        head = head->next;
    }
    fclose(file);
}
int main(){
    FILE *file;
    char s[100];
    unsigned char a,b,c;
    scanf(" %s",s);
    unsigned short trash, *tree_size,d,i;
    lista *tail = NULL, *byte_head = NULL;
    tree *raiz = NULL;

    file = fopen(s,"rb");
    if(file != NULL){
        fscanf(file,"%c%c",&a,&b);
        trash = lixo(a);
        tree_size = arv_tam(a,b);
    }
    raiz = add_arv(file,tree_size);
    while(fscanf(file,"%c",&c) != EOF){
        tail = add_node(tail,c);
        if(byte_head == NULL) byte_head = tail;
    }
    fclose(file);
    map(byte_head,raiz,trash);
    return 0;
}
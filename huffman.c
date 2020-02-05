#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct huffman{
	unsigned char caractere;
	int frequencia;
	struct huffman *next;
	struct huffman *left;
	struct huffman *right;
}node;

typedef struct hash_table{
	int h;
	unsigned short map;
}ht;

ht *caracteres[256];

node *add(node *head, unsigned char caractere){
	node *aux = head, *new;
	while(aux != NULL && aux->caractere != caractere){
		aux = aux->next;
	}
	if(aux == NULL){
		new = malloc(sizeof(node));
		new->caractere = caractere;
		new->frequencia = 1;
		new->next = head;
		new->left = NULL;
		new->right = NULL;
		return new;
	} else {
		aux->frequencia++;
		return head;
	}
}
node *add_arvore(node *head){
	node *new_head = head->next->next, *new;
	new = malloc(sizeof(node));
	new->frequencia = head->frequencia + head->next->frequencia;
	new->left = head;
	new->right = head->next;
	new->left->next = NULL;
	new->right->next = NULL;
	new->caractere = '*';
	new->next = NULL;

	if(new_head == NULL) return new;
	else{
		node *aux = new_head, *anterior = NULL;
		while(aux != NULL && aux->frequencia < new->frequencia){
			anterior = aux;
			aux = aux->next;
		}
		if(anterior == NULL){
			new->next = new_head;
			return new;
		} else {
			anterior->next = new;
			new->next = aux;
			return new_head;
		}
	}
}
node *ordenar(node *head){
    if(head->next == NULL){
        return head;
    } else {
        node *menor = head, *aux = head, *anterior = NULL, *ant_min = NULL;
        while(aux != NULL){
            if(aux->frequencia < menor->frequencia){
                ant_min = anterior;
                menor = aux;
            }
            anterior = aux;
            aux = aux->next;
        }
        if(head != menor){
            ant_min->next = menor->next;
            menor->next = ordenar(head);
            return menor;
        } else {
            head->next = ordenar(head->next);
            return head;
        }
    }
}

void printar(node *head, FILE *new){
	if(head != NULL){
		if(head->right == NULL && head->left == NULL && (head->caractere == '*' || head->caractere == '\\')){
			fprintf(new, "\\");
		}
		fprintf(new,"%c",head->caractere);
		printar(head->left,new);
		printar(head->right,new);
	}
}
void cria_ht(){
	int i;
	for(i = 0; i < 256; i++){
		caracteres[i] = NULL;
	}
}
int map_arvore(node *head, int height, unsigned short direction, int cont){
    if(head == NULL) return 0;
	if(head != NULL){
		if(head->right == NULL && head->left == NULL && (head->caractere == '\\' || head->caractere == '*')) cont++;
		if(head->right == NULL && head->left == NULL){
			int i = (int)head->caractere;
			caracteres[i] = malloc(sizeof(ht));
			caracteres[i]->h = height;
			caracteres[i]->map = direction;
		}
		cont = 1 + map_arvore(head->left, height+1, direction<<1, cont);
		cont = cont + map_arvore(head->right, height+1, (direction<<1)+1, cont);
	}
    return cont;
}

void create_file_ht(){
	int i;
	FILE *file;
	file = fopen("hash.txt", "wb");
	for(i = 0; i<256; i++){
		if(caracteres[i] != NULL){
			char c = (char)i;
			fprintf(file,"%c %d %d\n",i,caracteres[i]->h,caracteres[i]->map);
		}
	}
	fclose(file);
}
struct new_map{
    unsigned char byte;
    struct new_map *next;
};
typedef struct new_map map;

int bit_is_set(unsigned short c, int i){
    unsigned char comp = 1;
    c = c>>i;
    return comp & c;
}
map *add_byte(map *tail){
    map *new = malloc(sizeof(map));
    tail->next = new;
    new->next = NULL;
    return new;
}
unsigned char set_bit(unsigned char c){
	unsigned char comp = 1;
    c = c<<1;
    return c|comp;
}
int map_b(char s[], map *map_head){
    FILE *file = fopen(s,"rb");

    map *tail = map_head;

    int cont = 0, pos, h, i;

    unsigned char c;
    while(fscanf(file,"%c", &c) != EOF){
        pos = (int)c;
        h = caracteres[pos]->h;
        for(i = h-1 ; i >= 0 ; i--){
			if(cont == 8){
				tail = add_byte(tail);
				cont = 0;
			}
			if(bit_is_set(caracteres[pos]->map,i)){
				tail->byte = set_bit(tail->byte);
			} else {
				tail->byte = tail->byte<<1;
			}
			cont ++;
		}
    }
	tail->byte = tail->byte<<(8-cont);
    fclose(file);
    return 8-cont;
}
unsigned short trash(unsigned short lixo){
	lixo = lixo<<12;
	return lixo;
}
unsigned short tree(unsigned short inf, unsigned short tam){
	return inf|tam;
}
int main(){
	unsigned char caractere;
    char s[100];
	node *head = NULL;
	unsigned short tam_arv, lixo;
	map *map_head = malloc(sizeof(map));
	map_head->next = NULL;

    FILE *file,*new;
    scanf(" %[^\n]",s);
    file = fopen(s,"rb");

	if(file != NULL){
        while(fscanf(file,"%c",&caractere) != EOF){
            head = add(head,caractere);
        }
    }
    fclose(file);
	head = ordenar(head);
    
	while(head->next != NULL){
            head = add_arvore(head);
    }
    cria_ht();

    tam_arv = map_arvore(head,0,0,0);
	lixo = map_b(s,map_head);
	char n[100];
	printf("Digite o nome do arquivo.huff:\n");
	scanf(" %s",n);
	new = fopen(n,"wb");

	if(new != NULL){
		unsigned short info = trash(lixo);
		info = tree(info,tam_arv);
		unsigned char first_byte, second_byte;
		first_byte = (unsigned char)(info>>7);
		second_byte = (unsigned char)info;
		fprintf(file,"%c%c",first_byte,second_byte);
		printar(head, new);
		while(map_head != NULL){
			fprintf(file,"%c",map_head->byte);
			map_head = map_head->next;
		}
	}
    fclose(new);
	return 0;
}
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

struct tnode {
	char* word;
  int freq;
	struct tnode* left;
	struct tnode* right;
};
typedef struct tnode tnode;
struct snode{
  char* word;
  int freq;
  struct snode* next;
};
typedef struct snode snode;
struct slist{
  snode* head;
  snode* tail;
  size_t size;
};
typedef struct slist slist;
tnode* tree_create();
void treeprint(tnode*);
char* getword();
int getch(void);
void ungetch(int);
tnode* talloc(void);
snode* snode_create(char*, snode*);
slist* slist_create(void);
bool slist_empty(slist*);
size_t slist_size(slist*);
void slist_pushfront(slist*, char*);
bool search_slist(slist*, char*, snode* );
void slist_print(snode*);


slist* list;
char buf[BUFSIZ];
int bufp = 0;

int main(void){
	tnode *root;
	char *word;

  list = slist_create();
	root = NULL;
	while ((word = getword()) != NULL)
		if (isalpha(word[0]))
      slist_pushfront(list, word);
  root = tree_create(root, list->head);
	treeprint(root);
  slist_print(list->head);
	return 0;
}

tnode* tree_create(tnode* p, snode* curr){
  if(curr == NULL){ return p;}
  else if (p == NULL) {
    p = talloc();
    p->word = strdup(curr->word);
    p->freq = curr->freq;
    p->left = p->right = NULL;
  }
  else if(p->freq >= curr->freq){p->right = tree_create(p->right, curr->next);}
  else{p->left = tree_create(p->left, curr->next);}
return p;
}
void slist_print(snode* curr){
  printf("%dx: [%s]\n", curr->freq, curr->word);
  slist_print(curr->next);
}
void treeprint(tnode* p){
  int count = 0;
	if (p != NULL) {
    printf("%d time\n", count);
		treeprint(p->left);
		printf("Number of occurences %d : [%s]\n", p->freq, p->word);
		treeprint(p->right);
	}
}

char* getword(){
	static char word[100];
	int c, lim = 100;
	char* w = word;

	while (isspace(c = getch()) && c != '\n');
	if (c != EOF)
		*w++ = tolower(c);
	if (isalpha(c)) {
	while(--lim > 1){
			if (!isalnum(*w = tolower(getch())) && *w != '_') {
				ungetch(*w);
				break;
			}
      w++;
    }
	}
	*w = '\0';
	if (c == EOF)
		return NULL;
	else
		return word;
}
int getch(void){
  return(bufp > 0) ? buf[--bufp] : getchar();
}

void ungetch(int c){
    if(bufp >= BUFSIZ){ fprintf(stderr, "ungetch: Stack Overflow");}
    else{ buf[bufp++] = c;}
}

tnode* talloc(void){
	return (tnode*) malloc(sizeof(tnode));
}

snode* snode_create(char* word, snode* next){
  snode* p = (snode*)malloc(sizeof(snode));
  p->word = word;
  p->freq = 1;
  p->next = next;
  return p;
}

slist* slist_create(){
  slist* p = (slist*)malloc(sizeof(slist));
  p->head = NULL;
  p->tail = NULL;
  p->size = 0;
  return p;
}

bool slist_empty(slist* list){
  if(list->head == NULL){return true;}
return false;
}

size_t slist_size(slist* list){
  return list->size;
}

void slist_pushfront(slist* list, char* word){
  if(search_slist(list, word, list->head)){ return;}
  else{
  snode* no = snode_create(word, list->head);
  no->next = list->head;
  list->head = no;
  if(list->size == 0){list->head = no;}
  ++list->size;
  }
}

bool search_slist(slist* list, char* word, snode* curr){
  if(curr == NULL){ return false;}
  if(strcmp(curr->word, word) == 0){
    curr->freq++;
    return true;
  }
  search_slist(list, word, curr->next);
  return false;
}

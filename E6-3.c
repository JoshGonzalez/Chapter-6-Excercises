#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

#define MAXLINES 10           /* max # of lines to count for each word*/
static char* noises[] = {"a", "an", "and", "are", "for", "from", "in", "is", "it", "not", "of", "on", "or", "that", "the", "this", "to", "was", "with"};
struct tnode {
	char *word;
	struct tnode *left;
	struct tnode *right;
	unsigned lines[MAXLINES];
};
typedef struct tnode tnode;
tnode* addtree(tnode*, char*, int);
void treeprint(tnode*);
char* getword();
int noise_check(char*);
int getch(void);
void ungetch(int);
tnode* talloc(void);
char buf[BUFSIZ];
int bufp = 0;

int main(void){
	tnode *root;
	char *word;
	unsigned int line = 1;

	root = NULL;
	while ((word = getword()) != NULL)
		if (isalpha(word[0]) && !noise_check(word))
			root = addtree(root, word, line);
		else if (word[0] == '\n')
			line++;
	treeprint(root);
	return 0;
}

int noise_check(char* word){
	int cond;
	int low, high, mid;

	if (word[1] == '\0'){return 1;}
	low = 0;
	high = sizeof(noises) / sizeof(char *) - 1;
	while (low <= high) {
		mid = (low + high) / 2;
		if ((cond = strcmp(word, noises[mid])) < 0) {high = mid - 1;}
		else if (cond > 0) {low = mid + 1;}
		else {return 1;}
	}
	return 0;
}

tnode* addtree(tnode* p, char* w, int line){
	int cond;
	int i;

	if (p == NULL) {
		p = talloc();
		p->word = strdup(w);
		p->lines[0] = line;
		for (i = 1; i < MAXLINES; i++)
			p->lines[i] = 0;
		p->left = p->right = NULL;
	}
	else if ((cond = strcmp(w, p->word)) == 0) {
		for (i = 0; p->lines[i] && i < MAXLINES; i++);
		p->lines[i] = line;
	}
	else if (cond < 0) {p->left = addtree(p->left, w, line);}
	else if (cond > 0) {p->right = addtree(p->right, w, line);}
	return p;
}

void treeprint(tnode* p){
	int i;

	if (p != NULL) {
		treeprint(p->left);
		printf("%-16s", p->word);
    putchar('[');
		for (i = 0; i < MAXLINES && p->lines[i]; i++)
			printf("%s%d", i==0 ? "" : ", ", p->lines[i]);
    putchar(']');
		putchar('\n');
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

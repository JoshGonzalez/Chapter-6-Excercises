#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define IN 1
#define OUT 0
#define DEF_LIM 6
#define MAXWORD 256

struct tnode{
  char* word;
  int count;
  struct tnode *left;
  struct tnode *right;
};
typedef struct tnode tnode;

char buf[BUFSIZ];
int bufp = 0;
void treeprint(tnode*);
tnode* addtree(tnode*, char*, int);
int getword (char*, int);
int getch(void);
void ungetch(int);
tnode* talloc(void);

int main(int argc, const char* argv[]){
  tnode* root;
  char word[MAXWORD];
  int lim;

  if(argc == 1)
		lim = DEF_LIM;
	else if(argc == 2)
		lim = atoi(argv[1]);
	else {
		fprintf(stderr, "Error too many arguments.\n");
		exit(1);
	}

  root = NULL;
  while (getword(word, MAXWORD) != EOF)
    if (isalpha(word[0])) {root = addtree(root, word, lim);}
  treeprint(root);
  return 0;
}

void treeprint(tnode* curr)
{
	if (curr != NULL) {
        	treeprint(curr->left);
		if (curr->count)
   			printf("%s: %d times appeared\n", curr-> word, curr->count);
        	treeprint(curr->right);
	}
}

tnode* addtree(tnode* p, char* w, int lim)
{
	int cond;

	if(p == NULL) { /* new word */
		p = talloc();
		p->word = strdup(w);
		p->count = 1;
		p->left = p->right = NULL;
	}
	else if((cond = strncmp(w, p->word, lim)) == 0)
		p->count++;
	else if(cond < 0)
		p->left = addtree(p->left, w, lim);
	else
		p->right = addtree(p->right, w, lim);
	return p;
}

int getword (char* word, int lim)
{
	int c, d, comment, str, dir;
	char* w = word;
  comment = OUT;
  str = OUT;
  dir = OUT;
	while (isspace(c = getch()));
	if (c == '/') {
		if ((d = getch()) == '*')
			comment = IN;
		else
			ungetch(d);
	}
	else if (c == '\"')
		str = IN;
	else if (c == '#') {
		if ((d = getch()) != '\'')
			dir = IN;
		ungetch(d);
	}
	else if (c == '\\'){c = getch();}
	if (comment == OUT && str == OUT && dir == OUT) {
		if (c != EOF) {*w++ = c;}
		if (!isalnum(c) && c != '_') {
			*w = '\0';
			return c;
		}
		while(--lim > 0) {
			if (!isalnum(*w = getch()) && *w != '_') {
				ungetch(*w);
				break;
			}
      w++;
    }
		*w = '\0';
		return word[0];
	}
	else if (comment == IN) {
		*w++ = c;
		*w++ = d;
		while ((*w++ = c = getch())) {
			if (c == '*') {
				if ((c = getch()) == '/') {
					*w++ = c;
					comment = OUT;
					break;
				}
				else
					ungetch(c);
			}
		}
		*w = '\0';
	}
	else if (str == IN) {
		*w++ = c;
		while ((*w++ = getch()) != '\"')
			if (*w == '\\')	{*w++ = getch();}
		str = OUT;
		*w = '\0';
	}
	else if (dir == IN) {
		*w++ = c;
		while ((*w++ = c = getch()) != '\n')
			if (c == '\\'){*w++ = getch();}
		dir = OUT;
		*w = '\0';
	}
	return c;
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

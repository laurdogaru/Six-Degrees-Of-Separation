#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct nod {
	int val;
	struct nod *leg;

} nod;

typedef struct graf {
	int size;
	nod **noduri;
	char **nume;
} graf;

typedef struct queue {
	nod *tail,*head;
	int len;
} queue;

/* functie pentru initializarea grafului*/
void init_graph(graf **g, int n) {
	*g = malloc(sizeof(graf));
	(*g)->size = n;
	(*g)->noduri = (nod**) malloc((n+1) * sizeof(nod*));
	(*g)->nume = (char**) malloc((n+1) * sizeof(char*));
}

/*functie pentru crearea arcului intre nodul x si nodul y*/
void add_arc(graf *g, int x, int y) {
	nod *nou = (nod*) malloc(sizeof(nod));
	nou->val = y;
	nou->leg = g->noduri[x];
	g->noduri[x] = nou;
}

/*functie care verifica daca exista arc intre nodul x si nodul y*/
int arc(graf *g, int x, int y) {
	nod *aux = g->noduri[x];
	int c = 0;
	while(aux != NULL) {
		if(aux->val == y) {
			c = 1;
			break;
		}	
		aux = aux->leg;
	}
	return c;
}

/*functie pentru marirea memoriei alocate pentru un graf*/
void marire(graf **g, int n) {
	(*g)->noduri = (nod**) realloc((*g)->noduri, n * sizeof(nod*));
	(*g)->nume = (char**) realloc((*g)->nume, n * sizeof(char*));
}

/*functie care returneaza un graf format din datele citite intr-un fisier
primit ca parametru*/
graf *create_graf(FILE *f, int *size) {
	int n, m, i, j, k, l, *indici_actori, contor, aux;
	long poz;
	/* variabila auxiliara, in care se citeste fiecare nume
	(am presupus ca niciun nume nu va depasi 100 de caractere)*/
	char *string = (char*) malloc(101 * sizeof(char));
	graf *g;
	/*initial, alocam grafului memorie pentru doua noduri*/
	init_graph(&g, 2);
	/*se citeste numarul de filme*/
	fscanf(f, "%d", &n);
	/*se trece la linia urmatoare*/
	fseek(f, 1, 1);

	l = 1;
	for(i = 1; i <= n; i++) {
		contor = 0;
		/*se citeste numele filmului, pentru a trece pe urmatoarea linie*/
		fgets(string, 100, f);
		/*se citeste numarul de actori din film*/
		fscanf(f, "%d", &m);
		fseek(f, 1, 1);
		indici_actori = (int*) calloc(m + 1, sizeof(int));
		/* se memoreaza pozitia de unde se incepe citirea actorilor dintr-un
		film */
		poz = ftell(f);
		for(j = 1; j <= m; j++) {
			aux = 0;
			/*citirea unui nume*/
			fgets(string, 100, f);
			/*se verifica daca numele exista deja in graf*/
			for(k = 1; k < l; k++) {
				if(strcmp(g->nume[k], string) == 0) {
					aux = 1;
					break;
				}
			}
			/*daca numele nu este deja in graf, se adauga*/
			if(aux == 0) {
				/* daca se ajunge la capacitatea maxima, se dubleaza memoria
				alocata pentru graf*/
				if(l == g->size) {
					marire(&g, l*2);
					g->size = l*2;
				}
				/*se aloca campului din graf cata memorie trebuie pentru
				numele respectiv*/
				g->nume[l] = (char*) malloc((strlen(string) + 1) * sizeof(char));
				strcpy(g->nume[l], string);
				l++;
			}
		}
		/*cursorul se intoarce la inceputul listei de actori din filmul 
		respectiv*/
		fseek(f, poz, 0);
		/* se citesc iar actorii din filmul respectiv, dupa se parcurge tot
		graful pana in acel punct si se memoreaza in "indici_actori" pe ce
		pozitii apar numele in graf*/
		for(j = 1; j <=m; j++) {
			fgets(string, 100, f);
			for(k = 1; k < l; k++) {
				if(strcmp(g->nume[k], string) == 0) {
					indici_actori[contor] = k;
					contor++;
				}
			}

		}
		/* in caz ca nu exista deja arc intre nodurile care reprezinta doi
		actori, se creeaza unul*/
		for(j = 0; j < m; j++) {
			for(k = 0; k < m; k++ ) {
				if(indici_actori[k] != indici_actori[j] && 
					arc(g, indici_actori[j], indici_actori[k] ) == 0) {
					add_arc(g, indici_actori[j], indici_actori[k]);
					add_arc(g, indici_actori[k], indici_actori[j]);
				}
			}
		}
		free(indici_actori);
	}
	free(string);
	*size = l-1;
	return g;
}

/* parcurgere in adancime pornind din nodul "v" */
void dfs(graf *g, int v, int *vazut,int *contor) {
	vazut[v] = 1;
	nod *p;
	p = g->noduri[v];
	while(p != NULL) {
		if(vazut[p->val] == 0) {
			/* contor se incrementeaza la fiecare nod gasit*/
			*contor = *contor + 1;
			dfs(g, p->val, vazut, contor);
		}
		p = p->leg;
	}
}

/* real_size este marimea reala a grafului, care este mai mica sau egala cu
capacitatea acestuia*/
void df(graf *g ,int real_size, int *inceput, int *mx) {
	int *vazut = (int*) calloc(g->size + 1, sizeof(int));
	int i,contor;
	*mx = 0;
	/* practic, se fac parcurgeri in adancime pana sunt vizitate toate
	nodurile, in "mx* se va retine productia maxima, si in "inceput"
	se va retine de unde incepe*/
	for(i = 1; i <= real_size; i++) {
		if(vazut[i] == 0) {
			contor = 1;
			dfs(g, i, vazut,&contor);
			if(contor > *mx) {
				*mx = contor;
				*inceput = i;
			}
		}
	}
	free(vazut);
}

int pozitie;

void dfs2(graf *g, int v, int *vazut, int *indici) {
	vazut[v] = 1;
	nod *p;
	p = g->noduri[v];
	while(p != NULL) {
		if(vazut[p->val] == 0) {
			indici[pozitie] = p->val;
			pozitie++;
			dfs2(g, p->val, vazut, indici);
		}
		p = p->leg;
	}
}

/*din nodul cu indicele "inceput" obtinut mai sus se face o alta parcurgere
in adancime si se retin toti indicii actorilor din aceeasi productie*/
int *indici_comp_max(graf *g, int inceput, int mx) {
	pozitie = 1;
	int *indici = (int*) calloc(mx + 1, sizeof(int));
	int *vazut = (int*) calloc(g->size + 1, sizeof(int));
	indici[0] = inceput; 
	dfs2(g, inceput, vazut, indici);
	free(vazut);
	return indici;
}

/* functie pe care o va primi qsort ca argument, care compara doua siruri de
caractere */
int cmp(const void *a, const void *b) {
	char **a1 = (char**) a;
	char **b1 = (char**) b;
	return strcmp(*a1, *b1);
}

/* toate numele de actori din productia maxima se vor introduce intr-o matrice
de caractere, care va fi sortata si afisata, impreuna cu nr. de actori din
productia respectiva*/
void lista_nume(graf *g, int *indici, int mx, FILE *f) {
	char **lista,*nume;
	lista = (char**) malloc(mx * sizeof(char*));
	int i;
	for(i = 0 ; i < mx ; i++) {
		nume = g->nume[indici[i]];
		lista[i] = strdup(nume);
	}
	qsort(lista, mx, sizeof(*lista), cmp);
	fprintf(f, "%d\n", mx);
	for(i = 0; i < mx; i++) {
		fprintf(f, "%s", lista[i]);
	}
	for(i = 0; i < mx; i++) {
		free(lista[i]);
	}
	free(lista);
}

/* functie care intoarce indicele unui actor in graf*/
int find(char *actor, graf *g,int size, int length) {
	int i;
	for(i = 1; i <= size; i++) {
		if(strncmp(g->nume[i], actor, length) == 0) {
			return i;
		}
	}
	return 0;
}

/* initializare coada*/
void init_queue(queue **q) {
	*q = malloc(sizeof(queue));
	(*q)->len = 0;
	(*q)->tail = NULL;
	(*q)->head = NULL;
}

/* introducere in coada*/
void enqueue(queue *q, int val) {
	nod *aux;
	aux = (nod *)malloc(sizeof(nod));
	aux->val = val;
	aux->leg = NULL;
	if( q->head == NULL ) {
		q->head = q->tail = aux;
	}
	else {
		q->tail->leg = aux;
		q->tail = aux;
	}
	q->len++;
}	

/* scoatere din coada*/
int dequeue(queue *q) {
	int a;
	nod *aux;
	if(q->head == NULL) {
		return 0;
	}
	a = q->head->val;
	aux = q->head;
	q->head = q->head->leg;
	free(aux);
	q->len--;
	return a;
}

/* parcurgere in latime pornind din nodul v, iar in vectorul d se va retine
distanta de la v la toate celelalte noduri din graf*/
void bfs(graf *g, int v, int *vazut, int *d) {
	queue *q;
	nod *p;
	int x,y;
	init_queue(&q);
	enqueue(q, v);
	d[v] = 0;
	while(q->len > 0) {
		x = dequeue(q);
		if( !x ) {
			return;
		}
		vazut[x] = 1;
		p = g->noduri[x];
		while(p != NULL) {
			if(vazut[p->val] == 0) {
				y = p->val;
				vazut[y] = 1;
				enqueue(q, y);
				d[y] = d[x] + 1;
			}
			p = p->leg;
		}
	}
}

/* returneaza distanta dintre actorii cu indicii a1 si a2*/
int cerinta2(graf *g, int a1, int a2, FILE *f, int size) {
	int *vazut = (int*) malloc( (size + 1) * sizeof(int) );
	int *d = (int*) calloc( g->size + 1, sizeof(int) );
	bfs(g, a1, vazut, d);
	return d[a2];
}



int main(int argc, char **argv){
	int i, size, i1, i2, l1, l2;
	FILE *f1 = fopen(argv[2], "rt");
	FILE *f2 = fopen(argv[3], "wt");
	graf *g = create_graf(f1, &size);
	int inceput, mx, *indici;


	/* cerinta 1 */
	if(argv[1][2] == '1') {
		df(g, size, &inceput, &mx);
		indici = indici_comp_max(g, inceput, mx);
		lista_nume(g, indici, mx, f2);
		free(indici);
	}
	/* cerinta 2 */
	if(argv[1][2] == '2') {
		char *actor1,*actor2;
		actor1 = (char*) malloc(100 * sizeof(char));
		actor2 = (char*) malloc(100 * sizeof(char));

		fgets(actor1, 100, f1);
		fgets(actor2, 100, f1);
		i = strlen(actor1);
		/*am "eliminat" ultimul caracter pentru a gasi fix numele, fara \n*/
		actor1[i-1] = '\0';
		l1 = strlen(actor1);
		l2 = strlen(actor2);


		i1 = find(actor1, g, size, l1);
		i2 = find(actor2, g, size, l2);

		i = cerinta2(g, i1, i2, f2, size);
		if(i == 0) {
			fprintf(f2, "%d\n", -1);
		} else {
			fprintf(f2, "%d\n", i);
		}
		free(actor1);
		free(actor2);
	}

	/*eliberarea memoriei*/
	nod *p;
	for(i = 0 ; i < g->size ; i++) {
		while(g->noduri[i] != NULL) {
			p = g->noduri[i];
			g->noduri[i] = g->noduri[i]->leg;
			free(p);
		}
		free(g->noduri[i]);
		free(g->nume[i]);
	}
	free(g->nume);
	free(g->noduri);
	free(g);

	fclose(f1);
	fclose(f2);
	return 0;
}

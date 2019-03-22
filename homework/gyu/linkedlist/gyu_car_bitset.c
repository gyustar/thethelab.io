//형규

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define list_entry(ptr, type, member) \
		(type*)((char*)ptr - (unsigned long)&(((type*)0)->member))

struct node {
	struct node *prev;
	struct node *next;
};
struct node head = {&head, &head};

void __add_node(struct node *prev, struct node* next, struct node* new) {
	new->next = next;
	new->prev = prev;
	prev->next = new;
	next->prev = new;
}

void add_next(struct node *s, struct node* new) {
	__add_node(s, s->next, new);
}

void add_front(struct node *s, struct node* new) {
	__add_node(s->prev, s, new);
}

//CellDx	Sports Car	SUV	Wagon	Minivan	Pickup	AWD	RWD	Retail Price	Dealer Cost	EngineSize	Weight	Width

enum kind {
	CELLDX,
	SPORTSCAR,
	SUV,
	WAGON,
	MINIVAN,
	PICKUP,
	AWD,
	RWD,
	MAX = 8
};

typedef struct {
	unsigned int kind_bit[(MAX/(sizeof(unsigned int)*8)) + 1];
} Bitset_kind;

#define PUT_KIND(bitset, kind) \
	(bitset)->kind_bit[kind/32] |= 1 << (8 - ((kind) % 32))
#define CHECK_KIND(bitset, kind) \
	(bitset)->kind_bit[kind/32] & 1 << (8 - ((kind) % 32))
struct car {
	char *name;
	Bitset_kind kind;
	int retail;
	int dealer;
	double engine;
	int weight;
	int width;
	struct node links;
};
struct car *car;

struct firstline {
	char *str;
}firstline[14];

void read_firstline(FILE *fp) {
	int i=0;
	char buf[512];
	fgets(buf, sizeof(buf), fp);
	char *p = strtok(buf, ",");
	while(p != NULL) {
		firstline[i].str = strdup(p);
		p = strtok(NULL, ",");
		i++;
	}
}

void read_file(FILE *fp, int *c) {
	int i = 0, j = 0; 
	char buf[512];
	while(fgets(buf, sizeof(buf), fp)) {
		int len = strlen(buf);
		if (buf[len-1] == '\n') buf[len-1] = '\0';		
		j++;
		car = (struct car*)malloc(sizeof(struct car));
		if (car==NULL) printf ("malloc 실패");
		char *p = strtok(buf, ",");
		for(i = 0; p != NULL; p=strtok(NULL, ","),i++) {
			switch(i){
				case 0:
					car->name = strdup(p);
					break;
				case 1: case 2: case 3: case 4: case 5: case 6: case 7: case 8:
					if(atoi(p)) PUT_KIND(&car->kind, i);
					break;
				case 9:
					car->retail = atoi(p);
					break;
				case 10:
					car->dealer = atoi(p);
					break;
				case 11:
					car->engine = atof(p);
					break;
				case 12:
					car->weight = atoi(p);
					break;
				case 13:
					car->width = atoi(p);
					break;
			}
		}
		add_front(&head, &car->links);
	}
	*c = j;
}
void print_car_kind (struct car *car) {
	int i = 0;
	for (i = 0; i < 6; i++) {
		if (CHECK_KIND(&car->kind, (i+1))) printf ("%-11s", firstline[i+1].str);
	}
	if (car->kind.kind_bit[0] & 1 << 1) printf ("%-5s", firstline[7].str);
	else if (car->kind.kind_bit[0] & 1 << 0) printf ("%-5s", firstline[8].str);
	else printf ("NONE ");
}

int pivot_, sort_;
int compare (const void *a, const void *b) {
	struct car *p1 = *(struct car **)a;
	struct car *p2 = *(struct car **)b;
	int re=0;
	switch (pivot_-1) {
		case 0:
			re = sort_*(p1->retail - p2->retail);
			break;
		case 1:
			re = sort_*(p1->dealer - p2->dealer);
			break;
		case 2:
			if(p1->engine > p2->engine) re = sort_;
			else re = -1*sort_;
			break;
		case 3:
			re = sort_*(p1->weight - p2->weight);
			break;
		case 4:
			re = sort_*(p1->width - p2->width);
	}
	return re;
}

struct car *arr_save[397];
void make_sort (struct node *head, int kind, int pivot, int sort, int * sort_count) {
	int i=0, n=0;
	pivot_ = pivot;
	if(sort == 1) sort_ = 1;
	else if(sort == 2) sort_ = -1;
	struct node *current = head->next;
	struct car *p_arr[397];
	while (current != head) {
		struct car *car_p = list_entry(current, struct car, links);
		if (CHECK_KIND(&car_p->kind, kind)) {
			p_arr[n] = car_p;
			n++;
		}
		current = current->next;
	}
	*sort_count = n;
	qsort(p_arr,n,sizeof(struct car*),compare);
	for (i = 0; i < n; i++) arr_save[i] = p_arr[i];
	for (i = 0; i < n; i++) {
		printf ("%-45s  ", p_arr[i]->name);
	  print_car_kind(p_arr[i]);
	  printf ("%6d %6d %.1lf ", p_arr[i]->retail, p_arr[i]->dealer, p_arr[i]->engine);
	  printf ("%d %d\n", p_arr[i]->weight, p_arr[i]->width);
	}
}

void save_file (FILE *fp, int n) {
	int i = 0, j = 0;
	fprintf (fp, "%s,Type,WD,%s,%s,%s,%s,%s",
						firstline[0].str,
						firstline[9].str,
						firstline[10].str,
						firstline[11].str,
						firstline[12].str,
						firstline[13].str
						);
	while(i < n) {
		fprintf (fp, "%s,", arr_save[i]->name);

		for (j = 0; j < 6; j++) {
			if (CHECK_KIND(&arr_save[i]->kind, (j+1))) fprintf (fp, "%s,", firstline[j+1].str);
		}
		if (arr_save[i]->kind.kind_bit[0] & 1 << 1) fprintf (fp, "%s,", firstline[7].str);
		else if (arr_save[i]->kind.kind_bit[0] & 1 << 0) fprintf (fp, "%s,", firstline[8].str);
		else fprintf (fp, "NONE,");
	
		fprintf(fp, "%d,%d,%.1lf,%d,%d\n",
						arr_save[i]->retail,
						arr_save[i]->dealer,
						arr_save[i]->engine,
						arr_save[i]->weight,
						arr_save[i]->width
						);
		i++;
	}
}

int main () {
	FILE * fp;
	int count=0;

	fp = fopen("./car_data.csv", "r");
	if(fp == NULL) {
		printf ("파일 오픈 실패!");
		return -1;
	}

	read_firstline(fp);
	read_file(fp, &count);
	fclose(fp);
	printf ("%d개의 데이터가 로드되었습니다.\n\n", count);
	
	while(1) {
		int user_choice, kind_choice, pivot_choice, sort_choice, save;
		char * file_name;
		int i = 0, sort_count = 0;
		printf ("1. 검색\n2. 종료\n선택-> ");
		scanf ("%d", &user_choice);
		if (user_choice == 1) {
			printf ("\n\n");
			for (i = 0; i < 8; i++) printf ("%d. %s\n", i+1, firstline[i+1].str);
			printf ("선택-> ");
			scanf ("%d", &kind_choice);
			printf ("\n\n");
			for (i = 0; i < 5; i++) printf ("%d. %s\n", i+1, firstline[i+9].str);
			printf ("선택-> ");
			scanf ("%d", &pivot_choice);
			printf ("\n\n");
			printf ("1. 오름차순 정렬\n2. 내림차순 정렬\n선택-> ");
			scanf ("%d", &sort_choice);
			printf ("\n\n");

			make_sort(&head, kind_choice, pivot_choice, sort_choice, &sort_count);
			printf ("파일로 저장하시겠습니까?\n1. 예\n2. 아니오\n선택-> ");
			scanf ("%d", &save);
			if (save == 1) {
				printf ("\n파일이름-> ");
				file_name = (char *)malloc(sizeof(char)*30); 
				scanf ("%s", file_name);
				FILE * fp2 = fopen (file_name,"w");
				save_file(fp2, sort_count);
				fclose(fp2);
			}
			printf ("\n");
		}
		else if (user_choice == 2) break;
	}
	return 0;
}

#include <stdio.h>
#include <stdlib.h>

struct node {
	int data;
	struct node *prev;
	struct node *next;
};

struct node head = { 0, &head, &head };

// 아래처럼 만들면, 모든 노드는 동적 메모리를 통해서만 생성해야 한다.
// 지역 변수도 연결리스트로 엮고 싶고, 전역 변수도 연결리스트로 엮어 있어야 한다.
// => 메모리의 할당 로직을 삽입로직에서 분리해야 한다.
void __add_node(struct node *prev, struct node* next, struct node* new) {
	new->next = next;
	new->prev = prev;
	prev->next = new;
	next->prev = new;
}

// s, s->next, new
void add_next(struct node *s, struct node* new) {
	__add_node(s, s->next, new);
}

// s->prev, s, new
void add_front(struct node *s, struct node* new) {
	__add_node(s->prev, s, new);
}

#if 0
void add_next(struct node *s, struct node* new) {
	new->prev = s;
	new->next = s->next;
	s->next->prev = new;
	s->next = new;
}

void add_front(struct node *s, struct node* new) {
	new->prev = s->prev;
	new->next = s;
	s->prev->next = new;
	s->prev = new;
}
#endif

void print_list(struct node *head) {
	struct node *current = head->next;
	system("clear");

	printf("<head> ");
	// while (current) {
	while (current != head) {
	// while (1) {
		printf(" -> [%d]", current->data);
		current = current->next;
	}
	getchar();
}

void print_list2(struct node *head) {
	struct node *current = head->prev;
	system("clear");

	printf("<head> ");
	while (current != head) {
		printf(" -> [%d]", current->data);
		current = current->prev;
	}
	getchar();
}

struct node data[10];
int main() {
	int i;
	for (i = 0; i < 10; ++i) {
		data[i].data = i + 1;
		add_front(&head, data + i);
		print_list2(&head);
	}
}
































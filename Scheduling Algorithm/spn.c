#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>


typedef struct spn_queue* queue_pointer;
typedef struct spn_queue {
	int type;
	int process_id;
	int priority; 
	int computing_time;
	int enter_time;
	queue_pointer left;
	queue_pointer right;
}NODE; // spn_queue 구조체를 가리키는 포인터 변수

typedef struct queue_head* head_pointer; // spn_queue 의 첫번 째 노드를 가리키기 위한 포인터 변수
struct queue_head {
	queue_pointer right_link;
};


head_pointer head; // 큐의 첫 번째 노드를 가리키기 위한 전역변수 head 
int current_time = 0; // turn around time 을 측정하기 위한 전역변수
void insert_node(int, int, int, int); // process_id, priority, computing_time, enter_time 을 입력받기 위한 함수
void print_queue(void); // SPN 알고리즘에 따라 출력해주는 함수
void initialize_queue(void);
int delete_queue(void); // 스케쥴링 완료 후 완료 된 노드를 삭제하는 함수


int main(void) {
	int type, process_id, priority, computing_time, f_result;
	int enter_time = 0;
	int x;


	FILE* file = fopen("data.txt", "rt");
	if (NULL == file) {
		printf("파일 열기 실패\n");
	}

	initialize_queue(); // Head 초기화

	printf("Process_id\tpriority\tcomputing_time\tturn_around time\n");
	if (NULL != file) {
		while (1) {
			f_result = fscanf(file, "%d %d %d %d", &type, &process_id, &priority, &computing_time);

			if (type == -1) break;

			if (type == 0)
				insert_node(process_id, priority, computing_time, enter_time); // 삽입
			if (type == 1) {
				enter_time += 20; //  시간이 20이 되었음을 나타낸다.
				x = delete_queue(); // 스케쥴링 된 노드는 큐에서 삭제되고, 헤더는 다음 큐를 가리킨다.
				if (x == 1) return 0;
			}
		}
		print_queue();
		fclose(file);
	}
	return 0;
}

void insert_node(int process_id, int priority, int computing_time, int enter_time) {
	queue_pointer pointer, newNode; //  노드 삽입을 위한 변수

	newNode = (queue_pointer)malloc(sizeof(NODE)); // 동적으로 새로운 노드 할당
	newNode->left = NULL;
	newNode->right = NULL;
	newNode->priority = priority;
	newNode->computing_time = computing_time;
	newNode->enter_time = enter_time;
	newNode->process_id = process_id;

	// 큐가 비어있는 경우
	if (head->right_link == NULL) {
		head->right_link = newNode;
		return;
	}

	// 큐의 가장 끝에 삽입
	for (pointer = head->right_link; pointer != NULL; pointer = pointer->right) { 
		if (pointer->computing_time > computing_time) {
			if (pointer->left == NULL && pointer->computing_time > computing_time) // 맨 앞 
				break;
			else if (pointer->right == NULL && pointer->computing_time <= computing_time) // 맨 뒤
				break;
			pointer = pointer->left; // 중간에 노드를 삽입하기 위해 이전의 노드 주소를 지정한다.
			break;
		}
		else if (pointer->right == NULL)
			break;
	}

	if (pointer->computing_time > computing_time && pointer->left == NULL) { 
		pointer->left = newNode;
		head->right_link = newNode;
		newNode->right = pointer;
	}
	else if (pointer->right == NULL) { // 맨 뒤 삽입의 조건은 오른쪽 링크가 NULL 일 때 맨 마지막에 삽입
		pointer->right = newNode;
		newNode->left = pointer; // 맨 뒤에 삽입될 시, newNode 의 왼쪽링크는 포인터를 가리킨다.
		newNode->right = NULL; // 마지막의 오른쪽 링크는 NULL로 처리한다.
	}
	else { // 중간 노드에 삽입할 경우
		newNode->right = pointer->right;
		newNode->left = pointer;
		pointer->right->left = newNode;
		pointer->right = newNode;
		return;
	}
}

void print_queue(void) { // 스케쥴링 후 turn around time을 출력하고 끝난 시간을 return한다. 
	queue_pointer pointer;
	int turn_around_time;
	for (pointer = head->right_link; pointer != NULL; pointer = head->right_link) {
		current_time = current_time + pointer->computing_time; // 현재 스케쥴링 시간 + computing_time;

		turn_around_time = current_time - pointer->enter_time; 
		printf("%d\t\t%d\t\t%d\t\t%d\n", pointer->process_id, pointer->priority, pointer->computing_time, turn_around_time);

		if (pointer->right == NULL) {
			head->right_link = NULL;
			free(pointer);
		}
		else {
			head->right_link = pointer->right;
			pointer->right->left = NULL;
			pointer->right = NULL;
			free(pointer);
		}

	}
}

void initialize_queue(void) { // 초기화
	head = (head_pointer)malloc(sizeof(struct queue_head));
	head->right_link = NULL;
}


int delete_queue(void) {
	queue_pointer pointer;
	int turn_around_time;

	pointer = head->right_link;
	if (pointer == NULL) { // 큐에 노드가 존재하지 않는데 DELETE_QUEUE를 실행하는 경우 종료 처리
		printf("큐에 노드가 존재하지 않는데 스케줄링을 시도했습니다.\n");
		return 1;
	}
	current_time = current_time + pointer->computing_time; 
	turn_around_time = current_time - pointer->enter_time; 
	printf("%d\t\t%d\t\t%d\t\t%d\n", pointer->process_id, pointer->priority, pointer->computing_time, turn_around_time);
	if (pointer->right == NULL) {
		head->right_link = NULL;
		free(pointer);
		return 0;
	}
	else {
		head->right_link = pointer->right;
		pointer->right->left = NULL;
		free(pointer);
		return 0;
	}
}

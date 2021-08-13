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
}NODE; // spn_queue ����ü�� ����Ű�� ������ ����

typedef struct queue_head* head_pointer; // spn_queue �� ù�� ° ��带 ����Ű�� ���� ������ ����
struct queue_head {
	queue_pointer right_link;
};


head_pointer head; // ť�� ù ��° ��带 ����Ű�� ���� �������� head 
int current_time = 0; // turn around time �� �����ϱ� ���� ��������
void insert_node(int, int, int, int); // process_id, priority, computing_time, enter_time �� �Է¹ޱ� ���� �Լ�
void print_queue(void); // SPN �˰��� ���� ������ִ� �Լ�
void initialize_queue(void);
int delete_queue(void); // �����층 �Ϸ� �� �Ϸ� �� ��带 �����ϴ� �Լ�


int main(void) {
	int type, process_id, priority, computing_time, f_result;
	int enter_time = 0;
	int x;


	FILE* file = fopen("data.txt", "rt");
	if (NULL == file) {
		printf("���� ���� ����\n");
	}

	initialize_queue(); // Head �ʱ�ȭ

	printf("Process_id\tpriority\tcomputing_time\tturn_around time\n");
	if (NULL != file) {
		while (1) {
			f_result = fscanf(file, "%d %d %d %d", &type, &process_id, &priority, &computing_time);

			if (type == -1) break;

			if (type == 0)
				insert_node(process_id, priority, computing_time, enter_time); // ����
			if (type == 1) {
				enter_time += 20; //  �ð��� 20�� �Ǿ����� ��Ÿ����.
				x = delete_queue(); // �����층 �� ���� ť���� �����ǰ�, ����� ���� ť�� ����Ų��.
				if (x == 1) return 0;
			}
		}
		print_queue();
		fclose(file);
	}
	return 0;
}

void insert_node(int process_id, int priority, int computing_time, int enter_time) {
	queue_pointer pointer, newNode; //  ��� ������ ���� ����

	newNode = (queue_pointer)malloc(sizeof(NODE)); // �������� ���ο� ��� �Ҵ�
	newNode->left = NULL;
	newNode->right = NULL;
	newNode->priority = priority;
	newNode->computing_time = computing_time;
	newNode->enter_time = enter_time;
	newNode->process_id = process_id;

	// ť�� ����ִ� ���
	if (head->right_link == NULL) {
		head->right_link = newNode;
		return;
	}

	// ť�� ���� ���� ����
	for (pointer = head->right_link; pointer != NULL; pointer = pointer->right) { 
		if (pointer->computing_time > computing_time) {
			if (pointer->left == NULL && pointer->computing_time > computing_time) // �� �� 
				break;
			else if (pointer->right == NULL && pointer->computing_time <= computing_time) // �� ��
				break;
			pointer = pointer->left; // �߰��� ��带 �����ϱ� ���� ������ ��� �ּҸ� �����Ѵ�.
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
	else if (pointer->right == NULL) { // �� �� ������ ������ ������ ��ũ�� NULL �� �� �� �������� ����
		pointer->right = newNode;
		newNode->left = pointer; // �� �ڿ� ���Ե� ��, newNode �� ���ʸ�ũ�� �����͸� ����Ų��.
		newNode->right = NULL; // �������� ������ ��ũ�� NULL�� ó���Ѵ�.
	}
	else { // �߰� ��忡 ������ ���
		newNode->right = pointer->right;
		newNode->left = pointer;
		pointer->right->left = newNode;
		pointer->right = newNode;
		return;
	}
}

void print_queue(void) { // �����층 �� turn around time�� ����ϰ� ���� �ð��� return�Ѵ�. 
	queue_pointer pointer;
	int turn_around_time;
	for (pointer = head->right_link; pointer != NULL; pointer = head->right_link) {
		current_time = current_time + pointer->computing_time; // ���� �����층 �ð� + computing_time;

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

void initialize_queue(void) { // �ʱ�ȭ
	head = (head_pointer)malloc(sizeof(struct queue_head));
	head->right_link = NULL;
}


int delete_queue(void) {
	queue_pointer pointer;
	int turn_around_time;

	pointer = head->right_link;
	if (pointer == NULL) { // ť�� ��尡 �������� �ʴµ� DELETE_QUEUE�� �����ϴ� ��� ���� ó��
		printf("ť�� ��尡 �������� �ʴµ� �����ٸ��� �õ��߽��ϴ�.\n");
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

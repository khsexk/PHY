#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>

typedef struct RR_queue* queue_pointer;
struct RR_queue {
	int process_id;		// ���μ��� id
	int priority;		// �켱����
	int computing_time;	// ó���ð�
	int t_a_Time;		// turn-around time
	int arrival_Time;	// enter time
	queue_pointer left_link, right_link;
};

typedef struct queue_head* head_pointer;
struct queue_head {
	queue_pointer left_link;
	queue_pointer right_link;
};

head_pointer queue;	// Priority�� ������ �ʱ� ������ queue 1�� ����
int present_Time = 0;
int timeQuantum = 20;	// default time Quantum ����

int calTurn_AroundTime(int start, int finish) {
	return finish - start;
}

// init
void initQueue(void) {
	queue = (head_pointer)malloc(sizeof(struct queue_head));
	queue->left_link = NULL;
	queue->right_link = NULL;
}

// insert
int insert_queue(int process_id, int priority, int computing_time) {
	queue_pointer pointer, f_node;

	f_node = (queue_pointer)malloc(sizeof(struct RR_queue)); // ���ο� ��尪 ����
	f_node->left_link = NULL;
	f_node->right_link = NULL;
	f_node->process_id = process_id;
	f_node->priority = priority;
	f_node->computing_time = computing_time;
	f_node->arrival_Time = present_Time;

	// ť�� ����� ���(��尡 �������)
	if (queue->right_link == NULL) {
		queue->right_link = f_node;
		f_node->right_link = NULL;
		return 0;
	}

	// ť�� �ǵ� �˻�
	for (pointer = queue->right_link; pointer != NULL ; pointer=pointer->right_link) {
		if (pointer->right_link == NULL)
			break;
	}

	// �ǵڻ���
	pointer->right_link = f_node;
	f_node->left_link = pointer;
	f_node->right_link = NULL;

	return 0;
}

// Ÿ�������� ������ ��
int timeQuantum_queue() {
	queue_pointer pointer;
	queue_pointer last_pointer;

	if (queue->right_link == NULL) {
		printf("ó�� ���� ���μ����� �����ϴ�.\n");
		return 0;
	}

	// ť�� �ǵ� �˻�
	for (last_pointer = queue->right_link; last_pointer != NULL; last_pointer = last_pointer->right_link) {
		if (last_pointer->right_link == NULL)
			break;
	}

	pointer = queue->right_link;
	// ���μ����� computing_time�� timeQuantum���� �� ��
	if (pointer->computing_time > timeQuantum) {
		present_Time += timeQuantum;	// ���� �ð� ����

		printf("%d\t\t%d\t\t%d\t\t%s\n", pointer->process_id, pointer->priority, timeQuantum, "not Terminated");

		pointer->computing_time -= timeQuantum;	// computing_time���� ó���� �ð��� ����

		// ��尡 �ϳ� �̻��� �� �ش� ���μ����� �� ���������� �����ٸ�
		if (pointer->right_link != NULL) {
			// �� �տ��� ������
			queue->right_link = pointer->right_link;
			queue->right_link->left_link = NULL;
			pointer->right_link = NULL;
			// �� �ڿ� �ֱ�
			last_pointer->right_link = pointer;
			pointer->left_link = last_pointer;
		}
	}	// if_(ct > tQ)

	// ���μ����� computing_time�� timeQuantum�� ���� ��
	else if (pointer->computing_time == timeQuantum) {
		present_Time += timeQuantum;	// ���� �ð� ����

		pointer->t_a_Time = calTurn_AroundTime(pointer->arrival_Time, present_Time);

		printf("%d\t\t%d\t\t%d\t\t%d\n", pointer->process_id, pointer->priority, pointer->computing_time, pointer->t_a_Time);

		// ��尡 �ϳ��� ��
		if (pointer->right_link == NULL) {
			queue->right_link = NULL;
		}
		// ��尡 �ϳ� �̻��� �� 
		else {
			// �� �տ��� ������
			queue->right_link = pointer->right_link;
			queue->right_link->left_link = NULL;
		}
	}	// if_(ct == tQ)

	// ���μ����� computing_time�� timeQuantum���� ª�� ��
	else {
		int cnt = 0;	// ó���� ���μ��� ��
		int total_CT = 0;  // ó���� ���μ����� computing_time ��
		int same = 0;

		// �� ���� ���μ����� ó������ count
		for (pointer; pointer != NULL; pointer = pointer->right_link) {
			total_CT += pointer->computing_time;
			cnt++;

			if (total_CT > timeQuantum) {
				same = 0;
				break;
			}
			else if (total_CT <= timeQuantum) {
				same = 1;
				if (total_CT == timeQuantum)
					break;
			}
		}	// for_pointer

		total_CT = 0;
		// ���
		for (int i = 0; i < cnt; i++) {
			pointer = queue->right_link;
			// ���μ��� cnt���� computing_time ���� timeQuantum�� ���ų� ���� ���
			if (same == 1) {	
				present_Time += pointer->computing_time;
				pointer->t_a_Time = calTurn_AroundTime(pointer->arrival_Time, present_Time);
				printf("%d\t\t%d\t\t%d\t\t%d\n", pointer->process_id, pointer->priority, pointer->computing_time, pointer->t_a_Time);

				// ��尡 �ϳ� �� ��
				if (pointer->right_link == NULL) {
					queue->right_link = NULL;
				}
				// ��尡 �ϳ� �̻��� �� �� �տ��� ��
				else {
					queue->right_link = pointer->right_link;
					queue->right_link->left_link = NULL;
					pointer = queue->right_link;
				}
			}	// if_(same==1)

			// ���μ��� cnt���� computing_time ���� timeQuantum���� Ŭ ���
			else {
				if (i == (cnt - 1)) {		// ���������� ó���� ���μ���
					int n = timeQuantum - total_CT; // ���������� ó���� ��
					pointer->computing_time -= n;
					present_Time += n;

					printf("%d\t\t%d\t\t%d\t\t%s\n", pointer->process_id, pointer->priority, n, "not Terminated");
					
					// ��尡 �ϳ� �̻��� �� �ش� ���μ����� �� ���������� �����ٸ�
					if (pointer->right_link != NULL) {
						// �� �տ��� ������
						queue->right_link = pointer->right_link;
						queue->right_link->left_link = NULL;
						// �� �ڿ� �ֱ�
						last_pointer->right_link = pointer;
						pointer->left_link = last_pointer;
						pointer->right_link = NULL;
					}
				}	// if_finalprocess
				else {
					total_CT += pointer->computing_time;
					present_Time += pointer->computing_time;
					pointer->t_a_Time = calTurn_AroundTime(pointer->arrival_Time, present_Time);
					printf("%d\t\t%d\t\t%d\t\t%d\n", pointer->process_id, pointer->priority, pointer->computing_time, pointer->t_a_Time);

					queue->right_link = pointer->right_link;
					queue->right_link->left_link = NULL;
				}
			}	// else
		}	// for_i
	}	// else
	return 0;
}

// Output
void print_queue(void) {
	while (1) {
		if (queue->right_link == NULL)
			break;
		timeQuantum_queue();
	}
}

// Main
int main(void) {
	printf("Time Quantum�� �Է��Ͻÿ�: ");
	scanf("%d", &timeQuantum);

	int type, process_id, priority, computing_time, f_result;

	initQueue(); // ť �ʱ�ȭ

	FILE* file = fopen("data.txt", "rt");
	printf("Process_id\tpriority\tcomputing_time\tturn_around time\n");

	if (NULL != file) { // ���ϰ����� ���������� �Ǿ��� ��
		while (1) {
			f_result = fscanf(file, "%d %d %d %d", &type, &process_id, &priority, &computing_time);
			if (f_result == -1) 
				break;
			if (type == 0)
				insert_queue(process_id, priority, computing_time); // ����
			if (type == 1)
				timeQuantum_queue(); // ����
		}
		fclose(file);
	}
	print_queue(); // ���
}
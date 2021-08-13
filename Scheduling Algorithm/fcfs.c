#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>

typedef struct Queue* queue_pointer;
struct Queue {
	int process_id;	  // ���μ��� id
	int priority;	  // �켱����
	int computing_time;	  // CPU ó���ð�
	int t_a_t;	  // turn-around time
	int input_time;	  // �����ð�
	queue_pointer left_link, right_link;
};

typedef struct queue_head* head_pointer;
struct queue_head {
	queue_pointer left_link;
	queue_pointer right_link;
};

head_pointer queue; //���� ť ����

//-----------queue �ʱ�ȭ--------------
void initialize_queue(void) {
	queue = (head_pointer)malloc(sizeof(struct queue_head));
	queue->right_link = NULL;

}

//-------------��� ����---------
int insert_queue(int process_id, int priority, int computing_time, int time) {
	queue_pointer pointer, n_node;

	// ���ο� ��� �����Ҵ� �� ������ �ʱ�ȭ
	n_node = (queue_pointer)malloc(sizeof(struct Queue)); // ���ο� ��尪 ����
	n_node->left_link = NULL;
	n_node->right_link = NULL;
	n_node->process_id = process_id;
	n_node->priority = priority;
	n_node->computing_time = computing_time;
	n_node->input_time = time;

	// ť�� ����� ���(��尡 ���� ���)
	if (queue->right_link == NULL) {
		queue->right_link = n_node; // ����� ���ο� ��带 ����Ŵ
		return 0;
	}

	pointer = queue->right_link;

	// ��� ������ �˻� �� ����
	while (1) {

		//�����Ͱ� ������ ��带 ����Ű�鼭 ����
		if (pointer->right_link == NULL) {
			pointer->right_link = n_node;
			n_node->left_link = pointer;
			break;
		}
		else {
			pointer = pointer->right_link; //�����͸� ���� ���� �ű�
		}

	}
	return 0;
}


//����Լ� and ���μ��� cpu�ð� �Ҵ�
void print_queue(void) {

	queue_pointer pointer;
	int time = 0;
	printf(" Process_id\tPriority\tComputing Time\tturn_around time\t���Խð� \n");

	for (pointer = queue->right_link; pointer != NULL; pointer = pointer->right_link) {
		time = time + pointer->computing_time; //�� ���μ��� ���� ���� �ð��� ����(���� ���μ����� ���۽ð��� ��)

		pointer->t_a_t = time - pointer->input_time; //T_A_T ���
		printf(" %d\t\t%d\t\t%d\t\t%d\t\t\t%d ", pointer->process_id, pointer->priority, pointer->computing_time, pointer->t_a_t, pointer->input_time);
		printf("\n");
	}


	printf("-------------------------------------------------------------------\n");


	printf("[Queue]\n�������->");
	for (pointer = queue->right_link; pointer != NULL; pointer = pointer->right_link) {
		printf(" %d,%d,%d ", pointer->process_id, pointer->priority, pointer->computing_time);
		if (pointer->right_link == NULL)
			printf("\n\n"); // ����� ���� �� ���
		else
			printf("<->"); // ������尡 ���� �� ���
	}


}

int main() {
	int type, Process_id, priority, computing_time, f_result;
	int time = 0;
	initialize_queue(); // ť �ʱ�ȭ
	FILE* file = fopen("data.txt", "rt");

	if (NULL != file) { // ���ϰ����� ���������� �Ǿ��� ��
		while (1) {
			f_result = fscanf(file, "%d %d %d %d", &type, &Process_id, &priority, &computing_time); // ���� ����

			if (type == -1) break; // Ÿ���� -1�� ��� ����

			if (type == 0)
				insert_queue(Process_id, priority, computing_time, time); // ť �����Լ�

			if (type == 1)
				time += 20; //Ÿ�Կ� 1�� ���� ��� ����ð����� 20�� �帥 ���·� ����

		}
		fclose(file); // ���� �ݱ�
	}
	print_queue(); // ��� �� ����Լ�
}
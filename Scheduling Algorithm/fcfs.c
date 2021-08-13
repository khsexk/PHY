#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>

typedef struct Queue* queue_pointer;
struct Queue {
	int process_id;	  // 프로세스 id
	int priority;	  // 우선순위
	int computing_time;	  // CPU 처리시간
	int t_a_t;	  // turn-around time
	int input_time;	  // 도착시간
	queue_pointer left_link, right_link;
};

typedef struct queue_head* head_pointer;
struct queue_head {
	queue_pointer left_link;
	queue_pointer right_link;
};

head_pointer queue; //단일 큐 선언

//-----------queue 초기화--------------
void initialize_queue(void) {
	queue = (head_pointer)malloc(sizeof(struct queue_head));
	queue->right_link = NULL;

}

//-------------노드 삽입---------
int insert_queue(int process_id, int priority, int computing_time, int time) {
	queue_pointer pointer, n_node;

	// 새로운 노드 동적할당 및 포인터 초기화
	n_node = (queue_pointer)malloc(sizeof(struct Queue)); // 새로운 노드값 생성
	n_node->left_link = NULL;
	n_node->right_link = NULL;
	n_node->process_id = process_id;
	n_node->priority = priority;
	n_node->computing_time = computing_time;
	n_node->input_time = time;

	// 큐가 비었을 경우(노드가 없을 경우)
	if (queue->right_link == NULL) {
		queue->right_link = n_node; // 헤더는 새로운 노드를 가르킴
		return 0;
	}

	pointer = queue->right_link;

	// 노드 마지막 검색 후 삽입
	while (1) {

		//포인터가 마지막 노드를 가리키면서 삽입
		if (pointer->right_link == NULL) {
			pointer->right_link = n_node;
			n_node->left_link = pointer;
			break;
		}
		else {
			pointer = pointer->right_link; //포인터를 다음 노드로 옮김
		}

	}
	return 0;
}


//출력함수 and 프로세스 cpu시간 할당
void print_queue(void) {

	queue_pointer pointer;
	int time = 0;
	printf(" Process_id\tPriority\tComputing Time\tturn_around time\t진입시간 \n");

	for (pointer = queue->right_link; pointer != NULL; pointer = pointer->right_link) {
		time = time + pointer->computing_time; //한 프로세스 종료 시의 시간을 저장(다음 프로세스의 시작시간이 됨)

		pointer->t_a_t = time - pointer->input_time; //T_A_T 계산
		printf(" %d\t\t%d\t\t%d\t\t%d\t\t\t%d ", pointer->process_id, pointer->priority, pointer->computing_time, pointer->t_a_t, pointer->input_time);
		printf("\n");
	}


	printf("-------------------------------------------------------------------\n");


	printf("[Queue]\nㅣ헤더ㅣ->");
	for (pointer = queue->right_link; pointer != NULL; pointer = pointer->right_link) {
		printf(" %d,%d,%d ", pointer->process_id, pointer->priority, pointer->computing_time);
		if (pointer->right_link == NULL)
			printf("\n\n"); // 노드의 끝일 때 출력
		else
			printf("<->"); // 다음노드가 있을 때 출력
	}


}

int main() {
	int type, Process_id, priority, computing_time, f_result;
	int time = 0;
	initialize_queue(); // 큐 초기화
	FILE* file = fopen("data.txt", "rt");

	if (NULL != file) { // 파일개방이 정상적으로 되었을 때
		while (1) {
			f_result = fscanf(file, "%d %d %d %d", &type, &Process_id, &priority, &computing_time); // 파일 읽음

			if (type == -1) break; // 타입이 -1일 경우 종료

			if (type == 0)
				insert_queue(Process_id, priority, computing_time, time); // 큐 삽입함수

			if (type == 1)
				time += 20; //타입에 1이 들어온 경우 현재시간에서 20초 흐른 상태로 변함

		}
		fclose(file); // 파일 닫기
	}
	print_queue(); // 결과 값 출력함수
}
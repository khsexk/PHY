#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>

typedef struct RR_queue* queue_pointer;
struct RR_queue {
	int process_id;		// 프로세스 id
	int priority;		// 우선순위
	int computing_time;	// 처리시간
	int t_a_Time;		// turn-around time
	int arrival_Time;	// enter time
	queue_pointer left_link, right_link;
};

typedef struct queue_head* head_pointer;
struct queue_head {
	queue_pointer left_link;
	queue_pointer right_link;
};

head_pointer queue;	// Priority를 따지지 않기 때문에 queue 1개 생성
int present_Time = 0;
int timeQuantum = 20;	// default time Quantum 설정

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

	f_node = (queue_pointer)malloc(sizeof(struct RR_queue)); // 새로운 노드값 생성
	f_node->left_link = NULL;
	f_node->right_link = NULL;
	f_node->process_id = process_id;
	f_node->priority = priority;
	f_node->computing_time = computing_time;
	f_node->arrival_Time = present_Time;

	// 큐가 비었을 경우(노드가 없을경우)
	if (queue->right_link == NULL) {
		queue->right_link = f_node;
		f_node->right_link = NULL;
		return 0;
	}

	// 큐의 맨뒤 검색
	for (pointer = queue->right_link; pointer != NULL ; pointer=pointer->right_link) {
		if (pointer->right_link == NULL)
			break;
	}

	// 맨뒤삽입
	pointer->right_link = f_node;
	f_node->left_link = pointer;
	f_node->right_link = NULL;

	return 0;
}

// 타임퀀텀이 끝났을 때
int timeQuantum_queue() {
	queue_pointer pointer;
	queue_pointer last_pointer;

	if (queue->right_link == NULL) {
		printf("처리 중인 프로세스가 없습니다.\n");
		return 0;
	}

	// 큐의 맨뒤 검색
	for (last_pointer = queue->right_link; last_pointer != NULL; last_pointer = last_pointer->right_link) {
		if (last_pointer->right_link == NULL)
			break;
	}

	pointer = queue->right_link;
	// 프로세스의 computing_time이 timeQuantum보다 길 때
	if (pointer->computing_time > timeQuantum) {
		present_Time += timeQuantum;	// 현재 시간 조정

		printf("%d\t\t%d\t\t%d\t\t%s\n", pointer->process_id, pointer->priority, timeQuantum, "not Terminated");

		pointer->computing_time -= timeQuantum;	// computing_time에서 처리한 시간을 빼줌

		// 노드가 하나 이상일 때 해당 프로세스를 맨 마지막으로 스케줄링
		if (pointer->right_link != NULL) {
			// 맨 앞에서 빼내기
			queue->right_link = pointer->right_link;
			queue->right_link->left_link = NULL;
			pointer->right_link = NULL;
			// 맨 뒤에 넣기
			last_pointer->right_link = pointer;
			pointer->left_link = last_pointer;
		}
	}	// if_(ct > tQ)

	// 프로세스의 computing_time이 timeQuantum과 같을 때
	else if (pointer->computing_time == timeQuantum) {
		present_Time += timeQuantum;	// 현재 시간 조정

		pointer->t_a_Time = calTurn_AroundTime(pointer->arrival_Time, present_Time);

		printf("%d\t\t%d\t\t%d\t\t%d\n", pointer->process_id, pointer->priority, pointer->computing_time, pointer->t_a_Time);

		// 노드가 하나일 때
		if (pointer->right_link == NULL) {
			queue->right_link = NULL;
		}
		// 노드가 하나 이상일 때 
		else {
			// 맨 앞에서 빼내기
			queue->right_link = pointer->right_link;
			queue->right_link->left_link = NULL;
		}
	}	// if_(ct == tQ)

	// 프로세스의 computing_time이 timeQuantum보다 짧을 때
	else {
		int cnt = 0;	// 처리될 프로세스 수
		int total_CT = 0;  // 처리될 프로세스의 computing_time 합
		int same = 0;

		// 몇 개의 프로세스를 처리할지 count
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
		// 출력
		for (int i = 0; i < cnt; i++) {
			pointer = queue->right_link;
			// 프로세스 cnt개의 computing_time 합이 timeQuantum과 같거나 작을 경우
			if (same == 1) {	
				present_Time += pointer->computing_time;
				pointer->t_a_Time = calTurn_AroundTime(pointer->arrival_Time, present_Time);
				printf("%d\t\t%d\t\t%d\t\t%d\n", pointer->process_id, pointer->priority, pointer->computing_time, pointer->t_a_Time);

				// 노드가 하나 일 때
				if (pointer->right_link == NULL) {
					queue->right_link = NULL;
				}
				// 노드가 하나 이상일 때 맨 앞에서 뺌
				else {
					queue->right_link = pointer->right_link;
					queue->right_link->left_link = NULL;
					pointer = queue->right_link;
				}
			}	// if_(same==1)

			// 프로세스 cnt개의 computing_time 합이 timeQuantum보다 클 경우
			else {
				if (i == (cnt - 1)) {		// 마지막으로 처리될 프로세스
					int n = timeQuantum - total_CT; // 마지막으로 처리할 양
					pointer->computing_time -= n;
					present_Time += n;

					printf("%d\t\t%d\t\t%d\t\t%s\n", pointer->process_id, pointer->priority, n, "not Terminated");
					
					// 노드가 하나 이상일 때 해당 프로세스를 맨 마지막으로 스케줄링
					if (pointer->right_link != NULL) {
						// 맨 앞에서 빼내기
						queue->right_link = pointer->right_link;
						queue->right_link->left_link = NULL;
						// 맨 뒤에 넣기
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
	printf("Time Quantum을 입력하시오: ");
	scanf("%d", &timeQuantum);

	int type, process_id, priority, computing_time, f_result;

	initQueue(); // 큐 초기화

	FILE* file = fopen("data.txt", "rt");
	printf("Process_id\tpriority\tcomputing_time\tturn_around time\n");

	if (NULL != file) { // 파일개방이 정상적으로 되었을 때
		while (1) {
			f_result = fscanf(file, "%d %d %d %d", &type, &process_id, &priority, &computing_time);
			if (f_result == -1) 
				break;
			if (type == 0)
				insert_queue(process_id, priority, computing_time); // 삽입
			if (type == 1)
				timeQuantum_queue(); // 삭제
		}
		fclose(file);
	}
	print_queue(); // 출력
}
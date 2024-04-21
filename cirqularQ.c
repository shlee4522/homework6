#include <stdio.h>
#include <stdlib.h>
#define MAX_QUEUE_SIZE 4

typedef char element;
typedef struct {
    element queue[MAX_QUEUE_SIZE]; //큐의 원소들을 저장하는 배열
    int front, rear; 
} QueueType;



QueueType* createQueue();//큐를 생성하고 초기화하는 함수
int freeQueue(QueueType* cQ);//큐를 해제하는 함수
int isEmpty(QueueType* cQ);//큐가 비어있는지 확인하는 함수
int isFull(QueueType* cQ);//큐가 가득 찼는지 확인하는 함수
void enQueue(QueueType* cQ, element item);//큐에 원소를 삽입하는 함수
void deQueue(QueueType* cQ, element* item);//큐에서 원소를 삭제하고 반환하는 함수
void printQ(QueueType* cQ);//큐의 내용을 출력하는 함수
void debugQ(QueueType* cQ);//큐의 상태를 디버그하는 함수

element getElement();//원소를 입력받는 함수

int main(void)
{
    printf("[----- [LeeSeungHun] [2023041045] -----]");

    QueueType* cQ = createQueue(); //큐 생성 및 초기화
    element data;
    char command;

    do {
        printf("\n-----------------------------------------------------\n");
        printf("                     Circular Q                   \n");
        printf("------------------------------------------------------\n");
        printf(" Insert=i,  Delete=d,   PrintQ=p,   Debug=b,   Quit=q \n");
        printf("------------------------------------------------------\n");

        printf("Command = ");
        scanf(" %c", &command);

        switch (command) {
        case 'i': case 'I':
            data = getElement(); //원소를 입력 받음
            enQueue(cQ, data); //큐에 삽입
            break;
        case 'd': case 'D':
            deQueue(cQ, &data); //큐에서 삭제하고 반환
            break;
        case 'p': case 'P':
            printQ(cQ); //큐의 내용을 출력
            break;
        case 'b': case 'B':
            debugQ(cQ); //큐의 상태를 디버그
            break;
        case 'q': case 'Q':
            freeQueue(cQ); //큐를 해제하고 프로그램 종료
            break;
        default:
            printf("\n      wrong     \n");
            break;
        }

    } while (command != 'q' && command != 'Q');

    return 0;
}

QueueType* createQueue()
{
    QueueType* cQ;
    cQ = (QueueType*)malloc(sizeof(QueueType)); //동적 메모리 할당
    cQ->front = 0; //front 초기화
    cQ->rear = 0; //rear 초기화
    return cQ;
}

int freeQueue(QueueType* cQ)
{
    if (cQ == NULL) return 1; //이미 해제된 경우
    free(cQ); //메모리 해제
    return 1;
}

element getElement()
{
    element item;
    printf("Input element = ");
    scanf(" %c", &item);
    return item;
}

int isEmpty(QueueType* cQ)
{
    return (cQ->front == cQ->rear); //front와 rear가 같으면 큐가 비어있음
}

int isFull(QueueType* cQ)
{
    return ((cQ->rear + 1) % MAX_QUEUE_SIZE == cQ->front); //rear 다음이 front면 큐가 가득 참
}

void enQueue(QueueType* cQ, element item)
{
    if (isFull(cQ)) { // 큐가 가득 찬 경우
        printf("Queue is full.\n");
        return;
    }
    cQ->rear = (cQ->rear + 1) % MAX_QUEUE_SIZE; //rear를 다음 위치로 이동
    cQ->queue[cQ->rear] = item; //해당 위치에 원소 삽입
}

void deQueue(QueueType* cQ, element* item)
{
    if (isEmpty(cQ)) { //큐가 비어있는 경우
        printf("Queue is empty.\n");
        return;
    }
    cQ->front = (cQ->front + 1) % MAX_QUEUE_SIZE; //front를 다음 위치로 이동
    *item = cQ->queue[cQ->front]; //삭제된 원소를 item에 저장
}

void printQ(QueueType* cQ)
{
    int i, first, last;

    if (isEmpty(cQ)) { //큐가 비어있는 경우
        printf("Queue is empty.\n");
        return;
    }

    first = (cQ->front + 1) % MAX_QUEUE_SIZE; //첫 번째 원소
    last = (cQ->rear + 1) % MAX_QUEUE_SIZE; //마지막 원소

    printf("Circular Queue : [");

    i = first; //첫 번째 원소부터 시작
    while (i != last) { //마지막 원소까지 반복
        printf("%3c", cQ->queue[i]); //해당 위치의 원소 출력
        i = (i + 1) % MAX_QUEUE_SIZE; //다음 원소로 이동
    }

    printf(" ]\n");
}

void debugQ(QueueType* cQ)
{

    printf("\n---DEBUG\n");

    for (int i = 0; i < MAX_QUEUE_SIZE; i++)
    {
        if (i == cQ->front) { //front 위치인 경우
            printf("  [%d] = front\n", i);
            continue;
        }
        printf("  [%d] = %c\n", i, cQ->queue[i]); //해당 위치의 원소 출력

    }

    printf("front = %d, rear = %d\n", cQ->front, cQ->rear); //front와 rear 출력
}
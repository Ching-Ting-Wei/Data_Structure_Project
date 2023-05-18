#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <stdbool.h>
struct queue
{
	int size;
	int front;
	int rear;
	int *arr;
};

struct AcceptReq
{
	int id;
	bool flag;
	int distance;
	int dst;
	int *path;
};

void enqueue(struct queue *q, int);
int dequeue(struct queue *q);
int isEmpty(struct queue *q);
int isFull(struct queue *q);

int isEmpty(struct queue *q) // Check if the queue is empty
{
	if (q->rear == q->front)
		return 1;
	else
		return 0;
}

int isFull(struct queue *q)
{
	if (q->rear == q->size - 1)
		return 1;
	else
		return 0;
}

// Adding elements into queue
void enqueue(struct queue *q, int value)
{
	if (isFull(q))
		printf("\nQueue is Full!!");
	else
	{
		q->rear++;
		q->arr[q->rear] = value;
	}
}

// Removing elements from queue
int dequeue(struct queue *q)
{
	int a = -1;
	if (isEmpty(q))
	{
		return a;
	}
	else
	{
		q->front++;
		a = q->arr[q->front];
	}
	return a;
}

// BFS algorithm
void bfs(int **Graph, int startVertex, int endVertex, int numberOfNodes, struct AcceptReq *Acc, int *QuantumMemories)
{
	struct queue q;
	q.size = numberOfNodes * 4;
	q.front = 0;
	q.rear = 0;
	q.arr = (int *)malloc(q.size * sizeof(int));
	Acc->path = malloc(numberOfNodes * sizeof(int));	// 記錄當前的從v結點開始到各個結點的最短路徑長度
	int *visited = malloc(numberOfNodes * sizeof(int)); // 記錄結點時是否被訪問過，訪問過為1， 沒有訪問過為0
	int *dist = malloc(numberOfNodes * sizeof(int));	// 記錄最短路徑，存放的是該結點的上一個為最短路徑的前驅結點
	for (int i = 0; i < numberOfNodes; i++)
	{
		visited[i] = 0;
		Acc->path[i] = -1;
		dist[i] = 10000000;
	}
	if (startVertex == endVertex)
	{
		Acc->distance = 0;
		Acc->path[startVertex] = startVertex;
		Acc->flag = true;
		return;
	}
	dist[startVertex] = 0;
	visited[startVertex] = 1;
	enqueue(&q, startVertex);
	while (!isEmpty(&q)) // BFS implement
	{
		int currentVertex = dequeue(&q);
		for (int i = 0; i < numberOfNodes; i++)
		{
			if (i != startVertex && i != endVertex)
			{
				if (Graph[currentVertex][i] >= 1 && visited[i] == 0)
				{
					dist[i] = dist[currentVertex] + 1;
					Acc->path[i] = currentVertex;
					visited[i] = 1;
					enqueue(&q, i);
				}
			}
			else
			{
				if (Graph[currentVertex][i] >= 1 && visited[i] == 0)
				{
					dist[i] = dist[currentVertex] + 1;
					Acc->path[i] = currentVertex;
					visited[i] = 1;
					enqueue(&q, i);
					break;
				}
			}
		}
	}
	Acc->distance = dist[endVertex];
	free(q.arr);
	free(visited);
	free(dist);
	return;
}

void PrintPath(struct AcceptReq *Acc)
{
	int *route = malloc(((Acc->distance) + 1) * sizeof(int));
	route[0] = Acc->dst;
	int prior = Acc->path[Acc->dst];
	for (int i = 1; i <= Acc->distance; i++)
	{
		route[i] = prior;
		if (prior == -1)
		{
			break;
		}
		prior = Acc->path[prior];
	}
	for (int i = Acc->distance; i >= 0; i--)
	{
		printf("%d ", route[i]);
	}
	printf("\n");
	free(route);
}

int main()
{
	int numberOfNodes, numberOfLinks, numberOfReq;
	scanf("%d%d%d", &numberOfNodes, &numberOfLinks, &numberOfReq);
	int nodeId, numberOfQuantMemory, QuantumMemories[numberOfNodes];
	for (int i = 0; i < numberOfNodes; i++)
	{
		scanf("%d%d", &nodeId, &numberOfQuantMemory);
		QuantumMemories[nodeId] = numberOfQuantMemory;
	}

	int **Graph, linkId, linkEnd1, linkEnd2, channels;
	Graph = (int **)malloc(numberOfNodes * sizeof(int *));
	for (int i = 0; i < numberOfNodes; i++) // create graph
	{
		Graph[i] = (int *)malloc(numberOfNodes * sizeof(int));
	}
	for (int i = 0; i < numberOfNodes; i++) // reset graph
	{
		for (int j = 0; j < numberOfNodes; j++)
		{
			Graph[i][j] = 0;
		}
	}
	for (int i = 0; i < numberOfLinks; i++)
	{
		scanf("%d%d%d%d", &linkId, &linkEnd1, &linkEnd2, &channels);
		Graph[linkEnd1][linkEnd2] = channels;
		Graph[linkEnd2][linkEnd1] = channels;
	}

	struct AcceptReq Acc[numberOfReq];
	int AccReq = 0, ReqId, ReqSrc, ReqDst;
	for (int i = 0; i < numberOfReq; i++)
	{
		scanf("%d%d%d", &ReqId, &ReqSrc, &ReqDst);
		Acc[i].dst = ReqDst;
		Acc[i].id = ReqId;
		Acc[i].flag = false;
		if (QuantumMemories[ReqSrc] >= 1 && QuantumMemories[ReqDst] >= 1)
		{
			bfs(Graph, ReqSrc, ReqDst, numberOfNodes, &Acc[i], QuantumMemories);
			int flag = ReqDst;
			if (QuantumMemories[ReqDst] >= 1 && QuantumMemories[ReqSrc] >= 1 && !(Acc[i].flag) && Acc[i].path[ReqDst] != -1) // check path
			{
				flag = Acc[i].path[flag];

				while (flag != -1)
				{
					if (QuantumMemories[flag] < 2)
					{
						break;
					}
					flag = Acc[i].path[flag];
				}
				if (flag == -1)
				{
					AccReq++;
					Acc[i].flag = true;
				}
			}
			if (Acc[i].flag) // delete resource
			{
				int link1 = ReqDst;
				int link2 = Acc[i].path[ReqDst];
				QuantumMemories[ReqDst]--;
				QuantumMemories[ReqSrc]--;
				while (link2 != ReqSrc)
				{
					Graph[link1][link2]--;
					Graph[link2][link1]--;
					QuantumMemories[link2] -= 2;
					link1 = link2;
					link2 = Acc[i].path[link2];
				}
				Graph[link1][link2]--;
				Graph[link2][link1]--;
			}
		}
	}
	printf("%d\n", AccReq);
	for (int i = 0; i < numberOfReq; i++)
	{
		if (Acc[i].flag)
		{
			printf("%d ", Acc[i].id);
			PrintPath(&Acc[i]);
		}
	}
}
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>

typedef struct node
{
    struct node *left_child;
    struct node *right_child;
    unsigned long value;
    int level, nodeIndex;
} Node;
struct queue
{
    Node *node;
};
unsigned long MurmurOAAT32(char *key)
{
    unsigned long h = 3323198485ul;
    for (; *key; ++key)
    {
        h ^= *key;
        h *= 0x5bd1e995;
        h ^= h >> 15;
    }
    return h;
}

int TreeHeight(int nodes)
{
    return ceil(log2(nodes)) + 1;
}

struct node *new_node(unsigned long value, int level, int index)
{
    struct node *tmp = (struct node *)malloc(sizeof(struct node));
    tmp->value = value;
    tmp->level = level;
    tmp->nodeIndex = index;
    tmp->left_child = tmp->right_child = NULL;
    return tmp;
}

struct node *insert_node(struct node *node, unsigned long value, int level, int index, struct queue *queue, int *front, int *rear, int levelNodenum) // inserting nodes!
{

    if (node == NULL)
    {
        node = new_node(value, level, index);
        queue[*rear].node = node;
        (*rear)++;
        return node;
    }
    if (node->left_child == NULL)
    {
        node->left_child = insert_node(node->left_child, value, level, index, queue, front, rear, levelNodenum);
        if (index == levelNodenum - 1)
            (*front)++;
    }
    else
    {
        node->right_child = insert_node(node->right_child, value, level, index, queue, front, rear, levelNodenum);
        (*front)++;
    }
    return node;
}

void visit(char **worngString, struct node *treeNode, int *wrongCount, char **string)
{
    if (!treeNode)
        return;
    unsigned long cmp;
    printf("1 %d %d\n", treeNode->level, treeNode->nodeIndex);
    fflush(NULL);
    scanf("%lu", &cmp);
    if (cmp != treeNode->value)
    {
        if (treeNode->left_child == NULL && treeNode->right_child == NULL)
        {
            worngString[*wrongCount] = string[treeNode->nodeIndex];
            (*wrongCount)++;
            return;
        }
        else
        {
            if (treeNode->left_child != NULL)
                visit(worngString, treeNode->left_child, wrongCount, string);
            if (treeNode->right_child != NULL)
                visit(worngString, treeNode->right_child, wrongCount, string);
        }
    }
    return;
}
int main()
{
    int strNum, strLen;
    scanf("%d%d", &strNum, &strLen);
    int treeheight = TreeHeight(strNum);

    char **string = malloc(sizeof(char *) * strNum);
    for (int i = 0; i < strNum; i++)
        string[i] = malloc(sizeof(char) * (strLen + 5));

    for (int i = 0; i < strNum; i++)
        scanf("%s", string[i]);
    unsigned long **num = malloc(sizeof(unsigned long *) * treeheight);
    for (int i = 0; i < treeheight; i++)
    {
        num[i] = malloc(sizeof(unsigned long) * strNum);
    }

    int *levelNodesNum = malloc(sizeof(int) * treeheight);
    for (int i = 0; i < treeheight; i++)
        levelNodesNum[i] = 0;

    for (int i = 0; i < strNum; i++)
    {
        num[0][i] = MurmurOAAT32(string[i]);
        levelNodesNum[0]++;
        printf("%lu ", num[0][i]);
    }

    double temp = strNum;
    char buffer[1000];
    for (int i = 1; i <= treeheight; i++)
    {
        if ((int)temp % 2 == 0) // if have even nodes
        {
            temp = ceil(temp / 2);
            for (int j = 0; j < temp; j++)
            {
                sprintf(buffer, "%lu", num[i - 1][j * 2] + num[i - 1][j * 2 + 1]);
                num[i][j] = MurmurOAAT32(buffer);
                memset(buffer, 0, 1000);
                levelNodesNum[i]++;
                printf("%lu ", num[i][j]);
            }

            if (temp == 1)
                break;
        }
        else
        {
            temp = ceil(temp / 2);
            for (int j = 0; j < temp; j++)
            {
                if (j != temp - 1)
                {
                    sprintf(buffer, "%lu", num[i - 1][j * 2] + num[i - 1][j * 2 + 1]);
                    num[i][j] = MurmurOAAT32(buffer);
                    memset(buffer, 0, 1000);
                    levelNodesNum[i]++;
                    printf("%lu ", num[i][j]);
                }
                else
                {
                    sprintf(buffer, "%lu", num[i - 1][j * 2] + num[i - 1][j * 2]);
                    num[i][j] = MurmurOAAT32(buffer);
                    memset(buffer, 0, 1000);
                    levelNodesNum[i]++;
                    printf("%lu ", num[i][j]);
                }
            }
            if (temp == 1)
                break;
        }
    }

    struct node *root = NULL;
    struct node *tempnode = root;
    struct queue *q = malloc(sizeof(struct node) * strNum * 2);
    int fornt = 0;
    int rear = 0;
    for (int i = treeheight - 1; i >= 0; i--)
    {
        for (int j = 0; j < levelNodesNum[i]; j++)
        {
            tempnode = insert_node(tempnode, num[i][j], treeheight - i - 1, j, q, &fornt, &rear, levelNodesNum[i]);
            tempnode = q[fornt].node;
        }
    }
    root = q[0].node;

    char **wrongString = malloc(sizeof(char *) * strNum);
    for (int i = 0; i < strNum; i++)
        wrongString[i] = malloc(sizeof(char) * (strLen + 5));

    int wrongcount = 0;
    visit(wrongString, root, &wrongcount, string);
    printf("2 %d\n", wrongcount);
    for (int i = 0; i < wrongcount; i++)
        printf("%s\n", wrongString[i]);

    // for (int i = 1; i < strNum ; i++)
    //     free(string[i]);
    // free(string);
    for (int i = 0; i < treeheight; i++)
        free(num[i]);
    free(num);
    free(levelNodesNum);
    for (int i = 0; i < strNum; i++)
        free(wrongString[i]);
    free(wrongString);
}
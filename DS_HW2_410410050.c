#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
struct CandidateShape
{
    int numberOfShape;
    int **shape;
};
struct AcceptReq
{
    bool flag;
    int shape;
    int x;
    int y;
};
int main()
{
    int ResourceY, ResourceX, users;
    scanf("%d%d%d", &ResourceY, &ResourceX, &users);
    int userId;

    char **candidate;
    candidate = (char **)malloc(users * sizeof(char *)); // set candidate space to read string
    for (int i = 0; i < users; i++)
    {
        candidate[i] = (char *)malloc(4 * ResourceY * sizeof(char));
    }
    for (int i = 0; i < users; i++)
    {
        scanf("%d", &userId);
        fgets(candidate[i], 4 * ResourceY, stdin);
    }

    struct CandidateShape *request = malloc(users * sizeof(struct CandidateShape)); // set candidate's shape space
    for (int i = 0; i < users; i++)
    {
        request[i].shape = (int **)malloc(ResourceY / 2 * sizeof(int *));
    }
    for (int i = 0; i < users; i++)
    {
        for (int j = 0; j < ResourceY / 2; j++)
            request[i].shape[j] = (int *)malloc(2 * sizeof(int));
    }

    for (int i = 0; i < users; i++) // transfer string to int
    {
        char *pch;
        int count = 0;
        pch = strtok(candidate[i], " x");
        while (pch != NULL)
        {
            request[i].shape[count / 2][count % 2] = atoi(pch);
            pch = strtok(NULL, " x");
            count++;
        }
        request[i].numberOfShape = count / 2;
    }

    int shapeNum = 0;
    int *lengthY = malloc(ResourceY * sizeof(int)); // to store all type of shape
    for (int i = 0; i < users; i++)
    {
        for (int j = 0; j < request[i].numberOfShape; j++)
        {
            bool flag = true;
            for (int k = 0; k < shapeNum; k++)
            {
                if (request[i].shape[j][0] == lengthY[k])
                {
                    flag = false;
                    break;
                }
            }
            if (flag)
            {
                lengthY[shapeNum] = request[i].shape[j][0];
                shapeNum++;
            }
        }
    }

    for (int i = 0; i < shapeNum; i++) // sorting
    {
        for (int j = i + 1; j < shapeNum; j++)
        {
            if (lengthY[j] > lengthY[i])
            {
                int temp = lengthY[i];
                lengthY[i] = lengthY[j];
                lengthY[j] = temp;
            }
        }
    }

    int *shapeUse = malloc(sizeof(int) * shapeNum);
    for (int i = 0; i < shapeNum; i++)
    {
        shapeUse[i] = 0;
    }

    struct AcceptReq *acc1 = malloc(users * sizeof(struct AcceptReq));
    int originx = 0;
    int originy = 0;
    int lastx = -1; 
    int lasty = -1;
    int maxLength = request[0].shape[0][0] * request[0].shape[0][1];
    int acc1Req = 0;
    for (int i = 0; i < users; i++)
    {
        acc1[i].flag = false;
    }

    for (int k = 0; k < shapeNum; k++) // store from maxY
    {
        for (int i = 0; i < users; i++)
        {
            if (!acc1[i].flag)
            {
                for (int j = 0; j < request[i].numberOfShape; j++)
                {
                    if (request[i].shape[j][0] == lengthY[k] && (originx + maxLength / lengthY[k]) <= ResourceX && originy + lengthY[k] <= ResourceY)
                    {
                        acc1Req++;
                        shapeUse[k]++;
                        acc1[i].flag = true;
                        acc1[i].x = originx;
                        acc1[i].y = originy;
                        acc1[i].shape = lengthY[k];
                        originy += lengthY[k];
                        if (originy + lengthY[k] > ResourceY)
                        {
                            originx += maxLength / lengthY[k];
                            originy = 0;
                        }
                    }
                }
            }
        }
        if (shapeUse[k] != 0)   // if last shape can't store, it will point to last used coordinates
        {   
            lasty = originy;
            lastx = originx;
        }
        if (originy != 0)
            originx += maxLength / lengthY[k];
        originy = 0;
    }

    int unUse = -1;
    for (int i = 0; i < shapeNum; i++)
    {
        if (shapeUse[i] == 0)
        {
            unUse = i;
            break;
        }
    }

    if (unUse != -1)
    {
        for (int i = 0; i < users; i++)
        {
            if (!acc1[i].flag)
            {
                for (int j = 0; j < request[i].numberOfShape; j++)
                {
                    if (request[i].shape[j][0] == lengthY[unUse] && (lastx + maxLength / lengthY[unUse]) <= ResourceX && lasty + lengthY[unUse] <= ResourceY)
                    {
                        acc1Req++;
                        acc1[i].flag = true;
                        acc1[i].x = lastx;
                        acc1[i].y = lasty;
                        acc1[i].shape = lengthY[unUse];
                        lasty += lengthY[unUse];
                        if (lasty + lengthY[unUse] > ResourceY)
                        {
                            lastx += maxLength / lengthY[unUse];
                            lasty = 0;
                        }
                    }
                }
            }
        }
    }

    struct AcceptReq *acc2 = malloc(users * sizeof(struct AcceptReq));
    int acc2Req = 0;
    originx = 0;
    originy = 0;

    for (int i = 0; i < users; i++)
    {
        acc2[i].flag = false;
    }

    for (int k = shapeNum - 1; k >= 0; k--) // store from minY
    {
        for (int i = 0; i < users; i++)
        {
            if (!acc2[i].flag)
            {
                for (int j = 0; j < request[i].numberOfShape; j++)
                {
                    if (request[i].shape[j][0] == lengthY[k] && (originx + maxLength / lengthY[k]) <= ResourceX && originy + lengthY[k] <= ResourceY)
                    {
                        acc2Req++;
                        acc2[i].flag = true;
                        acc2[i].x = originx;
                        acc2[i].y = originy;
                        acc2[i].shape = lengthY[k];
                        originy += lengthY[k];
                        if (originy + lengthY[k] > ResourceY)
                        {
                            originx += maxLength / lengthY[k];
                            originy = 0;
                        }
                    }
                }
            }
        }
        if (originy != 0)
            originx += maxLength / lengthY[k];
        originy = 0;
    }

    if (acc1Req >= acc2Req)  // compare acc1Req and acc2Req
    {
        printf("%d\n", acc1Req);
        for (int i = 0; i < users; i++)
        {
            if (acc1[i].flag == true)
            {
                printf("%d %dx%d %d %d\n", i, acc1[i].shape, maxLength / acc1[i].shape, acc1[i].y, acc1[i].x);
            }
        }
    }
    else
    {
        printf("%d\n", acc2Req);
        for (int i = 0; i < users; i++)
        {
            if (acc2[i].flag == true)
            {
                printf("%d %dx%d %d %d\n", i, acc2[i].shape, maxLength / acc2[i].shape, acc2[i].y, acc2[i].x);
            }
        }
    }
    free(candidate);
    free(request);
    free(lengthY);
    free(acc1);
    free(acc2);
}
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
struct node
{
    int id;
    double CoordinateX;
    double CoordinateY;
};

struct dis // store vertex to vertex distance
{
    int start;
    int end;
    double distance;
};

struct UAV
{
    int nodenum;
    double cost;
    int *path;
};

double Distance(double x1, double y1, double x2, double y2)
{
    return sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));
}

int conmpare(const void *a, const void *b)
{
    return (*(struct dis *)a).distance > (*(struct dis *)b).distance ? 1 : -1;
}

int FindSetCollapsing(int *subset, int i)
{ // use recursive to do collapse

    int root; // root
    for (root = i; subset[root] >= 0; root = subset[root])
        ;

    while (i != root)
    {
        int parent = subset[i];
        subset[i] = root;
        i = parent;
    }

    return root;
}

void UnionSet(int *subset, int x, int y)
{

    int xroot = FindSetCollapsing(subset, x),
        yroot = FindSetCollapsing(subset, y);

    // 用rank比較, 負越多表示set越多element, 所以是值比較小的element比較多
    // xroot, yroot的subset[]一定都是負值
    if (subset[xroot] <= subset[yroot])
    { // x has more elements or equal, use x as root
        subset[xroot] += subset[yroot];
        subset[yroot] = xroot;
    }
    else
    { //  if (subset[xroot] > subset[yroot]), y has more elements
        subset[yroot] += subset[xroot];
        subset[xroot] = yroot;
    }
}

void findOdd(double **G, bool *odd, int nodesnum) // find the vertex with odd edges
{
    for (int i = 0; i < nodesnum; i++)
    {
        int count = 0;
        for (int j = 0; j < nodesnum; j++)
        {
            if (G[i][j] != 0)
                count++;
        }
        if (count % 2 == 1)
            odd[i] = true;
    }
}
void perfectMatching(struct node *nodes, int oddVerticesNum, int *oddVertices, double **g)
{
    int closest = -1;
    double length;
    int *vis = malloc(sizeof(int) * oddVerticesNum);
    for (int i = 0; i < oddVerticesNum; i++)
        vis[i] = 0;

    for (int i = 0; i < oddVerticesNum; i++)
    {
        length = 10000000000;
        if (vis[i] == 1)
            continue;
        // if this node is closer than the current closest, update closest and length
        for (int j = i + 1; j < oddVerticesNum; j++)
        {
            if (Distance(nodes[i].CoordinateX, nodes[i].CoordinateY, nodes[j].CoordinateX, nodes[j].CoordinateY) < length && vis[j] != 1)
            {
                length = Distance(nodes[i].CoordinateX, nodes[i].CoordinateY, nodes[j].CoordinateX, nodes[j].CoordinateY);
                closest = j;
            }
        }
        vis[i] = 1;
        vis[closest] = 1;
        g[i][closest] = length;
        g[closest][i] = length;
    } // two nodes are matched, end of list reached
    free(vis);
}
void DFS(double **G, int u, int *vis, int nodesnum, int *path, int *j)
{
    vis[u] = 1;
    path[*j] = u;
    for (int v = 0; v < nodesnum; v++)
    {
        if (!vis[v] && G[u][v])
        {
            *j += 1;
            DFS(G, v, vis, nodesnum, path, j);
        }
    }
}

int main()
{
    int nodesNum;
    double B;
    scanf("%d%lf", &nodesNum, &B);
    struct node *nodes = malloc(sizeof(struct node) * nodesNum);
    for (int i = 0; i < nodesNum; i++)
    {
        scanf("%d%lf%lf", &nodes[i].id, &nodes[i].CoordinateX, &nodes[i].CoordinateY);
    }

    struct dis *Tnodesdis = malloc(sizeof(struct node) * nodesNum * nodesNum); // store all vertices to vertices distance
    for (int i = 0; i < nodesNum; i++)
    {
        for (int j = 0; j < nodesNum; j++)
        {
            Tnodesdis[i * nodesNum + j].start = i;
            Tnodesdis[i * nodesNum + j].end = j;
            Tnodesdis[i * nodesNum + j].distance = Distance(nodes[i].CoordinateX, nodes[i].CoordinateY, nodes[j].CoordinateX, nodes[j].CoordinateY);
        }
    }

    int edgeNum = (nodesNum * nodesNum - nodesNum) / 2;
    qsort(Tnodesdis, nodesNum * nodesNum, sizeof(struct dis), conmpare);
    struct dis *nodesdis = malloc(sizeof(struct node) * edgeNum);
    for (int i = 0; i < edgeNum; i++)
    {
        nodesdis[i].distance = Tnodesdis[i * 2 + nodesNum].distance;
        nodesdis[i].start = Tnodesdis[i * 2 + nodesNum].start;
        nodesdis[i].end = Tnodesdis[i * 2 + nodesNum].end;
    }

    int *belongs = malloc(sizeof(int) * edgeNum);
    for (int i = 0; i < edgeNum; i++)
        belongs[i] = -1;

    struct dis *spanlist = malloc(sizeof(struct dis) * edgeNum);
    int spanlistNum = 0;
    for (int i = 0; i < edgeNum; i++)
    {
        if (FindSetCollapsing(belongs, nodesdis[i].start) != FindSetCollapsing(belongs, nodesdis[i].end))
        {
            spanlist[spanlistNum++] = nodesdis[i];
            UnionSet(belongs, nodesdis[i].start, nodesdis[i].end);
        }
    }

    double **graph = malloc(sizeof(double *) * nodesNum);
    for (int i = 0; i < nodesNum; i++)
    {
        graph[i] = malloc(sizeof(double) * nodesNum);
    }
    for (int i = 0; i < nodesNum; i++)
    {
        for (int j = 0; j < nodesNum; j++)
        {
            graph[i][j] = 0;
        }
    }
    for (int i = 0; i < spanlistNum; i++)
    {
        graph[spanlist[i].start][spanlist[i].end] = spanlist[i].distance;
        graph[spanlist[i].end][spanlist[i].start] = spanlist[i].distance;
    }
    for (int i = 0; i < spanlistNum; i++)
    {
        printf("%d %d %lf\n",spanlist[i].start,spanlist[i].end,spanlist[i].distance);
    }
    bool *odd = malloc(sizeof(bool) * nodesNum);
    for (int i = 0; i < nodesNum; i++)
        odd[i] = false;
    findOdd(graph, odd, nodesNum);

    int *oddVertices = malloc(sizeof(int) * nodesNum);
    int oddVerticesNum = 0;
    for (int i = 0; i < nodesNum; i++)
    {
        if (odd[i] == true)
        {
            oddVertices[oddVerticesNum] = i;
            oddVerticesNum++;
        }
    }

    int *vis = malloc(sizeof(int) * nodesNum);
    struct UAV *uav1 = malloc(sizeof(struct UAV) * nodesNum);
    for (int i = 0; i < nodesNum; i++)
    {
        vis[i] = 0;
        uav1[i].nodenum = 0;
        uav1[i].cost = 0;
        uav1[i].path = malloc(sizeof(int) * nodesNum);
        for (int j = 0; j < nodesNum; j++)
            uav1[i].path[j] = 0;
    }
    int uavnum1 = 0;
    int *path1 = malloc(sizeof(int) * nodesNum);
    int j = 0;
    for (int i = 0; i < nodesNum; i++)
    {
        if (!vis[i])
        {
            DFS(graph, i, vis, nodesNum, path1, &j);
        }
    }

    double *pathlen1 = malloc(sizeof(double) * nodesNum);
    for (int i = 1; i < nodesNum; i++)
    {
        pathlen1[i - 1] = Distance(nodes[path1[i - 1]].CoordinateX, nodes[path1[i - 1]].CoordinateY, nodes[path1[i]].CoordinateX, nodes[path1[i]].CoordinateY);
    }

    uav1[uavnum1].path[0] = 0;
    uav1[uavnum1].nodenum++;
    for (int i = 1; i < nodesNum; i++)
    {
        if (uav1[uavnum1].cost + pathlen1[i - 1] + Distance(nodes[uav1[uavnum1].path[0]].CoordinateX, nodes[uav1[uavnum1].path[0]].CoordinateY, nodes[path1[i]].CoordinateX, nodes[path1[i]].CoordinateY) <= B)
        {
            uav1[uavnum1].path[uav1[uavnum1].nodenum] = path1[i];
            uav1[uavnum1].nodenum++;
            uav1[uavnum1].cost += pathlen1[i - 1];
        }
        else
        {
            uavnum1++;
            uav1[uavnum1].path[uav1[uavnum1].nodenum] = path1[i];
            uav1[uavnum1].nodenum++;
        }
    }

    perfectMatching(nodes, oddVerticesNum, oddVertices, graph);
    struct UAV *uav2 = malloc(sizeof(struct UAV) * nodesNum);
    for (int i = 0; i < nodesNum; i++)
    {
        vis[i] = 0;
        uav2[i].nodenum = 0;
        uav2[i].cost = 0;
        uav2[i].path = malloc(sizeof(int) * nodesNum);
        for (int j = 0; j < nodesNum; j++)
            uav2[i].path[j] = 0;
    }
    int uavnum2 = 0;
    int *path2 = malloc(sizeof(int) * nodesNum);
    int k = 0;
    for (int i = 0; i < nodesNum; i++)
    {
        if (!vis[i])
        {
            DFS(graph, i, vis, nodesNum, path2, &k);
        }
    }

    double *pathlen2 = malloc(sizeof(double) * nodesNum);
    for (int i = 1; i < nodesNum; i++)
    {
        pathlen2[i - 1] = Distance(nodes[path2[i - 1]].CoordinateX, nodes[path2[i - 1]].CoordinateY, nodes[path2[i]].CoordinateX, nodes[path2[i]].CoordinateY);
    }

    uav2[uavnum2].path[0] = 0;
    uav2[uavnum2].nodenum++;
    for (int i = 1; i < nodesNum; i++)
    {
        if (uav2[uavnum2].cost + pathlen2[i - 1] + Distance(nodes[uav2[uavnum2].path[0]].CoordinateX, nodes[uav2[uavnum2].path[0]].CoordinateY, nodes[path2[i]].CoordinateX, nodes[path2[i]].CoordinateY) <= B)
        {
            uav2[uavnum2].path[uav2[uavnum2].nodenum] = path2[i];
            uav2[uavnum2].nodenum++;
            uav2[uavnum2].cost += pathlen2[i - 1];
        }
        else
        {
            uavnum2++;
            uav2[uavnum2].path[uav2[uavnum2].nodenum] = path2[i];
            uav2[uavnum2].nodenum++;
        }
    }
    if (uavnum1 < uavnum2)
    {
        printf("%d\n", uavnum1 + 1);
        for (int i = 0; i <= uavnum1; i++)
        {
            printf("%d ", i);
            for (int j = 0; j < uav1[i].nodenum; j++)
            {
                printf("%d ", uav1[i].path[j]);
            }
            printf("\n");
        }
    }
    else
    {
        printf("%d\n", uavnum2 + 1);
        for (int i = 0; i <= uavnum2; i++)
        {
            printf("%d ", i);
            for (int j = 0; j < uav2[i].nodenum; j++)
            {
                printf("%d ", uav2[i].path[j]);
            }
            printf("\n");
        }
    }

    free(nodes);
    free(Tnodesdis);
    free(nodesdis);
    free(belongs);
    free(spanlist);
    for (int i = 0; i < nodesNum; i++)
    {
        free(graph[i]);
    }
    free(graph);
    free(vis);
    for (int i = 0; i < nodesNum; i++)
    {
        free(uav1[i].path);
    }
    free(uav1);
    free(pathlen1);
    for (int i = 0; i < nodesNum; i++)
    {
        free(uav2[i].path);
    }
    free(uav2);
    free(pathlen2);
    free(odd);
}
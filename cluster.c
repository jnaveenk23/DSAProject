#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

// Define structures and constants
struct obj_t {
    int id;
    float x;
    float y;
};

struct cluster_t {
    int size;
    int capacity;
    struct obj_t *obj;
};

const int CLUSTER_CHUNK = 10;

// Function prototypes
void init_cluster(struct cluster_t *c, int cap);
void clear_cluster(struct cluster_t *c);
struct cluster_t *resize_cluster(struct cluster_t *c, int new_cap);
int load_clusters(char *filename, struct cluster_t **arr);
int parse_args(int argc, char *argv[], char **filename, int *N);

// Function definitions
void init_cluster(struct cluster_t *c, int cap) {
    assert(c != NULL);
    assert(cap >= 0);

    c->size = 0;
    c->capacity = cap;

    if (cap != 0)
        c->obj = malloc(cap * sizeof(struct obj_t));
}

void clear_cluster(struct cluster_t *c) {
    free(c->obj);
    c->size = 0;
    c->capacity = 0;
}

struct cluster_t *resize_cluster(struct cluster_t *c, int new_cap) {
    assert(c);
    assert(c->capacity >= 0);
    assert(new_cap >= 0);

    if (c->capacity >= new_cap)
        return c;

    size_t size = sizeof(struct obj_t) * new_cap;

    void *arr = realloc(c->obj, size);
    if (arr == NULL)
        return NULL;

    c->obj = (struct obj_t*)arr;
    c->capacity = new_cap;
    return c;
}

int load_clusters(char *filename, struct cluster_t **arr) {
    assert(arr != NULL);

    FILE *file = fopen(filename, "r");
    if (file == NULL)
        return -3;

    int size;
    if (fscanf(file, "count=%d", &size) != 1){
         return -2;
     }

    *arr = malloc(size * sizeof(struct cluster_t));

    for (int i = 0; i < size; i++)
    {
        init_cluster(&(*arr)[i], 1);
        struct obj_t obj;
        int check = fscanf(file, "%d %f %f", &obj.id, &obj.x, &obj.y);
        if (check != 3) {
            return -2;
        }

        if (!((obj.id == (int)obj.id) && (obj.x == (int)obj.x) && (obj.y == (int)obj.y))) {
            for (int error = 0; error <= i; error++){
                clear_cluster(&(*arr)[error]);
            }
            size = -2;
            break;
        }

        append_cluster(&(*arr)[i], obj);
    }
    fclose(file);

    return size;
}

int parse_args(int argc, char *argv[], char **filename, int *N) {
    *filename = argv[1];

    if (argc == 2) {
        *N = 1;
    }
    else if (argc < 2 || argc > 3){
        return -1;
    }
    else
    {
        *N = atoi(argv[2]);

        float f_N = atof(argv[2]);
        if (f_N != *N){
            return -1;
        }

        if (*N < 0)
            return -1;

        if (*N == 0){
            return -2;
        }

    }

    return 0;
}

int main(int argc, char *argv[]) {
    struct cluster_t *clusters;

    char *filename;
    int N;
    int check = parse_args(argc, argv, &filename, &N);

    if (check == -1 || check == -2){
        fprintf(stderr, "Error: Invalid arguments.\n");
        return 1;
    }

    int size = load_clusters(filename, &clusters);

    if (size == -1 || size == -2 || size == -3) {
        fprintf(stderr, "Error: Unable to load clusters from file.\n");
        return 1;
    }

    for (int i = 0; i < size; i++) {
        free(clusters[i].obj);
    }

    free(clusters);

    return 0;
}

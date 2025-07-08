#ifndef GOOFYLIB3_H
#define GOOFYLIB3_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

typedef struct {
    GLuint VAO, VBO, EBO;
    size_t MAX_MESHES;

    size_t meshCount;
    size_t* vertexOffsets;
    size_t* indexOffsets;
    size_t* vertexCounts;
    size_t* indexCounts;

    size_t currentVerticeOffset;
    size_t currentIndiceOffset;
} GOOFY_BUFFER;

typedef struct {
    float position[3];
    float colors[3];
    float normals[3];
    float texCoords[2];
    int texIndex; 
    unsigned int is3d;
} GOOFY_VERTICE;

typedef struct {
    GOOFY_VERTICE* vertices;
    size_t vertexCount;
    unsigned int* indices;
    size_t indexCount;
} GOOFY_MESH;

typedef enum {
    GOOFY_TRASH_MESH,
    GOOFY_TRASH_BUFFER
} GOOFY_TRASH;

typedef struct {
    GOOFY_TRASH type;
    void* item;
} GOOFY_TRASH_ITEM;

typedef struct {
    int maxSize;
    int trashCount;
    GOOFY_TRASH_ITEM* items;
} GOOFY_TRASH_BATCH;

// MAIN MODULE
GLFWwindow* goofy_initWindow(const char* windowName, short width, short height, char major_version, char minor_version);
GLuint goofy_initShaders(const char* vertexPath,const char* fragmentPath);
GOOFY_BUFFER goofy_initBuffer(GLuint maxVertices, GLuint maxIndices, size_t maxMeshes);
void goofy_renderMesh(GOOFY_BUFFER* buffer, GOOFY_MESH* mesh, GLuint shaderProgram);
void goofy_drawAllMeshes(GOOFY_BUFFER* buffer, GLuint shaderProgram);
void goofy_terminate();

// FILE-LOADING functions
GOOFY_MESH goofy_objMesh(const char* filepath);

// TEXTURE-RELATED functions
void goofy_initTextures(int textureWidth, int textureHeight, int numLayers);
int goofy_loadTexture(const char* path, int layerIndex);

// MESH-MODIFICATION functions
void goofy_transformMesh(GOOFY_MESH* mesh, float x, float y, float z);

// MEMORY-RELATED functions
void goofy_reallocateMesh(GOOFY_MESH* mesh, size_t newVertexCapacity, size_t newIndexCapacity);
GOOFY_MESH goofy_appendMesh(const GOOFY_MESH* a,const GOOFY_MESH* b);
void goofy_freeBuffer(GOOFY_BUFFER* buffer);
void goofy_freeMesh(GOOFY_MESH* mesh);
void goofy_initTrashBatch(GOOFY_TRASH_BATCH* batch,int maxSize,char autoClean);
void goofy_addToTrash(GOOFY_TRASH_BATCH* batch, GOOFY_TRASH type, void* item);
void goofy_clearTrash(GOOFY_TRASH_BATCH* batch);
void goofy_freeTrashBatch(GOOFY_TRASH_BATCH* batch);
GOOFY_MESH goofy_appendMesh(const GOOFY_MESH* a,const GOOFY_MESH* b);

// PRIMITIVE-CREATION functions
GOOFY_MESH goofy_cubeMesh(float x,float y,float z,unsigned int texture,float width,float height,float length,float r,float g,float b,float tX,float tY);

// MISCELLANEOUS functions
void goofy_printMesh(GOOFY_MESH* mesh);

#endif

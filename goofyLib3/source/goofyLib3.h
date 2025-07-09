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
} GOOFY_BUFFER; // can be (probably with context sharing)

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
} GOOFY_MESH; // can be shared between windows

typedef struct {
    GLuint textureArray;
    int numLayers;
    int currentLayers;
} GOOFY_TEXTURE_ARRAY; // cant be shared between windows

typedef enum {
    GOOFY_TRASH_MESH,
    GOOFY_TRASH_BUFFER,
    GOOFY_TRASH_TEXTURE_ARRAY
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

GLFWwindow* goofy_initWindow(const char* windowName, int width, int height, int major_version, int minor_version);
// Initializes a OpenGL window and context. Returns a pointer to a window, inputs are the name of window, width of window, height of window, major OpenGL version, minor OpenGL version
GLuint goofy_initShaders(const char* vertexPath,const char* fragmentPath);
// Initializes shaders from a vertex shader and fragment shader glsl file. Returns ID of shader. Inputs are filepaths of each shader file.
GOOFY_BUFFER goofy_initBuffer(GLuint maxVertices, GLuint maxIndices, size_t maxMeshes);
// Returns a GOOFY_BUFFER object for drawing. Returns the GOOFY_BUFFER object. MaxVertices determines how many vertices can fit in the buffer. Maxindices determine the max indices (vertex ids) which are stored. MaxMeshes are each independent mesh.
// YOU CAN SHARE BUFFERS BETWEEN WINDOWS
void goofy_renderMesh(GOOFY_BUFFER* buffer, GOOFY_MESH* mesh);
// Adds a mesh to a GOOFY_BUFFER rendering queue. Inputs are the GOOFY_BUFFER to add to, and the mesh pointer, and the shader ID.
void goofy_drawAllMeshes(GOOFY_BUFFER* buffer, GLuint shaderProgram,GOOFY_TEXTURE_ARRAY* textureArray);
// Draws all meshes within a GOOFY_BUFFER object.
void goofy_terminate();
// Deletes all GOOFY_TRASH_BATCH items and terminates stuff. YOU NEED TO RUN THIS AT THE END OF PROGRAM

// FILE-LOADING functions

GOOFY_MESH goofy_objMesh(const char* filepath);
// Loads an OBJ file of this format (v/vt/vn), MUST be that format.

// TEXTURE-RELATED functions

GOOFY_TEXTURE_ARRAY goofy_initTextures(int textureWidth, int textureHeight, int numLayers);
// Initializes the 3D texture array. You can imagine as a cube where each slice is a "Layer", the width and height of image can be the height and width of a cube.
// TEXTURES CANNOT BE SHARED BETWEEN WINDOWS (unless u use context sharing)
GLuint goofy_loadTexture(const char* path, int layerIndex,GOOFY_TEXTURE_ARRAY* textureArray);
// Loads a texture to the 3D Texture array.

// MESH-MODIFICATION functions
void goofy_transformMesh(GOOFY_MESH* mesh, float x, float y, float z);
// Transforms a mesh by adding the X,Y,Z values.
void goofy_resizeMesh(GOOFY_MESH* mesh, float x, float y, float z);
// resizes a mesh.
void goofy_setMeshColor(GOOFY_MESH* mesh, float r,float g,float b);
// sets a mesh color.
void goofy_setMeshTexture(GOOFY_MESH* mesh, int texIndex);
// Sets a mesh texture.
void goofy_rotateMesh(GOOFY_MESH* mesh, float angleRad, float axisX, float axisY, float axisZ);
// Rotates a mesh along an axis.

// MEMORY-RELATED functions
GOOFY_MESH* goofy_cloneMesh(const GOOFY_MESH* original);
// Lets you clone a mesh.

void goofy_reallocateMesh(GOOFY_MESH* mesh, size_t newVertexCapacity, size_t newIndexCapacity);
// Allows you to expand a meshes size.

GOOFY_MESH goofy_appendMesh(const GOOFY_MESH* a,const GOOFY_MESH* b);
// Appends the Mesh B data to Mesh A. This can be used for mesh editing.

void goofy_freeBuffer(GOOFY_BUFFER* buffer);
// Frees a buffer.

void goofy_freeMesh(GOOFY_MESH* mesh);
// Frees a mesh.

void goofy_freeTextureArray(GOOFY_TEXTURE_ARRAY* textureArray);
// Frees a texture array.

GOOFY_TRASH_BATCH goofy_initTrashBatch(int maxSize,char autoClean);
// Initializes a GOOFY_TRASH_BATCH instance.

void goofy_addToTrash(GOOFY_TRASH_BATCH* batch, GOOFY_TRASH type, void* item);
// Adds an item to a trash instance.

void goofy_clearTrash(GOOFY_TRASH_BATCH* batch);
// Empties items in a trash batch.

void goofy_freeTrashBatch(GOOFY_TRASH_BATCH* batch);
// Completely destroys a trash batch.

// PRIMITIVE-CREATION functions
GOOFY_MESH goofy_cubeMesh(float x,float y,float z,float width,float height,float length,float tX,float tY);
// Creates a simple cube mesh.
GOOFY_MESH* goofy_createSphere(float radius, unsigned int sectorCount, unsigned int stackCount);
// creates a sphere

// MISCELLANEOUS functions

void goofy_printMesh(GOOFY_MESH* mesh);
// Prints the data of a mesh.

#endif

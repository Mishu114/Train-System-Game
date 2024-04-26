
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <random>

#include "shader.h"
#include "camera.h"
#include "basic_camera.h"
#include "pointLight.h"
#include "directionLight.h"
#include "sphere.h"
#include "spotLight.h"
#include "cube.h"
#include "stb_image.h"
#include "cylinder.h"
#include "Model.h"
#include "Sphere2.h"

#include <iostream>

using namespace std;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);
void drawCube(unsigned int& cubeVAO, Shader& lightingShader, glm::mat4 model, float r, float g, float b);
unsigned int loadTexture(char const* path, GLenum textureWrappingModeS, GLenum textureWrappingModeT, GLenum textureFilteringModeMin, GLenum textureFilteringModeMax);

// settings
const unsigned int SCR_WIDTH = 1024;
const unsigned int SCR_HEIGHT = 856;

// modelling transform
float rotateAngle_X = 0.0;
float rotateAngle_Y = 0.0;
float rotateAngle_Z = 0.0;
float rotateAxis_X = 0.0;
float rotateAxis_Y = 0.0;
float rotateAxis_Z = 1.0;
float translate_X = 0.0;
float translate_Y = 0.0;
float translate_Z = 0.0;
float scale_X = 1.0;
float scale_Y = 1.0;
float scale_Z = 1.0;


//initial values

// camera
float camera_z = 0.0f;
Camera camera(glm::vec3(0.0f, 32.0f, -17.0f));
//Camera camera(glm::vec3(0.0f, 1.2f, 40.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

float eyeX = 0.0, eyeY = 10.0, eyeZ = 1.0;
float lookAtX = 0.0, lookAtY = 10.0, lookAtZ = 0.0;
glm::vec3 V = glm::vec3(0.0f, 1.0f, 0.0f);
BasicCamera basic_camera(eyeX, eyeY, eyeZ, lookAtX, lookAtY, lookAtZ, V);
SpotLight spotlight(
    0.0f, 1.0f, -3.0f,
    0.0f, -1.0f, 0.0f,
    .0f, .0f, .0f,
    .0f, .0f, .0f,
    0.0f, 0.0f, 0.0f,
    30.0f,
    1.0f,   //k_c
    0.09f,  //k_l
    0.032f //k_q
);

DirectionLight directlight(
    0.0f, 50.0f, 0.0f,
    1.0f, 1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f,   //k_c
    0.09f,  //k_l
    0.032f //k_q
);

// positions of the point lights
glm::vec3 pointLightPositions[] = {
    glm::vec3(0.0f,  5.0f,  20.0f),
    glm::vec3(7.0f,  30.0f,  5.0f),
    glm::vec3(5.0f,  30.0f,  10.0f),
    glm::vec3(0.0f,  30.0f,  15.0f),
    glm::vec3(3.0f,  30.0f,  20.0f),
    glm::vec3(3.0f,  30.0f,  25.0f),
    glm::vec3(0.0f, 35.0f, -20.0f),
    glm::vec3(0.0f, 35.0f, -20.0f),
    glm::vec3(-6.0f,  30.0f,  40.0f),
    glm::vec3(-13.0f,  30.0f,  50.0f),

    glm::vec3(12.0, 15.0, 15.0),
    glm::vec3(12.0, 15.0, 15.0),
    glm::vec3(-1.0f, 30.0f,  10.0f),
    glm::vec3(-7.0f, 30.0f,  15.0f),
    glm::vec3(-3.0f, 30.0f,  20.0f),
    glm::vec3(-18.5, 15.0, 15.0),
    glm::vec3(-18.5, 15.0, 15.0),
    glm::vec3(-7.0f, 30.0f,  35.0f),
    glm::vec3(-13.0f, 30.0f,  40.0f),
    glm::vec3(-20.0f, 30.0f,  50.0f),

};
PointLight pointlight1(

    pointLightPositions[0].x, pointLightPositions[0].y, pointLightPositions[0].z,  // position
    0.05f, 0.05f, 0.05f,     // ambient
    0.8f, 0.8f, 0.8f,     // diffuse
    1.0f, 1.0f, 1.0f,        // specular
    1.0f,   //k_c
    0.09f,  //k_l
    0.012f, //k_q
    1       // light number
);
PointLight pointlight2(

    pointLightPositions[1].x, pointLightPositions[1].y, pointLightPositions[1].z,  // position
    0.05f, 0.05f, 0.05f,     // ambient
    0.8f, 0.8f, 0.8f,     // diffuse
    1.0f, 1.0f, 1.0f,        // specular
    1.0f,   //k_c
    0.09f,  //k_l
    0.010f, //k_q
    2       // light number
);
PointLight pointlight3(

    pointLightPositions[2].x, pointLightPositions[2].y, pointLightPositions[2].z,  // position
    0.05f, 0.05f, 0.05f,     // ambient
    0.8f, 0.8f, 0.8f,     // diffuse
    1.0f, 1.0f, 1.0f,        // specular
    1.0f,   //k_c
    0.09f,  //k_l
    0.010f, //k_q
    3       // light number
);
PointLight pointlight4(

    pointLightPositions[3].x, pointLightPositions[3].y, pointLightPositions[3].z,  // position
    0.05f, 0.05f, 0.05f,     // ambient
    0.8f, 0.8f, 0.8f,     // diffuse
    1.0f, 1.0f, 1.0f,        // specular
    1.0f,   //k_c
    0.09f,  //k_l
    0.010f, //k_q
    4       // light number
);

PointLight pointlight5(

    pointLightPositions[4].x, pointLightPositions[4].y, pointLightPositions[4].z,  // position
    0.05f, 0.05f, 0.05f,     // ambient
    0.8f, 0.8f, 0.8f,     // diffuse
    1.0f, 1.0f, 1.0f,        // specular
    1.0f,   //k_c
    0.09f,  //k_l
    0.010f, //k_q
    5       // light number
);
PointLight pointlight6(

    pointLightPositions[5].x, pointLightPositions[5].y, pointLightPositions[5].z,  // position
    0.05f, 0.05f, 0.05f,     // ambient
    0.8f, 0.8f, 0.8f,     // diffuse
    1.0f, 1.0f, 1.0f,        // specular
    1.0f,   //k_c
    0.09f,  //k_l
    0.010f, //k_q
    6       // light number
);

PointLight pointlight7(

    pointLightPositions[6].x, pointLightPositions[6].y, pointLightPositions[6].z,  // position
    0.05f, 0.05f, 0.05f,     // ambient
    0.8f, 0.8f, 0.8f,     // diffuse
    1.0f, 1.0f, 1.0f,        // specular
    1.0f,   //k_c
    0.09f,  //k_l
    0.010f, //k_q
    7       // light number
);

PointLight pointlight8(

    pointLightPositions[7].x, pointLightPositions[7].y, pointLightPositions[7].z,  // position
    0.05f, 0.05f, 0.05f,     // ambient
    0.8f, 0.8f, 0.8f,     // diffuse
    1.0f, 1.0f, 1.0f,        // specular
    1.0f,   //k_c
    0.09f,  //k_l
    0.010f, //k_q
    8      // light number
);

PointLight pointlight9(

    pointLightPositions[8].x, pointLightPositions[8].y, pointLightPositions[8].z,  // position
    0.05f, 0.05f, 0.05f,     // ambient
    0.8f, 0.8f, 0.8f,     // diffuse
    1.0f, 1.0f, 1.0f,        // specular
    1.0f,   //k_c
    0.09f,  //k_l
    0.010f, //k_q
    9      // light number
);
PointLight pointlight10(

    pointLightPositions[9].x, pointLightPositions[9].y, pointLightPositions[9].z,  // position
    0.05f, 0.05f, 0.05f,     // ambient
    0.8f, 0.8f, 0.8f,     // diffuse
    1.0f, 1.0f, 1.0f,        // specular
    1.0f,   //k_c
    0.09f,  //k_l
    0.010f, //k_q
    10      // light number
);
PointLight pointlight11(

    pointLightPositions[10].x, pointLightPositions[10].y, pointLightPositions[10].z,  // position
    0.05f, 0.05f, 0.05f,     // ambient
    0.8f, 0.8f, 0.8f,     // diffuse
    1.0f, 1.0f, 1.0f,        // specular
    1.0f,   //k_c
    0.09f,  //k_l
    0.005f, //k_q
    11      // light number
);
PointLight pointlight12(

    pointLightPositions[11].x, pointLightPositions[11].y, pointLightPositions[11].z,  // position
    0.05f, 0.05f, 0.05f,     // ambient
    0.8f, 0.8f, 0.8f,     // diffuse
    1.0f, 1.0f, 1.0f,        // specular
    1.0f,   //k_c
    0.09f,  //k_l
    0.005f, //k_q
    12      // light number
);

PointLight pointlight13(

    pointLightPositions[12].x, pointLightPositions[12].y, pointLightPositions[12].z,  // position
    0.05f, 0.05f, 0.05f,     // ambient
    0.8f, 0.8f, 0.8f,     // diffuse
    1.0f, 1.0f, 1.0f,        // specular
    1.0f,   //k_c
    0.09f,  //k_l
    0.010f, //k_q
    13      // light number
);
PointLight pointlight14(

    pointLightPositions[13].x, pointLightPositions[13].y, pointLightPositions[13].z,  // position
    0.05f, 0.05f, 0.05f,     // ambient
    0.8f, 0.8f, 0.8f,     // diffuse
    1.0f, 1.0f, 1.0f,        // specular
    1.0f,   //k_c
    0.09f,  //k_l
    0.010f, //k_q
    14      // light number
);
PointLight pointlight15(

    pointLightPositions[14].x, pointLightPositions[14].y, pointLightPositions[14].z,  // position
    0.05f, 0.05f, 0.05f,     // ambient
    0.8f, 0.8f, 0.8f,     // diffuse
    1.0f, 1.0f, 1.0f,        // specular
    1.0f,   //k_c
    0.09f,  //k_l
    0.010f, //k_q
    15     // light number
);
PointLight pointlight16(

    pointLightPositions[15].x, pointLightPositions[15].y, pointLightPositions[15].z,  // position
    0.05f, 0.05f, 0.05f,     // ambient
    0.8f, 0.8f, 0.8f,     // diffuse
    1.0f, 1.0f, 1.0f,        // specular
    1.0f,   //k_c
    0.09f,  //k_l
    0.010f, //k_q
    16      // light number
);
PointLight pointlight17(

    pointLightPositions[16].x, pointLightPositions[16].y, pointLightPositions[16].z,  // position
    0.05f, 0.05f, 0.05f,     // ambient
    0.8f, 0.8f, 0.8f,     // diffuse
    1.0f, 1.0f, 1.0f,        // specular
    1.0f,   //k_c
    0.09f,  //k_l
    0.010f, //k_q
    17      // light number
);
PointLight pointlight18(

    pointLightPositions[17].x, pointLightPositions[17].y, pointLightPositions[17].z,  // position
    0.05f, 0.05f, 0.05f,     // ambient
    0.8f, 0.8f, 0.8f,     // diffuse
    1.0f, 1.0f, 1.0f,        // specular
    1.0f,   //k_c
    0.09f,  //k_l
    0.010f, //k_q
    18     // light number
);

PointLight pointlight19(

    pointLightPositions[18].x, pointLightPositions[18].y, pointLightPositions[18].z,  // position
    0.05f, 0.05f, 0.05f,     // ambient
    0.8f, 0.8f, 0.8f,     // diffuse
    1.0f, 1.0f, 1.0f,        // specular
    1.0f,   //k_c
    0.09f,  //k_l
    0.010f, //k_q
    19      // light number
);

PointLight pointlight20(

    pointLightPositions[19].x, pointLightPositions[19].y, pointLightPositions[19].z,  // position
    0.05f, 0.05f, 0.05f,     // ambient
    0.8f, 0.8f, 0.8f,     // diffuse
    1.0f, 1.0f, 1.0f,        // specular
    1.0f,   //k_c
    0.09f,  //k_l
    0.010f, //k_q
    20      // light number
);


// light settings
bool pointLightOn = true;
bool directionLighton = true;
bool spotlighton = true;
bool ambientToggle = true;
bool diffuseToggle = true;
bool specularToggle = true;

bool onTrack = true;

// timing
float deltaTime = 0.0f;    // time between current frame and last frame
float lastFrame = 0.0f;
float speed = 0.003;
int life = 3;
int level = 3;
bool t1_ON = false, t2_ON = false, t3_ON = false, t4_ON = false;
bool start = false;
bool GameFinished = false;
bool gameOver = false;

int main()
{
    
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "CSE 4208: Computer Graphics Laboratory", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetKeyCallback(window, key_callback);
    //glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // tell GLFW to capture our mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);

    // build and compile our shader zprogram
    // ------------------------------------
    Shader lightingShader("vertexShaderForPhongShading.vs", "fragmentShaderForPhongShading.fs");
    //Shader lightingShader("vertexShaderForGouraudShading.vs", "fragmentShaderForGouraudShading.fs");
    Shader ourShader("vertexShader.vs", "fragmentShader.fs");

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------

    float cube_vertices[] = {
        // positions      // normals
        0.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f,
        1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f,
        1.0f, 1.0f, 0.0f, 0.0f, 0.0f, -1.0f,
        0.0f, 1.0f, 0.0f, 0.0f, 0.0f, -1.0f,

        1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
        1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f,
        1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,

        0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
        0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,

        0.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, -1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,

        1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f,
        1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f,

        0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f,
        1.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f,
        1.0f, 0.0f, 1.0f, 0.0f, -1.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f, -1.0f, 0.0f
    };
    unsigned int cube_indices[] = {
        0, 3, 2,
        2, 1, 0,

        4, 5, 7,
        7, 6, 4,

        8, 9, 10,
        10, 11, 8,

        12, 13, 14,
        14, 15, 12,

        16, 17, 18,
        18, 19, 16,

        20, 21, 22,
        22, 23, 20
    };

    unsigned int cubeVAO, cubeVBO, cubeEBO;
    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &cubeVBO);
    glGenBuffers(1, &cubeEBO);

    glBindVertexArray(cubeVAO);

    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices), cube_vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cubeEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_indices), cube_indices, GL_STATIC_DRAW);


    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // vertex normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)12);
    glEnableVertexAttribArray(1);

    // second, configure the light's VAO (VBO stays the same; the vertices are the same for the light object which is also a 3D cube)
    unsigned int lightCubeVAO;
    glGenVertexArrays(1, &lightCubeVAO);
    glBindVertexArray(lightCubeVAO);

    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cubeEBO);
    // note that we update the lamp's position attribute's stride to reflect the updated buffer data
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    Sphere sphere = Sphere();
    Sphere point = Sphere();
    point.setRadius(0.001f);
    float inc2 = 0.1f;
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    Shader lightingShaderWithTexture("vertexShaderForPhongShadingWithTexture.vs", "fragmentShaderForPhongShadingWithTexture.fs");
    

    //-----------Load Texture---------//
    
    string sandPath = "field.jpg";
    string r1Path = "rail.png";
    string r2Path = "rail_turn.png";
    string woodPath = "wood.jpg";
    string stopRPath = "stop_r.png";
    string stopGPath = "stop_g.png";
    string stopBPath = "stop_b.png";
    string stopYPath = "stop_y.png";
    string goPath = "game_over.jpg";
    string l1Path = "life1.png";
    string l2Path = "life2.png";
    string l3Path = "life3.png";
    string finishPath = "finished.jpg";
    string startPath = "start_text.png";
    string t1SignPath = "t1.png";
    string t2SignPath = "t2.png";
    string t3SignPath = "t3.png";
    string t4SignPath = "t4.png";

    unsigned int sandMap = loadTexture(sandPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int r1Map = loadTexture(r1Path.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int r2Map = loadTexture(r2Path.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int woodMap = loadTexture(woodPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int stopRMap = loadTexture(stopRPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int stopGMap = loadTexture(stopGPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int stopBMap = loadTexture(stopBPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int stopYMap = loadTexture(stopYPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int goMap = loadTexture(goPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int l1Map = loadTexture(l1Path.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int l2Map = loadTexture(l2Path.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int l3Map = loadTexture(l3Path.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int finishMap = loadTexture(finishPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int StartMap = loadTexture(startPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int t1SignMap = loadTexture(t1SignPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int t2SignMap = loadTexture(t2SignPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int t3SignMap = loadTexture(t3SignPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int t4SignMap = loadTexture(t4SignPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);



    Cube sand = Cube(sandMap, sandMap, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);
    Cube rail = Cube(r1Map, r1Map, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);
    Cube rail_turn = Cube(r2Map, r2Map, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);
    Cube wood = Cube(woodMap, woodMap, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);
    Cube stopR = Cube(stopRMap, stopRMap, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);
    Cube stopG = Cube(stopGMap, stopGMap, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);
    Cube stopB = Cube(stopBMap, stopBMap, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);
    Cube stopY = Cube(stopYMap, stopYMap, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);
    Cube GO = Cube(goMap, goMap, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);
    Cube life1 = Cube(l1Map, l1Map, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);
    Cube life2 = Cube(l2Map, l2Map, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);
    Cube life3 = Cube(l3Map, l3Map, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);
    Cube finish = Cube(finishMap, finishMap, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);
    Cube startText = Cube(StartMap, StartMap, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);
    Cube t1Sign = Cube(t1SignMap, t1SignMap, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);
    Cube t2Sign = Cube(t2SignMap, t2SignMap, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);
    Cube t3Sign = Cube(t3SignMap, t3SignMap, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);
    Cube t4Sign = Cube(t4SignMap, t4SignMap, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    //collected coins

    string numPath = "digits/0.png";
    unsigned int diffNum = loadTexture(numPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int specNum = loadTexture(numPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube zero = Cube(diffNum, specNum, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    numPath = "digits/1.png";
    diffNum = loadTexture(numPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    specNum = loadTexture(numPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube one = Cube(diffNum, specNum, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    numPath = "digits/2.png";
    diffNum = loadTexture(numPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    specNum = loadTexture(numPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube two = Cube(diffNum, specNum, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    numPath = "digits/3.png";
    diffNum = loadTexture(numPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    specNum = loadTexture(numPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube three = Cube(diffNum, specNum, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    numPath = "digits/4.png";
    diffNum = loadTexture(numPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    specNum = loadTexture(numPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube four = Cube(diffNum, specNum, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    numPath = "digits/5.png";
    diffNum = loadTexture(numPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    specNum = loadTexture(numPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube five = Cube(diffNum, specNum, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);


    numPath = "digits/6.png";
    diffNum = loadTexture(numPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    specNum = loadTexture(numPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube six = Cube(diffNum, specNum, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);


    numPath = "digits/7.png";
    diffNum = loadTexture(numPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    specNum = loadTexture(numPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube seven = Cube(diffNum, specNum, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);


    numPath = "digits/8.png";
    diffNum = loadTexture(numPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    specNum = loadTexture(numPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube eight = Cube(diffNum, specNum, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);


    numPath = "digits/9.png";
    diffNum = loadTexture(numPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    specNum = loadTexture(numPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube nine = Cube(diffNum, specNum, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    Cube score_block[10] = { zero,one,two,three,four,five,six,seven,eight,nine };



    

    stbi_set_flip_vertically_on_load(true);
    Shader modelShader("1.model_loading.vs", "1.model_loading.fs");

    Model car("maruti_car/untitled.obj");
    Model train1("train/scene.gltf");
    Model train2("train2/scene.gltf");
    Model train3("train3/scene.gltf");
    Model train4("train4/scene.gltf");
    Model love("love/untitled.obj");
    //ourShader.use();
    //lightingShader.use();

    // render loop
    // -----------

    std::random_device rd;  // Seed for the random number engine
    std::mt19937 gen(rd());  // Mersenne Twister engine
    std::uniform_real_distribution<float> dis(-2.0f,2.0f);
    float randomValue = dis(gen);


    //-20.0, 0.5f, 20.0f
    float t1_x = -20.0, t1_y = 0.5f, t1_z = 20.0f, t1_ang = 0.0f, t1_speed=0.05f, t1_stopTime=0.0f, t1_reached =false;
    float t2_x = 20.0, t2_y = 0.5f, t2_z = 20.0f, t2_ang = 0.0f, t2_speed = 0.05f, t2_stopTime = 0.0f, t2_reached = false;
    float t3_x = 0.0, t3_y = 0.5f, t3_z = 40.0f, t3_ang = -90.0f, t3_speed = 0.05f, t3_stopTime = 0.0f, t3_reached = false;
    float t4_x = 0.0, t4_y = 0.5f, t4_z = 0.0f, t4_ang = 0.0f, t4_speed = 0.05f, t4_stopTime = 0.0f, t4_reached = false;

    
    bool crashed = false;
    float time = 0.0f;
    int level1_time = 12;
    int level2_time = 20;
    int level3_time = 20;
    int l3_time = 20;
    
    Sphere health = Sphere();
    health.diffuse = glm::vec3(1.0, 0.0, 0.0);

    bool down = true;
    bool frot = true;
    //camera.Position = glm::vec3(0.0, 0.0, 50.0);
    while (!glfwWindowShouldClose(window))
    {
        
        // per-frame time logic
        // --------------------
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        

        //camera.ProcessKeyboard(BACKWARD, deltaTime);
        // be sure to activate shader when setting uniforms/drawing objects
        lightingShader.use();
        lightingShader.setVec3("viewPos", camera.Position);

        // point light 1
        pointlight1.setUpPointLight(lightingShader);
        // point light 2
        pointlight2.setUpPointLight(lightingShader);
        // point light 3
        pointlight3.setUpPointLight(lightingShader);
        // point light 4
        pointlight4.setUpPointLight(lightingShader);
        pointlight5.setUpPointLight(lightingShader);
        pointlight6.setUpPointLight(lightingShader);
        pointlight7.setUpPointLight(lightingShader);
        pointlight8.setUpPointLight(lightingShader);
        pointlight9.setUpPointLight(lightingShader);
        pointlight10.setUpPointLight(lightingShader);
        pointlight11.setUpPointLight(lightingShader);
        pointlight12.setUpPointLight(lightingShader);
        pointlight13.setUpPointLight(lightingShader);
        pointlight14.setUpPointLight(lightingShader);
        pointlight15.setUpPointLight(lightingShader);
        pointlight16.setUpPointLight(lightingShader);
        pointlight17.setUpPointLight(lightingShader);
        pointlight18.setUpPointLight(lightingShader);
        pointlight19.setUpPointLight(lightingShader);
        pointlight20.setUpPointLight(lightingShader);


        directlight.setUpDirectionLight(lightingShader);

        spotlight.setUpSpotLight(lightingShader);

        // activate shader
        lightingShader.use();

        // pass projection matrix to shader (note that in this case it could change every frame)
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        //glm::mat4 projection = glm::ortho(-2.0f, +2.0f, -1.5f, +1.5f, 0.1f, 100.0f);
        lightingShader.setMat4("projection", projection);

        // camera/view transformation
        glm::mat4 view = camera.GetViewMatrix();
        
        //glm::mat4 view = basic_camera.createViewMatrix();
        lightingShader.setMat4("view", view);


        //cout << lookAtX << lookAtY << lookAtZ << endl;
        //cout << camera.Front.x <<"   "<< camera.Front.y << "   " << camera.Front.z << endl;


        // Modelling Transformation
        glm::mat4 identityMatrix = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
        glm::mat4 translateMatrix, rotateXMatrix, rotateYMatrix, rotateZMatrix, scaleMatrix, model;
        translateMatrix = glm::translate(identityMatrix, glm::vec3(translate_X, translate_Y, translate_Z));
        rotateXMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_X), glm::vec3(1.0f, 0.0f, 0.0f));
        rotateYMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_Y), glm::vec3(0.0f, 1.0f, 0.0f));
        rotateZMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_Z), glm::vec3(0.0f, 0.0f, 1.0f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(scale_X, scale_Y, scale_Z));
        model = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;
        lightingShader.setMat4("model", model);

        


        //-------------------------Imported Mesh Model----------------------------//

        modelShader.use();
        // view/projection transformations
        glm::mat4 Mprojection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 Mview = camera.GetViewMatrix();
        modelShader.setMat4("projection", Mprojection);
        modelShader.setMat4("view", Mview);


        /*

        glm::mat4 translateToPivot, translateFromPivot;
        glm::vec4 pivot = glm::vec4(glm::vec3(cx, cy, cz), 1.0f);
        translateToPivot = glm::translate(identityMatrix, glm::vec3(-pivot));
        translateFromPivot = glm::translate(identityMatrix, glm::vec3(pivot));


         rotateYMatrix = glm::rotate(identityMatrix, glm::radians(tmp_ang2), glm::vec3(0.0, 1.0, 0.0));
        model = translateFromPivot * rotateYMatrix * translateToPivot * model ;
        */

        /*
        translateMatrix = glm::translate(identityMatrix, glm::vec3( 0.0, 0.25f, 0.0f)); // translate it down so it's at the center of the scene
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.4f, 0.4f, 0.4f));	// it's a bit too big for our scene, so scale it down
        glm::mat4 rotateYMatrix2 = glm::rotate(identityMatrix, glm::radians(-180.0f), glm::vec3(0.0, 1.0, 0.0));
        model = translateMatrix * rotateYMatrix2 * scaleMatrix;
       
        modelShader.setMat4("model", model);
        car.Draw(modelShader);

        */


        translateMatrix = glm::translate(identityMatrix, glm::vec3(t1_x, 0.5f, t1_z));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(-1.0f, 2.0f, 2.0f));
        rotateXMatrix = glm::rotate(identityMatrix, glm::radians(-90.0f), glm::vec3(1.0, 0.0, 0.0));
        rotateYMatrix = glm::rotate(identityMatrix, glm::radians(t1_ang), glm::vec3(0.0f, 1.0f, 0.0f));
        model = translateMatrix * rotateYMatrix * rotateXMatrix * scaleMatrix;
        modelShader.setMat4("model", model);
        train1.Draw(modelShader);

        translateMatrix = glm::translate(identityMatrix, glm::vec3(t2_x, 0.5f, t2_z));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(1.0f, 2.0f, 2.0f));	
        rotateYMatrix = glm::rotate(identityMatrix, glm::radians(t2_ang), glm::vec3(0.0f, 1.0f, 0.0f));
        model = translateMatrix * rotateYMatrix * rotateXMatrix * scaleMatrix;
        modelShader.setMat4("model", model);
        train2.Draw(modelShader);

        if (level >= 2)
        {
            translateMatrix = glm::translate(identityMatrix, glm::vec3(t3_x, 0.5f, t3_z));
            scaleMatrix = glm::scale(identityMatrix, glm::vec3(1.0f, 2.0f, 2.0f));
            rotateYMatrix = glm::rotate(identityMatrix, glm::radians(t3_ang), glm::vec3(0.0, 1.0, 0.0));
            model = translateMatrix * rotateYMatrix * rotateXMatrix * scaleMatrix;
            modelShader.setMat4("model", model);
            train3.Draw(modelShader);
        }
        
        if (level >= 3)
        {
            translateMatrix = glm::translate(identityMatrix, glm::vec3(t4_x, 0.5f, t4_z));
            scaleMatrix = glm::scale(identityMatrix, glm::vec3(1.0f, 2.0f, 2.0f));
            rotateYMatrix = glm::rotate(identityMatrix, glm::radians(t4_ang), glm::vec3(0.0, 1.0, 0.0));
            model = translateMatrix * rotateYMatrix * rotateXMatrix * scaleMatrix;
            modelShader.setMat4("model", model);
            train4.Draw(modelShader);
        }
        








        lightingShaderWithTexture.use();
        lightingShaderWithTexture.setVec3("viewPos", camera.Position);
        lightingShaderWithTexture.setMat4("projection", projection);
        lightingShaderWithTexture.setMat4("view", view);

        lightingShaderWithTexture.use();
        // point light 1
        pointlight1.setUpPointLight(lightingShaderWithTexture);
        // point light 2
        pointlight2.setUpPointLight(lightingShaderWithTexture);
        // point light 3
        pointlight3.setUpPointLight(lightingShaderWithTexture);
        // point light 4
        pointlight4.setUpPointLight(lightingShaderWithTexture);
        pointlight5.setUpPointLight(lightingShaderWithTexture);
        pointlight6.setUpPointLight(lightingShaderWithTexture);
        pointlight7.setUpPointLight(lightingShaderWithTexture);
        pointlight8.setUpPointLight(lightingShaderWithTexture);
        pointlight9.setUpPointLight(lightingShaderWithTexture);
        pointlight10.setUpPointLight(lightingShaderWithTexture);
        pointlight11.setUpPointLight(lightingShaderWithTexture);
        pointlight12.setUpPointLight(lightingShaderWithTexture);
        pointlight13.setUpPointLight(lightingShaderWithTexture);
        pointlight14.setUpPointLight(lightingShaderWithTexture);
        pointlight15.setUpPointLight(lightingShaderWithTexture);
        pointlight16.setUpPointLight(lightingShaderWithTexture);
        pointlight17.setUpPointLight(lightingShaderWithTexture);
        pointlight18.setUpPointLight(lightingShaderWithTexture);
        pointlight19.setUpPointLight(lightingShaderWithTexture);
        pointlight20.setUpPointLight(lightingShaderWithTexture);

        //base
        translateMatrix = glm::translate(identityMatrix, glm::vec3(-40.0, 0.0, -20.0));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(80.0f, 0.1f, 80.0));
        glm::mat4 modelForBase = translateMatrix * scaleMatrix;
        sand.drawCubeWithTexture(lightingShaderWithTexture, modelForBase);

        //life
        translateMatrix = glm::translate(identityMatrix, glm::vec3(12.0, 10.0, 20.0));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(6.0, 2.0, 0.01));
        rotateXMatrix = glm::rotate(identityMatrix, glm::radians(45.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        glm::mat4 modelForLife = translateMatrix * rotateXMatrix * scaleMatrix;
        if (life == 1)
        {
            life1.drawCubeWithTexture(lightingShaderWithTexture, modelForLife);
        }

        else if (life == 2)
        {
            life2.drawCubeWithTexture(lightingShaderWithTexture, modelForLife);
        }

        else if (life == 3)
        {
            life3.drawCubeWithTexture(lightingShaderWithTexture, modelForLife);
        }

         

        if (GameFinished)
        {
            rotateXMatrix = glm::rotate(identityMatrix, glm::radians(50.0f), glm::vec3(1.0, 0.0, 0.0));
            translateMatrix = glm::translate(identityMatrix, glm::vec3(-5.0, 22.0, -15.0));
            scaleMatrix = glm::scale(identityMatrix, glm::vec3(10.0f, 10.0, 0.5f));
            glm::mat4 modelForFinish = translateMatrix * rotateXMatrix * scaleMatrix;
            finish.drawCubeWithTexture(lightingShaderWithTexture, modelForFinish);
        }
        

        if (life == 0)
        {
            if (level == 1)
            {
                gameOver = true;
                rotateXMatrix = glm::rotate(identityMatrix, glm::radians(50.0f), glm::vec3(1.0, 0.0, 0.0));
                translateMatrix = glm::translate(identityMatrix, glm::vec3(-5.0, 23.5, -16.0));
                scaleMatrix = glm::scale(identityMatrix, glm::vec3(10.0f, 10.0, 0.5f));
                glm::mat4 modelForGO = translateMatrix * rotateXMatrix * scaleMatrix;
                GO.drawCubeWithTexture(lightingShaderWithTexture, modelForGO);
            }
            else
            {
                if (level == 2)
                {
                    level = 1;
                    life = 3;
                    level1_time = 12;
                    t2_ang = 0.0;
                }

                if (level == 3)
                {
                    level = 2;
                    life = 3;
                    level2_time = 20;
                    t2_ang = -90.0;
                    t3_ang = -90.0;
                    down = true;
                    frot = true;
                }
            }
        }



        if (!start)
        {
            rotateXMatrix = glm::rotate(identityMatrix, glm::radians(45.0f), glm::vec3(1.0, 0.0, 0.0));
            translateMatrix = glm::translate(identityMatrix, glm::vec3(-2.5, 0.5, 19.0));
            scaleMatrix = glm::scale(identityMatrix, glm::vec3(5.0, 2.0, 0.05f));
            glm::mat4 modelForStart = translateMatrix * rotateXMatrix * scaleMatrix;
            startText.drawCubeWithTexture(lightingShaderWithTexture, modelForStart);
        }



        translateMatrix = glm::translate(identityMatrix, glm::vec3(22.0, 0.5, 20.0));
        rotateXMatrix = glm::rotate(identityMatrix, glm::radians(45.0f), glm::vec3(1.0, 0.0, 0.0));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(2.0, 2.0, 0.05f));
        glm::mat4 modelForSign = translateMatrix * rotateXMatrix * scaleMatrix;
        t2Sign.drawCubeWithTexture(lightingShaderWithTexture, modelForSign);

        translateMatrix = glm::translate(identityMatrix, glm::vec3(-24.0, 0.5, 20.0));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(2.0, 2.0, 0.05f));
        modelForSign = translateMatrix * rotateXMatrix * scaleMatrix;
        t1Sign.drawCubeWithTexture(lightingShaderWithTexture, modelForSign);

        if(level>=2)
        {
            translateMatrix = glm::translate(identityMatrix, glm::vec3(-1.0,2.0, 42.0));
            scaleMatrix = glm::scale(identityMatrix, glm::vec3(2.0, 2.0, 0.05f));
            modelForSign = translateMatrix * rotateXMatrix * scaleMatrix;
            t3Sign.drawCubeWithTexture(lightingShaderWithTexture, modelForSign);
        }

        if (level >= 3)
        {
            translateMatrix = glm::translate(identityMatrix, glm::vec3(-4.0, 0.5,3.0));
            scaleMatrix = glm::scale(identityMatrix, glm::vec3(2.0, 2.0, 0.05f));
            modelForSign = translateMatrix * rotateXMatrix * scaleMatrix;
            t4Sign.drawCubeWithTexture(lightingShaderWithTexture, modelForSign);
        }



        //-------------For Level 1---------------//

        if (level == 1)
        {
            //-------------rail Line------------//

            //first train

            translateMatrix = glm::translate(identityMatrix, glm::vec3(20.0, 0.0, 19.5));
            scaleMatrix = glm::scale(identityMatrix, glm::vec3(-19.0f, 0.3f, 0.2f));
            glm::mat4 modelForRail = translateMatrix * scaleMatrix;
            drawCube(cubeVAO, lightingShader, modelForRail, 0.020, 0.106, 0.188);

            translateMatrix = glm::translate(identityMatrix, glm::vec3(20.0, 0.0, 20.5));
            scaleMatrix = glm::scale(identityMatrix, glm::vec3(-19.0f, 0.3f, 0.2f));
            modelForRail = translateMatrix * scaleMatrix;
            drawCube(cubeVAO, lightingShader, modelForRail, 0.020, 0.106, 0.188);

            float xx = 20.0f;
            while (xx >= 1.0)
            {
                translateMatrix = glm::translate(identityMatrix, glm::vec3(xx, 0.0, 21.0));
                scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.2f, 0.3f, -2.0f));
                modelForRail = translateMatrix * scaleMatrix;
                drawCube(cubeVAO, lightingShader, modelForRail, 0.004, 0.125, 0.302);

                xx -= 2.0f;
            }

            rotateYMatrix = glm::rotate(identityMatrix, glm::radians(45.0f), glm::vec3(0.0, 1.0, 0.0));
            translateMatrix = glm::translate(identityMatrix, glm::vec3(1.0, 0.0, 21.0));
            scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.2f, 0.3f, -2.0f));
            modelForRail = translateMatrix * rotateYMatrix * scaleMatrix;
            drawCube(cubeVAO, lightingShader, modelForRail, 0.004, 0.125, 0.302);



            float zz = 22.0f;
            while (zz <=40.0f)
            {
                translateMatrix = glm::translate(identityMatrix, glm::vec3(1.0, 0.0, zz));
                scaleMatrix = glm::scale(identityMatrix, glm::vec3(-2.0f, 0.3f, 0.2f));
                modelForRail = translateMatrix * scaleMatrix;
                drawCube(cubeVAO, lightingShader, modelForRail, 0.004, 0.125, 0.302);

                zz += 2.0f;
            }

            translateMatrix = glm::translate(identityMatrix, glm::vec3(0.5, 0.0, 20.5));
            scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.2f, 0.3f, 20.0f));
            modelForRail = translateMatrix * scaleMatrix;
            drawCube(cubeVAO, lightingShader, modelForRail, 0.020, 0.106, 0.188);

            translateMatrix = glm::translate(identityMatrix, glm::vec3(-0.5, 0.0, 20.5));
            scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.2f, 0.3f, 20.0f));
            modelForRail = translateMatrix * scaleMatrix;
            drawCube(cubeVAO, lightingShader, modelForRail, 0.020, 0.106, 0.188);

            translateMatrix = glm::translate(identityMatrix, glm::vec3(1.0, 0.0, 19.5));
            rotateYMatrix = glm::rotate(identityMatrix, glm::radians(35.0f), glm::vec3(0.0, 1.0, 0.0));
            scaleMatrix = glm::scale(identityMatrix, glm::vec3(-2.0f, 0.3f, 0.2f));
            modelForRail = translateMatrix * rotateYMatrix * scaleMatrix;
            drawCube(cubeVAO, lightingShader, modelForRail, 0.020, 0.106, 0.188);

            translateMatrix = glm::translate(identityMatrix, glm::vec3(1.0, 0.0, 40.0));
            scaleMatrix = glm::scale(identityMatrix, glm::vec3(-2.0f, 0.3f, 4.0f));
            modelForRail = translateMatrix * scaleMatrix;
            drawCube(cubeVAO, lightingShader, modelForRail, 0.004, 0.125, 0.302);

            //second train
            translateMatrix = glm::translate(identityMatrix, glm::vec3(-20.0, 0.0, 19.5));
            scaleMatrix = glm::scale(identityMatrix, glm::vec3(19.5f, 0.3f, 0.2f));
            modelForRail = translateMatrix * scaleMatrix;
            drawCube(cubeVAO, lightingShader, modelForRail, 0.0, 0.392, 0.196);

            translateMatrix = glm::translate(identityMatrix, glm::vec3(-20.0, 0.0, 20.5));
            scaleMatrix = glm::scale(identityMatrix, glm::vec3(19.5f, 0.3f, 0.2f));
            modelForRail = translateMatrix * scaleMatrix;
            drawCube(cubeVAO, lightingShader, modelForRail, 0.0, 0.392, 0.196);

            xx = -20.0f;
            while (xx <= -1.0)
            {
                translateMatrix = glm::translate(identityMatrix, glm::vec3(xx, 0.0, 21.0));
                scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.2f, 0.3f, -2.0f));
                modelForRail = translateMatrix * scaleMatrix;
                drawCube(cubeVAO, lightingShader, modelForRail, 0.0, 0.392, 0.196);

                xx += 2.0f;
            }
            rotateYMatrix = glm::rotate(identityMatrix, glm::radians(45.0f), glm::vec3(0.0, 1.0, 0.0));
            translateMatrix = glm::translate(identityMatrix, glm::vec3(-1.0, 0.0, 19.0));
            scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.2f, 0.3f, 2.0f));
            modelForRail = translateMatrix * rotateYMatrix * scaleMatrix;
            drawCube(cubeVAO, lightingShader, modelForRail, 0.0, 0.392, 0.196);

            zz = 18.0f;
            while (zz >= 0.0f)
            {
                translateMatrix = glm::translate(identityMatrix, glm::vec3(1.0, 0.0, zz));
                scaleMatrix = glm::scale(identityMatrix, glm::vec3(-2.0f, 0.3f, 0.2f));
                modelForRail = translateMatrix * scaleMatrix;
                drawCube(cubeVAO, lightingShader, modelForRail, 0.0, 0.392, 0.196);

                zz -= 2.0f;
            }
            translateMatrix = glm::translate(identityMatrix, glm::vec3(1.0, 0.0, 0.0));
            scaleMatrix = glm::scale(identityMatrix, glm::vec3(-2.0f, 0.3f, -4.0f));
            modelForRail = translateMatrix * scaleMatrix;
            drawCube(cubeVAO, lightingShader, modelForRail, 0.0, 0.392, 0.196);

            translateMatrix = glm::translate(identityMatrix, glm::vec3(0.5, 0.0, 19.5));
            scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.2f, 0.3f, -20.0f));
            modelForRail = translateMatrix * scaleMatrix;
            drawCube(cubeVAO, lightingShader, modelForRail, 0.0, 0.392, 0.196);

            translateMatrix = glm::translate(identityMatrix, glm::vec3(-0.5, 0.0, 19.5));
            scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.2f, 0.3f, -20.0f));
            modelForRail = translateMatrix * scaleMatrix;
            drawCube(cubeVAO, lightingShader, modelForRail, 0.0, 0.392, 0.196);

            translateMatrix = glm::translate(identityMatrix, glm::vec3(0.9, 0.0, 19.4));
            rotateYMatrix = glm::rotate(identityMatrix, glm::radians(35.0f), glm::vec3(0.0, 1.0, 0.0));
            scaleMatrix = glm::scale(identityMatrix, glm::vec3(-2.0f, 0.3f, 0.2f));
            modelForRail = translateMatrix * rotateYMatrix * scaleMatrix;
            drawCube(cubeVAO, lightingShader, modelForRail, 0.0, 0.392, 0.196);

            translateMatrix = glm::translate(identityMatrix, glm::vec3(2.0, 0.0, 22.0));;
            scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.2f, 3.0f, 0.2f));
            glm::mat4 modelForStop = translateMatrix * scaleMatrix;
            wood.drawCubeWithTexture(lightingShaderWithTexture, modelForStop);

            translateMatrix = glm::translate(identityMatrix, glm::vec3(1.0, 3.0, 22.0));;
            scaleMatrix = glm::scale(identityMatrix, glm::vec3(2.0f, 2.0f, 0.01f));
            modelForStop = translateMatrix * scaleMatrix;
            stopB.drawCubeWithTexture(lightingShaderWithTexture, modelForStop);




            float dis = sqrt((t2_x - t1_x) * (t2_x - t1_x) + (t2_z - t1_z) * (t2_z - t1_z));
            if (dis <= 3.5)
            {
                t1_ON = false;
                t2_ON = false;
                start = false;
                t1_x = -20.0,  t1_z = 20.0f, t1_ang = 0.0f;
                t2_x = 20.0,  t2_z = 20.0f, t2_ang = 0.0f;
                t1_reached = false;
                t2_reached = false;
                if(life>0)
                    life--;
                level1_time = 12;
            }



            if (t1_ON)
            {
                if (t1_x <= 0.0f)
                {
                    t1_x += t1_speed;
                }
                else
                {
                    if (t1_ang < 90.0)
                        t1_ang += 2.0f;
                    else
                    {
                        if (t1_z > 1.0)
                            t1_z -= t1_speed;
                        else t1_reached = true;
                    }

                }
            }

            if (t2_ON)
            {
                if (t2_x >= 0.0f)
                {
                    t2_stopTime = currentFrame;
                    t2_x -= t2_speed;

                }
                else
                {
                    if (currentFrame - t2_stopTime > 2.0)
                    {
                        if (t2_ang < 90.0)
                            t2_ang += 2.0f;
                        else
                        {
                            if (t2_z < 39.0)
                                t2_z += t2_speed;
                            else
                                t2_reached = true;
                        }
                    }
                    

                }
            }

            if (t1_reached && t2_reached)
            {
                life = 3;
                level++;
                t1_reached = false;
                t2_reached = false;
                start = false;
                t1_ON = false;
                t2_ON = false;

                t1_x = -20.0, t1_z = 20.0f, t1_ang = 0.0f;
                t2_x = 20.0, t2_z = 20.0f, t2_ang = -90.0f;
            }


            if (start && currentFrame - time>=1.0)
            {
                time = currentFrame;
                if(level1_time>0)
                    level1_time--;
            }

            if (level1_time == 0)
            {
                t1_ON = false;
                t2_ON = false;
                start = false;
                t1_x = -20.0, t1_z = 20.0f, t1_ang = 0.0f;
                t2_x = 20.0, t2_z = 20.0f, t2_ang = 0.0f;
                t1_reached = false;
                t2_reached = false;
                if (life > 0)
                    life--;
                level1_time = 12;
            }
             
            int A[2];
            A[0] = 0;
            A[1] = 0;
            int ff = level1_time;
            int inddd = 1;
            while (ff > 0) {
                A[inddd] = ff % 10;
                ff /= 10;
                inddd--;
            }

            rotateXMatrix = glm::rotate(identityMatrix, glm::radians(45.0f), glm::vec3(1.0, 0.0, 0.0));

            translateMatrix = glm::translate(identityMatrix, glm::vec3(-17.0,10.0, 20.0));
            scaleMatrix = glm::scale(identityMatrix, glm::vec3(1.5f, 1.5, 0.002f));
            glm::mat4 modelForScore1 = translateMatrix * rotateXMatrix* scaleMatrix;
            score_block[A[0]].drawCubeWithTexture(lightingShaderWithTexture, modelForScore1);

            translateMatrix = glm::translate(identityMatrix, glm::vec3(-18.5, 10.0, 20.0));
            scaleMatrix = glm::scale(identityMatrix, glm::vec3(1.5f, 1.5, 0.002f));
            glm::mat4 modelForScore2 = translateMatrix * rotateXMatrix * scaleMatrix;
            score_block[A[1]].drawCubeWithTexture(lightingShaderWithTexture, modelForScore2);

        }

        //-------------For Level 2---------------//
        if (level == 2)
        {
            //-------------------------For train 2-------------------------//
            //-----||------//
            translateMatrix = glm::translate(identityMatrix, glm::vec3(19.5, 0.0, 20.0));;
            scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.2f, 0.3f, -20.0f));
            glm::mat4 modelForRail = translateMatrix * scaleMatrix;
            drawCube(cubeVAO, lightingShader, modelForRail, 0.020, 0.106, 0.188);

            translateMatrix = glm::translate(identityMatrix, glm::vec3(20.5, 0.0, 20.0));;
            scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.2f, 0.3f, -20.0f));
            modelForRail = translateMatrix * scaleMatrix;
            drawCube(cubeVAO, lightingShader, modelForRail, 0.020, 0.106, 0.188);

            float zz = 20.0f;
            while (zz > 0.0)
            {
                translateMatrix = glm::translate(identityMatrix, glm::vec3(19.0, 0.0, zz));;
                scaleMatrix = glm::scale(identityMatrix, glm::vec3(2.0f, 0.3f, 0.2f));
                modelForRail = translateMatrix * scaleMatrix;
                drawCube(cubeVAO, lightingShader, modelForRail, 0.020, 0.106, 0.188);
                zz -= 2.0;
            }

            rotateYMatrix = glm::rotate(identityMatrix, glm::radians(45.0f), glm::vec3(0.0, 1.0, 0.0));
            translateMatrix = glm::translate(identityMatrix, glm::vec3(19.0, 0.0, 1.0));
            scaleMatrix = glm::scale(identityMatrix, glm::vec3(2.0f, 0.3f, 0.2f));
            modelForRail = translateMatrix * rotateYMatrix * scaleMatrix;
            drawCube(cubeVAO, lightingShader, modelForRail, 0.020, 0.106, 0.188);

            //--------- == --------//
            translateMatrix = glm::translate(identityMatrix, glm::vec3(20.0, 0.0, 0.5));;
            scaleMatrix = glm::scale(identityMatrix, glm::vec3(-40.0f, 0.3f, 0.2f));
            modelForRail = translateMatrix * scaleMatrix;
            drawCube(cubeVAO, lightingShader, modelForRail, 0.020, 0.106, 0.188);

            translateMatrix = glm::translate(identityMatrix, glm::vec3(20.5, 0.0, -0.5));;
            scaleMatrix = glm::scale(identityMatrix, glm::vec3(-40.0f, 0.3f, 0.2f));
            modelForRail = translateMatrix * scaleMatrix;
            drawCube(cubeVAO, lightingShader, modelForRail, 0.020, 0.106, 0.188);

            float xx = 18.0f;
            while (xx > -20.0)
            {
                translateMatrix = glm::translate(identityMatrix, glm::vec3(xx, 0.0, 1.0));;
                scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.2f, 0.3f, -2.0f));
                modelForRail = translateMatrix * scaleMatrix;
                drawCube(cubeVAO, lightingShader, modelForRail, 0.020, 0.106, 0.188);
                xx -= 2.0;
            }

            translateMatrix = glm::translate(identityMatrix, glm::vec3(-19.0, 0.0, 1.0));;
            scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.2f, 0.3f, -2.0f));
            modelForRail = translateMatrix * rotateYMatrix * scaleMatrix;
            drawCube(cubeVAO, lightingShader, modelForRail, 0.020, 0.106, 0.188);

            //----------||------------//
            translateMatrix = glm::translate(identityMatrix, glm::vec3(-19.5, 0.0, 0.0));;
            scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.2f, 0.3f, 30.0f));
            modelForRail = translateMatrix * scaleMatrix;
            drawCube(cubeVAO, lightingShader, modelForRail, 0.020, 0.106, 0.188);

            translateMatrix = glm::translate(identityMatrix, glm::vec3(-20.5, 0.0, 0.0));;
            scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.2f, 0.3f, 30.0f));
            modelForRail = translateMatrix * scaleMatrix;
            drawCube(cubeVAO, lightingShader, modelForRail, 0.020, 0.106, 0.188);

            zz = 2.0f;
            while (zz <= 30.0)
            {
                translateMatrix = glm::translate(identityMatrix, glm::vec3(-19.0, 0.0, zz));;
                scaleMatrix = glm::scale(identityMatrix, glm::vec3(-2.0f, 0.3f, 0.2f));
                modelForRail = translateMatrix * scaleMatrix;
                drawCube(cubeVAO, lightingShader, modelForRail, 0.020, 0.106, 0.188);
                zz += 2.0;
            }
            translateMatrix = glm::translate(identityMatrix, glm::vec3(-19.0, 0.0, 30));;
            scaleMatrix = glm::scale(identityMatrix, glm::vec3(-2.0f, 0.3f, 4.0f));
            modelForRail = translateMatrix * scaleMatrix;
            drawCube(cubeVAO, lightingShader, modelForRail, 0.020, 0.106, 0.188);


            //---------------------For train 1-----------------------//
            translateMatrix = glm::translate(identityMatrix, glm::vec3(-20.0, 0.0, 19.5));
            scaleMatrix = glm::scale(identityMatrix, glm::vec3(29.5f, 0.3f, 0.2f));
            modelForRail = translateMatrix * scaleMatrix;
            drawCube(cubeVAO, lightingShader, modelForRail, 0.0, 0.392, 0.196);

            translateMatrix = glm::translate(identityMatrix, glm::vec3(-20.0, 0.0, 20.5));
            scaleMatrix = glm::scale(identityMatrix, glm::vec3(30.0f, 0.3f, 0.2f));
            modelForRail = translateMatrix * scaleMatrix;
            drawCube(cubeVAO, lightingShader, modelForRail, 0.0, 0.392, 0.196);

            xx = -20.0f;
            while (xx <= 9.0)
            {
                translateMatrix = glm::translate(identityMatrix, glm::vec3(xx, 0.0, 21.0));
                scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.2f, 0.3f, -2.0f));
                modelForRail = translateMatrix * scaleMatrix;
                drawCube(cubeVAO, lightingShader, modelForRail, 0.0, 0.392, 0.196);

                xx += 2.0f;
            }
            rotateYMatrix = glm::rotate(identityMatrix, glm::radians(45.0f), glm::vec3(0.0, 1.0, 0.0));
            translateMatrix = glm::translate(identityMatrix, glm::vec3(9.0, 0.0, 19.0));
            scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.2f, 0.3f, 2.0f));
            modelForRail = translateMatrix * rotateYMatrix * scaleMatrix;
            drawCube(cubeVAO, lightingShader, modelForRail, 0.0, 0.392, 0.196);

            zz = 18.0f;
            while (zz >= 0.0f)
            {
                translateMatrix = glm::translate(identityMatrix, glm::vec3(11.0, 0.0, zz));
                scaleMatrix = glm::scale(identityMatrix, glm::vec3(-2.0f, 0.3f, 0.2f));
                modelForRail = translateMatrix * scaleMatrix;
                drawCube(cubeVAO, lightingShader, modelForRail, 0.0, 0.392, 0.196);

                zz -= 2.0f;
            }
            translateMatrix = glm::translate(identityMatrix, glm::vec3(11.0, 0.0, 0.0));
            scaleMatrix = glm::scale(identityMatrix, glm::vec3(-2.0f, 0.3f, -4.0f));
            modelForRail = translateMatrix * scaleMatrix;
            drawCube(cubeVAO, lightingShader, modelForRail, 0.0, 0.392, 0.196);

            translateMatrix = glm::translate(identityMatrix, glm::vec3(10.5, 0.0, 19.5));
            scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.2f, 0.3f, -20.0f));
            modelForRail = translateMatrix * scaleMatrix;
            drawCube(cubeVAO, lightingShader, modelForRail, 0.0, 0.392, 0.196);

            translateMatrix = glm::translate(identityMatrix, glm::vec3(9.5, 0.0, 19.5));
            scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.2f, 0.3f, -20.0f));
            modelForRail = translateMatrix * scaleMatrix;
            drawCube(cubeVAO, lightingShader, modelForRail, 0.0, 0.392, 0.196);


            //--------------------For Train 3--------------------//
            translateMatrix = glm::translate(identityMatrix, glm::vec3(0.5, 0.0, 40.0));
            scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.2f, 0.3f, -40.0f));
            modelForRail = translateMatrix * scaleMatrix;
            drawCube(cubeVAO, lightingShader, modelForRail, 0.545, 0.0, 0.0);

            translateMatrix = glm::translate(identityMatrix, glm::vec3(-0.5, 0.0, 40.0));
            scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.2f, 0.3f, -40.0f));
            modelForRail = translateMatrix * scaleMatrix;
            drawCube(cubeVAO, lightingShader, modelForRail, 0.545, 0.0, 0.0);

            zz = 40.0f;
            while (zz >= 0.0)
            {
                translateMatrix = glm::translate(identityMatrix, glm::vec3(1.0, 0.0, zz));
                scaleMatrix = glm::scale(identityMatrix, glm::vec3(-2.0f, 0.3f, 0.2f));
                modelForRail = translateMatrix * scaleMatrix;
                drawCube(cubeVAO, lightingShader, modelForRail, 0.545, 0.0, 0.0);
                zz -= 2.0f;
            }
            translateMatrix = glm::translate(identityMatrix, glm::vec3(1.0, 0.0, 0.0));
            scaleMatrix = glm::scale(identityMatrix, glm::vec3(-2.0f, 0.3f, -4.0f));
            modelForRail = translateMatrix * scaleMatrix;
            drawCube(cubeVAO, lightingShader, modelForRail, 0.545, 0.0, 0.0);



            if (t1_ON)
            {
                if (t1_x <= 10.0)
                    t1_x += t1_speed;
                else
                {
                    if (t1_ang < 90.0)
                        t1_ang += 2.0f;
                    else
                    {
                        if (t1_z > 1.0)
                            t1_z -= t1_speed;
                        else t1_reached = true;
                    }
                }
            }

            if (!t3_reached && t3_ON)
            {
                if (t3_z > 1.0)
                    t3_z -= t3_speed;
                else
                    t3_reached = true;
            }

            if (!t2_reached && t2_ON)
            {
                if (down && t2_z >= 0.0)
                {
                    t2_z -= t2_speed;
                }
                else
                {
                    down = false;
                    if (frot && t2_ang < 0.0)
                        t2_ang += 2.0f;
                    else
                    {
                        frot = false;
                        if (t2_x > -20.0)
                            t2_x -= t1_speed;
                        else
                        {
                            if (t2_ang < 90.0)
                                t2_ang += 2.0f;
                            else
                            {
                                if (t2_z <= 29.0)
                                    t2_z += t2_speed;
                                else
                                    t2_reached = true;
                            }
                        }
                    }
                }
            }


            float dis12 = sqrt((t2_x - t1_x) * (t2_x - t1_x) + (t2_z - t1_z) * (t2_z - t1_z));
            float dis13 = sqrt((t3_x - t1_x) * (t3_x - t1_x) + (t3_z - t1_z) * (t3_z - t1_z));
            float dis23 = sqrt((t2_x - t3_x) * (t2_x - t3_x) + (t2_z - t3_z) * (t2_z - t3_z));

            if (dis12 < 3.5 || dis23 < 3.5 || dis13 < 3.5)
            {
                t1_ON = false;
                t2_ON = false;
                t3_ON = false;
                start = false;
                t1_reached = false;
                t2_reached = false;
                t3_reached = false;

                t1_x = -20.0, t1_z = 20.0f, t1_ang = 0.0f;
                t2_x = 20.0, t2_z = 20.0f, t2_ang = -90.0f;
                t3_x = 0.0, t3_z = 40.0f, t3_ang = -90.0f;;
                down = true;
                frot = true;
                if (life > 0)
                    life--;
                level2_time = 20;
            }

            if (t1_reached && t2_reached && t3_reached)
            {
                life = 3;
                level++;
                t1_reached = false;
                t2_reached = false;
                t3_reached = false;
                start = false;
                t1_ON = false;
                t2_ON = false;
                t3_ON = false;

                t1_x = -20.0, t1_z = 20.0f, t1_ang = 0.0f;
                t2_x = 20.0, t2_z = 20.0f, t2_ang = 0.0f;
                t3_x = 0.0, t3_z = 40.0f, t3_ang = -180.0f;
                t4_x = 0.0, t4_z = 0.0f, t4_ang = 0.0f;
            }


            if (start && currentFrame - time >= 1.0)
            {
                time = currentFrame;
                if (level2_time > 0)
                    level2_time--;
            }

            if (level2_time == 0)
            {
                t1_ON = false;
                t2_ON = false;
                t3_ON = false;
                down = true;
                frot = true;
                start = false;

                t1_reached = false;
                t2_reached = false;
                t3_reached = false;

                t1_x = -20.0, t1_z = 20.0f, t1_ang = 0.0f;
                t2_x = 20.0, t2_z = 20.0f, t2_ang = -90.0f;
                t3_x = 0.0, t3_z = 40.0f, t3_ang = -90.0f;
                if (life > 0)
                    life--;
                level2_time = 20;
            }

            int A[2];
            A[0] = 0;
            A[1] = 0;
            int ff = level2_time;
            int inddd = 1;
            while (ff > 0) 
            {
                A[inddd] = ff % 10;
                ff /= 10;
                inddd--;
            }

            rotateXMatrix = glm::rotate(identityMatrix, glm::radians(45.0f), glm::vec3(1.0, 0.0, 0.0));

            translateMatrix = glm::translate(identityMatrix, glm::vec3(-17.0, 10.0, 20.0));
            scaleMatrix = glm::scale(identityMatrix, glm::vec3(1.5f, 1.5, 0.002f));
            glm::mat4 modelForScore1 = translateMatrix * rotateXMatrix * scaleMatrix;
            score_block[A[0]].drawCubeWithTexture(lightingShaderWithTexture, modelForScore1);

            translateMatrix = glm::translate(identityMatrix, glm::vec3(-18.5, 10.0, 20.0));
            scaleMatrix = glm::scale(identityMatrix, glm::vec3(1.5f, 1.5, 0.002f));
            glm::mat4 modelForScore2 = translateMatrix * rotateXMatrix * scaleMatrix;
            score_block[A[1]].drawCubeWithTexture(lightingShaderWithTexture, modelForScore2);

        }


        //-----------------Level 3---------------//
        if (level == 3)
        {
            //-------------------------For train 2-------------------------//
            translateMatrix = glm::translate(identityMatrix, glm::vec3(20.0, 0.0, 19.5));;
            scaleMatrix = glm::scale(identityMatrix, glm::vec3(-19.7f, 0.3f, 0.2f));
            glm::mat4 modelForRail = translateMatrix * scaleMatrix;
            drawCube(cubeVAO, lightingShader, modelForRail, 0.020, 0.106, 0.188);

            translateMatrix = glm::translate(identityMatrix, glm::vec3(20.0, 0.0, 20.5));;
            scaleMatrix = glm::scale(identityMatrix, glm::vec3(-20.4f, 0.3f, 0.2f));
            modelForRail = translateMatrix * scaleMatrix;
            drawCube(cubeVAO, lightingShader, modelForRail, 0.020, 0.106, 0.188);

            float xx = 20.0f;
            while (xx > 0.0)
            {
                translateMatrix = glm::translate(identityMatrix, glm::vec3(xx, 0.0, 19.0));;
                scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.2f, 0.3f, 2.0f));
                modelForRail = translateMatrix * scaleMatrix;
                drawCube(cubeVAO, lightingShader, modelForRail, 0.020, 0.106, 0.188);
                xx -= 2.0;
            }

            rotateYMatrix = glm::rotate(identityMatrix, glm::radians(-45.0f), glm::vec3(0.0, 1.0, 0.0));
            translateMatrix = glm::translate(identityMatrix, glm::vec3(1.0, 0.0, 19.0));
            scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.2f, 0.3f, 2.0f));
            modelForRail = translateMatrix * rotateYMatrix * scaleMatrix;
            drawCube(cubeVAO, lightingShader, modelForRail, 0.020, 0.106, 0.188);

            //--------- || --------//
            translateMatrix = glm::translate(identityMatrix, glm::vec3(0.5, 0.0, 20.0));;
            scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.2f, 0.3f, -20.0f));
            modelForRail = translateMatrix * scaleMatrix;
            drawCube(cubeVAO, lightingShader, modelForRail, 0.020, 0.106, 0.188);

            translateMatrix = glm::translate(identityMatrix, glm::vec3(-0.5, 0.0, 20.0));;
            scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.2f, 0.3f, -20.0f));
            modelForRail = translateMatrix * scaleMatrix;
            drawCube(cubeVAO, lightingShader, modelForRail, 0.020, 0.106, 0.188);

            float zz = 18.0f;
            while (zz > 0.0)
            {
                translateMatrix = glm::translate(identityMatrix, glm::vec3(1.0, 0.0, zz));;
                scaleMatrix = glm::scale(identityMatrix, glm::vec3(-2.0f, 0.3f,0.2f));
                modelForRail = translateMatrix * scaleMatrix;
                drawCube(cubeVAO, lightingShader, modelForRail, 0.020, 0.106, 0.188);
                zz -= 2.0;
            }

            translateMatrix = glm::translate(identityMatrix, glm::vec3(1.0, 0.0, 0.0));;
            scaleMatrix = glm::scale(identityMatrix, glm::vec3(-2.0f, 0.3f, -4.0f));
            modelForRail = translateMatrix * scaleMatrix;
            drawCube(cubeVAO, lightingShader, modelForRail, 0.020, 0.106, 0.188);


          

            //---------------------For train 1-----------------------//
            translateMatrix = glm::translate(identityMatrix, glm::vec3(-20.0, 0.0, 19.5));;
            scaleMatrix = glm::scale(identityMatrix, glm::vec3(30.0, 0.3f, 0.2f));
            modelForRail = translateMatrix * scaleMatrix;
            drawCube(cubeVAO, lightingShader, modelForRail, 0.0, 0.392, 0.196);

            translateMatrix = glm::translate(identityMatrix, glm::vec3(-20.0, 0.0, 20.5));;
            scaleMatrix = glm::scale(identityMatrix, glm::vec3(30.0f, 0.3f, 0.2f));
            modelForRail = translateMatrix * scaleMatrix;
            drawCube(cubeVAO, lightingShader, modelForRail, 0.0, 0.392, 0.196);
            
            xx = -19.0f;
            while (xx <= 9.0)
            {
                translateMatrix = glm::translate(identityMatrix, glm::vec3(xx, 0.0, 21.0));
                scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.2f, 0.3f, -2.0f));
                modelForRail = translateMatrix * scaleMatrix;
                drawCube(cubeVAO, lightingShader, modelForRail, 0.0, 0.392, 0.196);

                xx += 2.0f;
            }
            rotateYMatrix = glm::rotate(identityMatrix, glm::radians(-45.0f), glm::vec3(0.0, 1.0, 0.0));
            translateMatrix = glm::translate(identityMatrix, glm::vec3(9.0, 0.0, 21.0));
            scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.2f, 0.3f, -2.0f));
            modelForRail = translateMatrix * rotateYMatrix * scaleMatrix;
            drawCube(cubeVAO, lightingShader, modelForRail, 0.0, 0.392, 0.196);

            
            zz = 22.0f;
            while (zz < 40.0f)
            {
                translateMatrix = glm::translate(identityMatrix, glm::vec3(11.0, 0.0, zz));
                scaleMatrix = glm::scale(identityMatrix, glm::vec3(-2.0f, 0.3f, 0.2f));
                modelForRail = translateMatrix * scaleMatrix;
                drawCube(cubeVAO, lightingShader, modelForRail, 0.0, 0.392, 0.196);

                zz += 2.0f;
            }
            translateMatrix = glm::translate(identityMatrix, glm::vec3(11.0, 0.0, 40.0));
            scaleMatrix = glm::scale(identityMatrix, glm::vec3(-2.0f, 0.3f, 4.0f));
            modelForRail = translateMatrix * scaleMatrix;
            drawCube(cubeVAO, lightingShader, modelForRail, 0.0, 0.392, 0.196);

            translateMatrix = glm::translate(identityMatrix, glm::vec3(10.5, 0.0, 19.5));
            scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.2f, 0.3f, 20.0f));
            modelForRail = translateMatrix * scaleMatrix;
            drawCube(cubeVAO, lightingShader, modelForRail, 0.0, 0.392, 0.196);

            translateMatrix = glm::translate(identityMatrix, glm::vec3(9.5, 0.0, 19.5));
            scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.2f, 0.3f, 20.0f));
            modelForRail = translateMatrix * scaleMatrix;
            drawCube(cubeVAO, lightingShader, modelForRail, 0.0, 0.392, 0.196);

            
            //--------------------For Train 3--------------------//
            translateMatrix = glm::translate(identityMatrix, glm::vec3(0.0, 0.0, 40.5));
            scaleMatrix = glm::scale(identityMatrix, glm::vec3(20.0, 0.3f, 0.2f));
            modelForRail = translateMatrix * scaleMatrix;
            drawCube(cubeVAO, lightingShader, modelForRail, 0.545, 0.0, 0.0);

            translateMatrix = glm::translate(identityMatrix, glm::vec3(0.0, 0.0, 39.5));
            scaleMatrix = glm::scale(identityMatrix, glm::vec3(20.0, 0.3f, 0.2));
            modelForRail = translateMatrix * scaleMatrix;
            drawCube(cubeVAO, lightingShader, modelForRail, 0.545, 0.0, 0.0);

            xx = 0.0;
            while (xx < 20.0)
            {
                translateMatrix = glm::translate(identityMatrix, glm::vec3(xx, 0.0, 39.0));
                scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.2f, 0.3f, 2.0f));
                modelForRail = translateMatrix * scaleMatrix;
                drawCube(cubeVAO, lightingShader, modelForRail, 0.545, 0.0, 0.0);
                xx += 2.0f;
            }
            rotateYMatrix = glm::rotate(identityMatrix, glm::radians(45.0f), glm::vec3(0.0, 1.0, 0.0));
            translateMatrix = glm::translate(identityMatrix, glm::vec3(19.0, 0.0, 39.0));
            scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.2f, 0.3f, 2.0f));
            modelForRail = translateMatrix * rotateYMatrix * scaleMatrix;
            drawCube(cubeVAO, lightingShader, modelForRail, 0.545, 0.0, 0.0);

            translateMatrix = glm::translate(identityMatrix, glm::vec3(20.5, 0.0, 40.5));
            scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.2, 0.3f, -30.5));
            modelForRail = translateMatrix * scaleMatrix;
            drawCube(cubeVAO, lightingShader, modelForRail, 0.545, 0.0, 0.0);

            translateMatrix = glm::translate(identityMatrix, glm::vec3(19.5, 0.0, 39.5));
            scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.2, 0.3f, -29.5));
            modelForRail = translateMatrix * scaleMatrix;
            drawCube(cubeVAO, lightingShader, modelForRail, 0.545, 0.0, 0.0);

            zz = 38.0;
            while (zz >= 10.0)
            {
                translateMatrix = glm::translate(identityMatrix, glm::vec3(19, 0.0, zz));
                scaleMatrix = glm::scale(identityMatrix, glm::vec3(2.0f, 0.3f, 0.2f));
                modelForRail = translateMatrix * scaleMatrix;
                drawCube(cubeVAO, lightingShader, modelForRail, 0.545, 0.0, 0.0);
                zz -= 2.0f;
            }

            translateMatrix = glm::translate(identityMatrix, glm::vec3(19, 0.0, 10.0));
            scaleMatrix = glm::scale(identityMatrix, glm::vec3(2.0f, 0.3f, -4.0f));
            modelForRail = translateMatrix * scaleMatrix;
            drawCube(cubeVAO, lightingShader, modelForRail, 0.545, 0.0, 0.0);


            //--------------------For Train 4--------------------//
            
            translateMatrix = glm::translate(identityMatrix, glm::vec3(0.0, 0.0, 0.5));
            scaleMatrix = glm::scale(identityMatrix, glm::vec3(-9.0, 0.3f, 0.2));
            modelForRail = translateMatrix * scaleMatrix;
            drawCube(cubeVAO, lightingShader, modelForRail, 0.722, 0.525, 0.043);

            translateMatrix = glm::translate(identityMatrix, glm::vec3(0.0, 0.0, -0.5));
            scaleMatrix = glm::scale(identityMatrix, glm::vec3(-10.0, 0.3f, 0.2));
            modelForRail = translateMatrix * scaleMatrix;
            drawCube(cubeVAO, lightingShader, modelForRail, 0.722, 0.525, 0.043);

            xx = 0.0;
            while (xx > -10.0)
            {
                translateMatrix = glm::translate(identityMatrix, glm::vec3(xx, 0.0, 1.0));
                scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.2f, 0.3f, -2.0f));
                modelForRail = translateMatrix * scaleMatrix;
                drawCube(cubeVAO, lightingShader, modelForRail, 0.722, 0.525, 0.043);
                xx -= 2.0f;
            }
            rotateYMatrix = glm::rotate(identityMatrix, glm::radians(45.0f), glm::vec3(0.0, 1.0, 0.0));
            translateMatrix = glm::translate(identityMatrix, glm::vec3(-9.0, 0.0, 1.0));
            scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.2f, 0.3f, -2.0f));
            modelForRail = translateMatrix * rotateYMatrix * scaleMatrix;
            drawCube(cubeVAO, lightingShader, modelForRail, 0.722, 0.525, 0.043);


            translateMatrix = glm::translate(identityMatrix, glm::vec3(-10.5, 0.0, -0.5));
            scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.2, 0.3f, 40.5));
            modelForRail = translateMatrix * scaleMatrix;
            drawCube(cubeVAO, lightingShader, modelForRail, 0.722, 0.525, 0.043);

            translateMatrix = glm::translate(identityMatrix, glm::vec3(-9.5, 0.0, 0.5));
            scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.2, 0.3f, 39.5));
            modelForRail = translateMatrix * scaleMatrix;
            drawCube(cubeVAO, lightingShader, modelForRail, 0.722, 0.525, 0.043);

            zz = 2.0;
            while (zz < 40.0)
            {
                translateMatrix = glm::translate(identityMatrix, glm::vec3(-9.0, 0.0, zz));
                scaleMatrix = glm::scale(identityMatrix, glm::vec3(-2.0f, 0.3f, 0.2f));
                modelForRail = translateMatrix * scaleMatrix;
                drawCube(cubeVAO, lightingShader, modelForRail, 0.722, 0.525, 0.043);
                zz += 2.0f;
            }
            translateMatrix = glm::translate(identityMatrix, glm::vec3(-9.0, 0.0, 40.0));
            scaleMatrix = glm::scale(identityMatrix, glm::vec3(-2.0f, 0.3f, 4.0f));
            modelForRail = translateMatrix * scaleMatrix;
            drawCube(cubeVAO, lightingShader, modelForRail, 0.722, 0.525, 0.043);
            
            //stop sign
            translateMatrix = glm::translate(identityMatrix, glm::vec3(8.0, 0.0, 22.0));;
            scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.2f, 3.0f, 0.2f));
            glm::mat4 modelForStop = translateMatrix * scaleMatrix;
            wood.drawCubeWithTexture(lightingShaderWithTexture, modelForStop);

            translateMatrix = glm::translate(identityMatrix, glm::vec3(7.0, 3.0, 22.0));;
            scaleMatrix = glm::scale(identityMatrix, glm::vec3(2.0f, 2.0f, 0.01f));
            modelForStop = translateMatrix * scaleMatrix;
            stopG.drawCubeWithTexture(lightingShaderWithTexture, modelForStop);


            if (!t1_reached && t1_ON)
            {
                if (t1_x <= 10.0)
                {
                    t1_stopTime = currentFrame;
                    t1_x += t1_speed;
                }
                else
                {
                    if (currentFrame - t1_stopTime > 3.0)
                    {
                        if (t1_ang > -90.0)
                            t1_ang -= 2.0f;
                        else
                        {
                            if (t1_z < 38.0)
                                t1_z += t1_speed;
                            else t1_reached = true;
                        }
                    }
                }
            }
            
            

            if (!t3_reached && t3_ON)
            {
                if (t3_x < 20.0)
                    t3_x += t3_speed;
                else
                {
                    if (t3_ang < -90.0)
                        t3_ang += 2.0f;
                    else
                    {
                        if (t3_z > 10.0)
                            t3_z -= t3_speed;
                        else t3_reached = true;
                    }
                }
            }


            if (!t4_reached && t4_ON)
            {
                if (t4_x > -10.0)
                    t4_x -= t4_speed;
                else
                {
                    if (t4_ang < 90.0)
                        t4_ang += 2.0f;
                    else
                    {
                        if (t4_z < 38.0)
                            t4_z += t4_speed;
                        else t4_reached = true;
                    }
                }
            }

            if (!t2_reached && t2_ON)
            {
                if (t2_x > 0.0)
                    t2_x -= t4_speed;
                else
                {
                    if (t2_ang > -90.0)
                        t2_ang -= 2.0f;
                    else
                    {
                        if (t2_z > 1.0)
                            t2_z -= t2_speed;
                        else t2_reached = true;
                    }
                }
            }
           


            float dis12 = sqrt((t2_x - t1_x) * (t2_x - t1_x) + (t2_z - t1_z) * (t2_z - t1_z));
            float dis13 = sqrt((t3_x - t1_x) * (t3_x - t1_x) + (t3_z - t1_z) * (t3_z - t1_z));
            float dis14 = sqrt((t4_x - t1_x) * (t4_x - t1_x) + (t4_z - t1_z) * (t4_z - t1_z));
            float dis23 = sqrt((t2_x - t3_x) * (t2_x - t3_x) + (t2_z - t3_z) * (t2_z - t3_z));
            float dis24 = sqrt((t2_x - t4_x) * (t2_x - t4_x) + (t2_z - t4_z) * (t2_z - t4_z));
            float dis34 = sqrt((t4_x - t3_x) * (t4_x - t3_x) + (t4_z - t3_z) * (t4_z - t3_z));

            if (dis12 < 3.5 || dis23 < 3.5 || dis13 < 3.5 || dis14<3.5|| dis24 < 3.5 || dis34 < 3.5)
            {
                t1_ON = false;
                t2_ON = false;
                t3_ON = false;
                t4_ON = false;

                start = false;
                t1_reached = false;
                t2_reached = false;
                t3_reached = false;
                t4_reached = false;

                t1_x = -20.0, t1_z = 20.0f, t1_ang = 0.0f;
                t2_x = 20.0, t2_z = 20.0f, t2_ang = 0.0f;
                t3_x = 0.0, t3_z = 40.0f, t3_ang = -180.0f;
                t4_x = 0.0, t4_z = 0.0f, t4_ang = 0.0f;
                if (life > 0)
                    life--;
                level3_time = l3_time;
            }

            if (t1_reached && t2_reached && t3_reached && t4_reached)
            {
                if (l3_time == 20)
                {
                    l3_time = 16;

                    t1_ON = false;
                    t2_ON = false;
                    t3_ON = false;
                    t4_ON = false;

                    start = false;
                    t1_reached = false;
                    t2_reached = false;
                    t3_reached = false;
                    t4_reached = false;

                    t1_x = -20.0, t1_z = 20.0f, t1_ang = 0.0f;
                    t2_x = 20.0, t2_z = 20.0f, t2_ang = 0.0f;
                    t3_x = 0.0, t3_z = 40.0f, t3_ang = -180.0f;
                    t4_x = 0.0, t4_z = 0.0f, t4_ang = 0.0f;

                    life = 3;
                    level3_time = l3_time;
                }
                    
                else
                    GameFinished = true;
            }


            if (start && currentFrame - time >= 1.0)
            {
                time = currentFrame;
                if (level3_time > 0)
                    level3_time--;
            }

            if (level3_time == 0)
            {
                t1_ON = false;
                t2_ON = false;
                t3_ON = false;
                t4_ON = false;
                start = false;

                t1_reached = false;
                t2_reached = false;
                t3_reached = false;
                t4_reached = false;

                t1_x = -20.0, t1_z = 20.0f, t1_ang = 0.0f;
                t2_x = 20.0, t2_z = 20.0f, t2_ang = 0.0f;
                t3_x = 0.0, t3_z = 40.0f, t3_ang = -180.0f;
                t4_x = 0.0, t4_z = 0.0f, t4_ang = 0.0f;
                if (life > 0)
                    life--;
                level3_time = l3_time;
            }

            int A[2];
            A[0] = 0;
            A[1] = 0;
            int ff = level3_time;
            int inddd = 1;
            while (ff > 0)
            {
                A[inddd] = ff % 10;
                ff /= 10;
                inddd--;
            }

            rotateXMatrix = glm::rotate(identityMatrix, glm::radians(45.0f), glm::vec3(1.0, 0.0, 0.0));

            translateMatrix = glm::translate(identityMatrix, glm::vec3(-17.0, 10.0, 20.0));
            scaleMatrix = glm::scale(identityMatrix, glm::vec3(1.5f, 1.5, 0.002f));
            glm::mat4 modelForScore1 = translateMatrix * rotateXMatrix * scaleMatrix;
            score_block[A[0]].drawCubeWithTexture(lightingShaderWithTexture, modelForScore1);

            translateMatrix = glm::translate(identityMatrix, glm::vec3(-18.5, 10.0, 20.0));
            scaleMatrix = glm::scale(identityMatrix, glm::vec3(1.5f, 1.5, 0.002f));
            glm::mat4 modelForScore2 = translateMatrix * rotateXMatrix * scaleMatrix;
            score_block[A[1]].drawCubeWithTexture(lightingShaderWithTexture, modelForScore2);
        }


        
        


        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &cubeVAO);
    glDeleteVertexArrays(1, &lightCubeVAO);
    glDeleteBuffers(1, &cubeVBO);
    glDeleteBuffers(1, &cubeEBO);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

void drawCube(unsigned int& cubeVAO, Shader& lightingShader, glm::mat4 model = glm::mat4(1.0f), float r = 1.0f, float g = 1.0f, float b = 1.0f)
{
    lightingShader.use();
    lightingShader.setVec3("material.ambient", glm::vec3(r, g, b));
    lightingShader.setVec3("material.diffuse", glm::vec3(r, g, b));
    lightingShader.setVec3("material.specular", glm::vec3(0.5f, 0.5f, 0.5f));
    lightingShader.setFloat("material.shininess", 22.0f);

    lightingShader.setMat4("model", model);

    glBindVertexArray(cubeVAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
}


void func(unsigned int& cubeVAO, Shader& lightingShader,  Cube base)
{
    float baseHeight = 0.1;
    float width = 80;
    float length = 100;

    //base-1
    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 translate = glm::mat4(1.0f);
    glm::mat4 scale = glm::scale(model, glm::vec3(width, baseHeight, length));
    translate = glm::translate(model, glm::vec3(-40.5, 0, -5.5));
    model = translate * scale;
    base.drawCubeWithTexture(lightingShader, model);

}



// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    
    if (!gameOver && !GameFinished && glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS) {

        start = true;
    }

}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{   
    if (!gameOver && !GameFinished && start && key == GLFW_KEY_1 && action == GLFW_PRESS)
    {
        t1_ON = true;
    }
    else if (!gameOver && !GameFinished && start && key == GLFW_KEY_2 && action == GLFW_PRESS)
    {
        t2_ON = true;
        
    }
    else if (level>1 && !gameOver && !GameFinished && start && key == GLFW_KEY_3 && action == GLFW_PRESS)
    {
        t3_ON = true;
    }
    else if (level > 2 && !gameOver && !GameFinished && start && key == GLFW_KEY_4 && action == GLFW_PRESS)
    {
        t4_ON = true;
    }

    else if ((gameOver || GameFinished) &&  key == GLFW_KEY_0 && action == GLFW_PRESS)
    {
        gameOver = false;
        GameFinished = false;
        level = 1;
        life = 3;
    }
    

}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}


// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}

unsigned int loadTexture(char const* path, GLenum textureWrappingModeS, GLenum textureWrappingModeT, GLenum textureFilteringModeMin, GLenum textureFilteringModeMax)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, textureWrappingModeS);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, textureWrappingModeT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, textureFilteringModeMin);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, textureFilteringModeMax);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}
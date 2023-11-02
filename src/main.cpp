#include <algorithm>
#include <memory>
#include <vector>
#include <iostream>

#include <GLFW/glfw3.h>
#define GLAD_GL_IMPLEMENTATION
#include <glad/gl.h>
#undef GLAD_GL_IMPLEMENTATION
#include <glm/glm.hpp>

#include "camera.h"
#include "opengl_context.h"
#include "utils.h"

#define ANGLE_TO_RADIAN(x) (float)((x)*M_PI / 180.0f) 
#define RADIAN_TO_ANGEL(x) (float)((x)*180.0f / M_PI) 

#define CIRCLE_SEGMENT 64

#define ROTATE_SPEED 1.0f
#define FLYING_SPEED ROTATE_SPEED / 20.f

#define RED 0.905f, 0.298f, 0.235f
#define BLUE 0.203f, 0.596f, 0.858f
#define GREEN 0.18f, 0.8f, 0.443f


void resizeCallback(GLFWwindow* window, int width, int height) {
  OpenGLContext::framebufferResizeCallback(window, width, height);
  auto ptr = static_cast<Camera*>(glfwGetWindowUserPointer(window));
  if (ptr) {
    ptr->updateProjectionMatrix(OpenGLContext::getAspectRatio());
  }
}

// 定義子彈的位置和速度
bool bulletFired = 0;
int bulletDist = 0;
int temp = 0;



  // 全局變數
float wingSwingAngle = 0.0f; 
float airplaneHeight = 2.0f;
float airplaneRotationY = 0.0f;
float airplaneY = 0.0f;
float airplaneX = 0.0f;
float rotationY = 0.0f;
float airplaneWingRotation = 0.0f;
float targetHeight = 0.0f;
float ascentSpeed = 0.01f;
float decentSpeed = 0.001f;
float swingSpeed = 1.0f;
float range = 20.0f;
float front = 0.0f;
bool flag = 0;
float rotationSpeed = 0.5f;

void keyCallback(GLFWwindow* window, int key, int, int action, int) {
  // There are three actions: press, release, hold(repeat)
  if (action == GLFW_REPEAT) 
      return;

  // Press ESC to close the window.
  if (key == GLFW_KEY_ESCAPE) {
    glfwSetWindowShouldClose(window, GLFW_TRUE);
    return;
  }
  /* TODO#4-1: Detect key-events, perform rotation or fly
   *       1. Use switch && case to find the key you want.
   *       2. Press "SPACE" for fly up, fly forward and wing rotate meanwhile. 
   *       3. Press "GLFW_KEY_LEFT" for turn left.
   *       4. Press "GLFW_KEY_RIGHT " for turn right.
   * Hint:
   *       glfw3's key list (https://www.glfw.org/docs/3.3/group__keys.html)
   *       glfw3's action codes (https://www.glfw.org/docs/3.3/group__input.html#gada11d965c4da13090ad336e030e4d11f)
   * Note:
   *       You should finish rendering your airplane first.
   *       Otherwise you will spend a lot of time debugging this with a black screen.
   */



  switch (key) {
    case GLFW_KEY_SPACE:

       // 按住空格鍵時執行飛行操作
       // 增加飛機的高度

       targetHeight += 3;
       airplaneWingRotation += 80;
       front += 1.5;
       flag = 1;

      break;


    case GLFW_KEY_LEFT:
        // 按下左箭頭鍵時執行向左轉的操作
        // 在這裡執行相應的操作，可以修改飛機的姿態
        // 例如向左旋轉飛機
        rotationY -= 5.0f;
        // 其他向左轉操作
        break;
    case GLFW_KEY_RIGHT:
        // 按下右箭頭鍵時執行向右轉的操作
        // 在這裡執行相應的操作，可以修改飛機的姿態
        // 例如向右旋轉飛機
        rotationY += 5.0f;
        // 其他向右轉操作
        break;
    case GLFW_KEY_G:
      temp += 3;
      if (bulletDist == 0) {
        bulletDist += 30;
      }
      break;


  }
}

void initOpenGL() {
  // Initialize OpenGL context, details are wrapped in class.
#ifdef __APPLE__
  // MacOS need explicit request legacy support
  OpenGLContext::createContext(21, GLFW_OPENGL_ANY_PROFILE);
#else
  //OpenGLContext::createContext(21, GLFW_OPENGL_ANY_PROFILE);
  OpenGLContext::createContext(43, GLFW_OPENGL_COMPAT_PROFILE);
#endif
  GLFWwindow* window = OpenGLContext::getWindow();
  /* TODO#0: Change window title to "HW1 - `your student id`"
   *        Ex. HW1 - 312550000 
   */
  glfwSetWindowTitle(window, "HW1 - 312551132");
  glfwSetKeyCallback(window, keyCallback);
  glfwSetFramebufferSizeCallback(window, resizeCallback);
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
#ifndef NDEBUG
  OpenGLContext::printSystemInfo();
  // This is useful if you want to debug your OpenGL API calls.
  OpenGLContext::enableDebugCallback();
#endif
}

/* TODO#2: Render body, wing, tail
 * Hint:
 *       glBegin/glEnd (https://registry.khronos.org/OpenGL-Refpages/gl2.1/xhtml/glBegin.xml)
 *       glColor3f (https://registry.khronos.org/OpenGL-Refpages/gl2.1/xhtml/glBegin.xml)
 *       glVertex3f (https://registry.khronos.org/OpenGL-Refpages/gl2.1/xhtml/glVertex.xml)
 *       glNormal (https://registry.khronos.org/OpenGL-Refpages/gl2.1/xhtml/glNormal.xml)
 *       glScalef (https://registry.khronos.org/OpenGL-Refpages/gl2.1/xhtml/glScale.xml)
 * Note:
 *       You can refer to ppt "Draw Cylinder" page and `CIRCLE_SEGMENT`
 *       You can use color defined above
 */

void drawUnitCube() {
  float centerX = 0.5f;     // 新的中心點X座標
  float halfLength = 0.5f;  // 正方體的半長度

  float vertices[][3] = {
      {centerX - halfLength, -halfLength, -halfLength}, {centerX + halfLength, -halfLength, -halfLength},
      {centerX + halfLength, halfLength, -halfLength},  {centerX - halfLength, halfLength, -halfLength},
      {centerX - halfLength, -halfLength, halfLength},  {centerX + halfLength, -halfLength, halfLength},
      {centerX + halfLength, halfLength, halfLength},   {centerX - halfLength, halfLength, halfLength}};


  int faces[][4] = {{0, 3, 2, 1}, {2, 3, 7, 6}, {0, 4, 7, 3}, {1, 2, 6, 5}, {4, 5, 6, 7}, {0, 1, 5, 4}};


  glBegin(GL_QUADS);
  for (int i = 0; i < 6; i++) {
    for (int j = 0; j < 4; j++) {
      int vertexIndex = faces[i][j];
      glVertex3fv(vertices[vertexIndex]);
    }
  }
  glEnd();
}




void renderAirplaneWings() {
  // 設置機翼的顏色和變換
  glColor3f(RED);  // 設定顏色，例如紅色

  glPushMatrix();
  glScalef(4.0f, 0.5f, 1.0f);          // 根據需要進行縮放
  drawUnitCube();                      // 渲染長方體
  glPopMatrix();
}

void renderAirplaneBody() {
  const float radius = 0.5f;
  const float height = 4.0f;
  const int segments = 64;
  const float slice = 360.0f / segments;
  glColor3f(BLUE);
  glTranslatef(0.0f, 0.0f, -2.0f);      // 根據需要進行平移
  glRotatef(90.0f, 1.0f, 0.0f, 0.0f); 
  glBegin(GL_QUAD_STRIP);

  for (int i = 0; i <= segments; i++) {
    float angle = slice * i;
    float x = radius * std::cos(ANGLE_TO_RADIAN(angle));
    float z = radius * std::sin(ANGLE_TO_RADIAN(angle));

    // Vertices on the side of the cylinder
    glVertex3f(x, 0.0f, z);
    glVertex3f(x, height, z);
  }

  glEnd();

  // Top and bottom faces
  glBegin(GL_TRIANGLE_FAN);
  glVertex3f(0.0f, 0.0f, 0.0f);  // Center of the bottom face

  for (int i = 0; i <= segments; i++) {
    float angle = slice * i;
    float x = radius * std::cos(ANGLE_TO_RADIAN(angle));
    float z = radius * std::sin(ANGLE_TO_RADIAN(angle));
    glVertex3f(x, 0.0f, z);
  }
  glEnd();

// Reverse the rendering order for the bottom face
  glBegin(GL_TRIANGLE_FAN);
  glVertex3f(0.0f, 4.0f, 0.0f);  // Center of the bottom face

  for (int i = segments; i >= 0 ; i--) {  // 改為遞增
    float angle = slice * i;
    float x = radius * std::cos(ANGLE_TO_RADIAN(angle));
    float z = radius * std::sin(ANGLE_TO_RADIAN(angle));  // 使用 std::sin
    glVertex3f(x, 4.0f, z);
  }
  glEnd();
}



void renderAirplaneTail() {
  const float bottomEdge = 2.0f;
  const float height1 = 1.0f;
  const float height2 = 0.5f;
  glColor3f(GREEN);

  glPushMatrix();  // 保存當前繪圖矩陣
  // 平移到四面體的中心
  //glTranslatef(0.0f, -2.0f, 3.0f);

    glBegin(GL_TRIANGLES);
    // up
    glVertex3f(-1.0f, 0.0f, 0.0f);
    glVertex3f(1.0f, 0.0f, 0.0f);
    glVertex3f(0.0f, 0.5f, 0.0f);

    // down behind
    glVertex3f(1.0f, 0.0f, 0.0f);
    glVertex3f(-1.0f, 0.0f, 0.0f);
    glVertex3f(0.0f, 0.0f, -1.0f);

    // down LEFT
    glVertex3f(0.0f, 0.0f, -1.0f);
    glVertex3f(0.0f, 0.5f, 0.0f);
    glVertex3f(1.0f, 0.0f, 0.0f);



    // down right
    glVertex3f(0.0f, 0.0f, -1.0f);

    glVertex3f(-1.0f, 0.0f, 0.0f);
    glVertex3f(0.0f, 0.5f, 0.0f);



    glEnd();

  glPopMatrix();  // 恢復之前保存的繪圖矩陣
}



float forwardAngle = airplaneRotationY - 90;

void renderAirplane() {
    // 渲染飛機的函數，包括機身、機翼和機尾
    // 渲染機身
    glPushMatrix();
    glTranslatef(airplaneX, airplaneHeight, airplaneY);  // 平移至飛機底部中心，增加高度
    glRotatef(-airplaneRotationY, 0.0f, 1.0f, 0.0f);   // 根據旋轉角度旋轉飛機
    renderAirplaneBody();                             // 渲染飛機機身
    glPopMatrix();

    // 渲染左机翼
    glPushMatrix();
    glTranslatef(airplaneX, airplaneHeight, airplaneY);
    glRotatef(-airplaneRotationY, 0.0f, 1.0f, 0.0f);  // 根據旋轉角度旋轉飛機
    glRotatef(180.0f - wingSwingAngle, 0.0f, 0.0f, 1.0f); 
    renderAirplaneWings();                                
    glPopMatrix();

    // 渲染右机翼
    glPushMatrix();
    glTranslatef(airplaneX, airplaneHeight, airplaneY);
    glRotatef(-airplaneRotationY, 0.0f, 1.0f, 0.0f);  // 根據旋轉角度旋轉飛機
    glRotatef(0.0f + wingSwingAngle, 0.0f, 0.0f, 1.0f); 
    renderAirplaneWings();                                
    glPopMatrix();

  // 計算前進方向的角度
    // 渲染機尾
    glPushMatrix();
    glTranslatef((airplaneX) - (3 * cos(ANGLE_TO_RADIAN(forwardAngle))), airplaneHeight,
                 ((-3) * sin(ANGLE_TO_RADIAN(forwardAngle)) + airplaneY + 0.5));
    glRotatef(-airplaneRotationY, 0.0f, 1.0f, 0.0f);


    glRotatef(180.0f, 0.0f, 0.0f, 1.0f);
    renderAirplaneTail();
    glPopMatrix();
}

float bulletX = (airplaneX) - (3 * cos(ANGLE_TO_RADIAN(forwardAngle - 180)));
float bulletY = ((-3) * sin(ANGLE_TO_RADIAN(forwardAngle)) + airplaneY + 0.5);
float bulletHeight = airplaneHeight;

void renderBullet() {
  const float radius = 0.1f;
  const float height = 0.6f;
  const int segments = 64;
  const float slice = 360.0f / segments;
  glColor3f(RED);
  glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
  glBegin(GL_QUAD_STRIP);

  for (int i = 0; i <= segments; i++) {
    float angle = slice * i;
    float x = radius * std::cos(ANGLE_TO_RADIAN(angle));
    float z = radius * std::sin(ANGLE_TO_RADIAN(angle));

    // Vertices on the side of the cylinder
    glVertex3f(x, 0.0f, z);
    glVertex3f(x, height, z);
  }

  glEnd();

  // Top and bottom faces
  glBegin(GL_TRIANGLE_FAN);
  glVertex3f(0.0f, 0.0f, 0.0f);  // Center of the bottom face

  for (int i = 0; i <= segments; i++) {
    float angle = slice * i;
    float x = radius * std::cos(ANGLE_TO_RADIAN(angle));
    float z = radius * std::sin(ANGLE_TO_RADIAN(angle));
    glVertex3f(x, 0.6f, z);
  }
  glEnd();

  // Reverse the rendering order for the bottom face
  glBegin(GL_TRIANGLE_FAN);
  glVertex3f(0.0f, -0.9f, 0.0f);  // Center of the bottom face

  for (int i = segments; i >= 0; i--) {  // 改為遞增
    float angle = slice * i;
    float x = radius * std::cos(ANGLE_TO_RADIAN(angle));
    float z = radius * std::sin(ANGLE_TO_RADIAN(angle));  // 使用 std::sin
    glVertex3f(x, 0.6f, z);
  }
  glEnd();
}
// 繪製子彈的函數
void drawBullet() {
  glPushMatrix();
  glTranslatef(bulletX, bulletHeight, bulletY);
  glRotatef(-airplaneRotationY, 0.0f, 1.0f, 0.0f);
  renderBullet();
  glEnd();
  glPopMatrix();
}

void light() {
  GLfloat light_specular[] = {0.6, 0.6, 0.6, 1};
  GLfloat light_diffuse[] = {0.6, 0.6, 0.6, 1};
  GLfloat light_ambient[] = {0.4, 0.4, 0.4, 1};
  GLfloat light_position[] = {50.0, 75.0, 80.0, 1.0};
  // z buffer enable
  glEnable(GL_DEPTH_TEST);
  // enable lighting
  glEnable(GL_LIGHTING);
  glShadeModel(GL_SMOOTH);
  glEnable(GL_COLOR_MATERIAL);
  glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
  glEnable(GL_NORMALIZE);
  // set light property
  glEnable(GL_LIGHT0);
  glLightfv(GL_LIGHT0, GL_POSITION, light_position);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
  glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
  glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
}

int main() {
  initOpenGL();
  GLFWwindow* window = OpenGLContext::getWindow();

  // Init Camera helper
  Camera camera(glm::vec3(0, 5, 10));
  camera.initialize(OpenGLContext::getAspectRatio());
  // Store camera as glfw global variable for callbasks use
  glfwSetWindowUserPointer(window, &camera);

  // Main rendering loop
  while (!glfwWindowShouldClose(window)) {
    // Polling events.
    glfwPollEvents();
    // Update camera position and view
    camera.move(window);
    // GL_XXX_BIT can simply "OR" together to use.
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    /// TO DO Enable DepthTest
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    // Projection Matrix
    glMatrixMode(GL_PROJECTION);
    glLoadMatrixf(camera.getProjectionMatrix());
    // ModelView Matrix
    glMatrixMode(GL_MODELVIEW);
    glLoadMatrixf(camera.getViewMatrix());


#ifndef DISABLE_LIGHT   
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glClearDepth(1.0f);
    light();
#endif

    /* TODO#4-2: Update 
     *       You may update position and orientation of airplane here or not.
     *       Feel free to not follow TA's structure. However, don't violate the spec. 
     * 
     * Hint: 
     * Note:
     *       You can use `ROTATE_SPEED` and `FLYING_SPEED` as the speed constant. 
     *       If the rotate/flying speed is too slow or too fast, please change `ROTATE_SPEED` or `FLYING_SPEED` value.
     *       You should finish keyCallback first.
     */

    // Render a white board
    glPushMatrix();
    glScalef(3, 1, 3);
    glBegin(GL_TRIANGLE_STRIP);
    glColor3f(1.0f, 1.0f, 1.0f);
    glNormal3f(0.0f, 1.0f, 0.0f);
    glVertex3f(-5.0f, 0.0f, -5.0f);
    glVertex3f(-5.0f, 0.0f, 5.0f);
    glVertex3f(5.0f, 0.0f, -5.0f);
    glVertex3f(5.0f, 0.0f, 5.0f);
    glEnd();
    glPopMatrix();
    

    /* TODO#3: Render the airplane    
     *       1. Render the body.
     *       2. Render the wings.(Don't forget to assure wings rotate at the center of body.)
     *       3. Render the tail.
     * Hint:
     *       glPushMatrix/glPopMatrix (https://registry.khronos.org/OpenGL-Refpages/gl2.1/xhtml/glPushMatrix.xml)
     *       glRotatef (https://registry.khronos.org/OpenGL-Refpages/gl2.1/xhtml/glRotate.xml)
     *       glTranslatef (https://registry.khronos.org/OpenGL-Refpages/gl2.1/xhtml/glTranslate.xml) 
     *       glColor3f (https://registry.khronos.org/OpenGL-Refpages/gl2.1/xhtml/glColor.xml)
     *       glScalef (https://registry.khronos.org/OpenGL-Refpages/gl2.1/xhtml/glScale.xml)
     * Note:
     *       You may implement functions for drawing components of airplane first
     *       You should try and think carefully about changing the order of rotate and translate
     */
    ascentSpeed = 0.05f;
    rotationSpeed = 0.5f;
    float decentSpeed = 0.05f;
    float flySpeed = 0.1f;
    float bulletAngle = forwardAngle;
    if (airplaneWingRotation > 0) {
      if (flag == 1) {
        wingSwingAngle += swingSpeed;
        airplaneWingRotation -= swingSpeed;
      } else if (flag == 0) {
        wingSwingAngle -= swingSpeed;
        airplaneWingRotation -= swingSpeed;
      }
    }

    if (front > 0) {
      forwardAngle = airplaneRotationY - 90;  // 計算前進方向的角度
      airplaneX += flySpeed * cos(ANGLE_TO_RADIAN(forwardAngle));
      airplaneY += flySpeed * sin(ANGLE_TO_RADIAN(forwardAngle));
      front -= flySpeed;
    }

    if (wingSwingAngle == 20 || wingSwingAngle == -20) {
      flag = 1 - flag;
    }
    if (rotationY > 0) {
      airplaneRotationY += rotationSpeed;
      rotationY -= rotationSpeed;
      forwardAngle = airplaneRotationY - 90;
    } else if (rotationY < 0) {
      airplaneRotationY -= rotationSpeed;
      rotationY += rotationSpeed;
      forwardAngle = airplaneRotationY - 90;
    }

    if (targetHeight > 0) {
      airplaneHeight += ascentSpeed;
      targetHeight -= ascentSpeed;
    }
    else if (airplaneHeight > 2) {
      airplaneHeight -= decentSpeed;
    }
    renderAirplane();
    if (bulletDist == 30) {
      bulletAngle = forwardAngle-180;
      bulletX = (airplaneX) - (3 * cos(ANGLE_TO_RADIAN(bulletAngle)));
      bulletY = ((-3) * sin(ANGLE_TO_RADIAN(bulletAngle)) + airplaneY + 0.5);
      bulletHeight = airplaneHeight;
      if (bulletDist == 0 && temp>0) {
        bulletDist = 30;
      }
    }

    if (temp > 0) {
      if (bulletDist >= 0) {
        bulletX += flySpeed * cos(ANGLE_TO_RADIAN(bulletAngle));
        bulletY += flySpeed * sin(ANGLE_TO_RADIAN(bulletAngle));
        bulletDist -= 1;
      }
      drawBullet();
      if (bulletDist <= 0) {
        temp -= 1;
        bulletDist = 0;
      }
    }

#ifdef __APPLE__
    // Some platform need explicit glFlush
    glFlush();
#endif
    glfwSwapBuffers(window);
  }
  return 0;
}

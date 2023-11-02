# renderAirplane
NYCU Computer-graphic aut 2023 Homework1. 
In this work we need to :
1. Implement `camera.cpp`
2. Render an airplane
3. implement some airplane control

## Rules
| Airplane | Body | 
| --- | --- | 
| ![image](https://github.com/CodeStone1125/renderAirplane/assets/72511296/708182a2-d63b-475e-9a40-90e51f69bae5)|![image](https://github.com/CodeStone1125/renderAirplane/assets/72511296/08d7e6c1-8443-416f-99ed-0b35e9a6f833)| 
| Wing | Tail |
|![image](https://github.com/CodeStone1125/renderAirplane/assets/72511296/52ef7f89-45d9-4ddc-a577-5b28c178e28f)| ![image](https://github.com/CodeStone1125/renderAirplane/assets/72511296/e42eb88b-626b-4b10-b548-010259865640)| 
| Location spec | Control |
| ![image](https://github.com/CodeStone1125/renderAirplane/assets/72511296/18c36686-56c2-456b-98b7-7f8bcdee468f)|![image](https://github.com/CodeStone1125/renderAirplane/assets/72511296/f8eda160-3f8f-4186-93cc-5ea8e0f20e15)|

## Dependencies

- [glfw](https://github.com/glfw/glfw)
- [glad](https://github.com/Dav1dde/glad)
- [glm](https://github.com/g-truc/glm)

### Dependencies for Windows

Visual Studio

### Dependencies for macOS

Xcode

### Dependencies for Unix-like systems other than macOS with X11

On *Debian* and derivatives like *Ubuntu* and *Linux Mint*

`sudo apt install xorg-dev`

On *Fedora* and derivatives like *Red Hat*

`sudo dnf install libXcursor-devel libXi-devel libXinerama-devel libXrandr-devel`

On *FreeBSD*

`pkg install xorgproto`

## Build instruction

### CMake

Build in release mode
```bash=
cmake -S . -B build -D CMAKE_BUILD_TYPE=Release
cmake --build build --config Release --parallel 8
cd bin
./HW1
```

Build in debug mode
```bash=
cmake -S . -B build -D CMAKE_BUILD_TYPE=Debug
cmake --build build --config Debug --parallel 8
cd bin
./HW1
```

### Visual Studio 2019

- Open `vs2019/HW1.sln`
- Select config then build (CTRL+SHIFT+B)
- Use F5 to debug or CTRL+F5 to run.

# Reports
## Implementation(HOW & WHY)
### 1. camera
For `camera.cpp`, I finish it with the fountions below:
* `front = original_front * rotation`: For caculate front direction of camera
* `up = original_up * rotation;`: For caculate up direction of camera
* `right = glm::cross(front, up)`: Caculate rightside of the camera with corss up and front
* `viewMatrix = glm::lookAt(position, position + front, up)`: create a camera in 3D world by the given
  position and face the given direction
* `projectionMatrix = glm::perspective(FOV, aspectRatio, zNear, zFar)`: Project the 3D world
  on 2D computer screen 

```cpp
void Camera::updateViewMatrix() {
  constexpr glm::vec3 original_front(0, 0, -1);
  constexpr glm::vec3 original_up(0, 1, 0);

  /* TODO#1-1: Calculate lookAt matrix
   *       1. Rotate original_front and original_up using this->rotation.
   *       2. Calculate right vector by cross product.
   *       3. Calculate view matrix with position.
   * Hint:
   *       You can calculate the matrix by hand, or use
   *       glm::lookAt (https://glm.g-truc.net/0.9.9/api/a00247.html#gaa64aa951a0e99136bba9008d2b59c78e)
   * Note: You must not use gluLookAt
   */

  front = original_front * rotation;
  up = original_up * rotation;
  right = glm::cross(front, up);

  viewMatrix = glm::lookAt(position, position + front, up);
}

void Camera::updateProjectionMatrix(float aspectRatio) {
  constexpr float FOV = glm::radians(45.0f);
  constexpr float zNear = 0.1f;
  constexpr float zFar = 100.0f;
  /* TODO#1-2: Calculate perspective projection matrix
   * Hint: You can calculate the matrix by hand, or use
   *       glm::perspective (https://glm.g-truc.net/0.9.9/api/a00243.html#ga747c8cf99458663dd7ad1bb3a2f07787)
   * Note: You must not use gluPerspective
   */

  projectionMatrix = glm::perspective(FOV, aspectRatio, zNear, zFar);
}
```
### 2. Render airplane
In this section, there 4 parts:
* Body
* tail
* wing
* Airplane

#### I. Body
* Step1. First, I set length and radius to fit the specification in the "Rules"
* Step2. Render a cylinder by draw bottom circle, top circle and side of the cylinder perspective
* Step3. rotate it to make it look like airplane body and move it to location set in the "Rules"

```cpp
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
```

#### II. tail
* Step1. I render tail by draw four triange, which is up, down behind, down LEFT, down right perspective
* Step2. Move it to the right place and make sure it always be.
  
  Note: In Step2, I met some troubles and they will be explained in "Tail's location goes wrong"

```cpp
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
```
#### III. wing
This part is easy just render a cube and make sure it will in right location
```cpp
void renderAirplaneWings() {
  // 設置機翼的顏色和變換
  glColor3f(RED);  // 設定顏色，例如紅色

  glPushMatrix();
  glScalef(4.0f, 0.5f, 1.0f);          // 根據需要進行縮放
  drawUnitCube();                      // 渲染長方體
  glPopMatrix();
}
```
#### IV. Airplane
In this fountion I render airplane with `renderAirplaneWings()`,  `renderAirplaneTail() ` 
and `renderAirplaneBody() `, then move the components to the right place.
```cpp
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
```
### 3. airplane control
In this section, there 4 parts:
* Wings swing
* fly an forward
* Airplane body rotation
* Trace location and angle
#### I.Wings swing, fly an forward
* Step1. Everytime `SPACE` be pressed, it will increase the total angle that wings need to swing.
* Step2. The direction of wing swing will change whenever it meet `20` or `-20` degree.
  Note:`flag` for decide the direction of wing Swing and it;s default to 1
```cpp
   case GLFW_KEY_SPACE:
      airplaneWingRotation += 80;
      flag = 1;
    break;

    if (airplaneWingRotation > 0) {
      if (flag == 1) {
        wingSwingAngle += swingSpeed;
        airplaneWingRotation -= swingSpeed;
      } else if (flag == 0) {
        wingSwingAngle -= swingSpeed;
        airplaneWingRotation -= swingSpeed;
      }
    }
```

#### II.
#### III.Airplane body rotation
#### IV.Trace location and angle
## Problems you encountered
### 1. GFX Glitch
### 2. Tail's location goes wrong
### 3. Can't slowly raise airplane height

static void error ( int error , const char * description ) { fputs(description,stderr); }


static void resize ( GLFWwindow * window , int width , int height ) {
    glViewport(0,0,width,height);
    updateProjection(PROJECTION);
}


static void keyboard ( GLFWwindow * window , int key , int scancode , int action , int mods ) {

    switch (key) {
        default: return;
        // http://www.glfw.org/docs/latest/group__keys.html
        // http://www.glfw.org/docs/latest/group__mods.html

        case GLFW_KEY_A:
            if (mods == GLFW_MOD_CONTROL) switch (action) { // toggle axis
                case GLFW_PRESS: axis = not axis; break;
                case GLFW_RELEASE: moveL = false; break;
                default: break;
            }
            else switch (action) { // strafe left
                case GLFW_PRESS:
                case GLFW_REPEAT:  moveL = true;  break;
                case GLFW_RELEASE: moveL = false; break;
                default: break;
            }
            break;

        case GLFW_KEY_C: // toggle culling
            if (mods == GLFW_MOD_CONTROL and action == GLFW_PRESS) {
                cull = not cull;
                if (cull) glEnable(GL_CULL_FACE);
                else     glDisable(GL_CULL_FACE);
            }
            break;

        case GLFW_KEY_D: // strafe right
            switch (action) {
                case GLFW_PRESS:
                case GLFW_REPEAT:  moveR = true;  break;
                case GLFW_RELEASE: moveR = false; break;
                default: break;
            }
            break;


        case GLFW_KEY_G: // toggle grid
            if (mods == GLFW_MOD_CONTROL and action == GLFW_PRESS) grid = not grid;
            break;

        case GLFW_KEY_H: // help
            if (action == GLFW_PRESS) {
                putchar('\n');
                puts("Navigation");
                puts("arrows      look around");
                puts("scroll      zoom");
                puts("a,d,s,w     move camera");
                puts("keypad      move selection");
                putchar('\n');
                puts("Selections");
                puts("0           light source");
                puts("1           skeleton");
                putchar('\n');
                puts("Controls");
                puts("ctrl-scroll adjust FOV");
                puts("ctrl-#      toggle selection");
                puts("ctrl-a      toggle axis");
                puts("ctrl-c      toggle culling");
                puts("ctrl-g      toggle grid");
                puts("ctrl-h      help");
                puts("ctrl-p      animate selection");
                puts("ctrl-q      quit");
                puts("ctrl-w      toggle wireframe");
            }
            break;

        case GLFW_KEY_P:
            if (mods == GLFW_MOD_CONTROL and action == GLFW_PRESS) skeletonPlay = not skeletonPlay;
            break;

        case GLFW_KEY_Q: // quit
            if (mods == GLFW_MOD_CONTROL and action == GLFW_PRESS) glfwSetWindowShouldClose(window,GL_TRUE);
            break;

        case GLFW_KEY_S:
            switch (action) { // move backward
                case GLFW_PRESS:
                case GLFW_REPEAT:  moveB = true;  break;
                case GLFW_RELEASE: moveB = false; break;
                default: break;
            }
            break;

        case GLFW_KEY_W:
            if (mods == GLFW_MOD_CONTROL) switch (action) { // toggle wireframe
                case GLFW_PRESS:
                    wire = not wire;
                    if (wire) glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
                    else      glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
                    break;
                case GLFW_RELEASE: moveF = false; break;
                default: break;
            }
            else switch (action) { // move forward
                case GLFW_PRESS:
                case GLFW_REPEAT:  moveF = true;  break;
                case GLFW_RELEASE: moveF = false; break;
                default: break;
            }
            break;

        case GLFW_KEY_PAGE_UP: // ascend
            switch (action) {
                case GLFW_PRESS:
                case GLFW_REPEAT:  moveU = true;  break;
                case GLFW_RELEASE: moveU = false; break;
                default: break;
            }
            break;

        case GLFW_KEY_PAGE_DOWN: // descend
            switch (action) {
                case GLFW_PRESS:
                case GLFW_REPEAT:  moveD = true;  break;
                case GLFW_RELEASE: moveD = false; break;
                default: break;
            }
            break;

        case GLFW_KEY_HOME: // orbit CW
            switch (action) {
                case GLFW_PRESS:
                case GLFW_REPEAT:  orbitCW = true;  break;
                case GLFW_RELEASE: orbitCW = false; break;
                default: break;
            }
            break;

        case GLFW_KEY_END: // orbit CCW
            switch (action) {
                case GLFW_PRESS:
                case GLFW_REPEAT:  orbitCCW = true;  break;
                case GLFW_RELEASE: orbitCCW = false; break;
                default: break;
            }
            break;

        case GLFW_KEY_LEFT: // look left
            switch (action) {
                case GLFW_PRESS:
                case GLFW_REPEAT:  lookL = true;  break;
                case GLFW_RELEASE: lookL = false; break;
                default: break;
            }
            break;

        case GLFW_KEY_RIGHT: // look right
            switch (action) {
                case GLFW_PRESS:
                case GLFW_REPEAT:  lookR = true;  break;
                case GLFW_RELEASE: lookR = false; break;
                default: break;
            }
            break;

        case GLFW_KEY_DOWN: // look down
            switch (action) {
                case GLFW_PRESS:
                case GLFW_REPEAT:  lookD = true;  break;
                case GLFW_RELEASE: lookD = false; break;
                default: break;
            }
            break;

        case GLFW_KEY_UP: // look up
            switch (action) {
                case GLFW_PRESS:
                case GLFW_REPEAT:  lookU = true;  break;
                case GLFW_RELEASE: lookU = false; break;
                default: break;
            }
            break;

        case GLFW_KEY_0:
            if (mods == GLFW_MOD_CONTROL) {
                if (action == GLFW_PRESS) toggleLight();
            }
            else selection = 0;
            break;

        case GLFW_KEY_1:
            if (mods == GLFW_MOD_CONTROL) {
                if (action == GLFW_PRESS) skeleton = not skeleton;
            }
            else selection = 1;
            break;

        case GLFW_KEY_SPACE:
            pEye   = glm::translate(glm::mat4(),glm::vec3(5,6,5))*pSelection[selection];
            pFocus = pSelection[selection];
            break;

        case GLFW_KEY_KP_2: // move back
            switch (action) {
                case GLFW_PRESS:
                case GLFW_REPEAT:  selectionMoveB[selection] = true;  break;
                case GLFW_RELEASE: selectionMoveB[selection] = false; break;
                default: return;
            }
            break;

        case GLFW_KEY_KP_4: // move left
            switch (action) {
                case GLFW_PRESS:
                case GLFW_REPEAT:  selectionMoveL[selection] = true;  break;
                case GLFW_RELEASE: selectionMoveL[selection] = false; break;
                default: return;
            }
            break;

        case GLFW_KEY_KP_6: // move right
            switch (action) {
                case GLFW_PRESS:
                case GLFW_REPEAT:  selectionMoveR[selection] = true;  break;
                case GLFW_RELEASE: selectionMoveR[selection] = false; break;
                default: return;
            }
            break;

        case GLFW_KEY_KP_8: // move forth
            switch (action) {
                case GLFW_PRESS:
                case GLFW_REPEAT:  selectionMoveF[selection] = true;  break;
                case GLFW_RELEASE: selectionMoveF[selection] = false; break;
                default: return;
            }
            break;

        case GLFW_KEY_KP_SUBTRACT: // move down
            switch (action) {
                case GLFW_PRESS:
                case GLFW_REPEAT:  selectionMoveD[selection] = true;  break;
                case GLFW_RELEASE: selectionMoveD[selection] = false; break;
                default: return;
            }
            break;

        case GLFW_KEY_KP_ADD: // move up
            switch (action) {
                case GLFW_PRESS:
                case GLFW_REPEAT:  selectionMoveU[selection] = true;  break;
                case GLFW_RELEASE: selectionMoveU[selection] = false; break;
                default: return;
            }
            break;
    }
}


static void click ( GLFWwindow * window , int button , int action , int mods ) {
    switch (button) {
        default: return;
        // http://www.glfw.org/docs/latest/group__buttons.html
        // http://www.glfw.org/docs/latest/group__mods.html
        case GLFW_MOUSE_BUTTON_LEFT:
        case GLFW_MOUSE_BUTTON_MIDDLE:
        case GLFW_MOUSE_BUTTON_RIGHT:  break;
    }
}


static void hover ( GLFWwindow * window , double xpos , double ypos ) {
    vCursor = glm::vec3(xpos,ypos,1)-pCursor;
    pCursor = glm::vec3(xpos,ypos,1);
}


static void scroll ( GLFWwindow * window , double xoffset , double yoffset ) {
    if (glfwGetKey(window,GLFW_KEY_LEFT_CONTROL)  == GLFW_PRESS
    or  glfwGetKey(window,GLFW_KEY_RIGHT_CONTROL) == GLFW_PRESS) {
        const GLfloat s = rad(1.0);
             if (yoffset > 0) lens -= s;
        else if (yoffset < 0) lens += s;
    }
    else {
        const GLfloat s = 0.05;
             if (yoffset > 0) zoom += s;
        else if (yoffset < 0) zoom -= s;
    }
}

#define GRID_DELTA    10 // world units per grid unit
#define GRID_LENGTH   FAR/2
#define GRID_COLOR_Q1 BASIC_BLACK
#define GRID_COLOR_Q2 BASIC_BLACK
#define GRID_COLOR_Q3 BASIC_BLACK
#define GRID_COLOR_Q4 BASIC_BLACK

bool      axis = false, grid = false;
GPUbuffer axisBuffer,   gridBuffer;


void initAxis ( ) {
    static bool uninitialized = true;
    if (uninitialized) {
        
        const GLfloat l = GRID_LENGTH;
        glm::vec4 axisXYZW[12] = {
            AXIS_ORIGIN, l*AXIS_X,
            AXIS_ORIGIN,-l*AXIS_X,
            AXIS_ORIGIN, l*AXIS_Y,
            AXIS_ORIGIN,-l*AXIS_Y,
            AXIS_ORIGIN, l*AXIS_Z,
            AXIS_ORIGIN,-l*AXIS_Z 
        };
        glm::vec4 axisRGBA[12] = {
            palette[BASIC_RED]  ,palette[BASIC_WHITE], // +x
            palette[BASIC_RED]  ,palette[BASIC_BLACK], // -x
            palette[BASIC_GREEN],palette[BASIC_WHITE], // +y
            palette[BASIC_GREEN],palette[BASIC_BLACK], // -y
            palette[BASIC_BLUE] ,palette[BASIC_WHITE], // +z
            palette[BASIC_BLUE] ,palette[BASIC_BLACK]  // -z
        };
        
        Cobject(
            &axisBuffer,
            12,
            &axisXYZW[0],
            &axisRGBA[0]
        );
    }
    else cleanCobject(&axisBuffer);
    uninitialized = not uninitialized;
}


void initGrid ( ) {
    static bool uninitialized = true;
    if (uninitialized) {
        
        const GLfloat l = GRID_LENGTH;
        const GLint gridNaturals = l/GRID_DELTA;
        
        glm::vec4
            gridXYZW[gridNaturals*16],
            gridRGBA[gridNaturals*16];
        
        int p = 0;
        for (int i=0; i < gridNaturals ;++i) {
            
            int offset = i*GRID_DELTA;
            if (offset > FAR) offset = FAR;
            
            // Quadrant 1 (+X,-Z)
            gridXYZW[p] = glm::vec4( offset,0, 0,1);
            gridRGBA[p] = palette[GRID_COLOR_Q1];
                   ++p;
            gridXYZW[p] = glm::vec4( offset,0,-l,1);
            gridRGBA[p] = palette[GRID_COLOR_Q1];
                   ++p;
            
            // Quadrant 1 (-Z,+X)
            gridXYZW[p] = glm::vec4( 0,0,-offset,1);
            gridRGBA[p] = palette[GRID_COLOR_Q1];
                   ++p;
            gridXYZW[p] = glm::vec4( l,0,-offset,1);
            gridRGBA[p] = palette[GRID_COLOR_Q1];
                   ++p;
            
            // Quadrant 2 (-X,-Z)
            gridXYZW[p] = glm::vec4(-offset,0, 0,1);
            gridRGBA[p] = palette[GRID_COLOR_Q2];
                   ++p;
            gridXYZW[p] = glm::vec4(-offset,0,-l,1);
            gridRGBA[p] = palette[GRID_COLOR_Q2];
                   ++p;
            
            // Quadrant 2 (-Z,-X)
            gridXYZW[p] = glm::vec4( 0,0,-offset,1);
            gridRGBA[p] = palette[GRID_COLOR_Q2];
                   ++p;
            gridXYZW[p] = glm::vec4(-l,0,-offset,1);
            gridRGBA[p] = palette[GRID_COLOR_Q2];
                   ++p;
            
            // Quadrant 3 (-X,+Z)
            gridXYZW[p] = glm::vec4(-offset,0, 0,1);
            gridRGBA[p] = palette[GRID_COLOR_Q3];
                   ++p;
            gridXYZW[p] = glm::vec4(-offset,0, l,1);
            gridRGBA[p] = palette[GRID_COLOR_Q3];
                   ++p;
            
            // Quadrant 3 (+Z,-X)
            gridXYZW[p] = glm::vec4( 0,0, offset,1);
            gridRGBA[p] = palette[GRID_COLOR_Q3];
                   ++p;
            gridXYZW[p] = glm::vec4(-l,0, offset,1);
            gridRGBA[p] = palette[GRID_COLOR_Q3];
                   ++p;
            
            // Quadrant 4 (+X,+Z)
            gridXYZW[p] = glm::vec4( offset,0, 0,1);
            gridRGBA[p] = palette[GRID_COLOR_Q4];
                   ++p;
            gridXYZW[p] = glm::vec4( offset,0, l,1);
            gridRGBA[p] = palette[GRID_COLOR_Q4];
                   ++p;
            
            // Quadrant 4 (+Z,+X)
            gridXYZW[p] = glm::vec4( 0,0, offset,1);
            gridRGBA[p] = palette[GRID_COLOR_Q4];
                   ++p;
            gridXYZW[p] = glm::vec4( l,0, offset,1);
            gridRGBA[p] = palette[GRID_COLOR_Q4];
                   ++p;
        }
        
        Cobject(
            &gridBuffer,
             gridNaturals*16,
            &gridXYZW[0],
            &gridRGBA[0]
        );
    }
    else cleanCobject(&gridBuffer);
    uninitialized = not uninitialized;
}


void initCartesian ( ) {
    static bool uninitialized = true;
    if (uninitialized) {
        initAxis();
        initGrid();
    }
    else {
        initGrid();
        initAxis();
    }
    uninitialized = not uninitialized;
}


void drawCartesian ( glm::mat4 model  = glm::mat4() ) {
    
    currentShader(Cshader);
    
    if (axis) {
        glBindVertexArray(axisBuffer.VAO);
        glUniformMatrix4fv(Cmodel,1,GL_FALSE,glm::value_ptr(model));
        glDrawArrays(GL_LINES,0,12);
    }
    
    if (grid) {
        glBindVertexArray(gridBuffer.VAO);
        glUniformMatrix4fv(Cmodel,1,GL_FALSE,glm::value_ptr(model));
        
        const int adjust = (axis) ? 1 : 0;
        glDrawArrays(GL_LINES,16*adjust,16*((GRID_LENGTH/GRID_DELTA)-adjust));
    }
}

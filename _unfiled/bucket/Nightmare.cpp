glm::vec3 angle, delta;
glm::mat4 mvMatrix, rotMatrix;
glm::quat4 rotQuat;

// Look angles
if (keyUp) angle.x -= 10.0 * dt;
if (keyDown) angle.x += 10.0 * dt;

if (keyLeft) angle.y += 10.0 * dt;
if (keyRight) angle.y -= 10.0 * dt;

// Wrap angles
if (angle.x > 360.0f)	angle.x -= 360.0f;
else if (angle.x < 0.0f)angle.x += 360.0f;
if (angle.y > 360.0f)	angle.y -= 360.0f;
else if (angle.y < 0.0f)angle.y += 360.0f;

// Movement
if (keyW) delta.z += 10.0 * dt; 
if (keyS) delta.z -= 10.0 * dt; 

if (keyA) delta.x -= 10.0 * dt; 
if (keyD) delta.x += 10.0 * dt; 

// Built Quaternion from angles (only x/y for traditional fps camera)
rotQuat =
       glm::angleAxis(angle.x, 1.0f, 0.0f, 0.0f) *
       glm::angleAxis(angle.y, 0.0f, 1.0f, 0.0f);

// Convert to mat4
rotMat = glm::toMat4(rotQuat);

// Convert delta based on direction
delta = delta * rotQuat;

// Move camera position based on new delta
position += delta;

// Reset Delta
delta = glm::vec3(0.0f);

// Rotate and Translate mvMatrix from identity 
mvMatrix = glm::translate(glm::mat4(1.0f) * rotMat, -position);
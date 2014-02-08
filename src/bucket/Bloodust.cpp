#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <SFML/Window.hpp>

class RotatingCamera
{
public:
	RotatingCamera() : 
	  forw(0,0,1),
		up(0,1,0),
		alfa(0),
		beta(0),
		x_rotation_accumulation(0),
		y_rotation_accumulation(0)
	{
	}
	RotatingCamera(const glm::vec3 &forward, const glm::vec3 &up) : 
		forw(forward),
		up(up),
		alfa(0),
		beta(0),
		x_rotation_accumulation(0),
		y_rotation_accumulation(0)
	{
	}	
	void update_rotation(float timestep)
	{
		rotate(timestep*glm::vec2(x_rotation_accumulation, y_rotation_accumulation));
		x_rotation_accumulation = 0;
		y_rotation_accumulation = 0;
	}
	void mouse_move(float deltax, float deltay)
	{
		x_rotation_accumulation += deltax;
		y_rotation_accumulation += deltay;
	}
	
	glm::vec3 toCartesian(const glm::vec3 &v)const
	{
		return glm::vec3(
			v.x*cos(v.y)*cos(v.z),
			v.x*sin(v.y),
			v.x*cos(v.y)*sin(v.z));
	}
	glm::vec3 forw,up;
	double alfa,beta;
private:
	float x_rotation_accumulation, y_rotation_accumulation;
	void rotate(const glm::vec2 &r)
	{
		alfa -=r.y; 
		beta -=r.x;
		if(alfa > 3.14/2)
			alfa = 3.14/2;
		else if( alfa < -3.14/2)
			alfa = -3.14/2;
		if(beta > 2*3.14)
			beta = beta-2*3.14;
		else if(beta < -2*3.14)
			beta = beta+2*3.14;
		
		glm::vec3 direction(
				cos(alfa) * sin(beta),
				sin(alfa),
				cos(alfa) * cos(beta)
			);
		glm::vec3 right = glm::vec3(
				sin(beta - 3.14f/2.0f),
				0,
				cos(beta - 3.14f/2.0f)
			);
		forw = direction;
		up = glm::cross( right, direction );
	}
};
class MovingCamera : public RotatingCamera
{
public:
	bool forward,backward,left,right,fast,downward,upward;

	MovingCamera() : 
		position(glm::vec3(0,0,0)),
		speed(1)
	{ 
		forward = backward = left = right = fast = downward = upward = false; 
	}
	MovingCamera(const glm::vec3 &position) : 
		position(position),
		speed(1)
	{
		forward = backward = left = right = fast = downward = upward = false;
	}

	glm::mat4 create_look_at()const
	{
		return glm::lookAt(position, position+forw, up);
	}
	void update(float timestep)
	{
		update_rotation(timestep);
		const glm::vec3 kerroin(10,10,10);
		if(forward)
			position += speed*(fast ? glm::normalize(RotatingCamera::forw)*kerroin : glm::normalize(RotatingCamera::forw));
		else if(backward)
			position -= speed*(fast ? glm::normalize(RotatingCamera::forw)*kerroin : glm::normalize(RotatingCamera::forw));
		if(left)
			position -= speed*(fast ? glm::normalize(glm::cross(RotatingCamera::forw,up))*kerroin : glm::normalize(glm::cross(RotatingCamera::forw, up)));
		else if(right)
			position += speed*(fast ? glm::normalize(glm::cross(RotatingCamera::forw,up))*kerroin : glm::normalize(glm::cross(RotatingCamera::forw, up)));
	}
	void input(const sf::Event &event)
	{
		if(event.type == sf::Event::KeyPressed){
			switch(event.key.code){
				case sf::Keyboard::A:
					left = true;
					break;
				case sf::Keyboard::D:
					right = true;
					break;
				case sf::Keyboard::W:
					forward = true;
					break;
				case sf::Keyboard::S:
					backward = true;
					break;
				case sf::Keyboard::RShift:
				case sf::Keyboard::LShift:
					fast = true;
					break;
				default:
					break;
			}
		}
		else if(event.type == sf::Event::KeyReleased){
			switch(event.key.code){
				case sf::Keyboard::A:
					left = false;
					break;
				case sf::Keyboard::D:
					right = false;
					break;
				case sf::Keyboard::W:
					forward = false;
					break;
				case sf::Keyboard::S:
					backward = false;
					break;
				case sf::Keyboard::RShift:
				case sf::Keyboard::LShift:
					fast = false;
					break;
				default:
					break;
			}
		}
	}

	glm::vec3 position;
	glm::vec3 speed;
};
class ThirdPersonCamera : public RotatingCamera
{
public:
	ThirdPersonCamera(float distance=5) : distance(distance){}
	glm::mat4 create_look_at(const glm::vec3& player_position)const
	{
		return glm::lookAt(player_position+position-forw*distance, 
			player_position, 
			up);
	}
	void input(const sf::Event &event)
	{
		if(event.type == sf::Event::MouseWheelMoved)
			distance += event.mouseWheel.delta/2.0f;
	}
	glm::vec3 position;
	float distance;
};

#endif

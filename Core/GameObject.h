#pragma once
#include "Drawable.h"
class GameObject : public Drawable
{
protected:
	color fillColor;	//game object fill color
	color borderColor;	//game object border color

public:
	GameObject(Game* r_pGame, point ref, int r_width, int r_height, color fc, color bc);
	virtual void draw() const = 0; //for a game object to draw itself on the screen
	void setRefPoint(point p);
	bool CollisionDetection(const GameObject& gObj);



};



#include "Bullet.h"


Bullet::Bullet()
{
	TTL = 0;
	isActive = false;
}


Bullet::~Bullet()
{
}
void Bullet::update(double dt)
{
	TTL -= dt;
	if(TTL <= 0){
		isActive = false;
		return;
	}
	last_pos = pos;
	pos = pos + vel * dt;
}
void Bullet::render(){
	glBegin(GL_LINES);
		glColor3f(1,1,0);
		glVertex3f(pos.x, pos.y, pos.z);
		Vector3 aux = vel;
		Vector3 end = pos- aux.normalize()*10;
		glVertex3f( end.x, end.y, end.z);
	glEnd();
	glColor3f(1,1,1);
}

BulletManager* BulletManager::instance = NULL;
BulletManager* BulletManager::getInstance(){
	if (instance == NULL)
		instance = new BulletManager();
	return instance;
}

max_bullets maxBullets = 5000; // número max de bales
BulletManager::BulletManager()
{
	bullets.reserve(maxBullets);
	for(int i = 0; i<maxBullets; ++i)
	{
		Bullet* b = new Bullet();
		bullets.push_back(b);
	}
}


BulletManager::~BulletManager()
{
}

void BulletManager::render()
{
	for(std::vector<Bullet*>::iterator it = bullets.begin();it != bullets.end();++it)
	{
		if((*it)->isActive) 
 			(*it)->render();
	}
}
void BulletManager::update(double dt)
{
	for(std::vector<Bullet*>::iterator it = bullets.begin();it != bullets.end();++it)
	{
		(*it)->update(dt);
	}
}
void BulletManager::createBullet(Vector3 pos, Vector3 vel, std::string author_id)
{

	for(std::vector<Bullet*>::iterator it = bullets.begin();it != bullets.end();++it)
	{
		if(!(*it)->isActive)
		{		
			(*it)->pos = pos;
			(*it)->last_pos = pos;
			(*it)->vel = vel;
			(*it)->TTL = 20;
			(*it)->isActive = true;
				break;
		}
		if(it == bullets.end()-1){
			//std::cout << "fi: "<<bullets.size() << " " << i << std::endl;
		}
	}

}




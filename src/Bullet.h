#pragma once
#include "framework.h"
#include "includes.h"

class Bullet
{
public:
	Bullet();
	~Bullet();
	void update(double dt);
	void render();

	Vector3 last_pos; // pq la vala puede ir tan rapida que atraviese el objeto
					// conesto trazamos un rayo
	Vector3 pos;
	Vector3 vel;
	//char type;
	double TTL;
	std::string author_id; // pq chocara con sigo mismo al principio
	bool isActive;
};

typedef int max_bullets;
class BulletManager
{
private:
	BulletManager();
public:
	static BulletManager* instance;

	static BulletManager* getInstance();
	/*gravedad*/// G = Vector3(0,-1,0);
	~BulletManager();
	//max maxim = 10000;
	std::vector<Bullet*> bullets; // como tendra q hacer muxos resizes = mucho coste
	/*cada vez q keramos una bala recorremos y substituimos para tener un ta´ño fijo durante todo el contenedor
		esto es un POOL
			para evitar recorrer tot el vector cada vez para saber donde esta el libre, cada vez podemos hacer un swap con el último
			*/
	void render();// como tendra alfa queremos que lo haga al final de todo

	void update(double dt);/*{
			for (totes les bales)
				bullet &b = bullet[i];  ==> b.pos
				b.pos = b.pos + b.vel*dt;
				/*tiro parabolico*/ //b.vel = b.vel + G*dt
			
		//}
	void createBullet(Vector3 pos, Vector3 vel, std::string author_id);
};

/* EVENTO EN ENTITY: onBulletCollision
	devuelve si la bala destruirse o seguir su camino
*/



/*ENTITY*/
/*

bullets-resize(max)
for totes{
	bullet b,
	b.pos.randorm(199);
	bullets.pushback();
}

MESH bulletsm;
bm.vertices.resize(bullets.size());

for(size_t i =0; < bullets.size){
	bullet &b = bullet[i];
	bm.vertices[i] = b.pos;
}

*/
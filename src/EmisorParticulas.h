#pragma once
#include "Mesh.h"
#include "texture.h"
#include "World.h"
#include "camera.h"
#include <algorithm>    // std::sort

class Particula
{
public: 
	Vector3 pos;
	Vector3 vel;
	float TTL;
	float fixedTTL;
	float size;
	float alpha;
	Texture* texture;
	//Particula(Vector3 pos, Texture* texture) { this->pos = pos; this->texture = texture; }
	Particula(Vector3 pos, Vector3 vel, int TTL, float size, float alpha, Texture* texture);
	std::vector<Particula*> surtidor;
	Particula * parent;
	void addParticulaChildren( Particula* p);

	static int sharedId;
	int id;
};

class EmisorParticulas
{
private:
	static EmisorParticulas* instance;
	std::map<std::string, int> textures;
	std::vector<Particula*> particulas;

	std::vector<int> particulasToErase;

	int numParticulasPorFuente;

	EmisorParticulas();
public:
	~EmisorParticulas();
	static EmisorParticulas* getInstance()
	{
		if(instance == NULL)
		{
			instance = new EmisorParticulas();
		}
		return instance;
	}

	void render();
	void update(double dt);
	Vector3 top;
	Vector3 right;
	bool addParticula(Particula* p);
	void killParticula();
	void killParticula(int id);
	Particula* getParticula(int id);
};


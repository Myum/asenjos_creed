#ifndef MANAGER_H
#define MANAGER_H

#include <map>

template <class T> class Manager
{
	std::map< char*, T*> objects;
  public:
    Manager();
	~Manager();

	T* getObject( char* );
};

#endif

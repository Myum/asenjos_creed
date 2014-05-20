#include "Manager.h"

template <class T> T* Manager<T>::getObject( char* objectId )
{
	//return NULL;
	std::map< char*, T* >::iterator it = this->objects.find(objectId);

	if(it == this->objects.end())
	{
		T* object = new T();
		T->load( objectId );
		this->objects.insert( std::pair< char* , T* >( objectId, object ) );

		return object;
	}
	else
	{
		return it->second;
	}
}
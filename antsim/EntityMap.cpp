#include "EntityMap.h"














template<typename T>
int EntityMap<T>::registerEntity(T* object, Vector2 pos) {

	Entity* newEntity = new Entity(object, pos);


	int handle = newId;
	entityList[handle] = newEntity;

	newId++;

	return handle;
}

template<typename T>
inline void EntityMap<T>::setPosition(int handle, Vector2 pos) {
	entityList[handle]->pos = pos;

}







//template<typename T>
//inline int EntityMap<T>::wrapWidth(int val) {
//	if (val >= width) return val - width;
//	if (val < 0) return val + width;
//	return val;
//}
//
//template<typename T>
//inline int EntityMap<T>::wrapHeight(int val) {
//	if (val >= height) return val - height;
//	if (val < 0) return val + height;
//	return val;
//}

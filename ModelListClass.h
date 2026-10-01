#pragma once

#ifndef _MODELLISTCLASS_H_
#define _MODELLISTCLASS_H_

#include <stdlib.h>
#include <time.h>

class ModelListClass
{
private:
	struct ModelInfoType
	{
		float positionX, positionY, positionZ;
	};

public:
	ModelListClass();
	ModelListClass(const ModelListClass& other);
	~ModelListClass();

	void Initialize(int numModels);
	void Shutdown();

	int GetModelCount();
	void GetData(int index, float& positionX, float& positionY, float& positionZ);

private:
	int m_modelCount;
	ModelInfoType* m_ModelInfoList;
};

#endif // !_MODELLISTCLASS_H_
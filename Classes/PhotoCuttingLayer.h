#pragma once
#include <stdio.h>
#include "cocos2d.h"

USING_NS_CC;

class PhotoCuttingLayer : public Layer {
protected:
	int _rows;
	int _columns;
	int _sliceW;
	int _sliceH;
	std::vector<std::vector<Sprite*>> _slices;
public:
	PhotoCuttingLayer();
	virtual ~PhotoCuttingLayer();

	static PhotoCuttingLayer* create(char* photo, int rows, int columns);

	bool initWithPhoto(char* photo, int rows, int columns);

	bool initSlices(char* photo);

	void initTouchEventListener();

	void move(int x, int y);

	void restart();
};


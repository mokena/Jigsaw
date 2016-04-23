#include "PhotoCuttingLayer.h"

PhotoCuttingLayer::PhotoCuttingLayer():
_rows(3),
_columns(3){
}

PhotoCuttingLayer::~PhotoCuttingLayer() {
	this->removeAllChildren();
}

PhotoCuttingLayer* PhotoCuttingLayer::create(char* photo, int rows, int columns) {
	PhotoCuttingLayer* pc = new PhotoCuttingLayer();

	if (pc&&pc->initWithPhoto(photo, rows, columns)) {
		pc->autorelease();
		return pc;
	}
	CC_SAFE_RELEASE(pc);
	return nullptr;
}

bool PhotoCuttingLayer::initWithPhoto(char* photo, int rows, int columns) {
	bool result = false;
	if (Layer::init()) {
		_rows = rows;
		_columns = columns;
		if (initSlices(photo)) {
			initTouchEventListener();
			restart();
			result = true;
		}
	}

	return result;
}

bool PhotoCuttingLayer::initSlices(char* photo) {
	Texture2D* texture = Director::getInstance()->getTextureCache()->addImage(photo);

	if (texture == nullptr) {
		return false;
	}

	auto size = texture->getContentSize();
	this->setContentSize(size);
	_sliceW = size.width / _columns;
	_sliceH = size.height / _rows;

	for (int i = 0; i < _rows; i++) {
		std::vector<Sprite*> slicesC;
		for (int j = 0; j < _columns; j++) {
			slicesC.push_back(nullptr);
		}
		_slices.push_back(slicesC);
	}

	_slices[_rows-1][_columns-1] = nullptr;

	Sprite* lastSlice = Sprite::createWithTexture(texture, Rect(_sliceW*(_columns - 1), _sliceH*(_rows - 1), _sliceW, _sliceH));
	lastSlice->setOpacity(50);
	lastSlice->setAnchorPoint(Vec2(0, 0));
	lastSlice->setPosition(Vec2(_sliceW*(_columns - 1), 0));
	addChild(lastSlice);

	for (int i = 0; i < _rows; i++) {
		for (int j = 0; j < _columns; j++) {
			if (i == _rows - 1 && j == _columns - 1) {
				continue;
			}
			Sprite* slice = Sprite::createWithTexture(texture, Rect(_sliceW*j, _sliceH*i, _sliceW, _sliceH));
			slice->setAnchorPoint(Vec2(0, 0));
			slice->setPosition(Vec2(_sliceW*j, _sliceH*(_rows - 1 - i)));
			addChild(slice);
			_slices[i][j] = slice;
		}
	}

	return true;
}

void PhotoCuttingLayer::initTouchEventListener() {
	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = [&](Touch* t, Event* e) {
		auto loc = this->convertToNodeSpace(t->getLocation());
		auto size = this->getContentSize();
		auto rect = Rect(0, 0, size.width, size.height);
		if (rect.containsPoint(loc)) {
			return true;
		}

		return false;
	};

	listener->onTouchEnded = [&](Touch* t, Event* e) {
		auto loc = this->convertToNodeSpace(t->getLocation());
		int x = static_cast<int>(floor(loc.x / _sliceW));
		int y = static_cast<int>(floor(loc.y / _sliceH));
		move(x, y);
	};
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

void PhotoCuttingLayer::move(int x, int y) {
	y = _columns - y - 1;
	CCLOG("_slices[x][y + 1]: %d, %d, %d\n", x, y+1, _slices[x][y + 1]);
	CCLOG("_slices[x][y - 1]: %d, %d, %d\n", x, y+1, _slices[x][y - 1]);
	CCLOG("_slices[x - 1][y]: %d, %d, %d\n", x, y+1, _slices[x - 1][y]);
	CCLOG("_slices[x + 1][y]: %d, %d, %d\n", x, y+1, _slices[x + 1][y]);
	if (y < _rows - 1 && _slices[x][y + 1] == nullptr) {
		_slices[x][y + 1] = _slices[x][y];
		_slices[x][y] = nullptr;
	}
	else if (y>0 && _slices[x][y - 1] == nullptr) {
		_slices[x][y - 1] = _slices[x][y];
		_slices[x][y] = nullptr;
	}
	else if (x > 0 && _slices[x - 1][y] == nullptr) {
		_slices[x - 1][y] = _slices[x][y];
		_slices[x][y] = nullptr;
	}
	else if (x < _columns - 1 && _slices[x + 1][y] == nullptr) {
		_slices[x + 1][y] = _slices[x][y];
		_slices[x][y] = nullptr;
	}
}

void PhotoCuttingLayer::restart() {
	std::vector<Sprite*> slices;
	auto slice = _slices[0][0];
	for (int i = 0; i < _rows; i++) {
		for (int j = 0; j < _columns; j++) {
			if (i == _rows - 1 && j == _columns - 1) {
				continue;
			}
			if (_slices[i][j] != nullptr) {
				slices.push_back(_slices[i][j]);
			}
		}
	}

	for (int i = 0; i < _rows; i++) {
		for (int j = 0; j < _columns; j++) {
			if (i == _rows - 1 && j == _columns - 1) {
				_slices[i][j] = nullptr;
			}
			else {
				int index = static_cast<int>(floor(CCRANDOM_0_1()*slices.size()));
				_slices[i][j] = slices[index];
				auto slice = slices[index];
				slices.erase(slices.begin() + index);
			}
		}
	}

}

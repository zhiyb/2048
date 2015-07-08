#ifndef TILE_H
#define TILE_H

#include <QColor>
#include <QFont>

class Tile
{
public:
	enum DataType {Background = 0, Foreground = 1, FontSize = 2};

	static QColor foreground(const unsigned int value);
	static QColor background(const unsigned int value);
	static QFont font(const unsigned int value);

private:
	static unsigned long data(const unsigned long index, const DataType type);
	static unsigned long toIndex(const unsigned long value);

	static const unsigned long d[13][3];
};

#endif // TILES_H

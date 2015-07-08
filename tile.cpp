#include "tile.h"
#include <QColor>

const unsigned long Tile::d[13][3] = {
	// Background	Foreground	Font size
	{0xfff5f2,	0x776e65,	55},	// 1
	{0xeee4da,	0x776e65,	55},	// 2
	{0xede0c8,	0x776e65,	55},	// 4
	{0xf2b179,	0xf9f6f2,	55},	// 8
	{0xf59563,	0xf9f6f2,	55},	// 16
	{0xf67c5f,	0xf9f6f2,	55},	// 32
	{0xf65e3b,	0xf9f6f2,	55},	// 64
	{0xedcf72,	0xf9f6f2,	45},	// 128
	{0xedcc61,	0xf9f6f2,	45},	// 256
	{0xedc850,	0xf9f6f2,	45},	// 512
	{0xedc53f,	0xf9f6f2,	35},	// 1024
	{0xedc22e,	0xf9f6f2,	35},	// 2048
	{0x3c3a32,	0xf9f6f2,	30},	// super
};

unsigned long Tile::toIndex(const unsigned long value)
{
	if (value == 0)
		return 0;
	int idx = 1;
	for (int i = 1; i != 4096; i <<= 1, idx++)
		if (value & i)
			break;
	return idx;
}

unsigned long Tile::data(const unsigned long index, const DataType type)
{
	if (index == 0 || index > 13)
		if (type ==  FontSize)
			return d[0][FontSize];
	return d[index - 1][type];
}

QColor Tile::foreground(const unsigned int value)
{
	unsigned long index = toIndex(value);
	if (index == 0 || index > 13)
		return 0xBBADA0;
	return data(index, Foreground);
}

QColor Tile::background(const unsigned int value)
{
	unsigned long index = toIndex(value);
	if (index == 0 || index > 13)
		return QColor(205, 192, 180);
	return data(index, Background);
}

#include <QtCore>
QFont Tile::font(const unsigned int value)
{
	QFont f;
	f.setFamily("Clear Sans, Helvetica Neue, Arial, sans-serif");
	f.setPixelSize(data(toIndex(value), FontSize));
	f.setBold(true);
	return f;
}

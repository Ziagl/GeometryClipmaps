#include <iostream>

#include "Heightfield_basics.h"

std::ostream& operator<<(std::ostream& out, const Rect &r )
{
	out << "( l: " << r.left << " r: " << r.right << " b: " << r.bottom << " t: " << r.top << " )";
	return out;
}
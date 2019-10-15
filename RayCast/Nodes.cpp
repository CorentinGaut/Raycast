#include "Nodes.h"

Nodes::Nodes(std::variant<Nodes*, Leaf> f, std::variant<Nodes*, Leaf> s, BoundingBox b) : first(f), second(s), bb(b) {}


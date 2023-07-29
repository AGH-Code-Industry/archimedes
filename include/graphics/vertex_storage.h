#include "graphics/buffer_object.h"
#include "graphics/primitives.h"

#include <vector>

namespace arch {

class VertexStorage {
public:
	void set(const std::vector<Vertex> &vertices, const std::vector<uint32_t> &indices);
	void use() const;
	size_t get_num_of_indices() const { return _num_of_indices; }
private:
	VertexBuffer _vertex_buffer;
	ElementBuffer _element_buffer;
	VertexArray _vertex_array;
	size_t _num_of_indices {0};
};


}

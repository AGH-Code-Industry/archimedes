#include "graphics/vertex_storage.h"

namespace arch {

void VertexStorage::set(const std::vector<Vertex> &vertices, const std::vector<uint32_t> &indices) {
	_vertex_array.bind();
	_vertex_buffer.fill(vertices.data(), vertices.size());
	_vertex_buffer.bind();
	_element_buffer.fill(indices.data(), indices.size());
	_element_buffer.bind();
	_vertex_array.enable();
	_num_of_indices = indices.size();
}

void VertexStorage::use() const {
	_vertex_array.bind();
}

}

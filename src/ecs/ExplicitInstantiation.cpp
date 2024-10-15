#include <Ecs.h>

namespace arch::ecs {

namespace _details {

template class EntityTraits<e32>;
template class EntityTraits<e64>;
template class SparseSet<e32>;
template class SparseSet<e64>;
template class CommonComponentPool<e32>;
template class CommonComponentPool<e64>;

} // namespace _details

template class Domain<e32>;
template class Domain<e64>;
template class EntityPool<e32>;
template class EntityPool<e64>;

} // namespace arch::ecs

template class arch::utils::ReadonlyCounter<size_t>;

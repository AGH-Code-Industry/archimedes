#include "ComponentPoolEntityIterator.h"

#define TEMPLATE_C template<class C>
#define ITER_C ComponentPoolEntityIterator<C>

namespace arch::ecs::_details {

TEMPLATE_C
ITER_C::ComponentPoolEntityIterator(const ComponentPoolIterator<C>& base) noexcept: _base{ base } {}

TEMPLATE_C
void ITER_C::swap(ITER_C& other) noexcept {
	std::swap(_base, other._base);
}

TEMPLATE_C
ITER_C& ITER_C::operator++() noexcept {
	++_base;
	return *this;
}

TEMPLATE_C
ITER_C ITER_C::operator++(int) noexcept {
	auto temp = *this;
	++(*this);
	return temp;
}

TEMPLATE_C
ITER_C& ITER_C::operator--() noexcept {
	--_base;
	return *this;
}

TEMPLATE_C
ITER_C ITER_C::operator--(int) noexcept {
	auto temp = *this;
	--(*this);
	return temp;
}

TEMPLATE_C
const Entity& ITER_C::operator*() const noexcept {
	return _base.entity();
}

TEMPLATE_C
const Entity* ITER_C::operator->() const noexcept {
	return &_base.entity();
}

TEMPLATE_C
const Entity& ITER_C::operator[](std::ptrdiff_t n) const noexcept {
	return *(*this + n);
}

TEMPLATE_C
ITER_C& ITER_C::operator+=(std::ptrdiff_t n) noexcept requires(!Traits::inPlace)
{
	_base += n;
	return *this;
}

TEMPLATE_C
ITER_C ITER_C::operator+(std::ptrdiff_t n) const noexcept requires(!Traits::inPlace)
{
	auto temp = *this;
	temp += n;
	return temp;
}

TEMPLATE_C
ITER_C& ITER_C::operator-=(std::ptrdiff_t n) noexcept requires(!Traits::inPlace)
{
	_base -= n;
	return *this;
}

TEMPLATE_C
ITER_C ITER_C::operator-(std::ptrdiff_t n) const noexcept requires(!Traits::inPlace)
{
	auto temp = *this;
	temp -= n;
	return temp;
}

TEMPLATE_C
std::ptrdiff_t ITER_C::operator-(const ComponentPoolEntityIterator& other) const noexcept requires(!Traits::inPlace)
{
	return _base - other._base;
}

TEMPLATE_C
bool ITER_C::operator==(const ComponentPoolEntityIterator& other) const noexcept {
	return _base == other._base;
}

TEMPLATE_C
std::strong_ordering ITER_C::operator<=>(const ComponentPoolEntityIterator& other) const noexcept {
	return _base <=> other._base;
}

TEMPLATE_C
requires(!_details::ComponentTraits<C>::inPlace) ITER_C operator+(std::ptrdiff_t n, const ITER_C& i) noexcept {
	return i + n;
}

} // namespace arch::ecs::_details

#undef ITER_C
#undef TEMPLATE_C

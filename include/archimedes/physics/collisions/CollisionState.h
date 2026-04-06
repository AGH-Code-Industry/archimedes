#pragma once

namespace arch::physics {
enum CollisionState {
    Entered,
    Exited,
    Lasting,
    NotExisting,
    CurrentlyFound
};
} // namespace arch::physics
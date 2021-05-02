# NonSTL
Custom implementations of C++ components with a heavy focus on containers.

The goal is to be as close to compliant with the standard interfaces as possible while gaining performance over stl.

All components are noexcept safe and allocator aware.

# Completed components
containers/vector

containers/circular_buffer - A ring / circular buffer implementation of templated size

# In progress
None

# Future work
containers/deque

containers/linkedhashmap - A LinkedHashmap implementation following the standard interface and guarantees for <unordered_map> while maintaining insertion order

containers/stack

Experimental map / set implementations

Benchmarks for containers/vector vs std::vector

Networking components for easy use of sockets

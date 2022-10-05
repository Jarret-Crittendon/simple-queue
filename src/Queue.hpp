#include <memory>

// Required for parameters in swap
template <typename> class Queue;
// Foward template declaration needed for friends in Queue
template <typename T> void swap(Queue<T>&, Queue<T>&);

template <typename T>
class Queue {
    friend void swap<T>(Queue<T>&, Queue<T>&);
public:
    using size_type = size_t;
    // Constructors
    Queue(): start(nullptr), first_free(nullptr), cap(nullptr) { }
    Queue(const Queue&);
    
    // Operators
    //Queue& operator=(const Queue&);
    Queue& operator=(Queue);

    // public member functions

    // Push item into Queue
    void enqueue(const T& item);
    // Remove first item from Queue
    T dequeue();
    // Get first item from queue
    T peek() const { return *start; }
    // Check if the queue if empty
    bool empty() const
        { return start == first_free; }
    // Get current size of the queue
    size_type size() const
        { return first_free - start; }
    // Check current capacity of queue
    size_type capacity() const
        { return cap - start; }
    // Get iterator to the beginning of the current queue
    T* begin() const { return start; }
    // Get iterator to one past the end of the current queue
    T* end() const { return first_free; }
    
    // Destructor
    ~Queue();

private:
    // Check if capacity has been reached; if so, call reallocate()
    void chk_n_alloc();
    // Reallocate more space, then move data structure to new space
    void reallocate();
    // Destroy and dellocate entire Queue
    void free();

    T* start;
    T* first_free;
    T* cap;

    std::allocator<T> alloc;
    std::allocator_traits<decltype(alloc)> alloc_t;
};

// Copy Constructor definition
template <typename T>
Queue<T>::Queue(const Queue& rhs)
{
    // Only perform if this isn't self-assignment
    if (this != &rhs) {
        // Allocate new storage (returns iterator to first element)
        start = alloc_t.allocate(alloc, rhs.capacity());

        // Copy rhs elements into lhs Queue (returns iterator to element past the last element copied)
        first_free = std::uninitialized_copy_n(rhs.begin(), rhs.size(), start);

        // Set capacity pointer
        cap = start + rhs.capacity();
    }
}

// copy-assignment operator using copy & swap idiom
template <typename T>
Queue<T>& Queue<T>::operator=(Queue rhs)
{
    swap(*this, rhs);
    return *this;
}

// Swap
template <typename T>
void swap(Queue<T> &lhs, Queue<T> &rhs)
{
    using std::swap;
    std::cout << "SWAP\n";
    swap(lhs.start, rhs.start);
    swap(lhs.first_free, rhs.first_free);
    swap(lhs.cap, rhs.cap);

    swap(lhs.alloc, rhs.alloc);
    swap(lhs.alloc_t, rhs.alloc_t);
}

template <typename T>
void Queue<T>::chk_n_alloc()
{
    if ( size() == capacity() ) {
        reallocate();
    }
}

template <typename T>
void Queue<T>::enqueue(const T& item)
{
    // Check if there's space; reallocate if not
    chk_n_alloc();

    // Construct value in first free space passing the argument to the constructor
    alloc_t.construct(alloc, first_free++, item);
}

template <typename T>
T Queue<T>::dequeue()
{
    if (empty()) {
        throw std::out_of_range("Error: Nothing to pop from Queue; no damage done.");
    }

    // Move values in sequential memory one back

    // point at first element to be dequeued
    auto current = start;
    // point at element after the dequeued element
    auto next = start + 1;
    auto value = *current;
    while (next != first_free) {
        // Copy value from following element into dequeued element,
        // then increment pointers
        *current++ = *next++;
    }

    // Destroy value at back of Queue
    alloc_t.destroy(alloc, --first_free);

    return value;
}

template <typename T>
void Queue<T>::reallocate()
{
    size_type block_size = size() ? 2 * capacity() : 1;
    auto new_start = alloc_t.allocate(alloc, block_size);
    auto new_cap = new_start + block_size;

    // Copy existing values from old Queue to new one
    auto new_first_free = std::uninitialized_copy(start, first_free, new_start);

    // free() destroys and deallocates old Queue
    free();

    // Have start, first_free, and cap point to new Queue
    start = new_start;
    first_free = new_first_free;
    cap = new_cap;
}

template <typename T>
void Queue<T>::free()
{
    // Destroy old elements
    while (first_free != start) {
        alloc_t.destroy(alloc, --first_free);
    }

    // Deallocate old Queue
    alloc_t.deallocate(alloc, start, cap - start);
}

template <typename T>
Queue<T>::~Queue()
{
    free();
    start = nullptr;
    first_free = nullptr;
    cap = nullptr;
}
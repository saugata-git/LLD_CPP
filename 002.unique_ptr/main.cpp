
#include <iostream>
#include <utility>   // std::move, std::forward

template <class T>
class UniquePtr {
private:
    T* ptr;  // the owned raw pointer

public:
    // 1) default constructor: owns nothing
    UniquePtr() : ptr(nullptr) {}

    // 2) take ownership of a raw pointer
    explicit UniquePtr(T* p) : ptr(p) {}

    // 3) destructor: RAII cleanup
    ~UniquePtr() {
        delete ptr; // safe even if ptr == nullptr
    }

    // 4) disable copy (prevents double-delete)
    UniquePtr(const UniquePtr&) = delete;
    UniquePtr& operator=(const UniquePtr&) = delete;

    // 5) move constructor: steal ownership
    UniquePtr(UniquePtr&& other) noexcept : ptr(other.ptr) {
        other.ptr = nullptr;
    }

    // 6) move assignment: delete current, then steal
    UniquePtr& operator=(UniquePtr&& other) noexcept {
        if (this != &other) {
            delete ptr;          // free what we currently own
            ptr = other.ptr;     // steal
            other.ptr = nullptr; // leave source empty
        }
        return *this;
    }

    // ---- Observers ----
    T* get() const { return ptr; }
    explicit operator bool() const { return ptr != nullptr; }

    T& operator*() const { return *ptr; }
    T* operator->() const { return ptr; }

    // ---- Modifiers ----
    // release ownership without deleting (caller must manage)
    T* release() {
        T* temp = ptr;
        ptr = nullptr;
        return temp;
    }

    // delete current and optionally take new pointer
    void reset(T* p = nullptr) {
        if (ptr != p) {
            delete ptr;
            ptr = p;
        }
    }

    // swap ownership
    void swap(UniquePtr& other) {
        std::swap(ptr, other.ptr);
    }
};

// make_unique-like helper (small version)
template <class T, class... Args>
UniquePtr<T> make_unique(Args&&... args) {
    return UniquePtr<T>(new T(std::forward<Args>(args)...));
}

// Demo type
struct Foo {
    int x;
    Foo(int v) : x(v) { std::cout << "Foo ctor (" << x << ")\n"; }
    ~Foo() { std::cout << "Foo dtor (" << x << ")\n"; }
    void hello() { std::cout << "Hello x=" << x << "\n"; }
};

int main() {
    std::cout << "Create p\n";
    auto p = make_unique<Foo>(10);
    p->hello();

    std::cout << "\nMove p -> q\n";
    UniquePtr<Foo> q = std::move(p);  // ownership transfer
    std::cout << "p is " << (p ? "not null" : "null") << "\n";
    q->hello();

    std::cout << "\nReset q to new Foo\n";
    q.reset(new Foo(99));             // deletes old Foo(10), owns Foo(99)
    q->hello();

    std::cout << "\nRelease q (manual delete needed)\n";
    Foo* raw = q.release();           // q no longer owns it
    std::cout << "q is " << (q ? "not null" : "null") << "\n";
    raw->hello();
    delete raw;                       // since we released ownership

    std::cout << "\nEnd of main (destructors run)\n";
    return 0;
}

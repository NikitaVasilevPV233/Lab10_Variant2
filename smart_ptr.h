#ifndef SMART_PTR_H
#define SMART_PTR_H

template <typename T>
class SmartPtr {
public:
    explicit SmartPtr(T* ptr = nullptr) : pointer(ptr) {}

    ~SmartPtr() { delete pointer; }

    SmartPtr(const SmartPtr&) = delete;
    SmartPtr& operator=(const SmartPtr&) = delete;

    SmartPtr(SmartPtr&& other) noexcept : pointer(other.pointer) {
        other.pointer = nullptr;
    }
    SmartPtr& operator=(SmartPtr&& other) noexcept {
        if (this != &other) {
            delete pointer;
            pointer = other.pointer;
            other.pointer = nullptr;
        }
        return *this;
    }

    T* operator->() const { return pointer; }
    T& operator*() const  { return *pointer; }
    T* get() const { return pointer; }

    void reset(T* newPtr = nullptr) {
        if (pointer != newPtr) {
            delete pointer;
            pointer = newPtr;
        }
    }

private:
    T* pointer;
};

#endif // SMART_PTR_H

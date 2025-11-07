/**
 * DEE WALLET - Secure Memory Utilities
 * RAII-based secure memory management
 */

#ifndef SECUREMEMORY_H
#define SECUREMEMORY_H

#include <QByteArray>
#include <memory>

/**
 * Secure allocator that locks memory and wipes on destruction
 */
template<typename T>
class SecureAllocator {
public:
    using value_type = T;

    SecureAllocator() = default;

    template<typename U>
    SecureAllocator(const SecureAllocator<U>&) {}

    T* allocate(std::size_t n);
    void deallocate(T* p, std::size_t n);
};

/**
 * Secure byte array that wipes memory on destruction
 */
class SecureBytes {
public:
    SecureBytes();
    explicit SecureBytes(size_t size);
    explicit SecureBytes(const QByteArray &data);
    ~SecureBytes();

    // Disable copy
    SecureBytes(const SecureBytes&) = delete;
    SecureBytes& operator=(const SecureBytes&) = delete;

    // Enable move
    SecureBytes(SecureBytes&& other) noexcept;
    SecureBytes& operator=(SecureBytes&& other) noexcept;

    // Data access
    char* data();
    const char* data() const;
    size_t size() const;
    bool isEmpty() const;

    // Convert to QByteArray (creates copy)
    QByteArray toByteArray() const;

    // Secure wipe
    void clear();

private:
    QByteArray m_data;
};

/**
 * RAII wrapper for private keys
 */
class PrivateKey {
public:
    PrivateKey();
    explicit PrivateKey(const QByteArray &key);
    ~PrivateKey();

    // Disable copy
    PrivateKey(const PrivateKey&) = delete;
    PrivateKey& operator=(const PrivateKey&) = delete;

    const SecureBytes& data() const;
    bool isValid() const;

private:
    SecureBytes m_key;
};

#endif // SECUREMEMORY_H

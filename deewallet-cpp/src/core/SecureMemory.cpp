/**
 * DEE WALLET - Secure Memory Implementation
 */

#include "SecureMemory.h"
#include <openssl/crypto.h>

#ifdef _WIN32
#include <windows.h>
#else
#include <sys/mman.h>
#endif

template<typename T>
T* SecureAllocator<T>::allocate(std::size_t n)
{
    T* ptr = static_cast<T*>(::operator new(n * sizeof(T)));

    // Lock memory to prevent swapping to disk
#ifdef _WIN32
    VirtualLock(ptr, n * sizeof(T));
#else
    mlock(ptr, n * sizeof(T));
#endif

    return ptr;
}

template<typename T>
void SecureAllocator<T>::deallocate(T* p, std::size_t n)
{
    // Securely wipe memory
    OPENSSL_cleanse(p, n * sizeof(T));

    // Unlock memory
#ifdef _WIN32
    VirtualUnlock(p, n * sizeof(T));
#else
    munlock(p, n * sizeof(T));
#endif

    ::operator delete(p);
}

// Explicit instantiation for common types
template class SecureAllocator<char>;
template class SecureAllocator<unsigned char>;

SecureBytes::SecureBytes()
{
}

SecureBytes::SecureBytes(size_t size)
    : m_data(size, 0)
{
}

SecureBytes::SecureBytes(const QByteArray &data)
    : m_data(data)
{
}

SecureBytes::~SecureBytes()
{
    clear();
}

SecureBytes::SecureBytes(SecureBytes&& other) noexcept
    : m_data(std::move(other.m_data))
{
}

SecureBytes& SecureBytes::operator=(SecureBytes&& other) noexcept
{
    if (this != &other) {
        clear();
        m_data = std::move(other.m_data);
    }
    return *this;
}

char* SecureBytes::data()
{
    return m_data.data();
}

const char* SecureBytes::data() const
{
    return m_data.constData();
}

size_t SecureBytes::size() const
{
    return m_data.size();
}

bool SecureBytes::isEmpty() const
{
    return m_data.isEmpty();
}

QByteArray SecureBytes::toByteArray() const
{
    return m_data;
}

void SecureBytes::clear()
{
    if (!m_data.isEmpty()) {
        OPENSSL_cleanse(m_data.data(), m_data.size());
        m_data.clear();
    }
}

PrivateKey::PrivateKey()
{
}

PrivateKey::PrivateKey(const QByteArray &key)
    : m_key(key)
{
}

PrivateKey::~PrivateKey()
{
    // SecureBytes destructor will handle wiping
}

const SecureBytes& PrivateKey::data() const
{
    return m_key;
}

bool PrivateKey::isValid() const
{
    return !m_key.isEmpty();
}

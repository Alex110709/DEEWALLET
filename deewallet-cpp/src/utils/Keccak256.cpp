/**
 * DEE WALLET - Keccak256 Implementation
 * Based on FIPS 202 (SHA-3) specification
 */

#include "Keccak256.h"
#include <cstring>

const uint64_t Keccak256::ROUND_CONSTANTS[ROUNDS] = {
    0x0000000000000001ULL, 0x0000000000008082ULL, 0x800000000000808aULL,
    0x8000000080008000ULL, 0x000000000000808bULL, 0x0000000080000001ULL,
    0x8000000080008081ULL, 0x8000000000008009ULL, 0x000000000000008aULL,
    0x0000000000000088ULL, 0x0000000080008009ULL, 0x000000008000000aULL,
    0x000000008000808bULL, 0x800000000000008bULL, 0x8000000000008089ULL,
    0x8000000000008003ULL, 0x8000000000008002ULL, 0x8000000000000080ULL,
    0x000000000000800aULL, 0x800000008000000aULL, 0x8000000080008081ULL,
    0x8000000000008080ULL, 0x0000000080000001ULL, 0x8000000080008008ULL
};

const int Keccak256::RHO_OFFSETS[STATE_SIZE] = {
    0,  1,  62, 28, 27,
    36, 44, 6,  55, 20,
    3,  10, 43, 25, 39,
    41, 45, 15, 21, 8,
    18, 2,  61, 56, 14
};

QByteArray Keccak256::hash(const QByteArray &input)
{
    // Keccak-256 parameters
    constexpr int RATE = 136;  // 1088 bits / 8
    constexpr int OUTPUT_LENGTH = 32;  // 256 bits / 8

    uint64_t state[STATE_SIZE] = {0};

    // Absorb phase
    int blockSize = RATE;
    const uint8_t *data = reinterpret_cast<const uint8_t*>(input.constData());
    size_t dataLen = input.size();

    while (dataLen >= blockSize) {
        for (int i = 0; i < blockSize / 8; ++i) {
            uint64_t value = 0;
            for (int j = 0; j < 8; ++j) {
                value |= static_cast<uint64_t>(data[i * 8 + j]) << (8 * j);
            }
            state[i] ^= value;
        }
        keccakF(state);
        data += blockSize;
        dataLen -= blockSize;
    }

    // Padding
    uint8_t padded[RATE] = {0};
    std::memcpy(padded, data, dataLen);
    padded[dataLen] = 0x01;  // Keccak padding (not SHA3)
    padded[blockSize - 1] |= 0x80;

    for (int i = 0; i < blockSize / 8; ++i) {
        uint64_t value = 0;
        for (int j = 0; j < 8; ++j) {
            value |= static_cast<uint64_t>(padded[i * 8 + j]) << (8 * j);
        }
        state[i] ^= value;
    }
    keccakF(state);

    // Squeeze phase
    QByteArray output;
    for (int i = 0; i < OUTPUT_LENGTH / 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            output.append(static_cast<char>((state[i] >> (8 * j)) & 0xFF));
        }
    }

    return output;
}

void Keccak256::keccakF(uint64_t state[STATE_SIZE])
{
    for (int round = 0; round < ROUNDS; ++round) {
        theta(state);
        rho(state);
        pi(state);
        chi(state);
        iota(state, round);
    }
}

void Keccak256::theta(uint64_t state[STATE_SIZE])
{
    uint64_t C[5];
    for (int x = 0; x < 5; ++x) {
        C[x] = state[x] ^ state[x + 5] ^ state[x + 10] ^ state[x + 15] ^ state[x + 20];
    }

    uint64_t D[5];
    for (int x = 0; x < 5; ++x) {
        D[x] = C[(x + 4) % 5] ^ rotateLeft(C[(x + 1) % 5], 1);
    }

    for (int x = 0; x < 5; ++x) {
        for (int y = 0; y < 5; ++y) {
            state[x + 5 * y] ^= D[x];
        }
    }
}

void Keccak256::rho(uint64_t state[STATE_SIZE])
{
    for (int i = 0; i < STATE_SIZE; ++i) {
        state[i] = rotateLeft(state[i], RHO_OFFSETS[i]);
    }
}

void Keccak256::pi(uint64_t state[STATE_SIZE])
{
    uint64_t temp[STATE_SIZE];
    std::memcpy(temp, state, sizeof(temp));

    for (int x = 0; x < 5; ++x) {
        for (int y = 0; y < 5; ++y) {
            state[y + 5 * ((2 * x + 3 * y) % 5)] = temp[x + 5 * y];
        }
    }
}

void Keccak256::chi(uint64_t state[STATE_SIZE])
{
    for (int y = 0; y < 5; ++y) {
        uint64_t temp[5];
        for (int x = 0; x < 5; ++x) {
            temp[x] = state[x + 5 * y];
        }
        for (int x = 0; x < 5; ++x) {
            state[x + 5 * y] = temp[x] ^ ((~temp[(x + 1) % 5]) & temp[(x + 2) % 5]);
        }
    }
}

void Keccak256::iota(uint64_t state[STATE_SIZE], int round)
{
    state[0] ^= ROUND_CONSTANTS[round];
}

uint64_t Keccak256::rotateLeft(uint64_t value, int shift)
{
    return (value << shift) | (value >> (64 - shift));
}

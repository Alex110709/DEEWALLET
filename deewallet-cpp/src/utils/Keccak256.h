/**
 * DEE WALLET - Keccak256 Implementation
 * For Ethereum address generation
 */

#ifndef KECCAK256_H
#define KECCAK256_H

#include <QByteArray>
#include <cstdint>

class Keccak256 {
public:
    static QByteArray hash(const QByteArray &input);

private:
    static constexpr int ROUNDS = 24;
    static constexpr int STATE_SIZE = 25;

    static void keccakF(uint64_t state[STATE_SIZE]);
    static void theta(uint64_t state[STATE_SIZE]);
    static void rho(uint64_t state[STATE_SIZE]);
    static void pi(uint64_t state[STATE_SIZE]);
    static void chi(uint64_t state[STATE_SIZE]);
    static void iota(uint64_t state[STATE_SIZE], int round);

    static uint64_t rotateLeft(uint64_t value, int shift);

    static const uint64_t ROUND_CONSTANTS[ROUNDS];
    static const int RHO_OFFSETS[STATE_SIZE];
};

#endif // KECCAK256_H

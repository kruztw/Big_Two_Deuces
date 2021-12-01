#include <algorithm>
#include <vector>
#include <unordered_set>

bool isStraightFlush(const std::vector<int> &input);
bool isStraight(const std::vector<int> &input);
bool isFlush(const std::vector<int> &input);

constexpr int cardNum = 5;
constexpr int colorNum = 4;
constexpr int minCardIdx = 0x2;
constexpr int maxCardIdx = 0xe;
constexpr int oneColorCardNum = maxCardIdx - minCardIdx;



bool isRoyalFlush(const std::vector<int> &input) {
    return ((*min_element(input.begin(), input.end()))&0xf) == 0xa && isStraightFlush(input);
}

bool isStraightFlush(const std::vector<int> &input) {
    return isFlush(input) && isStraight(input);
}

bool isFourOfAKind(const std::vector<int> &input) {
    std::vector<int> tmp(cardNum);

    for (int i = 0; i < cardNum; ++i)
        tmp[i] = input[i] & 0xf;

    std::sort(tmp.begin(), tmp.end(), [](int a, int b) { return a <= b; });

    return (tmp[0] == tmp[3]) || (tmp[1] == tmp[4]);
}

bool isFullHouse(const std::vector<int> &input) {
    std::vector<int> tmp(cardNum);

    for (int i = 0; i < cardNum; ++i)
        tmp[i] = input[i] & 0xf;

    std::sort(tmp.begin(), tmp.end(), [](int a, int b) { return a <= b; });

    return (tmp[0] == tmp[2] && tmp[3] == tmp[4]) ||
           (tmp[0] == tmp[1] && tmp[2] == tmp[4]);
}

bool isFlush(const std::vector<int> &input) {
    int color = input[0] >> 8;
    for (int i = 1; i < cardNum; ++i)
        if (input[i] >> 8 != color)
            return false;

    return true;
}

bool isStraight(const std::vector<int> &input) {
    std::vector<int> tmp(cardNum);
    for (int i = 0; i < cardNum; ++i)
        tmp[i] = input[i] & 0xf;

    std::sort(tmp.begin(), tmp.end(), [](int a, int b) { return a <= b; });

    int val = tmp[0];
    for (int i = 1; i < cardNum; ++i)
        if (tmp[i] != val + i) {
            if (i == cardNum - 1 && tmp[i] == maxCardIdx &&
                tmp[0] == 2) // A2345
                return true;
            return false;
        }

    return true;
}


bool isThreeOfAKind(const std::vector<int> &input) {
    std::vector<int> tmp(cardNum);

    for (int i = 0; i < cardNum; ++i)
        tmp[i] = input[i] & 0xf;

    std::sort(tmp.begin(), tmp.end(), [](int a, int b) { return a <= b; });

    return (tmp[0] == tmp[2] && tmp[3] != tmp[4]) ||
           (tmp[1] == tmp[3] && tmp[0] != tmp[4]) ||
           (tmp[2] == tmp[4] && tmp[0] != tmp[1]);
}

bool isTwoPairs(const std::vector<int> &input) {
    std::vector<int> tmp(cardNum);
    std::vector<bool> is_used(cardNum);

    for (int i = 0; i < cardNum; ++i)
        tmp[i] = input[i] & 0xf;

    std::sort(tmp.begin(), tmp.end(), [](int a, int b) { return a <= b; });

    int pairs = 0;
    for (int i = 1; i < cardNum; ++i)
        if (tmp[i] == tmp[i - 1] && !is_used[i - 1]) {
            ++pairs;
            is_used[i] = true;
        }

    return pairs == 2;
}

bool isOnePair(const std::vector<int> &input) {
    std::vector<int> tmp(cardNum);

    for (int i = 0; i < cardNum; ++i)
        tmp[i] = input[i] & 0xf;

    std::sort(tmp.begin(), tmp.end(), [](int a, int b) { return a <= b; });

    bool pair = false;
    for (int i = 1; i < cardNum; ++i)
        if (tmp[i] == tmp[i - 1]) {
            if (pair)
                return false;
            pair = true;
        }

    return pair;
}

enum e_kind {
    RoyalFlush = 1,
    StraightFlush,
    FourOfAKind,
    FullHouse,
    Flush,
    Straight,
    ThreeOfAKind,
    TwoPairs,
    OnePair,
    HighCard
};

int check(const std::vector<int> &input) {
    if (input.size() != cardNum) // Cards' number must be five.
        return -1;

    std::unordered_set<int> set_values;
    for (int i = 0; i < cardNum; ++i) {
        int color = input[i] >> 8;
        int value = input[i] & 0xff;
        if (color < 1 || color > 4 || value < 2 || value > 0xe) // invalid input
            return -1;

        if (set_values.count(input[i])) // invalid input
            return -1;

        set_values.insert(input[i]);
    }

    if (isRoyalFlush(input))
        return RoyalFlush;

    if (isStraightFlush(input))
        return StraightFlush;

    if (isFourOfAKind(input))
        return FourOfAKind;

    if (isFullHouse(input))
        return FullHouse;

    if (isFlush(input))
        return Flush;

    if (isStraight(input))
        return Straight;

    if (isThreeOfAKind(input))
        return ThreeOfAKind;

    if (isTwoPairs(input))
        return TwoPairs;

    if (isOnePair(input))
        return OnePair;

    return HighCard;
}




// unitest

#include <iostream>
#include <cstdlib>

int main() {

    int a;
    std::vector<int> input = {0x20a, 0x20b, 0x10c, 0x10d, 0x10e};

    while (true) {
        for (int i = 0; i < cardNum; ++i) {
            input[i] = ((rand() % 4 + 1) << 8) + (rand()%(0xd) + 2);
            //std::cout << std::hex << input[i] << " ";
        }
        std::cout << std::endl;
        std::cout << std::dec << check(input) << std::endl;

        std::sort(input.begin(), input.end(),
                  [](int a, int b) { return (a & 0xf) <= (b & 0xf); });
        for (int i = 0; i < cardNum; ++i)
            std::cout << std::hex << input[i] << " ";
        std::cout << std::endl;
        std::cin >> a;          // easy way to pause
    }
}

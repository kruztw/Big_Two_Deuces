#include <algorithm>
#include <vector>
#include <unordered_set>

#include <iostream>

bool isStraightFlush(const std::vector<int> &input);
bool isStraight(const std::vector<int> &input);
bool isFlush(const std::vector<int> &input);

constexpr int cardNum = 5;
constexpr int colorNum = 4;
constexpr int minCardIdx = 0x2;
constexpr int maxCardIdx = 0xe;
constexpr int oneColorCardNum = maxCardIdx - minCardIdx;
constexpr int wildCardNum = 2;

inline bool isWildCard(int val) { return val == 0x50f || val == 0x610; }   // 二張鬼牌, 0x50f & 0x610

bool isRoyalFlush(const std::vector<int> &input) {
    for (int i = 0; i < cardNum; ++i)
        if ((input[i] & 0xf) < 0xa && !isWildCard(input[i]))
            return false;

    return isStraightFlush(input);
}

bool isStraightFlush(const std::vector<int> &input) {
    return isFlush(input) && isStraight(input);
}

bool isFourOfAKind(const std::vector<int> &input) {
    std::vector<int> tmp(cardNum);
    for (int i = 0; i < cardNum; ++i)
        tmp[i] = isWildCard(input[i]) ? input[i] : input[i] & 0xf;

    std::sort(tmp.begin(), tmp.end(), [](int a, int b) { return a <= b; });

    if (isWildCard(tmp[cardNum - 2]))
        return tmp[0] == tmp[1] || tmp[1] == tmp[2];
    if (isWildCard(tmp[cardNum - 1]))
        return tmp[0] == tmp[2] || tmp[1] == tmp[3];

    return tmp[0] == tmp[3] || tmp[1] == tmp[4];
}

bool isFullHouse(const std::vector<int> &input) {
    std::vector<int> tmp(cardNum);
    for (int i = 0; i < cardNum; ++i)
        tmp[i] = isWildCard(input[i]) ? input[i] : input[i] & 0xf;

    std::sort(tmp.begin(), tmp.end(), [](int a, int b) { return a <= b; });

    if (isWildCard(tmp[cardNum - 2]))
        return tmp[0] == tmp[1] || tmp[1] == tmp[2];
    if (isWildCard(tmp[cardNum - 1]))
        return (tmp[0] == tmp[1] && tmp[1] == tmp[2]) ||
	       (tmp[0] == tmp[2] || tmp[1] == tmp[3]);

    return (tmp[0] == tmp[2] && tmp[3] == tmp[4]) ||
           (tmp[0] == tmp[1] && tmp[2] == tmp[4]);
}

bool isFlush(const std::vector<int> &input) {
    int color, i;

    for (i = 0; i < cardNum; ++i)
        if (!isWildCard(input[i])) {
            color = input[i] >> 8;
            break;
        }

    for (; i < cardNum; ++i) {
        if (isWildCard(input[i]))
	    continue;
        if ((input[i] >> 8) != color)
            return false;
    }

    return true;
}

bool isStraight(const std::vector<int> &input) {
    std::vector<int> tmp(cardNum);
    for (int i = 0; i < cardNum; ++i)
        tmp[i] = isWildCard(input[i]) ? input[i] : input[i] & 0xf;

    std::sort(tmp.begin(), tmp.end(), [](int a, int b) { return a <= b; });

    int val = tmp[0];
    for (int i = 1; i < cardNum; ++i) {
        if (isWildCard(tmp[i]))
            continue;

        if (tmp[i] == tmp[i - 1])
            return false;

        if (tmp[i] - val >= cardNum) {
            if (tmp[i] == maxCardIdx) { // 2345A
                for (int j = 0; j < i; ++j)
                    if (tmp[j] > 5)
                        return false;

                return true;
            }

            return false;
        }
    }

    return true;
}


bool isThreeOfAKind(const std::vector<int> &input) {
    std::vector<int> tmp(cardNum);
    for (int i = 0; i < cardNum; ++i)
        tmp[i] = isWildCard(input[i]) ? input[i] : input[i] & 0xf;

    std::sort(tmp.begin(), tmp.end(), [](int a, int b) { return a <= b; });

    if (isWildCard(tmp[cardNum - 2]))
        return true;
    if (isWildCard(tmp[cardNum - 1]))
        return tmp[0] == tmp[1] || tmp[1] == tmp[2] || tmp[2] == tmp[3];

    return tmp[0] == tmp[2] || tmp[1] == tmp[3] || tmp[2] == tmp[4];
}

bool isTwoPairs(const std::vector<int> &input) {
    std::vector<int> tmp(cardNum);
    std::vector<bool> is_used(cardNum);

    for (int i = 0; i < cardNum; ++i)
        tmp[i] = isWildCard(input[i]) ? input[i] : input[i] & 0xf;

    std::sort(tmp.begin(), tmp.end(), [](int a, int b) { return a <= b; });

    int pairs = 0;
    for (int i = 1; i < cardNum; ++i)
        if ((tmp[i] == tmp[i-1] || isWildCard(tmp[i])) && !is_used[i - 1]) {
            ++pairs;
            is_used[i] = true;
        }

    return pairs == 2;
}

bool isOnePair(const std::vector<int> &input) {
    std::vector<int> tmp(cardNum);
    for (int i = 0; i < cardNum; ++i)
        tmp[i] = isWildCard(input[i]) ? input[i] : input[i] & 0xf;

    std::sort(tmp.begin(), tmp.end(), [](int a, int b) { return a <= b; });

    bool pair = false;
    for (int i = 1; i < cardNum; ++i)
        if (tmp[i] == tmp[i-1] || isWildCard(tmp[i])) {
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
        if (color < 1 || color > 4 || value < 2 || value > 0xe) {
	    if (isWildCard(input[i]) && !set_values.count(input[i])) {
		    set_values.insert(input[i]);
		    continue;
	    }
            return -1;
	}

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

#include <cstdlib>
#include <unistd.h>

int main() {

    int a;
    std::vector<int> input = { 0x406, 0x409, 0x40b, 0x50f, 0x610};

    while (true) {

        std::cout << std::endl;
        std::cout << std::dec << check(input) << std::endl;


        std::sort(input.begin(), input.end(),
                  [](int a, int b) { return (a & 0xff) <= (b & 0xff); });

        for (int i = 0; i < cardNum; ++i)
            std::cout << std::hex << input[i] << " ";


        std::cout << std::endl << std::endl;
        std::cin >> a;                         // easy way to pause

        for (int i = 0; i < cardNum; ++i) {
            input[i] = ((rand() % 4 + 1) << 8) + (rand()%(0xd) + 2);
            //std::cout << std::hex << input[i] << " ";
        }
	for (int i = 0; i < 1; ++i)
    	    input[rand()%cardNum] = 0x610;

	for (int i = 0; i < 1; ++i)
    	    input[rand()%cardNum] = 0x50f;
    }
}

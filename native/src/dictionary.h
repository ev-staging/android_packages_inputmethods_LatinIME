/*
 * Copyright (C) 2009 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef LATINIME_DICTIONARY_H
#define LATINIME_DICTIONARY_H

#include "bigram_dictionary.h"
#include "char_utils.h"
#include "correction.h"
#include "defines.h"
#include "proximity_info.h"
#include "unigram_dictionary.h"
#include "words_priority_queue_pool.h"

namespace latinime {

class Dictionary {
 public:
    Dictionary(void *dict, int dictSize, int mmapFd, int dictBufAdjust, int typedLetterMultipler,
            int fullWordMultiplier, int maxWordLength, int maxWords);

    int getSuggestions(ProximityInfo *proximityInfo, int *xcoordinates, int *ycoordinates,
            int *codes, int codesSize, int flags, unsigned short *outWords, int *frequencies) {
        return mUnigramDictionary->getSuggestions(proximityInfo, mWordsPriorityQueuePool,
                mCorrection, xcoordinates, ycoordinates, codes,
                codesSize, flags, outWords, frequencies);
    }

    // TODO: Call mBigramDictionary instead of mUnigramDictionary
    int getBigrams(unsigned short *word, int length, int *codes, int codesSize,
            unsigned short *outWords, int *frequencies, int maxWordLength, int maxBigrams) {
        return mBigramDictionary->getBigrams(word, length, codes, codesSize, outWords, frequencies,
                maxWordLength, maxBigrams);
    }

    bool isValidWord(unsigned short *word, int length);
    void *getDict() { return (void *)mDict; }
    int getDictSize() { return mDictSize; }
    int getMmapFd() { return mMmapFd; }
    int getDictBufAdjust() { return mDictBufAdjust; }
    ~Dictionary();

    // public static utility methods
    // static inline methods should be defined in the header file
    static int wideStrLen(unsigned short *str);

 private:
    bool hasBigram();

    const unsigned char *mDict;

    // Used only for the mmap version of dictionary loading, but we use these as dummy variables
    // also for the malloc version.
    const int mDictSize;
    const int mMmapFd;
    const int mDictBufAdjust;

    const bool IS_LATEST_DICT_VERSION;
    UnigramDictionary *mUnigramDictionary;
    BigramDictionary *mBigramDictionary;
    WordsPriorityQueuePool *mWordsPriorityQueuePool;
    Correction *mCorrection;
};

// public static utility methods
// static inline methods should be defined in the header file
inline int Dictionary::wideStrLen(unsigned short *str) {
    if (!str) return 0;
    unsigned short *end = str;
    while (*end)
        end++;
    return end - str;
}
} // namespace latinime

#endif // LATINIME_DICTIONARY_H

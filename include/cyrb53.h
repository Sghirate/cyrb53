// cyrb53 1.0.0
/*
    Based on: https://github.com/bryc/code/blob/master/jshash/experimental/cyrb53.js
    C port by: Frederick Parotat (frederick@parotat.de)

    Original copyright notice below:
*/
/*
    cyrb53 (c) 2018 bryc (github.com/bryc)
    A fast and simple hash function with decent collision resistance.
    Largely inspired by MurmurHash2/3, but with a focus on speed/simplicity.
    Public domain. Attribution appreciated.
*/
#ifndef _CYRB53_H_
#define _CYRB53_H_

#include <stddef.h>
#include <stdint.h>

uint64_t cyrb53(const char* a_str, size_t a_length, uint64_t a_seed) {
        uint64_t h1 = 0xdeadbeef ^ a_seed;
        uint64_t h2 = 0x41c6ce57 ^ a_seed;
        for (int i = 0; i < a_length; ++i) {
            uint64_t ch = a_str[i];
            h1 = (uint32_t)((h1 ^ ch) * 2654435761);
            h2 = (uint32_t)((h2 ^ ch) * 1597334677);
        }
        h1 = (uint32_t)(((h1 ^ (h1 >> 16)) * 2246822507) ^ ((h2 ^ (h2 >> 13)) * 3266489909));
        h2 = (uint32_t)(((h2 ^ (h2 >> 16)) * 2246822507) ^ ((h1 ^ (h1 >> 13)) * 3266489909));
        return 4294967296 * (uint64_t)(2097151 & h2) + (h1 >> 0);
}

uint64_t wcyrb53(const wchar_t* a_str, size_t a_length, uint64_t a_seed) {
        uint64_t h1 = 0xdeadbeef ^ a_seed;
        uint64_t h2 = 0x41c6ce57 ^ a_seed;
        for (int i = 0; i < a_length; ++i) {
            uint64_t ch = a_str[i];
            h1 = (uint32_t)((h1 ^ ch) * 2654435761);
            h2 = (uint32_t)((h2 ^ ch) * 1597334677);
        }
        h1 = (uint32_t)(((h1 ^ (h1 >> 16)) * 2246822507) ^ ((h2 ^ (h2 >> 13)) * 3266489909));
        h2 = (uint32_t)(((h2 ^ (h2 >> 16)) * 2246822507) ^ ((h1 ^ (h1 >> 13)) * 3266489909));
        return 4294967296 * (uint64_t)(2097151 & h2) + (h1 >> 0);
}

#ifdef CYRB53_TEST_IMPLEMENETATION

#include <locale.h>
#include <stdbool.h>
#include <string.h>
#include <wchar.h>

int cyrb53_test_case(char* a_str, wchar_t* a_wstr, uint64_t a_seed, uint64_t a_expected, size_t* inout_n, size_t* inout_successful) {
    // char array
    {
        ++(*inout_n);
        uint64_t hash = cyrb53(a_str, strlen(a_str), a_seed);
        if (hash == a_expected) {
            ++(*inout_successful);
            wprintf(L" \u2705  cyrb53( %24s ) => %20llu == %20llu\n", a_str, hash, a_expected);
        } else {
            wprintf(L" \u274C  cyrb53( %24s ) => %20llu != %20llu\n", a_str, hash, a_expected);
        }
    }
    // wide char array
    {
        ++(*inout_n);
        uint64_t hash = wcyrb53(a_wstr, wcslen(a_wstr), a_seed);
        if (hash == a_expected) {
            ++(*inout_successful);
            wprintf(L" \u2705 wcyrb53( %24ls ) => %20llu == %20llu\n", a_wstr, hash, a_expected);
        } else {
            wprintf(L" \u274C wcyrb53( %24ls ) => %20llu != %20llu\n", a_wstr, hash, a_expected);
        }
    }
}
int cyrb53_test() {
    setlocale(LC_ALL, "en_US.utf8");
    wprintf(L"CYRB53 Tests:\n");
    size_t n = 0;
    size_t successful = 0;
#define CYRB53_TEST(str, seed, expected) cyrb53_test_case(#str, L ## #str, seed##ULL, expected##ULL, &n, &successful)
    CYRB53_TEST(a                      , 0 , 7929297801672961 );
    CYRB53_TEST(b                      , 0 , 8684336938537663 );
    CYRB53_TEST(revenge                , 0 , 4051478007546757 );
    CYRB53_TEST(revenue                , 0 , 8309097637345594 );
    CYRB53_TEST(revenue                , 1 , 8697026808958300 );
    CYRB53_TEST(revenue                , 2 , 2021074995066978 );
    CYRB53_TEST(revenue                , 3 , 4747903550515895 );
    CYRB53_TEST(Sequence               , 0 , 7756856452616112 );
    CYRB53_TEST(Selector               , 0 , 1448451565510171 );
    CYRB53_TEST(AlwaysSucceed          , 0 , 401772658894367  );
    CYRB53_TEST(AlwaysFail             , 0 , 2971229458788787 );
    CYRB53_TEST(Invert                 , 0 , 8366452664421685 );
    CYRB53_TEST(Loop                   , 0 , 7485715072875018 );
    CYRB53_TEST(RepeatUntil            , 0 , 881904786666027  );
    CYRB53_TEST(IsSet                  , 0 , 1547735175870215 );
    CYRB53_TEST(Parallel               , 0 , 4521263870987759 );
    CYRB53_TEST(BBClear                , 0 , 8917156704886134 );
    CYRB53_TEST(BBSet                  , 0 , 6015821779488260 );
    CYRB53_TEST(BBIf                   , 0 , 4421743012074668 );
    CYRB53_TEST(BBCondition            , 0 , 278180481297486  );
    CYRB53_TEST(BBWhile                , 0 , 7607420050815094 );
    CYRB53_TEST(BBRandom               , 0 , 4668719395419390 );
    CYRB53_TEST(Exec                   , 0 , 7676683540213405 );
    CYRB53_TEST(Check                  , 0 , 1819149779628785 );
    CYRB53_TEST(If                     , 0 , 7438581250448222 );
    CYRB53_TEST(While                  , 0 , 1392948405323523 );
    CYRB53_TEST(HasVar                 , 0 , 5453053682216216 );
    CYRB53_TEST(Log                    , 0 , 4774511274977645 );
    CYRB53_TEST(Wait                   , 0 , 5024019079844329 );
    CYRB53_TEST(State                  , 0 , 7028365944634507 );
    CYRB53_TEST(StateTransition        , 0 , 4544604791720493 );
    CYRB53_TEST(StateMachine           , 0 , 7749037758784483 );
    CYRB53_TEST(BBOptional             , 0 , 4751955790483525 );
    CYRB53_TEST(BBRequire              , 0 , 6702231717910081 );
#undef CYRB53_TEST
    wprintf(L"%ls CYRB53 (%llu / %llu)\n", (successful == n ? L"\u2705" : L"\u274C"), successful, n);
    return successful == n ? 0 : -1;
}

#endif // CYRB53_TEST_IMPLEMENETATION

#endif // _CYRB53_H_

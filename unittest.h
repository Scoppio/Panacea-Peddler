/*  Panacea Pedler
 *  @author scoppio
 *  @date 2019-2020
 */
#include "lib/unittestcodes.h"
#ifndef MAIN
#define MAIN
#include "main.h"
#endif

unsigned char result = 0;

void test_point_count(void) {
    // add 4 cards on table
    (*table_ptr)[0] = &blue_cards[3];
    (*table_ptr)[1] = &blue_cards[3];
    (*table_ptr)[2] = &blue_cards[3];
    (*table_ptr)[3] = &blue_cards[3];

    result = count_points();
    assert_equals(0, result);
}

void test_point_count2(void) {
    // add 4 cards on table
    (*table_ptr)[0] = &blue_cards[3];
    (*table_ptr)[1] = &blue_cards[3];
    (*table_ptr)[2] = &blue_cards[3];
    (*table_ptr)[3] = &blue_cards[13];

    result = count_points();
    assert_equals(2, result);
}

void test_point_count3(void) {
    // add 4 cards on table
    (*table_ptr)[0] = &blue_cards[3];
    (*table_ptr)[1] = &blue_cards[3];
    (*table_ptr)[2] = &blue_cards[13];
    (*table_ptr)[3] = &blue_cards[13];

    result = count_points();
    assert_equals(0, result);
}

void test_point_count4(void) {
    // add 4 cards on table
    blue_cards[3].Lmodifier = BLUE_CARD;
    blue_cards[3].Rmodifier = BLUE_CARD;
    (*table_ptr)[0] = &blue_cards[3];
    (*table_ptr)[1] = &blue_cards[3];
    (*table_ptr)[2] = &blue_cards[13];
    (*table_ptr)[3] = &blue_cards[13];

    result = count_points();
    assert_equals(2, result);
}

void run_tests(void) {
    test_point_count();
    test_point_count2();
    test_point_count3();
    test_point_count4();
}

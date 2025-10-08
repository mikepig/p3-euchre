#include "Card.hpp"
#include "unit_test_framework.hpp"
#include <iostream>

using namespace std;


TEST(test_card_ctor) {
    Card c(ACE, HEARTS);
    ASSERT_EQUAL(ACE, c.get_rank());
    ASSERT_EQUAL(HEARTS, c.get_suit());
}

#include "Card.hpp"
#include "unit_test_framework.hpp"
#include <sstream>
using namespace std;

TEST(test_string_to_rank_basic) {
  ASSERT_EQUAL(string_to_rank("Ace"), ACE);
  ASSERT_EQUAL(string_to_rank("Two"), TWO);
  ASSERT_EQUAL(string_to_rank("Jack"), JACK);
}

TEST(test_rank_stream_in_out) {
  ostringstream oss;
  oss << KING;
  ASSERT_EQUAL(oss.str(), "King");
  istringstream iss("Queen");
  Rank r = TWO;
  iss >> r;
  ASSERT_EQUAL(r, QUEEN);
}

TEST(test_string_to_suit_basic) {
  ASSERT_EQUAL(string_to_suit("Spades"), SPADES);
  ASSERT_EQUAL(string_to_suit("Hearts"), HEARTS);
  ASSERT_EQUAL(string_to_suit("Clubs"), CLUBS);
  ASSERT_EQUAL(string_to_suit("Diamonds"), DIAMONDS);
}

TEST(test_suit_stream_in_out) {
  ostringstream oss;
  oss << DIAMONDS;
  ASSERT_EQUAL(oss.str(), "Diamonds");
  istringstream iss("Hearts");
  Suit s = SPADES;
  iss >> s;
  ASSERT_EQUAL(s, HEARTS);
}

TEST(test_card_default_ctor_and_accessors) {
  Card c;
  ASSERT_EQUAL(c.get_rank(), TWO);
  ASSERT_EQUAL(c.get_suit(), SPADES);
}

TEST(test_card_ctor_and_accessors) {
  Card c(ACE, HEARTS);
  ASSERT_EQUAL(c.get_rank(), ACE);
  ASSERT_EQUAL(c.get_suit(), HEARTS);
}

TEST(test_is_face_or_ace_basic_true_cases) {
  ASSERT_TRUE(Card(JACK, CLUBS).is_face_or_ace());
  ASSERT_TRUE(Card(QUEEN, SPADES).is_face_or_ace());
  ASSERT_TRUE(Card(KING, DIAMONDS).is_face_or_ace());
  ASSERT_TRUE(Card(ACE, HEARTS).is_face_or_ace());
}

TEST(test_is_face_or_ace_false_cases) {
  ASSERT_FALSE(Card(TEN, HEARTS).is_face_or_ace());
  ASSERT_FALSE(Card(TWO, CLUBS).is_face_or_ace());
}

TEST(test_suit_next_pairs) {
  ASSERT_EQUAL(Suit_next(SPADES), CLUBS);
  ASSERT_EQUAL(Suit_next(CLUBS), SPADES);
  ASSERT_EQUAL(Suit_next(HEARTS), DIAMONDS);
  ASSERT_EQUAL(Suit_next(DIAMONDS), HEARTS);
}

TEST(test_is_right_bower_and_left_bower_basic) {
  Suit trump = HEARTS;
  Card right_bower(JACK, HEARTS);
  Card left_bower(JACK, DIAMONDS);
  Card not_bower(JACK, SPADES);

  ASSERT_TRUE(right_bower.is_right_bower(trump));
  ASSERT_TRUE(left_bower.is_left_bower(trump));
  ASSERT_FALSE(not_bower.is_right_bower(trump));
  ASSERT_FALSE(not_bower.is_left_bower(trump));
}

TEST(test_is_trump_basic) {
  ASSERT_TRUE(Card(TEN, CLUBS).is_trump(CLUBS));
  ASSERT_FALSE(Card(TEN, CLUBS).is_trump(SPADES));
}

TEST(test_get_suit_with_trump_and_left_bower_aliasing) {
  Suit trump = SPADES;
  Card left_bower(JACK, CLUBS);
  Card normal(TEN, SPADES);
  Card off(TEN, DIAMONDS);

  ASSERT_EQUAL(left_bower.get_suit(trump), SPADES);
  ASSERT_EQUAL(normal.get_suit(trump), SPADES);
  ASSERT_EQUAL(off.get_suit(trump), DIAMONDS);
}

TEST(test_card_stream_out) {
  Card c(ACE, HEARTS);
  ostringstream oss;
  oss << c;
  ASSERT_EQUAL(oss.str(), "Ace of Hearts");
}

TEST(test_card_stream_in) {
  istringstream iss("Nine of Clubs");
  Card c;
  iss >> c;
  ASSERT_EQUAL(c.get_rank(), NINE);
  ASSERT_EQUAL(c.get_suit(), CLUBS);
}


TEST(test_card_eq_neq) {
  Card a(KING, CLUBS);
  Card b(KING, CLUBS);
  Card c(KING, SPADES);
  ASSERT_TRUE(a == b);
  ASSERT_TRUE(a != c);
}

TEST(test_card_less_rank_then_suit) {
  Card a(QUEEN, SPADES);
  Card b(KING, SPADES);
  Card c(KING, CLUBS);
  ASSERT_TRUE(a < b);
  ASSERT_TRUE((c < b) == false);
  ASSERT_TRUE((c < b) || (b < c));}

TEST(test_card_le_ge_gt) {
  Card a(TEN, DIAMONDS);
  Card b(TEN, DIAMONDS);
  Card c(JACK, DIAMONDS);
  ASSERT_TRUE(a <= b);
  ASSERT_TRUE(c >= b);
  ASSERT_TRUE(c > a);
  ASSERT_TRUE(!(a > c));
}


TEST(test_card_less_with_trump_basic) {
  Suit trump = CLUBS;
  Card off1(KING, HEARTS);
  Card off2(ACE, HEARTS);
  Card trump_low(NINE, CLUBS);
  Card right(JACK, CLUBS);
  Card left(JACK, SPADES);

  ASSERT_TRUE(Card_less(off1, off2, trump));
  ASSERT_TRUE(Card_less(off2, trump_low, trump));
  ASSERT_TRUE(Card_less(trump_low, left, trump));
  ASSERT_TRUE(Card_less(left, right, trump));
  ASSERT_FALSE(Card_less(right, left, trump));
}

TEST(test_card_less_with_trump_equal_rank_suit_tiebreak) {
  Suit trump = DIAMONDS;
  Card a(QUEEN, SPADES);
  Card b(QUEEN, CLUBS);
  ASSERT_TRUE(Card_less(a, b, trump) == (a < b));
}

TEST(test_card_less_with_led_no_trump_played) {
  Suit trump = HEARTS;
  Card led_card(NINE, SPADES);
  Card a(KING, SPADES);
  Card b(ACE, CLUBS);
  ASSERT_TRUE(Card_less(b, a, led_card, trump));
  ASSERT_FALSE(Card_less(a, b, led_card, trump));
}

TEST(test_card_less_with_led_trump_present) {
  Suit trump = SPADES;
  Card led_card(TEN, DIAMONDS);
  Card a(KING, DIAMONDS);
  Card b(NINE, SPADES);
  ASSERT_TRUE(Card_less(a, b, led_card, trump));
}

TEST(test_card_less_with_led_bower_aliasing) {
  Suit trump = HEARTS;
  Card led_card(QUEEN, HEARTS);
  Card normal_heart(TEN, HEARTS);
  Card left_bower(JACK, DIAMONDS);
  ASSERT_TRUE(Card_less(normal_heart, left_bower, led_card, trump));
  ASSERT_FALSE(Card_less(left_bower, normal_heart, led_card, trump));
}

TEST_MAIN()

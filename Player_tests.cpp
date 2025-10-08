#include "Player.hpp"
#include "unit_test_framework.hpp"

#include <iostream>

using namespace std;

TEST(test_player_get_name) {
    Player * alice = Player_factory("Alice", "Simple");
    ASSERT_EQUAL("Alice", alice->get_name());

    delete alice;
}

TEST(test_player_factory_human_simple) {
    Player *h = Player_factory("Bob", "Human");
    Player *s = Player_factory("Eve", "Simple");
    ASSERT_EQUAL("Bob", h->get_name());
    ASSERT_EQUAL("Eve", s->get_name());
    delete h;
    delete s;
}

TEST(test_simpleplayer_add_card_and_sort) {
    Player *p = Player_factory("Alice", "Simple");
    Card c1(ACE, HEARTS);
    Card c2(NINE, CLUBS);
    Card c3(KING, SPADES);
    p->add_card(c1);
    p->add_card(c2);
    p->add_card(c3);
    Suit trump = HEARTS;
    Card lead = p->lead_card(trump);
    ASSERT_TRUE(lead.get_rank() == KING || lead.get_rank() == ACE || lead.get_rank() == NINE);
    delete p;
}

TEST(test_simpleplayer_make_trump_round1_accept_and_reject) {
    Player *p = Player_factory("Alice", "Simple");
    Card upcard(JACK, HEARTS);
    p->add_card(Card(ACE, HEARTS));
    p->add_card(Card(KING, HEARTS));
    Suit order_up;
    bool accept = p->make_trump(upcard, false, 1, order_up);
    ASSERT_TRUE(accept);
    ASSERT_EQUAL(order_up, HEARTS);

    Player *q = Player_factory("Eve", "Simple");
    q->add_card(Card(NINE, CLUBS));
    q->add_card(Card(TEN, DIAMONDS));
    Suit order2;
    bool reject = q->make_trump(upcard, false, 1, order2);
    ASSERT_FALSE(reject);
    delete p;
    delete q;
}

TEST(test_simpleplayer_make_trump_round2_dealer_auto_accept) {
    Player *p = Player_factory("Alice", "Simple");
    Card upcard(ACE, HEARTS);
    Suit order_up;
    bool accept = p->make_trump(upcard, true, 2, order_up);
    ASSERT_TRUE(accept);
    ASSERT_EQUAL(order_up, Suit_next(HEARTS));
    delete p;
}

TEST(test_simpleplayer_add_and_discard) {
    Player *p = Player_factory("Alice", "Simple");
    p->add_card(Card(TEN, HEARTS));
    p->add_card(Card(JACK, CLUBS));
    p->add_and_discard(Card(ACE, SPADES));
    delete p;
}

TEST(test_simpleplayer_lead_card_prefers_nontrump) {
    Player *p = Player_factory("Alice", "Simple");
    p->add_card(Card(ACE, CLUBS));
    p->add_card(Card(KING, SPADES));
    p->add_card(Card(QUEEN, SPADES));
    Suit trump = SPADES;
    Card lead = p->lead_card(trump);
    ASSERT_EQUAL(lead.get_suit(), CLUBS);
    delete p;
}

TEST(test_simpleplayer_play_card_follow_suit) {
    Player *p = Player_factory("Alice", "Simple");
    Suit trump = HEARTS;
    p->add_card(Card(NINE, CLUBS));
    p->add_card(Card(KING, SPADES));
    p->add_card(Card(QUEEN, SPADES));
    Card led(ACE, SPADES);
    Card play = p->play_card(led, trump);
    ASSERT_EQUAL(play.get_suit(), SPADES);
    delete p;
}

TEST(test_simpleplayer_play_card_no_follow_suit) {
    Player *p = Player_factory("Alice", "Simple");
    Suit trump = HEARTS;
    p->add_card(Card(NINE, CLUBS));
    p->add_card(Card(TEN, DIAMONDS));
    p->add_card(Card(JACK, CLUBS));
    Card led(ACE, SPADES);
    Card play = p->play_card(led, trump);
    ASSERT_TRUE(play.get_suit() == CLUBS || play.get_suit() == DIAMONDS);
    delete p;
}

TEST(test_player_insertion_operator) {
    Player *p = Player_factory("Alice", "Simple");
    ostringstream oss;
    oss << *p;
    ASSERT_EQUAL(oss.str(), "Alice");
    delete p;
}

TEST_MAIN()

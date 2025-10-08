#include "Pack.hpp"
#include "unit_test_framework.hpp"

#include <iostream>

using namespace std;

Card c(Rank r, Suit s) { 
    return Card(r, s);
 }
TEST(test_pack_default_ctor_first_several) {
  Pack p;
  Card a = p.deal_one();
  Card b = p.deal_one();
  Card c1 = p.deal_one();

  ASSERT_EQUAL(a.get_rank(), NINE);
  ASSERT_EQUAL(a.get_suit(), SPADES);

  ASSERT_EQUAL(b.get_rank(), TEN);
  ASSERT_EQUAL(b.get_suit(), SPADES);

  ASSERT_EQUAL(c1.get_rank(), JACK);
  ASSERT_EQUAL(c1.get_suit(), SPADES);
}

TEST(test_pack_deal_progress_and_empty) {
  Pack p;
  for (int i = 0; i < 23; ++i) {
    (void)p.deal_one();
  }
  ASSERT_FALSE(p.empty());
  (void)p.deal_one();
  ASSERT_TRUE(p.empty());
}

TEST(test_pack_reset_returns_to_start) {
  Pack p;
  (void)p.deal_one();
  (void)p.deal_one();
  p.reset();
  Card a = p.deal_one();
  ASSERT_EQUAL(a.get_rank(), NINE);
  ASSERT_EQUAL(a.get_suit(), SPADES);
  ASSERT_FALSE(p.empty());
}

TEST(test_pack_istream_ctor_custom_order) {
  ostringstream oss;
  for (int s = DIAMONDS; s >= SPADES; --s) {
    for (int r = ACE; r >= NINE; --r) {
      oss << static_cast<Rank>(r) << " of " << static_cast<Suit>(s) << "\n";
    }
  }
  istringstream iss(oss.str());
  Pack p(iss);

  Card first = p.deal_one();
  ASSERT_EQUAL(first.get_rank(), ACE);
  ASSERT_EQUAL(first.get_suit(), DIAMONDS);

  Card second = p.deal_one();
  ASSERT_EQUAL(second.get_rank(), KING);
  ASSERT_EQUAL(second.get_suit(), DIAMONDS);

  for (int i = 0; i < 4; ++i) (void)p.deal_one(); 
  Card seventh = p.deal_one();
  ASSERT_EQUAL(seventh.get_rank(), ACE);
  ASSERT_EQUAL(seventh.get_suit(), HEARTS);
}
TEST(test_pack_shuffle_top_card_matches_spec) {
  Pack p;
  p.shuffle();
  Card first = p.deal_one();
  ASSERT_EQUAL(first, Card(KING, CLUBS)); 
}

TEST(test_pack_shuffle_is_permutation_and_resets_next) {

  Pack p1, p2;
  vector<Card> original, after;
  for (int i = 0; i < 24; ++i) original.push_back(p1.deal_one());

  p2.shuffle();  
  for (int i = 0; i < 24; ++i) after.push_back(p2.deal_one());

  sort(original.begin(), original.end());
  sort(after.begin(), after.end());
  ASSERT_EQUAL(original.size(), after.size());
  for (size_t i = 0; i < original.size(); ++i) {
    ASSERT_EQUAL(original[i], after[i]);  
  }


  ASSERT_TRUE(p2.empty());
}

TEST(test_pack_multiple_shuffles) {
  Pack p;
  p.shuffle();
  p.shuffle();
  vector<Card> v;
  for (int i = 0; i < 24; ++i) v.push_back(p.deal_one());
  sort(v.begin(), v.end());
  for (int i = 1; i < 24; ++i) {
    ASSERT_TRUE(!(v[i] != v[i-1]) || (v[i] != v[i-1] && !(v[i] == v[i-1])));
  }
  ASSERT_TRUE(find(v.begin(), v.end(), Card(NINE, SPADES)) != v.end());
  ASSERT_TRUE(find(v.begin(), v.end(), Card(ACE, DIAMONDS)) != v.end());
}

TEST_MAIN()

#include "Pack.hpp"
#include <cassert>
#include <iostream>
#include <array>
#include "Card.hpp"
#include <string>


using namespace std;

Pack::Pack(){
    next = 0;
    int count=0;
    for (int s = SPADES; s <= DIAMONDS; ++s) {
        Suit suit = static_cast<Suit>(s);
        for (int j = TWO; j <=ACE; ++j){
            Rank rank = static_cast<Rank>(j);
            Card Card(rank, suit);
            cards[0] = Card;
            count+=1;
        }
    }

}

 Pack::Pack(std::istream& pack_input){
    next = 0;
    string of;
    Rank rank;
    Suit suit;
    for(int i = 0; i!= PACK_SIZE; ++i){
        pack_input >> rank >> of >> suit;
        Card card(rank, suit);
        cards[i] = card;
    }
 }

 Card Pack::deal_one(){
    Card dealing = cards[next];
    next+=1;
    return dealing;
 }

 void Pack:: reset(){
    next = 0;
 }

 void Pack::shuffle(){

    std::array<Card, PACK_SIZE> new_pack;
    for (int i = 0; i<7; ++i){
        for (int j = 0; int k = 0; j<PACK_SIZE; j+=2;++k){
            new_pack[j]=cards[j];
            new_pack[j+1]=cards[j+]
        }
    }
 }

 bool Pack::empty() const{
    if (cards.size()==0){
        return true;
    }
    return false;
 }
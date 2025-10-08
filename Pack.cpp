#include "Pack.hpp"
#include <cassert>
#include <iostream>
#include <array>
#include "Card.hpp"
#include <string>


using namespace std;
const int next_card = 0;
Pack::Pack(){
    next = next_card;
    int count = 0;
    for (int s = SPADES; s <= DIAMONDS; ++s) {
        for (int j = NINE; j <=ACE; ++j){
            Suit suit = static_cast<Suit>(s);
            Rank rank = static_cast<Rank>(j);
            Card Card(rank, suit);
            cards[count] = Card;
            count +=1;
        }
    }
    

}

 Pack::Pack(std::istream& pack_input){
    next = next_card;
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
    int half = PACK_SIZE/2;
    
    for (int i = 0;i<7; ++i){
        std::array<Card, PACK_SIZE> new_pack;
        int j =0;
        for (int k = 0;  k<half; ++k){
            new_pack[j]=cards[half+k];
            new_pack[j+1]=cards[k];
            j+=2;
        }
        cards=new_pack;
    }
    next = 0;
 }

 bool Pack::empty() const{
    return next==24;

    }
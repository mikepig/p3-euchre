#include "Player.hpp"
#include <cassert>
#include <iostream>
#include <array>
#include "Card.hpp"
#include "Pack.hpp"
#include <algorithm>

using namespace std;

class SimplePlayer : public Player {
    private:
        string name;
        std::vector<Card> hand;
    public:
        SimplePlayer(const std::string &name_in): name(name_in){}
          //EFFECTS returns player's name

        const std::string & get_name() const override{
            return name;
        }

        void add_card(const Card &c)override{
            assert(hand.size()<MAX_HAND_SIZE);
            hand.push_back(c);
        }
        bool make_trump(const Card &upcard, bool is_dealer,
                          int round, Suit &order_up_suit) const override{
            assert(round ==1 ||round ==2);

            
            if(round ==1){
                int count = 0;
                for (int i = 0; i<hand.size(); i++){
                    if ((hand[i].is_face_or_ace()&&hand[i].is_trump(upcard.get_suit()))||hand[i].is_left_bower(upcard.get_suit())){
                        count+=1;
                    if (count>=2){
                        order_up_suit = upcard.get_suit();
                    }
                    else {
                        return false;
                    }
                    }
                }
            }
            else if (round == 2){
                std::sort(hand.begin(),hand.end()); 
                if (is_dealer){
                    order_up_suit = Suit_next(upcard.get_suit());
                }
                for (int i = 0; i<hand.size(); i++){
                    if ((hand[i].is_trump(upcard.get_suit()) && hand[i].is_face_or_ace())
                    || hand[i].is_left_bower(upcard.get_suit())) {
                        order_up_suit = Suit_next(upcard.get_suit());
                        return true;
                    }
                }
            }
                          }    
        

        void add_and_discard(const Card &upcard) override{
            hand.push_back(upcard);
            std::sort(hand.begin(),hand.end()); 
            hand.erase(hand.begin());
        }

        Card lead_card(Suit trump) override{
            std::sort(hand.begin(),hand.end()); 
            std::reverse(hand.begin(),hand.end());
            int count = 0;

            Card trump_card;
            for (int i = 0; i!=hand.size();i++){
                if (!hand[i].is_trump(trump)){
                    return hand[i];
                    hand.erase(hand.begin()+i);
                }
                else{
                    count+=1;
                }
            }
            if (count ==hand.size()){
                return hand[0];
            }

        }

        Card play_card(const Card &led_card, Suit trump)override{
            std::sort(hand.begin(),hand.end());
            std::reverse(hand.begin(),hand.end());
            Card played;
            int count = 0;
            for (int i = 0; i!=hand.size(); i++){
                if (hand[i].get_suit(trump)==led_card.get_suit(trump)){
                    played = hand[i];
                    hand.erase(hand.begin()+i);
                    count+=1;
                    return played;
                }
            }
            if (count==0){
                played  = hand[0];
                hand.erase(hand.begin());
                return played;
            }
        }
        ~SimplePlayer()override {}
    };

        
        
class HumanPlayer: public Player{
    private:
        string name;
        std::vector<Card> hand;
    public:
        HumanPlayer(const std::string &name_in): name(name_in){}
        
        const std::string & get_name() const override{
                return name;
            }
            void add_card(const Card &c)override{
                assert(hand.size()<MAX_HAND_SIZE);
                hand.push_back(c);
                
            }

            void print_hand() const {
                for (size_t i=0; i < hand.size(); ++i)
                    cout << "Human player " << name << "'s hand: "
                    << "[" << i << "] " << hand[i] << "\n";
            }
            bool make_trump(const Card &upcard, bool is_dealer,
                            int round, Suit &order_up_suit) const override{

                print_hand();

                cout << "Human player " << name << ", please enter a suit, or \"pass\":\n";
                string decision;
                cin >>decision;
                if (decision != "pass"){
                    order_up_suit = string_to_suit(decision);
                    return true;
                }
                else{
                    return false;
                }
            }

            void add_and_discard(const Card &upcard) override{
                add_card(upcard);
                std::sort(hand.begin(),hand.end()); 
                print_hand();
                cout << "Discard upcard: [-1]\n";
                cout << "Human player " << name << ", please select a card to discard:\n";
                int card_number;
                cin >> card_number;
                if (card_number == -1){
                    hand.pop_back();
                }
                else {
                    hand.erase(hand.begin() + card_number);
                }   
            }

            Card lead_card(Suit trump) override{
                        int card_number;
                Card lead_card;
                std::sort(hand.begin(), hand.end());

                for (int i = 0, size = hand.size(); i != size; ++i){
                    cout << "Human player "<< name << "'s hand: " << "[" << i << "] " << hand[i] << endl;
                }
                cout << "Human player " << name << ", please select a card:" <<endl;
                cin >> card_number;
                cout << hand[card_number] << " led by " << name << endl;
                
                lead_card = hand[card_number];
                hand.erase(hand.begin() + card_number);
                
                return lead_card;

            }   
            Card play_card(const Card &led_card, Suit trump) override{
                assert(hand.size()>0);
                return lead_card(trump);
            }
            ~HumanPlayer()override{}
        };


Player * Player_factory(const std::string &name, const std::string &strategy) {
  if (strategy == "Simple") {
    // The "new" keyword dynamically allocates an object.
    return new SimplePlayer(name);
  }
  // Repeat for each other type of Player
  else if (strategy == "Human"){
    return new HumanPlayer(name);
  }
  // Invalid strategy if we get here
  assert(false);
  return nullptr;
}

std::ostream & operator<<(std::ostream &os, const Player &p) {
    os << p.get_name();

    return os;
}
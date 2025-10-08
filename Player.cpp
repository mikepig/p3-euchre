#include "Player.hpp"
#include <cassert>
#include <iostream>
#include <array>
#include "Card.hpp"
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
            assert(hand.size()<=MAX_HAND_SIZE);
            hand.push_back(c);
            std::sort(hand.begin(),hand.end()); 
        }
        bool make_trump(const Card &upcard, bool is_dealer,
                          int round, Suit &order_up_suit) const override{
            assert(round ==1 ||round ==2);

            
            if(round ==1){
                int count = 0;
                for (int i = 0; i<hand.size(); i++){
                    if ((hand[i].is_face_or_ace()&&hand[i].is_trump(upcard.get_suit()))||
                    hand[i].is_left_bower(upcard.get_suit(upcard.get_suit()))){
                        count+=1;
                    }

                }
                if (count>=2){
                    order_up_suit = upcard.get_suit();
                    return true;
                    }
                return false;
            }
            if (is_dealer){
                order_up_suit = Suit_next(upcard.get_suit());
                return true;
            }
            for (int i = 0; i<hand.size(); i++){
                if ((hand[i].is_trump(Suit_next(upcard.get_suit())) && hand[i].is_face_or_ace())
                || hand[i].is_left_bower(upcard.get_suit())) {
                    order_up_suit = Suit_next(upcard.get_suit());
                    return true;
                }
            }
            return false;
        }

        
        

        void add_and_discard(const Card &upcard) override{
            add_card(upcard);
            hand.erase(hand.begin());
        }

        Card lead_card(Suit trump) override{
            int ind = -1;
            for (int i = 0; i!=hand.size();i++){

                if (!hand[i].is_trump(trump)&&(ind==-1||Card_less(hand[ind],hand[i],trump))){
                    ind =i;
                    }
    

            }
            if (ind ==-1){
                for (int i =0; i!=hand.size();i++){
                    if (ind ==-1|| Card_less(hand[ind], hand[i],trump)){
                    ind =i;
                    }
                }
            }
            Card Max = hand[ind];
            hand.erase(hand.begin()+ind);
            return Max;
    }


        Card play_card(const Card &led_card, Suit trump)override{
            assert(!hand.empty());
            int ind = -1;
            for(int i = 0; i!=hand.size(); i++){
                if (hand[i].get_suit(trump)==led_card.get_suit(trump)){
                    if (ind ==-1 || Card_less(hand[ind],hand[i],trump)){
                        ind = i;
                    }

                }
            }
            int pick;
            if (ind!=-1){
                pick = ind;
            }
            else {
                int index2 = 0;
                for (int i =0; i!=hand.size();i++){
                    if (Card_less(hand[i],hand[index2],trump)){
                        index2 = i;
                    }
                }
                pick = index2;
            }
            Card Max = hand[pick];
            hand.erase(hand.begin()+pick);
            return Max;
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
                std::sort(hand.begin(),hand.end()); 
                
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
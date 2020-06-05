#ifndef OBERON_CORE_COINBASE_H
#define OBERON_CORE_COINBASE_H


#include "reconstructor_impl.h"
#include <boost/property_tree/ptree.hpp>

#include <map>

#include "reconstructor.h"
#include <string>
using namespace std;

namespace oberon { namespace core { class Store; }  } 

namespace oberon {

namespace core {

class CoinbaseReconstructor : public ReconstructorImplementation {
  private:
    class CoinbaseReceived : public MessageHandler::Received {
      public:
        CoinbaseReceived(const boost::property_tree::ptree & tree);

    };
    
    class CoinbaseMatch : public MessageHandler::Filled {
    };
    
    class CoinbaseDone : public MessageHandler::FullyCanceled {
      public:
        CoinbaseDone(const boost::property_tree::ptree & tree);

    };
    
    class CoinbaseOpen : public MessageHandler::Opened {
      public:
        CoinbaseOpen(const boost::property_tree::ptree & tree);

        ~CoinbaseOpen();

    };
    

  protected:
    virtual vector<std::unique_ptr<MessageHandler::Message>> extract(const boost::property_tree::ptree & tree) override final;


  public:
    explicit CoinbaseReconstructor( Store * store);


  private:
    map<Id, Volume> remaining_size_;

};
class Deduce_Size_Coinbase_Extensions : public MessageHandler {
  protected:
    Volume previousSize_;

    map<Id, Volume> previousSizes_;

    std::unique_ptr<MessageHandler::ExchangeMessage> em_;


  public:
    virtual bool message() = 0;


  protected:
    virtual bool volumeIncremented();

    virtual bool priceAdvanced();

    virtual bool priceReceded();

    virtual bool partiallyCanceled();

    virtual bool fullyCanceled();

    virtual bool opened();

    virtual string getHandlerName();

};
// implement the state machine Deduce Size Coinbase
class Deduce_Size_Coinbase : public Deduce_Size_Coinbase_Extensions {
  protected:
    // Mother class of all the classes representing states
    class AnyState {
      public:
        virtual ~AnyState();

        // return the upper state
        virtual AnyState * _upper(Deduce_Size_Coinbase &) = 0;

        // perform the 'do activity'
        virtual void _do(Deduce_Size_Coinbase &);

        // the current state doesn't manage the event received, give it to the upper state
        virtual void received(Deduce_Size_Coinbase & stm);

        // the current state doesn't manage the event elapsed, give it to the upper state
        virtual void elapsed(Deduce_Size_Coinbase & stm);

        // the current state doesn't manage the event era, give it to the upper state
        virtual void era(Deduce_Size_Coinbase & stm);

        // the current state doesn't manage the event filled, give it to the upper state
        virtual void filled(Deduce_Size_Coinbase & stm);

        // the current state doesn't manage the event message, give it to the upper state
        virtual void message(Deduce_Size_Coinbase & stm);

        virtual void create(Deduce_Size_Coinbase &);

    };
    
    // implement the state Deduce Size Coinbase
    class Deduce_Size_Coinbase_State : public AnyState {
      public:
        // implement the state Wait
        class Wait_State : public AnyState {
          public:
            virtual ~Wait_State();

            // to manage the event received
            virtual void received(Deduce_Size_Coinbase & stm);

            // to manage the event elapsed
            virtual void elapsed(Deduce_Size_Coinbase & stm);

            // to manage the event era
            virtual void era(Deduce_Size_Coinbase & stm);

            // to manage the event filled
            virtual void filled(Deduce_Size_Coinbase & stm);

            // to manage the event message
            virtual void message(Deduce_Size_Coinbase & stm);

            // perform the 'exit behavior'
            void _doexit(Deduce_Size_Coinbase & stm);

            // returns the state containing the current
            virtual AnyState * _upper(Deduce_Size_Coinbase & stm);

        };
        
        // implement the state Save
        class Save_State : public AnyState {
          public:
            virtual ~Save_State();

            virtual bool _completion(Deduce_Size_Coinbase & stm);

            // to manage the event create
            virtual void create(Deduce_Size_Coinbase & stm);

            // perform the 'entry behavior'
            void _doentry(Deduce_Size_Coinbase & stm);

            // returns the state containing the current
            virtual AnyState * _upper(Deduce_Size_Coinbase & stm);

        };
        
        // implement the state Deduce
        class Deduce_State : public AnyState {
          public:
            // implement the state Size change
            class Size_change_State : public AnyState {
              public:
                virtual ~Size_change_State();

                virtual bool _completion(Deduce_Size_Coinbase & stm);

                // to manage the event create
                virtual void create(Deduce_Size_Coinbase & stm);

                // perform the 'entry behavior'
                void _doentry(Deduce_Size_Coinbase & stm);

                // returns the state containing the current
                virtual AnyState * _upper(Deduce_Size_Coinbase & stm);

            };
            
            // implement the state Remaining size
            class Remaining_size_State : public AnyState {
              public:
                virtual ~Remaining_size_State();

                virtual bool _completion(Deduce_Size_Coinbase & stm);

                // to manage the event create
                virtual void create(Deduce_Size_Coinbase & stm);

                // perform the 'entry behavior'
                void _doentry(Deduce_Size_Coinbase & stm);

                // returns the state containing the current
                virtual AnyState * _upper(Deduce_Size_Coinbase & stm);

            };
            
            virtual ~Deduce_State();

            // memorize the instance of the state Size change, internal
            Size_change_State _size_change_state;

            // memorize the instance of the state Remaining size, internal
            Remaining_size_State _remaining_size_state;

            // to manage the exit point 'exit', defined because probably shared
            void _exit1(Deduce_Size_Coinbase & stm);

            // perform the 'entry behavior'
            void _doentry(Deduce_Size_Coinbase & stm);

            // perform the 'exit behavior'
            void _doexit(Deduce_Size_Coinbase & stm);

            // returns the state containing the current
            virtual AnyState * _upper(Deduce_Size_Coinbase & stm);

        };
        
        virtual ~Deduce_Size_Coinbase_State();

        // memorize the instance of the state Wait, internal
        Wait_State _wait_state;

        // memorize the instance of the state Deduce, internal
        Deduce_State _deduce_state;

        // memorize the instance of the state Save, internal
        Save_State _save_state;

        // to manage the event create
        virtual void create(Deduce_Size_Coinbase & stm);

        // returns the state containing the current
        virtual AnyState * _upper(Deduce_Size_Coinbase &);

    };
    
    // memorize the instance of the state Deduce Size Coinbase, internal
    Deduce_Size_Coinbase_State _deduce_size_coinbase_state;


  public:
    Deduce_Size_Coinbase();

    virtual ~Deduce_Size_Coinbase();

    // the operation you call to signal the event received
    bool received();

    // the operation you call to signal the event elapsed
    bool elapsed();

    // the operation you call to signal the event era
    bool era();

    // the operation you call to signal the event filled
    bool filled();

    // the operation you call to signal the event message
    bool message();

  friend class Deduce_Size_Coinbase_State::Wait_State;
    // the operation you call to signal the event create
    bool create();

  friend class Deduce_Size_Coinbase_State::Deduce_State::Size_change_State;
  friend class Deduce_Size_Coinbase_State::Deduce_State::Remaining_size_State;
  friend class Deduce_Size_Coinbase_State::Deduce_State;
  friend class Deduce_Size_Coinbase_State::Save_State;
  friend class Deduce_Size_Coinbase_State;
    // to execute the current state 'do activity'
    void doActivity();


  protected:
    // change the current state, internal
    inline void _set_currentState(AnyState & st);

    // execution done, internal
    void _final();

    // contains the current state, internal
    AnyState * _current_state;

};
// change the current state, internal
inline void Deduce_Size_Coinbase::_set_currentState(Deduce_Size_Coinbase::AnyState & st) {
    _current_state = &st;
}

// implement the state machine Coinbase Deduplicator
class Coinbase_Deduplicator : public MessageHandler {
  protected:
    // Mother class of all the classes representing states
    class AnyState {
      public:
        virtual ~AnyState();

        // return the upper state
        virtual AnyState * _upper(Coinbase_Deduplicator &) = 0;

        // perform the 'do activity'
        virtual void _do(Coinbase_Deduplicator &);

        virtual void create(Coinbase_Deduplicator &);

        // the current state doesn't manage the event received, give it to the upper state
        virtual void received(Coinbase_Deduplicator & stm);

        // the current state doesn't manage the event elapsed, give it to the upper state
        virtual void elapsed(Coinbase_Deduplicator & stm);

        // the current state doesn't manage the event opened, give it to the upper state
        virtual void opened(Coinbase_Deduplicator & stm);

        // the current state doesn't manage the event filled, give it to the upper state
        virtual void filled(Coinbase_Deduplicator & stm);

        // the current state doesn't manage the event fullyCanceled, give it to the upper state
        virtual void fullyCanceled(Coinbase_Deduplicator & stm);

        // the current state doesn't manage the event era, give it to the upper state
        virtual void era(Coinbase_Deduplicator & stm);

        // the current state doesn't manage the event stop, give it to the upper state
        virtual void stop(Coinbase_Deduplicator & stm);

        // the current state doesn't manage the event priceAdvanced, give it to the upper state
        virtual void priceAdvanced(Coinbase_Deduplicator & stm);

        // the current state doesn't manage the event priceReceded, give it to the upper state
        virtual void priceReceded(Coinbase_Deduplicator & stm);

        // the current state doesn't manage the event partiallyCanceled, give it to the upper state
        virtual void partiallyCanceled(Coinbase_Deduplicator & stm);

        // the current state doesn't manage the event volumeIncremented, give it to the upper state
        virtual void volumeIncremented(Coinbase_Deduplicator & stm);

    };
    
    // implement the state Coinbase Deduplicator
    class Coinbase_Deduplicator_State : public AnyState {
      public:
        // implement the state Wait
        class Wait_State : public AnyState {
          public:
            virtual ~Wait_State();

            // to manage the event received
            virtual void received(Coinbase_Deduplicator & stm);

            // to manage the event elapsed
            virtual void elapsed(Coinbase_Deduplicator & stm);

            // to manage the event opened
            virtual void opened(Coinbase_Deduplicator & stm);

            // to manage the event filled
            virtual void filled(Coinbase_Deduplicator & stm);

            // to manage the event fullyCanceled
            virtual void fullyCanceled(Coinbase_Deduplicator & stm);

            // to manage the event era
            virtual void era(Coinbase_Deduplicator & stm);

            // to manage the event stop
            virtual void stop(Coinbase_Deduplicator & stm);

            // returns the state containing the current
            virtual AnyState * _upper(Coinbase_Deduplicator & stm);

            // to manage the event priceAdvanced
            virtual void priceAdvanced(Coinbase_Deduplicator & stm);

            // to manage the event priceReceded
            virtual void priceReceded(Coinbase_Deduplicator & stm);

            // to manage the event partiallyCanceled
            virtual void partiallyCanceled(Coinbase_Deduplicator & stm);

            // to manage the event volumeIncremented
            virtual void volumeIncremented(Coinbase_Deduplicator & stm);

        };
        
        // implement the state Fail
        class Fail_State : public AnyState {
          public:
            virtual ~Fail_State();

            // to manage the event create
            virtual void create(Coinbase_Deduplicator & stm);

            // perform the 'do activity'
            virtual void _do(Coinbase_Deduplicator & stm);

            // returns the state containing the current
            virtual AnyState * _upper(Coinbase_Deduplicator & stm);

        };
        
        // implement the state Output
        class Output_State : public AnyState {
          public:
            virtual ~Output_State();

            virtual bool _completion(Coinbase_Deduplicator & stm);

            // to manage the event create
            virtual void create(Coinbase_Deduplicator & stm);

            // perform the 'do activity'
            virtual void _do(Coinbase_Deduplicator & stm);

            // returns the state containing the current
            virtual AnyState * _upper(Coinbase_Deduplicator & stm);

        };
        
        virtual ~Coinbase_Deduplicator_State();

        // memorize the instance of the state Wait, internal
        Wait_State _wait_state;

        // memorize the instance of the state Output, internal
        Output_State _output_state;

        // to manage the event create
        virtual void create(Coinbase_Deduplicator & stm);

        // returns the state containing the current
        virtual AnyState * _upper(Coinbase_Deduplicator &);

        // memorize the instance of the state Fail, internal
        Fail_State _fail_state;

    };
    
    // memorize the instance of the state Coinbase Deduplicator, internal
    Coinbase_Deduplicator_State _coinbase_deduplicator_state;


  public:
    Coinbase_Deduplicator();

    virtual ~Coinbase_Deduplicator();

    // the operation you call to signal the event create
    bool create();

    // the operation you call to signal the event received
    bool received();

    // the operation you call to signal the event elapsed
    bool elapsed();

    // the operation you call to signal the event opened
    bool opened();

    // the operation you call to signal the event filled
    bool filled();

    // the operation you call to signal the event fullyCanceled
    bool fullyCanceled();

    // the operation you call to signal the event era
    bool era();

    // the operation you call to signal the event stop
    bool stop();

  friend class Coinbase_Deduplicator_State::Wait_State;
  friend class Coinbase_Deduplicator_State::Output_State;
  friend class Coinbase_Deduplicator_State;
    // to execute the current state 'do activity'
    void doActivity();


  protected:
    // change the current state, internal
    inline void _set_currentState(AnyState & st);

    // execution done, internal
    void _final();

    // contains the current state, internal
    AnyState * _current_state;


  public:
    // the operation you call to signal the event priceAdvanced
    bool priceAdvanced();

    // the operation you call to signal the event priceReceded
    bool priceReceded();

    // the operation you call to signal the event partiallyCanceled
    bool partiallyCanceled();

    // the operation you call to signal the event volumeIncremented
    bool volumeIncremented();

  friend class Coinbase_Deduplicator_State::Fail_State;

  protected:
    virtual string getHandlerName();

};
// change the current state, internal
inline void Coinbase_Deduplicator::_set_currentState(Coinbase_Deduplicator::AnyState & st) {
    _current_state = &st;
}


} // namespace oberon::core

} // namespace oberon
#endif

#ifndef _SERVICE_QUEUE_H
#define _SERVICE_QUEUE_H

#include <iostream>
#include <vector>
#include <utility>


//include this to not repeat multiple times through the code.
using std::vector;

class ServiceQueue {

  private:
        /** Your private data members here! 
        *   (you should have NO PUBLIC data members! 
        *                                            
        * Nested private types also go here.  
        * For example, if your design needs some kind of
        *   structure, you would specify it here.
        */
    
   
    
    //structure for a single node : DLL
    struct Node {
        int buzzerIDnumber; //int val
        Node *next;
        Node *prev;
        bool check = true;
    };
    
    //structure for the node to include a front and a back
    struct list {
        Node *front;
        Node *back;
        int allbuzzersnumber;
        int size;
    };
    
    list *Queue;
    list *used_buzzers;
    vector<int> allbuzzers;
    vector<Node*> allbuzzersNode;
    
    

    //HELPER FUNCTIONS FOR QUEUE LIST AND USED_BUZZERS LIST
    
    //function created to pop node from Queue list and subract size
    Node *popfrontQueue(list *p){
        Node* temp;
        temp = p->front;
        //int buzz;
        
        if(temp == nullptr){
            return nullptr;
        }
    
        //buzz = temp->buzzerIDnumber;

       // temp = p->front;
        
        //front is now pointing to the 2nd node in the list
        p->front = temp->next;
        //p->front->prev = nullptr;
        temp->next = nullptr;
       
        //delete temp;
        
        if(p->front == nullptr){
            p->back = nullptr;
        }
        //subtract from size
        p->size--;
        
        return temp;
    }
    
    //PROBABLY needed to push te bribe node to the front of the list, size should not be upadated--> USE PUSHFRONTUSED_BUZZERS
    void pushfrontQueue(list *p, Node *temp){
        
        temp->next = p->front;
        
        //if the list is empty
        if(p->front == nullptr){
           
            //point both front and back node to temp cause it is the front and back.
            p->front = temp;
            p->back = temp;
            temp->prev = nullptr;   //*********
            
            //increment size
            p->size++;
          //  std::cout <<"HERE" << p->size << std::endl;
        }
        else {
        //if the list is not empty
            p->front->prev = temp;
            p->front = temp;
            temp->prev = nullptr;
            
            //increment size
            p->size++;
            //std::cout <<"if there is a list"  <<p->size << std::endl;
        }
    }
   
    //funtion created to push node to the back of the list
    void pushbackQueue(list *p, Node *temp){
        
        //if list is empty
        if (p->front == nullptr){
           //use push front to create a new node and place at the front
            pushfrontQueue(p, temp);
            
            //no need to increment size, cause its done within the pushfrontQueue function
        }
        else{
            //the node should contain a value that is then being sent into a linked list
            //connect the node to the back of the list
            temp->prev = p->back;
            p->back->next = temp;
            p->back = temp;
            temp->next = nullptr;
            p->size++;
        }
    }
    
    
   
  public:

	/**
	 * Constructor
	 * Description: intializes an empty service queue.
	 * 
	 * RUNTIME REQUIREMENT: O(1)
	 *
         * TODO
	 */
         ServiceQueue() {
             Queue = new list;
             used_buzzers = new list;
             
             //start up the Queue
             Queue->front = nullptr;
             Queue->back = nullptr;
             Queue->size = 0;
             Queue->allbuzzersnumber = 0;
             
             //start up the used_buzzers
             used_buzzers->front = nullptr;
             used_buzzers->back = nullptr;
             
             //no need to start up vector, C library should do it.
         }
    
	/**
	 * Destructor
	 * Description:  deallocates all memory associated
	 *   with service queue 
	 *
	 * RUNTIME REQUIREMENT:  O(N_b) where N_b is the number of buzzer 
	 *	IDs that have been used during the lifetime of the
	 *	service queue; in general, at any particular instant
	 *	the actual queue length may be less than N_b.
	 *
	 *	[See discussion of "re-using buzzers" below]
	 *
         * TODO
	 */
	~ServiceQueue() {
        //deallocate the Queue
        Node *currA = Queue->front;
        Node *nxtA;
        
        while(currA != nullptr){
            nxtA = currA->next;
            delete currA;
            currA = nxtA;
        }
        
        //deallocate the used_buzzers
        Node *currB = used_buzzers->front;
        Node *nxtB;
        
        while (currB != nullptr){
            nxtB = currB->next;
            delete currB;
            currB = nxtB;
        }
    
        allbuzzers.erase(allbuzzers.begin(),allbuzzers.end());
    }

	/**
	 * Function: snapshot()
         * param:  buzzrs is an integer vector passed by ref
	 * Description:  populates buzzers vector with a "snapshot"
         *               of the queue as a sequence of buzzer IDs 
         *
	 *
	 * RUNTIME REQUIREMENT:  O(N)  (where N is the current queue
	 *		length).
	 */
	void snapshot(std::vector<int> &buzzers) {
             buzzers.clear();   // you don't know the history of the 
                                //   buzzers vector, so we had better
                                //   clear it first.
       //list *p = Queue;
        
        Node *p = Queue->front;
        int a;
        
        while(p != nullptr){
            a = p->buzzerIDnumber;
            buzzers.push_back(a);
            p = p->next;
        }
    }//DONE

	/**
	 * Function: length()
	 * Description:  returns the current number of
	 *    entries in the queue.
	 *
	 * RUNTIME REQUIREMENT:  O(1)
	 */
	int  length() {
        
        return Queue->size;
    }// DONE

    
    /**
	 * Function: give_buzzer()
         * Return:   buzzer-ID (integer) assigned to the new customer.
	 * Description:  This is the "enqueue" operation.  For us
	 *    a "buzzer" is represented by an integer (starting
	 *    from zero).  The function selects an available buzzer 
	 *    and places a new entry at the end of the service queue 
	 *    with the selected buzer-ID. 
	 *    This buzzer ID is returned.
	 *    The assigned buzzer-ID is a non-negative integer 
	 *    with the following properties:
	 *
	 *       (1) the buzzer (really it's ID) is not currently 
	 *         taken -- i.e., not in the queue.  (It
	 *         may have been in the queue at some previous
	 *         time -- i.e., buzzer can be re-used).
	 *	  This makes sense:  you can't give the same
	 *	  buzzer to two people!
	 *
	 *       (2) Reusable Buzzers:  A re-usable buzzer is 
	 *	  a buzzer that _was_ in the queue at some previous
	 *	  time, but currently is not.
	 *
         *         REQUIREMENT:  If there is one or more reusable
         *         buzzer, you MUST return one of them; furthermore,
         *         it must be the buzzer that became reusable most 
         *         MOST RECENTLY.
	 *
	 *       (3) if there are no previously-used / reusable buzzers, 
         *         the SMALLEST possible buzzer-ID is used (retrieved from 
         *         inventory).  
	 *	    Properties in this situation (where N is the current
	 *	      queue length):
	 *
	 *		- The largest buzzer-ID used so far is N-1
	 *
	 *		- All buzzer-IDs in {0..N-1} are in the queue
	 *			(in some order).
	 *
	 *		- The next buzzer-ID (from the basement) is N.
	 *
	 *    In other words, you can always get more buzzers (from
	 *    the basement or something), but you don't fetch an
	 *    additional buzzer unless you have to (i.e., no reusable buzzers).
	 *
	 * Comments/Reminders:
	 *
	 *	Rule (3) implies that when we start from an empty queue,
	 *	the first buzzer-ID will be 0 (zero).
	 *
	 *	Rule (2) does NOT require that the _minimum_ reuseable 
	 *	buzzer-ID be used.  If there are multiple reuseable buzzers, 
	 *	any one of them will do.
	 *	
	 *	Note the following property:  if there are no re-useable 
	 *	buzzers, the queue contains all buzzers in {0..N-1} where
	 *       N is the current queue length (of course, the buzzer IDs 
	 *	may be in any order.)
	 *
	 * RUNTIME REQUIREMENT:  O(1)  ON AVERAGE or "AMORTIZED"  
	 *          In other words, if there have been M calls to 
	 *		give_buzzer, the total time taken for those 
	 *		M calls is O(M).
	 *
	 *		An individual call may therefore not be O(1) so long
	 *		as when taken as a whole they average constant time.
	 *
	 */
	int  give_buzzer() {
        int theBuzzer;
        Node *temp;
        
        
        //check to see if there is any buzzers in the USED_BUZZERS list (USE THOSE FIRST)
        if (used_buzzers->front != nullptr){
            
            temp = popfrontQueue(used_buzzers);
            temp->check = true;
           
            theBuzzer = temp->buzzerIDnumber;
            
            /*Node *temp = new Node;
            temp->buzzerIDnumber = theBuzzer;
            temp->next = nullptr; //at the end of the list so it has to equal nullptr*/
            
            //if the list is empty, it will get thrown into the first condition for pushfront.
            //this function will increment the size++.
            pushbackQueue(Queue, temp);
            
            return theBuzzer;
        }
        else{
            
            theBuzzer = Queue->allbuzzersnumber;
            Node *temp = new Node;
            temp->buzzerIDnumber = theBuzzer;
            temp->next = nullptr; //at the end of the list so it has to equal nullptr
            
            //push to the back of the QUEUE list
            //this function will increment the size++.
            pushbackQueue(Queue, temp);
            
            //VECTORS******************************
            allbuzzers.push_back(theBuzzer);
            allbuzzersNode.push_back(temp);
            
            //if a buzzer is not found in the used_Buzzers list, create a new number.
            Queue->allbuzzersnumber++;
            
            return theBuzzer;
        }
    }

	/**
	 * function: seat()
	 * description:  if the queue is non-empty, it removes the first 
	 *	 entry from (front of queue) and returns the 
	 *	 buzzer ID.
	 *	 Note that the returned buzzer can now be re-used.
	 *
	 *	 If the queue is empty (nobody to seat), -1 is returned to
	 *	 indicate this fact.
         *
         * Returns:  buzzer ID of dequeued customer, or -1 if queue is empty.
	 *
	 * RUNTIME REQUIREMENT:  O(1)
	 */
	int seat() {
        int theBuzzer;
        int p;
        Node *temp;
        
        if (Queue->front == nullptr){
            //if there is nothing in the queue return -1.
            return -1;
        }
        else{
            temp = popfrontQueue(Queue);
            
            temp->check = false;
            
            p = temp->buzzerIDnumber;
           /* std::cout << "Popped " << p <<std::endl;
            Node *tmp = Queue->front;
            while (tmp != nullptr){
                std::cout << "Current list" << tmp->buzzerIDnumber << "\n";
                tmp = tmp->next;
            }*/
            //store in theBuzzer
            theBuzzer = p;
            /*Node *temp = new Node;
            temp->buzzerIDnumber = theBuzzer;*/
            
            //Node set to the front of the used_buzzers list.
            pushfrontQueue(used_buzzers,temp);
            
            return theBuzzer;
        }

    }


	/**
	 * function: kick_out()
	 *
	 * description:  Some times buzzer holders cause trouble and
	 *		a bouncer needs to take back their buzzer and
	 *		tell them to get lost.
	 *
	 *		Specifially:
	 *
	 *		If the buzzer given by the 2nd parameter is 
	 *		in the queue, the buzzer is removed (and the
	 *		buzzer can now be re-used) and 1 (one) is
	 *		returned (indicating success).
	 *
	 * Return:  If the buzzer isn't actually currently in the
	 *		queue, the queue is unchanged and false is returned
	 *		(indicating failure).  Otherwise, true is returned.
	 *
	 * RUNTIME REQUIREMENT:  O(1)
	 */
	bool kick_out(int buzzer) {
        Node *temp;
        signed int size = allbuzzers.size();
       if ((size <= buzzer) || (allbuzzersNode[buzzer]->check == false)){
           //allbuzzers.size() *allbuzzers.end()
           //std::cout << "if returned false" << std::endl;
           //std::cout << allbuzzers.size() << std::endl;
           //std::cout << allbuzzersNode[buzzer]->check << std::endl;
           return false;
       }
       else{
            
           temp = allbuzzersNode[buzzer];
           
           if (temp == Queue->front){
               Queue->front = temp->next;
           }
           else if (temp == Queue->back){
               Queue->back = temp->prev;
               Queue->back->next = nullptr;
           }
           else{
               temp->prev->next = temp->next;
               temp->next->prev = temp->prev;
               
           }
               temp->next = nullptr;
               temp->prev = nullptr;
               
               temp->check = false;
           
               Queue->size--;
               
               pushfrontQueue(used_buzzers, temp);
               
               return true;
           }
        return false;
       }

	/**
	 * function:  take_bribe()
	 * description:  some people just don't think the rules of everyday
	 *		life apply to them!  They always want to be at
	 *		the front of the line and don't mind bribing
	 *		a bouncer to get there.
	 *
	 *	        In terms of the function:
	 *
	 *		  - if the given buzzer is in the queue, it is 
	 *		    moved from its current position to the front
	 *		    of the queue.  1 is returned indicating success
	 *		    of the operation.
	 *		  - if the buzzer is not in the queue, the queue 
	 *		    is unchanged and 0 is returned (operation failed).
	 *
	 * Return:  If the buzzer isn't actually currently in the
	 *		queue, the queue is unchanged and false is returned
	 *		(indicating failure).  Otherwise, true is returned.
	 *
	 * RUNTIME REQUIREMENT:  O(1)
	 */
	bool take_bribe(int buzzer) {
        Node *temp;
        signed int size = allbuzzers.size();
        if ((size <= buzzer) || (allbuzzersNode[buzzer]->check == false)){
            //std::cout << "if returned false" << std::endl;
            //std::cout << allbuzzers.size() << std::endl;
            //std::cout << allbuzzersNode[buzzer]->check << std::endl;
            return false;
        }
        else{
            
            temp = allbuzzersNode[buzzer];
            
            if (temp == Queue->front){
                return true;
            }
            else if (temp == Queue->back){
                Queue->back = temp->prev;
                Queue->back->next = nullptr;
            }
            else{
                temp->prev->next = temp->next;
                temp->next->prev = temp->prev;
                
            }
            temp->next = nullptr;
            temp->prev = nullptr;
            
            pushfrontQueue(Queue, temp);
            
            Queue->size--;
            
            return true;
        }
        return false;
    }

};   // end ServiceQueue class

#endif


/*
    File: reqchannel.H

    Author: R. Bettati
            Department of Computer Science
            Texas A&M University
    Date  : 2012/07/11

*/

#ifndef _reqchannel_H_                   // include file only once
#define _reqchannel_H_

/*--------------------------------------------------------------------------*/
/* DEFINES */
/*--------------------------------------------------------------------------*/

/* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* INCLUDES */
/*--------------------------------------------------------------------------*/

#include <iostream>
#include <fstream>

#include <string>

using namespace std;

/*--------------------------------------------------------------------------*/
/* DATA STRUCTURES */ 
/*--------------------------------------------------------------------------*/

/* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* FORWARDS */ 
/*--------------------------------------------------------------------------*/

/* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* CLASS   R e q u e s t C h a n n e l */
/*--------------------------------------------------------------------------*/

class RequestChannel {

public:

  typedef enum {SERVER_SIDE, CLIENT_SIDE} Side;
 
  typedef enum {READ_MODE, WRITE_MODE} Mode;

private:

  string   my_name;

  Side     my_side;

  /*  The current implementation uses named pipes. */ 

  struct my_msgbuf {
    long mtype;
    char mtext[255];
  } buf;

  int msgqid;

public:

  /* -- CONSTRUCTOR/DESTRUCTOR */

  RequestChannel(const string _name, const Side _side);
  /* Creates a "local copy" of the channel specified by the given name. 
     If the channel does not exist, the associated IPC mechanisms are 
     created. If the channel exists already, this object is associated with the channel.
     The channel has two ends, which are conveniently called "SERVER_SIDE" and "CLIENT_SIDE".
     If two processes connect through a channel, one has to connect on the server side 
     and the other on the client side. Otherwise the results are unpredictable.

     NOTE: If the creation of the request channel fails (typically happens when too many
     request channels are being created) and error message is displayed, and the program
     unceremoniously exits.

     NOTE: It is easy to open too many request channels in parallel. In most systems,
     limits on the number of open files per process limit the number of established
     request channels to 125.
  */

  string send_request(string _request);
  /* Send a string over the channel and wait for a reply. */

  string cread();
  /* Blocking read of data from the channel. Returns a string of characters
     read from the channel. Returns NULL if read failed. */

  int cwrite(string _msg);
  /* Write the data to the channel. The function returns the number of characters written
     to the channel. */

  string name();
  /* Returns the name of the request channel. */

};


#endif



/* 
    File: requestchannel.C

    Author: R. Bettati
            Department of Computer Science
            Texas A&M University
    Date  : 2012/07/11

*/

/*--------------------------------------------------------------------------*/
/* DEFINES */
/*--------------------------------------------------------------------------*/

    /* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* INCLUDES */
/*--------------------------------------------------------------------------*/

#include <cassert>
#include <cstring>
#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

#include <errno.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#include "reqchannel.h"

using namespace std;

#define MSGPERM 0600    // msg queue permission

/*--------------------------------------------------------------------------*/
/* DATA STRUCTURES */ 
/*--------------------------------------------------------------------------*/

    /* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* CONSTANTS */
/*--------------------------------------------------------------------------*/

    /* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* FORWARDS */
/*--------------------------------------------------------------------------*/

    /* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* PRIVATE METHODS FOR CLASS   R e q u e s t C h a n n e l  */
/*--------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------*/
/* CONSTRUCTOR/DESTRUCTOR FOR CLASS   R e q u e s t C h a n n e l  */
/*--------------------------------------------------------------------------*/

RequestChannel::RequestChannel(const string _name, const Side _side) : my_name(_name), my_side(_side) {

  // Creates message queue
  msgqid = msgget(IPC_PRIVATE, MSGPERM|IPC_CREAT|IPC_EXCL);
  
  if (msgqid < 0)
  {
    perror("\n[msgget error]\n");
    printf("Failed to create message queue with msgqid = %d\n", msgqid);
    exit(1);
  }
  
  //printf("Message queue %d created\n",msgqid);

}

/*--------------------------------------------------------------------------*/
/* READ/WRITE FROM/TO REQUEST CHANNELS  */
/*--------------------------------------------------------------------------*/

const int MAX_MESSAGE = 255;

string RequestChannel::send_request(string _request) {
  cwrite(_request);
  string s = cread();
  return s;
}

string RequestChannel::cread() {

  if (msgrcv(msgqid, &buf, sizeof buf.mtext, 0, 0) < 0)
  {
      perror("\n[msgrcv error]\n");
      exit(1);
  }
  //printf("\nReceived: %s\n", buf.mtext);
  string s = buf.mtext;
  return s;
}

int RequestChannel::cwrite(string _msg) {

  if (_msg.length() >= MAX_MESSAGE)
  {
    cerr << "Message too long for Channel!\n";
    return -1;
  }

  buf.mtype = 1;
  strncpy(buf.mtext, _msg.c_str(), sizeof(buf.mtext));
  //printf("Message queue here write: %d \n",msgqid);

  int rc = msgsnd(msgqid, &buf, sizeof(buf.mtext), 0); // the last param can be: 0, IPC_NOWAIT, MSG_NOERROR, or IPC_NOWAIT|MSG_NOERROR.
  if (rc < 0)
  {
    perror( strerror(errno) );
    printf("\n[msgsnd failed]\t rc = %d\n", rc);
    exit(1);
  }
  //printf("\nSent msg: %s\n", buf.mtext);
}

/*--------------------------------------------------------------------------*/
/* ACCESS THE NAME OF REQUEST CHANNEL  */
/*--------------------------------------------------------------------------*/

string RequestChannel::name() {
  return my_name;
}




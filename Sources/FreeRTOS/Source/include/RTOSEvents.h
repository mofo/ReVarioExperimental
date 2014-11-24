/*
 * RTOSEvents.h
 *
 *  Created on: Sep 28, 2014
 *      Author: gian
 */

#ifndef RTOSEVENTS_H_
#define RTOSEVENTS_H_

/*
 * Exception handlers.
 */
void xPortPendSVHandler( void ) __attribute__ (( naked ));
void xPortSysTickHandler( void );
void vPortSVCHandler( void ) __attribute__ (( naked ));


#endif /* RTOSEVENTS_H_ */


#ifndef _APP_THREADBLOCKER_H_
#define _APP_THREADBLOCKER_H_

namespace APP {

/**
 * The threadblocker will block the current thread until inSignal
 * is received.
 */
void ThreadBlocker (int inSignal);

} // end namespace APP

#endif // _APP_THREADBLOCKER_H_

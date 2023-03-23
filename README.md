# Bowling_SSOOI_22-23
The execution of the programme will create a number of processes that will transmit signals to each other.

The execution of the programme will create a number of processes that will transmit signals to each other.
Once the tree is built, the parent process dies and the others remain alive, blocked and not consuming CPU. At this point, we will have regained control from our shell and send process A a SIGTERM signal. The signal will be transmitted to other processes according to the scheme indicated below, dumping other boluses along the way.

After four seconds, process A will check how many processes are still alive, print a picture of the situation on the screen, execute a ps -fu user command as a check and terminate, leaving no process alive.

If a process receives the SIGTERM signal and it is not an end process (it is not G, H, I or J) it can do one of four things:
1. not send the signal to any process.
2. Send the signal to the process below it on the right.
3. Send the signal to the process below on the left.
4. Send the signal to both processes.

Whether it does one or the other will depend on the system clock. Query the microseconds set by the system clock via a gettimeofday call. It will divide the value by four and get the remainder. Depending on the value of the remainder of the division (0, 1, 2 or 3), the corresponding action from the list above will be performed.

To let process A know if the E, H or I pins are still alive, you can use the non-blocking version of the waitpid system call. To find out what has happened to the processes in the B-D-G chain, proceed as follows:
1. If A did not send the signal to B, you know that all three are up.
2. If A sent the signal to B, it will do a blocking wait and B will tell it in its return code how many pins of the B-D-G string are still standing. To let B know how many pins are still standing, the same thing is done, but now A's role is taken over by B and B's role is taken over by D.
In the case of the string C-F-J, we proceed in the same way.

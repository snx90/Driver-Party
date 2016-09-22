# On Exercise 1 about System Threads in Bruce's RE Book
It seems that calling PsCreateSystemThread with a NULL ProcessHandle, the API creates the thread in the context of System process (as expected).

![foo](AsSystem.png)

(blah, more conclusions TODO)
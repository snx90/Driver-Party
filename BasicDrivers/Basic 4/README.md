# On Exercise 1 about System Threads (Chapter 3) in Bruce's RE Book
It seems that calling PsCreateSystemThread with a NULL ProcessHandle, the API creates the thread in the context of System process (as expected).

 ![foo](https://www.dropbox.com/s/dfvmct2nq5aiqjw/AsSystem.png)

(blah, more conclusions TODO)
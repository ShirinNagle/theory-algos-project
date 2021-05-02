 # theory-algos-project
Repo to calculate the SHA512 value of an input file


   ### Description
   Repository contains a program written in the C programming language to calculate the SHA512 value of an input file. The program takes the name (or path of the file)
   as a command line argument and outputs the SHA512 digest of the file. The program compiles using gcc, please see the next section called Compiliation Instructions for
   further explanation. 
   The following files are included in the repository, SHA512 file, Makefile, test files and this README.


   ### Compilation instructions
   To run the program, clone the repository by clicking on the green button titled code. Click on the copy button to the rigth of the address or copy the address.
   Open a folder on your local machine. On the command line navigate to the folder, for example if the folder is called SHA512Algorithm and is located on the Desktop        
of your local machine the path should look something like the below.<br>
   C:\Users\xxxxxx\Desktop\SHA512Algorithm<br>
   Once in the correct location enter git clone and the copied address. Press enter. This will clone the repository to this folder.
   change directory into the folder theory-of-algos-project, enter cd theory-of-algos-project on the command line. To run the program you can enter the following 
   command make 512SHA, then enter ./512SHA input.txt this will generate the sha512 digest of the file. To test if the digest is correct there is a make test option.
   Simply enter make test and the program will output an expected result using the built in sha512sum function, an actual result using the 512SHA program I have written
   If the expected and actual results match the program will display pass, if they don't match the program will display fail. 
   

   ### An explanation of what the SHA-512 algorithm is and why it is important.
   Secure Hash Algorithms are a family of cryptographic hash functions published by the National Institute of Standards and Technology(NSIT). 
   These include SHA-2 algorithms, which is a family of two similar hash functions, with different block sizes, kown as SHA-256 and SHA-512. 
   They differ in the word size; SHA-256 uses 32-byte words where SHA-512 uses 64-byte words. These functions were designed by the National Security Agency(NSA).[1]
   This project is using the SHA-512 algorithm as a basis for a program written in C.

   A cryptographic hash function is a mathematical algorithm that maps data (called the "message") to a bit array of a fixed size, called the "hash value", 
   "hash or "message digest". It is a one way function that is practically infeasible to invert. Currently, the only way to find a message that produces a given hash                
   is to attempt a brute-force search of possible inputs to see if they produce a match.

   Cryptographic hash functions should have the following main properties:

   Deterministic, meaning that the same message always results in the same hash.<br>
   Quick to compute the hash value for any given message.<br>
   Infeasible to generate a message that yields a given hash value (i.e. to reverse the process that generated the given hash value).<br>
   Infeasible to find two different messages with the same hash value.<br>
   A small change to a message should change the hash value so extensively that a new hash value appears uncorrelated with the old hash value.[2]

   When a message of any length less than 2^64 bits (forSHA-1, SHA-224 and SHA-256) or less than 2^128 bits (for SHA-384, SHA-512, SHA-512/224and SHA-512/256) is
   input to a hash algorithm, the result is an output called a message digest. The message digests range in length from 160 to 512 bits, depending on the algorithm.
   Secure hash algorithms are typically used with other cryptographic algorithms, such as digital signature algorithms and keyed-hash message authentication codes, or 
   in the generation of random numbers (bits).[3]
   
   SHA-512 is more secure than SHA-256 and is commonly faster than SHA-256 on 64-bit machines such as AMD64.[4]
   The SHA-512 algorithm is important because it is one of the strongest and most secure algorithms, currently the only known
   way to try and break it is by brute force, that is trying every possible combination one by one.
   A hash collision is a random match in hash values that occurs when a hashing algorithm produces the same hash value for two district pieces of data.   
   The chance of a 512-bit hash collision occuring is 1.4 x 10 ^77[3 + 1 https://en.wikipedia.org/wiki/Birthday_problem#Probability_table]
   It should be impossible to calculate or guess which values will collide. You can't check all possible values until you find a collision you would run out of time before you'd find a collision.
   Time could mean anything from 1 minute to hundreds or thousands of years or more.



    
   ### Why can't we reverse the SHA512 algorithm to retrieve the original message from a hash digest?
   A SHA512 algorithm produces a 512 bit (64 bytes) hash value, which is a hexadecimal number made up of 128 digits.
   Hash functions are used as one-way methods, they take the data(message)and calculate hash values(digests). 
   It can't be reversed.[https://www.freeformatter.com/sha512-generator.html]
   Using SHA-512 on text data with for examlple 250,000 characters, the output is just a 128 digit digest. 
   If you wanted to work backwards from the 128 digit digest, you'd need to know how many characters were originally 
   in the message, using just a 128 bit digest it would be impossible to work out what the starting number of characters is, 
   and if you happened to find the correct amount of characters how would you know you have the correct order of characters
   it seems like an impossible task, even if you had lots of powerful computers running code to try and reverse a 128 digit digest 
   there would not be enough time to figure this out, also it would be very expensive to run computers working non stop trying to break just 
   one 128 digit digest. 
   Why can't the one way method be reversed? Hash functions discard information using the modulo operator. The modulo operator is 
   not reversible, for example 32 mod 9 is 5, the result of this modulo operation is 5 but not knowing the starting point makes it impossible
   to figure out what the original number was, what it's modulo was, you just know that the result is 5, it could be an infinite combination of numbers
   that give a result of 5. In SHA512 a lot of data is discarded during the hash process.The input message can be as big or small as you want, 
   the output is always a 128 digit digest. Because of the discarded data, it would be impossible to work out the origial message from the
   resulting 128 digit digest.
   [https://privacycanada.net/hash-functions/why-are-hashes-irreversible/]
        
   ### Can you design an algorithm that, given enough time, will find input messages that give each of the possible 512-bit strings?
   My understanding of this question is it possible that someone could write an algorithm that could find is there any 512-bit string that is not an output of the algorithm.
   Given that the algorithm computes 512 bit strings from any sized input it would be impossible to test the possibile 512 bit outputs of an infinite number of inputs. In theory
   I would say it's a possibility to find a 512-bit string that is not an output of the algorithm, but how much time is enough time? Thousands of years or millions of computers working on the problem in parallell to 
   cut down the amount of time - but then the associated cost of equiment and energy and how long would it take. 
   The chances of a 512 bit hash collision occuring is 1.4 x 10^77, given that the estimated total number of fundamental particles in the observable universe is 1 x 10^80, I would say it's in practice impossible to find out if 
   there any 512-bit string that is not an output of the algorithm. When do you know every possible input has been calculated, where do you store this information, how do you store this information is it even possible to store 
   this amount of data? The only way to know if you have a 512 bit string that is not an output of the algorithm is if you know every possible input has been computed, how is decided that everything has been calculated, who or what 
   decides every possible input has been calculated. I don't think this is a possibility as the amount of time required is beyond imagination. In the future this type of computation may be possible with something like Quantum computing
   using something like Grovers algorithm[https://en.wikipedia.org/wiki/Grover%27s_algorithm]  or a new yet to be invented method of computing. Grovers algorithm is a quantum algorithm for searching an unsorted database with
   N entries in O(N1/2) time and using O(logN) storage space. It was invented by Lov Grover in 1996. Grover's algorithm, which takes O(N1/2) time, is the fastest possible quantum algorithm for searching an unsorted database. 
   It provides "only" a quadratic speedup, so if it took a computer one million searches to find something in a database, using Grovers algorithm it would take one thousand searches. Unlike other quantum algorithms, which can provide exponential speedup over their classical counterparts. However, even quadratic speedup is considerable when N, the number of entries is large.
   Grover's algorithm is probabilistic, in the sense that it gives the correct answer with high probability. The probability of failure can be decreased by repeating the algorithm.[https://www.quantiki.org/wiki/grovers-search-algorithm]
   Grover's algorithm could brute-force a 128-bit symmetric cryptographic key in roughly 2 ^ 64 iterations, or a 256-bit key in roughly 2 ^ 128 iterations. 
   As a result, it is sometimes suggested that symmetric key lengths be doubled to protect against future quantum attacks.[https://en.wikipedia.org/wiki/Grover%27s_algorithm]
   For now I think it is not possible to design an algorithm. If the
   time allowed was infinite and the input is infinite there's no way of drawing a line in the sand and saying everything has been computed..and who'd know we'd all be dead.
   [https://www.physicsoftheuniverse.com/numbers.html]

   ### How difficult is it to find a hash digest beginning with at least twelve zeros?
   It seems like it's  difficult to find a hash beginning with at least 12 zeros, but not impossible, from researching online finding a hash digest with at least twelve zeros is an  activity carried out in bit coin mining, finding a hash with a certain amount of leading 0's.
   People run programmes on their computers or in a pool of computers to compute a hash with at least 12 leading zeros, though I think the number now is 17 leading zeros.
   The reason for 12 leading zeros is that this is a low number and difficult to find. According to Ken Shirrif's blog from 2014[https://www.righto.com/2014/02/bitcoin-mining-hard-way-algorithms.html], finding a hash with 15 leading 0's is of the order  less than 1 in  10 ^ 19 chance, unfortunately my probability logic is not good, 
   but given the chance of 15 leading 0's is 1 in 10 ^ 19, the chance of finding 12 leading 0's will be a bit better but still a very large number. Or as Ken Sherriff said, it would be like trying to find on particular grain of sand from all the sand  currently on earth. 





  ## References
  [1] Secure Hash Algorithms; https://en.wikipedia.org/wiki/Secure_Hash_Algorithms
  [2] Cryptographic hash function explantions; https://en.wikipedia.org/wiki/Cryptographic_hash_function
  [3] Secure Hash Standard (SHS); Information Technology Laboratory National Institute of Standards and Technology Gaithersburg, MD 20899-8900
  [4] Security of SHA-512; https://en.wikipedia.org/wiki/Cryptographic_hash_function
  

  ### Research on Endianness
  https://en.wikipedia.org/wiki/Endianness
  https://developer.ibm.com/technologies/systems/articles/au-endianc/

  ### Links for tutorials used in the course of this peoject
  [a] How to write a MakeFile; https://www.cs.colby.edu/maxwell/courses/tutorials/maketutor/

  ### Links for other SHA-512 information
  [i] SHA-512 vs SHA-256 Structure; https://en.wikipedia.org/wiki/SHA-2





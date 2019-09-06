/*
  Simply placing "#define DEBUG_KPA" in the main file and then
  using that in other files such as HadrontherapyEventAction.cc
  (for example, inside the EndOfEventAction(..) method) in order
  to print debug lines didn't seem to work. Then I googled about
  the problem and went to here and there but this site
  (https://stackoverflow.com/questions/14275950/is-it-possible-to-use-define-from-other-cpp-file)
  seemed to have some helpful tips. For example, one of the comment
  was as follows:



  Defines inside a source file aren't seen by other translation units. Implementation files are compiled separately.

  You can either

  put them in a header and include it
  use your compiler's options
  do it the sane way - extern const int A = 1; in an implementation file and declare it when you want to use it extern const int A;.

  Of these, I'd say the first option is possibly the worst you can use.

 */
#ifndef DEBUG_KPA
//#define DEBUG_KPA

#endif

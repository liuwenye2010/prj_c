cppcheck --enable --enable=all .

or please add alias in the bash.bashrc 
alias check='cppcheck --enable=all . &> cppcheck.rpt; notepad++ ./cppcheck.rpt &'

if you want to check with single file 
you can also use invocate the cppcheckgui to analysis,that's better then command line 

cppcheckgui 

Let above work with Vscode, you will find it's useful and can help to improve you code quality. 
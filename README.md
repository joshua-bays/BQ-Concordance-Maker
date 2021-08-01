Required dependencies:
	git
	g++
	make
	sed

Most of these dependencies are already installed by default on most linux distributions. Some retooling may be needed for the programs to work with Windows.

How to setup and use the program:
	1. Run "bash setup.sh"
	2. Run "./main verses.txt [minimum frequency] [maximum frequency] > concordance.txt" (it will read whatever singular file that is passed through, and the output file may be named anything)

The program reads a text file that separates a verse reference (i.e. Mark 10:45) and its actual text with a '|' character (i.e. Mark 10:45|For even the Son of man came not to be ministered unto, but to minister, and to give his life a ransom for many.). Any file may be used.

The word of interest within the verse will be surrounded by asterisks (*). In a document editor (i.e. LibreOffice Writer), one can search for all of such statements and bold them (find and replace -> check the regular expressions box -> enter "\*[a-zA-z]*\*" in the find box -> select the bold button in the editing menu) and then delete the asterisks (find and replace -> check the regular expressions box -> enter "\*" in the find box -> select replace all without providing replacement text).

Note: The file "sbq-verses.txt" is a sample file that can be used for Bible Quizzing purposes.

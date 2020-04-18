# Inverted-Index
Main purpose of this project is to build an inverted index.



Depenendies
-----------
* wxWidgets 
* json.hpp 

Operation
-------
You can enter the path of the directory you want to index. The inverted index would be built using a tire.
Then we store this trie into a txt file and a json file, in the Database directory. Each database file would be named as input directory name.
If this directory was already indexed, then it would load the indexed json file directly, and no need for re-indexing.

The seach result, would return a full path to the files that has this word, and how many times this word occured in each file.

extended-ideas
-------
* update the database file is a new file is added to input directory
* add functionality to such for mutiple words using AND and OR operators

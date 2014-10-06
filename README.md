Emul-MIPS
=========


**pour compiler la version debuggable**
$ make debug 

**pour compiler la version release**
$ make release

**pour creer une archive**
$ make archive


**pour executer**

  ./EXENAME [fichier de scripts] 



**les sources se trouvent dans**
./src/

**les includes dans**
./include/ 

**Les fichiers de script de test se trouvent dans**
./Tests/tests

Pour les exécuter, se placer dans le fichier de l'exécutable et entrer :
./Tests/simpleUnitTest.sh -e projet -b Tests/test/set/*.emu

Ici, la commande set sera testée.
La plupart de nos tests chargent un fichier boucle.o à l'adresse par défaut présent dans ./Tests

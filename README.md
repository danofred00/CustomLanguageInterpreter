# Interpreter
Ce depot contient le code source de l'interpreter de notre langage de programmation. Pour le moment sans nom.

Pour les regles syntaxiques, nous avons opter pour une syntaxe proche du C, mais avec quelques petits ajustements.

```
Declaration de variables: <type> <identifier> [= <expr>];

Operations arithmetiques (sur les nombres): <number> <op> <number>

Operations logiques: and, or , not

Conditions : if, if ... else

Boucles : while, for, do ... while

Fonctions : <type> <identifier> ([<args>]) { <body> }
<args> = <type1> <id1>, [<type2> <id2>, ... , <typen> <idn>]
```

#### Les types disponible
+ **number** : pour tous les nombres (entier, floatant)
+ **string** : pour les chaines de caracteres
+ **bool** : pour les booleen

#### Operations associees aux type
+ **number** : toutes les operations possibles sur les entiers et floatant (addition, soustraction, multiplication, division, modulo, negation, ...)
+ **string** : concatenation, size
+ **bool** : les operations logiques
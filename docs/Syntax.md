# Syntaxe du langage

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

## Declaration de fonction
Pour declarer une fonction, on utilise le mot cle `fn`.
```
fn somme(number x, number y) {
    return x + y
}

fn sayHello(string name) {
    print("Hello", name, "!")
}
```
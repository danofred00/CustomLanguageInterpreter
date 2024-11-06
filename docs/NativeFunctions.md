# Fonctions de bases 
Ce document vous informe des fonctions natives disponible dans notre langage.

# Sommaire
### [I. Entree Sortie](#inputoutput)
1. [print - afficher les elements en console](#printargs)
2. [read - Lecture au clavier](#readprompt-format) 


## Input/Output
Ici vous avez toutes les fonctions lies aux entrees/sorties en langage Formal.

### print(args)

Utiliser pour l'affichage a l'ecran
```
    >>> print("hello", "world")
```

### read(prompt, format)

Utiliser pour la lecture. Le prompt et le format doivent obligatoirement entre des chaines de caracteres.

- `prompt`: represente le message qui accompagne la lecture
- `format`: qui represente le format au quel la donnee devra etre lu. `s` pour le type `string` et `n` pour le type `number`

```
>>> number old = read("How old have you ?", "n"); 
>>> string name = read("What's your name ? ", "s");
```
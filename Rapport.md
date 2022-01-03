---
title: Algorithmique Avancée Projet 9B
subtitle: Compression par gestion de graphe non orienté
author: Dibassi Brahima 19005521
date: 3 Janvier, 2022
---
 
\newpage
 
# Introduction
 
Dans le cadre de la validation du cours de programmation avancée,\newline
j'ai choisi pour projet final la Compression d'image par gestion de graphe non orienté a l'aide des listes de successeurs. \newline
Je vais dans ce rapport expliquer le code de mon projet, les choix fait et pour finir voir les avantages et défauts de notre algorithme
 
## Énoncé du projet
 
Nous considérons ici qu’une image est un graphe où chaque pixel est un noeud lié au plus à ses quatre voisins.\newline
Nous ajoutons une contrainte : un pixel p est lié à son voisin v, si et seulement si p et v sont effectivement voisins (au sens des quatre voisins) et ont la "même" couleur.
\newline
Étudier ce graphe consiste à le gérer, avec son million de noeuds.\newline
Mettez en place une méthode de recherche des parties connexes du graphe.\newline
Votre méthode doit vous avoir fourni un numéro pour chaque partie connexe.\newline
Il suffit maintenant de parcourir une partie connexe en cherchant les sommets qui n’ont pas quatre successeurs, ce sont les sommets de la frontière de la région.\newline
Ces sommets seront conservés dans une liste (ou un vecteur). \newline
Il est conseillé de se souvenir aussi des pixels liés ou des pixels non liés (au choix).\newline
La liste de ces listes compose les pixels de bords des régions. Ils organisent donc une partition de l’image.\newline
Avec cette liste de listes et la couleur qui y est associée, vous créez une nouvelle façon de stocker l’image.\newline
Il faut maintenant, mettre en place un système de sauvegarde et un système de restauration de l’image.\newline
 
\newpage
# Documentation
 
## Utilisation
 
### Demo Compression et Décompression Rapide
 
```bash
   bash ./Exec.sh [FileName] [0 >= Max_Delta <= 20]
```
\
FileName : Un nom de fichier sans son extension dans Img/ \newline \newline
Max_Delta: La somme des écarts maximaux entre les 3 composantes RGB de deux couleurs qui est acceptée. \newline
\newline
La sortie de la Décompression se trouve dans le fichier Demo_Output.ppm
 
### Compression
 
```py
  ./Comp [FilePath] [0 >= Max_Delta <= 20]
```
\
FilePath : Un Path d'un fichier.ppm \newline \newline
Max_Delta: La somme des écarts maximaux entre les 3 composantes RGB de deux couleurs qui est acceptée. \newline
\newline
La sortie de la Compression se trouve dans le fichier [FilePath].Compressed
 
### Décompression
 
```py
  ./Decomp [FilePath]
```
\
FilePath : Un Path d'un fichier.Compressed \newline \newline
La sortie de la Décompression se trouve dans le fichier ./Demo_Output.ppm
\newpage
 
## Fichiers
> Résumé des fichiers du projets et leurs contenu
 
List : Gestion des listes chaînée génériques
 
Ima : Chargement/Ecriture Fichier ppm
 
Shared_Comp_Decomp : Structures et Fonctions partagées entre la compression et la décompression
 
Comp : Compression d'une image
Decomp : Décompression d'une image
\newpage
 
# Déroulé du projet
 
## Choix
 
### Suppression ou non des petites zones
 
L'une des plus forte limitation de cette algorithme de compression est les petites zones.\newline
Dans ce projet j'ai fait le choix après réflexion de ne pas les supprimer,\newline
car je n'aime pas l'idée d'induire de la perte "non contrôlée".\newline
Lorsque je supprime les zones puis que la décompression essaye de les remplir j'ai des résultats très différents d'un test à l'autre.\newline
De plus je trouve que leurs supression cache les faiblesses de l'algorithme
 
### Limitation de la taille de l'image a 4096 * 4096
 
Tout simplement car une immense partie des images sont inférieures à cette taille.\newline
De plus cette limitation permet d'écrire les index des pixels non plus sur 4 octets mais sur 3 octets.\newline
On gagne instantanément en taux compression.\newline
 
## Problèmes Rencontrés
 
### Décompression
 
J'ai eu beaucoup de mal à trouver comment recréer mon image à partir des bords des zones.\newline
Après pas mal de réflexion j'ai eu l'idée d'utiliser des paires de pixels de bords cela me permet de dire avec certitude qu'elle est la couleur entre ses deux extrémités.\newline
 
J'ai pu grâce à cette idée réduire énormément le nombre de bords que j'écris dans mon fichier compressé.\newline
J'écris désormais que les bords à l'extrémité de chaques "lignes"
Dans les meilleurs cas, il est possible d'écrire une zone entière sur seulement 15 octets.
 
### Code Complexe
 
Lors de mon premier jet du projet j'ai écris du code très difficile à déboguer et donc a amélioré.\newline
Lorsque j'ai repris le cours et après avoir compartimenté chacun des modules du projet,
j'ai pu refaire le projet sur de bonnes base ce qui m'a permis d'aller plus loin.
 
# Améliorations
 
## Apportées
 
### Region Merging
L'idée est asser simple puisque maintenant l'image est ecrite en zone de couleur,\newline
il s'agit simplement ici de regrouper avant l'écriture les zones par couleurs.\newline
Cela permet dans une moindre mesure d'absorber les zones à faible nombre de pixels.
 
### Bords Pairs
L'idée ici est de ne pas écrire tous les bords mais des paires de bords Debut_Ligne Fin_Ligne.\newline
Nous savons qu'il ya que des Pixels(Bords inclus) de notre Zone entre ses deux bornes.
 
## Possibles
 
Ici ce sont juste des hypothèses dont il faudrait tester pour certaines l'efficacité
 
### Huffman Encoding
 
Simplement encoder le [file.Compressed] selon le codage d'huffman.\newline
On gagne pas mal en compression.\newline
Puisque ce n'est pas le sujet du projet je ne l'ai pas ajouté à l'archive.
 
### Analyze Automatique
 
Le but serait d'ajouter un choix de [Max_Delta] automatique, par couleurs.\newline
Cela demande de trouver une formulation mathématique de la différence "visuel" entre deux couleurs autre que la simple distance euclidienne
\newline(Je n'ai pas pu le faire par moi même).\newline
Un Exemple du type de formulation mathématique [**Ici**](https://www.compuphase.com/cmetric.htm).
 
# Conclusion
 
## Analyze de l'algorithme
 
Avec de nombreux tests sur des images très diverses est variée on se rends compte des faiblesse et des force de notre algorithme.
 
### Pros
 
- Taux de compression spectaculaire sur image adapté
- Extrêmement Rapide sur image adapté
- Possibilité avec ou sans perte
 
### Cons
 
Cet Algorithme a une grosse faiblesse c'est qu'il est extrêmement dépendants de l'image que ce soit en mémoire,vitesse ou taux de compression en tout cas dans l'implémentation que j'en ai fait.
 
\newpage
 
### Exécutions
 
Jeux d'exécutions avec des delta qui permettent de retrouver l'image sans différences visibles.\newline
Toutes les images se trouvent dans l'archive
 
 
| Image        | Max_Delta | Taux Compression | Temps Compression |
| :----------- | :-------: | :--------------: | :---------------: |
| Tintin       |    20     |      44.92%      |      14 sec       |
| Tintin       |    10     |      21.92%      |      30 sec       |
| Black        |     0     |      98.04%      |       8 sec       |
| Red          |     0     |       98%        |     0.66 sec      |
| Code_Cat     |     5     |       97%        |      24 sec       |
| Cool_Cat     |    10     |       96 %       |      2.5 sec      |
| Among_us     |    10     |       97%        |     0.23 sec      |
| Pixel_Art    |     5     |       41%        |       4 sec       |
| SpriteSheet1 |     0     |      68.64%      |     1.69 sec      |
| SpriteSheet2 |     0     |      81.04%      |     0.56 sec      |


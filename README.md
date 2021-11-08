# Projet 9B Dibassi Brahima
## Par gestion de graphe(List) non orienté

### Structures a Crée (Sujet a changement)
Graphe : 
*   Region List 
  
Region (Composante Connexte) :
*	Node Start
*	Bord : Node List
*	Color

Node (Pixel):
*    Index: (Index dans le tableau de Pixels)
*    Nexts : Node List
	

### Etape Compression :
Creation du Graphe 4-Connec\
(Connecter si même couleur et up,down,left,right)\
Output = 
* File: Une Liste de Region non connectés\
  (Dont on écrit dans un ficher que les bords de la Region + Sa couleur)
  
Ecrire File
### Etape Decompression :
Lire File\
Recreation des regions du Graphe a partir des Bords using fill method\
Recreation de l’image en recréent le tableau de pixel a partir du Graphe\
Output = 
* File : Image de base sans perte

Brahima Dibassi\
L3 Informatique
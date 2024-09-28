# Notre encodeur JPEG à nous

Bienvenue sur la page d'accueil de _votre_ projet JPEG, un grand espace de liberté, sous le regard bienveillant de vos enseignants préférés.
Le sujet sera disponible dès le lundi 2 mai à l'adresse suivante : [https://formationc.pages.ensimag.fr/projet/jpeg/jpeg/](https://formationc.pages.ensimag.fr/projet/jpeg/jpeg/).

Vous pouvez reprendre cette page d'accueil comme bon vous semble, mais elle devra au moins comporter les infos suivantes **avant la fin de la première semaine (vendredi 6 mai)** :

1. des informations sur le découpage des fonctionnalités du projet en modules, en spécifiant les données en entrée et sortie de chaque étape ;
2. (au moins) un dessin des structures de données de votre projet (format libre, ça peut être une photo d'un dessin manuscrit par exemple) ;
3. une répartition des tâches au sein de votre équipe de développement, comportant une estimation du temps consacré à chacune d'elle (là encore, format libre, du truc cracra fait à la main, au joli Gantt chart).

Rajouter **régulièrement** des informations sur l'avancement de votre projet est aussi **une très bonne idée** (prendre 10 min tous les trois chaque matin pour résumer ce qui a été fait la veille, établir un plan d'action pour la journée qui commence et reporter tout ça ici, par exemple).

# Liens utiles

- Bien former ses messages de commits : [https://www.conventionalcommits.org/en/v1.0.0/](https://www.conventionalcommits.org/en/v1.0.0/) ;
- Problème relationnel au sein du groupe ? Contactez [Pascal](https://fr.wikipedia.org/wiki/Pascal,_le_grand_fr%C3%A8re) !
- Besoin de prendre l'air ? Le [Mont Rachais](https://fr.wikipedia.org/wiki/Mont_Rachais) est accessible à pieds depuis la salle E301 !
- Un peu juste sur le projet à quelques heures de la deadline ? Le [Montrachet](https://www.vinatis.com/achat-vin-puligny-montrachet) peut faire passer l'envie à vos profs de vous mettre une tôle !

# Découpage des fonctionnalités:

- Module traitement:  Gestion des paramètres sur la ligne de commande

	- entrée : atguments en ligne de commande
	- sortie: chemin vers le fichier + nv_nom de l'image + les facteurs d'échantillonnage

- Module PPM_FILE:  Lecture des données depuis un fichier PPM et les rassembler dans une matrice de pixels

	- entrée :  nom image
	- sortie : matrice des pixels de l'image et les dimensions de l'image
	
- Module decoupage:  Découper une matrice de données en MCUs 

	- entrée : matrice des pixels issu de PPM_FILE
	- sortie : matrice des MCUs apres decoupage
	
- Module RGB:  Recalcule les composants RGB des pixels en composants YCbCr

	- entrée : MCUs avec les composants RGB
	- sortie : MCUs avec composants YCbCr
	
- Module DCT:  Le calcule des valeurs dans le domaine fréquentielle

	- entrée : matrice YCbCr
	- sortie : matrice fréq.

- Module zigzag:  Transformation d'une matrice en un vecteur parcouru en zigzag

	- entrée : matrice
	- sortie : vecteur parcouru en zigzag


- Module Quantification:  Fait le calcule de la quantification 

	- entrée : matrice
	- sortie : quantification Y ou Y et C
	
- Module Codage AC_RLE et DC:  Traite la partie codage des données

	- entrée : MCUs
	- sortie : Vecteur flux codé en RLE et DC
	
	
- Module downsampling : ----
	- entrée : MCU
	- sortie : MCU sous échantillonné 

- Module compression:  C'est le module qui va englober tous les autres et réaliser ce qui est demandé: Transformation d'une image ppm en jpg
    
	- entrée : image ppm
	- sortie : image jpg
	
# structures de données:

Pour cette section, on a push un fichier pdf nommé "Structure_de_donnés.pdf" où on a dessiné les structures de données utilisées.

# Répartition des tâches:

- Isamil : Module decoupage - Module zigzag - Module Quantification- Module Codage AC_RLE et DC .
- David  : Module PPM_FILE -  Module DCT - Module Codage AC_RLE et DC .
- Mourad : Module traitement - Module RGB - Module compression .

# Projet-test

<img src="Images/image-capteur-graphite.png" alt="Image capteur" width="200">

## Table des matières
* [Contacts](#contacts) 
* [Contexte](#contexte) 
* [Livrable](#livrable) 
* [Matériel requis](#matériel-requis) <br>
* [1. Simulation électronique sous LTSpice](#1--simulation-électronique-sous-ltspice) <br>
* [2. Design du PCB sous Kicad](#2--design-du-pcb-sous-kicad) <br>
* [3. Code sur Arduino](#3--code-sur-arduino) <br>
* [4. Application Android](#4--application-android) <br>
* [5. Réalisation du shield](#5--réalisation-du-shield) <br>
* [6. Banc de test](#6--banc-de-test) <br>
* [7. Résultats](#7--résultats) <br>
* [8. Datasheet du capteur](#8--datasheet-du-capteur) 
* [Conclusion](#conclusion)
------------
## Contacts
Esther BOIRON : eboiron@insa-toulouse.fr  
Margot HERNANDEZ : mhernan2@insa-toulouse.fr

------------
## Contexte
Dans le cadre de l’UF de quatrième année du département Génie Physique intitulée *Du capteur au banc de test*, nous avons développé et évalué la performance d’un capteur low-tech en graphite. Il est constitué d’un simple morceau de papier recouvert d’une trace de crayon à papier, formant une fine couche de graphite. Lorsque le papier se déforme, le nombre de particules de graphite reliées varie, entraînant une modification de la résistance du capteur. Ce phénomène permet ainsi de mesurer la déformation, à la manière d’une jauge de contrainte traditionnelle.

Ce travail s’appuie sur les recherches de scientifiques américains (Cheng-Wei Lin*, Zhibo Zhao*, Jaemyung Kim & Jiaxing Huang), qui ont publié en 2014 l’article [Pencil Drawn Strain Gauges and Chemiresistors on Paper](https://www.researchgate.net/publication/259846610_Pencil_Drawn_Strain_Gauges_and_Chemiresistors_on_Paper).

Vous trouverez ici l’ensemble des réalisations effectuées dans le cadre de ce projet : simulations électroniques, conception du PCB, programmation Arduino, développement d’une application Android et élaboration d’une datasheet.

------------
## Livrable
Plusieurs livrables sont attendus :

✓ Un shield PCB connecté à une carte Arduino UNO, intégrant : un capteur en graphite, un montage amplificateur transimpédance, un module Bluetooth, un écran OLED, un encodeur rotatif, un potentiomètre numérique ainsi qu'un capteur de flexion (flex sensor).<br>

✓ Un code Arduino assurant le contrôle de l’ensemble des composants et la réalisation des mesures.<br>

✓ Une application Android servant d’interface entre le PCB et le code Arduino.<br>

✓ Un code Arduino dédié aux essais du capteur sur le banc de test.<br>

✓ Une datasheet détaillant les caractéristiques du capteur en graphite.<br>

------------
## Matériel requis
Afin de réaliser notre dispositif, voici la liste des composants nécessaires :

Pour le montage amplificateur transimpédance :

* Des résistances : 1 de 1 kΩ, 1 de 10 kΩ, 2 de 100 kΩ
* Un potentiomètre numérique : ici, le MCP41050
* Des condensateurs : trois de 100 nF et un de 1 μF
* Un amplificateur opérationnel : ici, le LTC1050

Pour le reste du dispositif :
* Un module Bluetooth : ici, le HC05
* Un écran OLED : ici, un écran de dimensions 128x64
* Un encodeur rotatif
* Un capteur de flexion (flex sensor)
* Une résistance de 47 kΩ
* Une carte Arduino UNO
------------
## Simulation électronique sous LTSpice
Notre objectif est d’extraire l’information utile de notre capteur en graphite, dont la résistance est de l’ordre du gigaohm (GΩ). Sous une tension de 5V, correspondant à celle délivrée par l’Arduino, le capteur délivre un courant extrêmement faible, de l’ordre de quelques nanoampères (nA). Or selon les caractéristiques techniques de l’Arduino (cf datasheet), un courant aussi faible ne peut pas être mesuré directement. Il est donc nécessaire de l’amplifier en utilisant un amplificateur de transimpédance, qui convertit ce courant en une tension exploitable par le convertisseur analogique-numérique (CAN) de l’Arduino. Pour cela, nous avons choisi l’amplificateur opérationnel  LTC1050.

Associé à cet amplificateur, nous réalisons un montage structuré en trois zones principales qui correspondent à des filtres pour améliorer la qualité du signal mesuré :
* Filtrage en entrée de l'amplificateur (R1 et C1) : un filtre passe-bas passif de fréquence de coupure de 16 Hz qui permet de filtrer le bruit en courant sur le signal d'entrée
* Filtrage couplé à l'amplificateur (R3 et C4) : un filtre passe-bas actif de fréquence de coupure de 1.6 Hz qui permet de filtrer la composante 50 Hz issue du réseau électrique 
* Filtrage à la sortie de l'amplificateur (R6 et C2) : filtre passe bas passif de fréquence de coupure de 1.6 kHz qui permet de réduit le bruit créé lors du traitement du signal (bruits des alimentations, de l’horloge…)

<img src="Images/circuit-LTSpice.png" alt="Image circuit LTSpice" width="900">

Afin de pouvoir ajuster dynamiquement le gain de l’amplificateur en fonction des besoins de mesure, la résistance R2 sera remplacée ultérieurement par un potentiomètre digital.

Nous avons validé notre montage sur LTSpice en deux étapes :

Tout d'abord, nous avons réalisé une première simulation afin de s’assurer que l’amplification fonctionne correctement. L’image ci-dessous montre que le signal est bien amplifié à 1 V : le signal a donc une valeur suffisante pour être interprété par l’Arduino.

<img src="Images/simulation1-LTSpice.PNG" alt="Image circuit LTSpice" width="900">

Ensuite, nous avons réalisé une seconde simulation afin d'observer la réponse à un courant alternatif afin de vérifier l’efficacité du filtrage. Les variations de la phase observées sur le graphe ci-dessous illustrent le passage du signal à travers les trois filtres successifs.

<img src="Images/simulation2-LTSpice.png" alt="Image circuit LTSpice" width="900">

------------
## Design du PCB sous Kicad
Après avoir réalisés les simulations sur LTSpice, nous avons conçu notre PCB à l’aide du logiciel KiCad. Pour cela, nous sommes parties du schéma du montage décrit précédemment, auquel nous avons apporté quelques modifications.
Parmi ces modifications, nous avons notamment remplacé la résistance R2 par un potentiomètre numérique permettant un réglage précis du gain de l’amplificateur. Nous avons également ajouté plusieurs composants afin d’améliorer les fonctionnalités de notre système et de comparer les résultats obtenus :
* Le module Bluetooth HC-05 : permet d'assurer la communication entre le PCB et un téléphone via une application Android.
* Un écran OLED : utilisé pour afficher différents menus en temps réel.
* Un encodeur rotatif : facilite la navigation entre les différentes options affichées sur l’écran OLED.
* Un flex sensor : utilisé pour comparer les mesures du capteur en graphite avec celles d’un capteur commercial lors des tests finaux

__Création des bibliothèques et symboles__
La première étape de la conception a consisté à créer notre propre bibliothèque de symboles pour les composants spécifiques du montage. Nous avons dessiné les symboles correspondants aux différents composants utilisés. L’image ci-dessous illustre, par exemple, le symbole de l’écran OLED.

<img src="Images/symbole-OLED-Kicad.PNG" alt="Image circuit LTSpice" width="400">

La deuxième étape a été de créer les empreintes de nos composants. Pour cela, nous devions tenir compte des dimensions de nos composants et de leurs caractéristiques comme le diamètre des trous. Ci-dessous, l’empreinte de l’écran OLED.

__Création des empreintes des composants__

La deuxième étape a consisté à créer les empreintes des composants. Cette phase est essentielle pour garantir une bonne intégration des composants sur le circuit imprimé. Pour cela, nous avons pris en compte la dimension physique des composants ainsi que le diamètre des trous. L’image ci-dessous illustre l’empreinte de l’écran OLED.

<img src="Images/empreinte-OLED-Kicad.PNG" alt="Image circuit LTSpice" width="400">

Afin d’obtenir une visualisation 3D réaliste de notre PCB final, nous avons associé à chaque empreinte une modélisation 3D de son composant correspondant. Cette étape permet de vérifier l’agencement des composants, d’anticiper d’éventuels problèmes d’encombrement et d’optimiser le design du circuit imprimé.
Les modèles 3D utilisés ont été récupérés sur la plateforme GrabCAD, qui propose une large bibliothèque de fichiers compatibles avec KiCad.
L’image ci-dessous illustre la modélisation 3D de l’écran OLED.

<img src="Images/modèle3D-avant-OLED-Kicad.PNG" alt="Image circuit LTSpice" width="400">
<img src="Images/modèle3D-arrière-OLED-Kicad.PNG" alt="Image circuit LTSpice" width="400">

Ensuite, nous avons procédé à la création de la schématique électrique de l'ensemble du montage. Lors de cette étape, nous avons choisi un modèle basé sur l'Arduino UNO pour avoir les bonnes dimensions de la carte et les pins de connexion adaptées. Lors de cette étape, nous avons dû concevoir certains composants électroniques nous-mêmes dans la partie Schématique (écran OLED, module Bluetooth, l'amplificateur LTC1050C, l'encodeur rotatoire et le potentiomètre digital).

__Création de la schématique électrique__

Après avoir défini les empreintes et les modèles 3D des composants, nous avons procédé à la création de la schématique électrique du montage. Lors de cette étape, nous avons opté pour un modèle basé sur l’Arduino UNO afin de garantir la correspondance correcte des pins de connexion avec l’Arduino.

<img src="Images/schématique-Kicad.PNG" alt="Image circuit LTSpice" width="400">

Enfin, nous sommes passés à la disposition des composants sur le PCB. L’objectif est de disposer les en considérant les connexions entre les composants et éviter d'avoir recours aux vias. Les image ci-dessous correspondent au résultat final de notre PCB et sa vue en 3D correspondante :

__Disposition des composants sur le PCB__

La dernière étape de notre conception a été l’agencement des composants sur le PCB. L’objectif principal est d’optimiser leur disposition en tenant compte des connexions électriques tout en minimisant l’utilisation de vias. Les images ci-dessous présentent le résultat final de notre PCB, ainsi que sa vue en 3D, permettant de visualiser l’implantation des composants.

<img src="Images/PCB-Kicad.PNG" alt="Image circuit LTSpice" width="400">
<img src="Images/PCB-visu3D-Kicad.PNG" alt="Image circuit LTSpice" width="400">

------------
## Code sur Arduino
------------
## Application Android
------------
## Réalisation du shield
Après la conception et la validation du PCB, nous avons procédé à la fabrication physique du shield en suivant plusieurs étapes :
* Vérification du PCB sur KiCad : utilisation de l’outil Contrôle des règles de conception (DRC) pour s’assurer qu’il n’y a pas d’erreur dans les connexions et les espacements des pistes.
* Impression du masque de gravure sur un papier transparent 
* Insolation UV d’une plaquette d’époxy recouverte d’une couche de cuivre et de résine photosensible : la plaquette est exposée aux UV à travers le masque
* Révélation du motif : la plaquette est immergée dans un révélateur (soude très diluée) afin d'éliminer la résine exposée aux UV et laisser les pistes de cuivre protégées 
* Gravure des pistes : la plaquette est immergée dans un bain de perchlorure de fer pour graver les pistes
* Nettoyage de la plaquette : utilisation d'acétone pour retirer les résidus de résine 
* Perçage des trous et soudure des composants sur le PCB

<p align="center">
  <img src="Images/photo-insolationUV.jpg" alt="Image photo insolation UV" width="150">
  <img src="Images/photo-immersion-PCB-solution-de-FeCl3.jpg" alt="Image imersion PCB" width="150">
  <img src="Images/photo-PCB-non-troué.jpg" alt="Image PCB non troué" width="150">
  <img src="Images/photo-perçage-trous.jpg" alt="Image perçage trous" width="150">
  <img src="Images/photo-PCB-KiCad-placement.jpg" alt="Image PCB cablâge" width="150">
</p>


------------
## Banc de test
------------
## Résultats
------------
## Datasheet du capteur
Vous pouvez trouver la datasheet du capteur ici. (mettre lien du PDF)

------------
## Conclusion


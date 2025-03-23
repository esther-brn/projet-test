# Projet-test

<img src="Images/image-capteur-graphite.PNG" alt="Image capteur" width="200">

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

Ce travail s’appuie sur les recherches de scientifiques américains (Cheng-Wei Lin*, Zhibo Zhao*, Jaemyung Kim & Jiaxing Huang), qui ont publié en 2014 l’article [Pencil Drawn Strain Gauges and Chemiresistors on Paper](https://moodle.insa-toulouse.fr/mod/resource/view.php?id=60418).

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

<img src="Images/circuit-LTSpice.PNG" alt="Image circuit LTSpice" width="400">

------------
## Design du PCB sous Kicad
------------
## Code sur Arduino
------------
## Application Android
------------
## Réalisation du shield
------------
## Banc de test
------------
## Résultats
------------
## Datasheet du capteur
Vous pouvez trouver la datasheet du capteur ici. (mettre lien du PDF)

------------
## Conclusion


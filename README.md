<a name="readme-top"></a>

<!-- GENERAL GAME INFO -->
<br />
<div align="center">

  <h1 align="center">Scott Pilgrim VS. The World</h1>

  <p align="center">
    A retro style side-scrolling beat 'em up.
    <br />
    <strong>Original game : </strong>
    <a href="https://en.wikipedia.org/wiki/Scott_Pilgrim_vs._the_World:_The_Game"><strong>General info »</strong></a>
    ·
    <a href="https://youtu.be/L7qtF4An6Yg"><strong>Youtube video »<strong></a>
    <br />
    <br />
  </p>
</div>



<!-- TABLE OF CONTENTS -->
<details>
  <summary>Table of Contents</summary>
  <ol>
    <li>
      <a href="#about-the-project">About The Project</a>
    </li>
    <li>
      <a href="#my-version">My version</a>
    </li>
    <li>
      <a href="#getting-started">Getting Started</a>
    </li>
    <li><a href="#how-to-play">How To Play</a></li>
    <li><a href="#class-structure">Class structure</a></li>
    <li><a href="#checklist">Checklist</a></li>
    <li><a href="#contact">Contact</a></li>
    <li><a href="#acknowledgments">Acknowledgments</a></li>
  </ol>
</details>



<!-- ABOUT THE PROJECT -->
## About The Project

![](https://github.com/HowestDAE/DAE13-KennoRat/blob/main/ScottVSWorld.jpg)

Here's why:
* I wanted to make a side-scrolling beat'em up.
* The game has the potential to get full marks.
* The soundtrack is great and the sprites are easy to get online.
* I liked the movie and the animated serie.

<p align="right">(<a href="#readme-top">back to top</a>)</p>


## My version

This section gives a clear and detailed overview of which parts of the original game I planned to make.

### The minimum I will most certainly develop:
* Player Moves: Light attack, Heavy attack, Block. 
* Different Enemy types.
* Moving Camera.
* Pick ups: Different Coins, Weapons.
* Menu-, Pause Screen.

### What I will probably make as well:
* A pit you can fall in.
* Settings.

### What I plan to create if I have enough time left:
* Add more enemy types.
* Stage events.

<p align="right">(<a href="#readme-top">back to top</a>)</p>


<!-- GETTING STARTED -->
## Getting Started
Detailed instructions on how to run your game project are in this section.

### Prerequisites

* Visual Studio 2022

### How to run the project

* Set "GameProject" as Startup Project.
* For "GameProject" Dependencies set "Engine" as Project Dependency.

<p align="right">(<a href="#readme-top">back to top</a>)</p>



<!-- HOW TO PLAY -->
## How to play

You move and fight till you reach the right side of the map.
![](https://github.com/HowestDAE/DAE13-KennoRat/blob/main/ProjectGameplay.jpg)
![](https://github.com/HowestDAE/DAE13-KennoRat/blob/main/HelpAndOptions.jpg)

### Controls
* Movement: WASD (ZQSD for AZERTY Users)
* Jump: Space
* Sprint: double tap Left or Right Movement
* Light Attack: J 
* Heavy Attack: K
* Block: L
* JumpKick: J during jump

### Menu Controls
* UP: W (Z)
* DOWN: S
* ENTER: RETURN button or SPACEBAR


<p align="right">(<a href="#readme-top">back to top</a>)</p>



<!-- CLASS STRUCTURE -->
## Class structure 

### Object Aggregation
The "Soundeffect" member is part of multiple other classes (Level Manager, Enemies, Player), but initialised in Game.h.
The different classes uses the member to play sound effects that are all initaliased in the "SoundEffect" class.

### Object Association
The "Objects" class interacts with "Enemies" and the "Player". This is to know which object the Player or enemy is holding since there are multiple objects in the world.
The objects also uses the interaction to know which position to follow if it is picked up.

### Inheritance 
Inheritance is used for the different Enemy Types. Each derived enemy has different health, percentage to block and texture. 
They all use the same variable members and functions as the base class (EnemyMike). Only one derived enemy (EnemyLee) is using polymorphism to have diffent behaviour.

### Object composition 
Not Used.



<p align="right">(<a href="#readme-top">back to top</a>)</p>


<!-- CHECKLIST -->
## Checklist

- [x] Accept / set up github project
- [x] week 01 topics applied
    - [x] const keyword applied proactively (variables, functions,..)
    - [x] static keyword applied proactively (class variables, static functions,..)
    - [ ] object composition (optional)
- [x] week 02 topics applied
- [x] week 03 topics applied
- [x] week 04 topics applied
- [ ] week 05 topics applied
- [x] week 06 topics applied
- [x] week 07 topics applied
- [x] week 08 topics applied
- [ ] week 09 topics applied (optional)
- [ ] week 10 topics applied (optional)

<p align="right">(<a href="#readme-top">back to top</a>)</p>

<!-- CONTACT -->
## Contact
Andre Ken De Decker 1GD13
- Personal Email: dedeckerken@gmail.com
- School Email: ken.de.decker@student.howest.be

Project Link: [https://github.com/HowestDAE/DAE13-KenDeDecker](https://github.com/HowestDAE/DAE13-KenDeDecker)

<p align="right">(<a href="#readme-top">back to top</a>)</p>


<!-- ACKNOWLEDGMENTS -->
## Acknowledgments

Use this space to list resources you find helpful and would like to give credit to. 

* [cpp reference on std::vector](https://en.cppreference.com/w/cpp/container/vector)
* [Spritesheets](https://www.spriters-resource.com/xbox_360/scottpilgrimvstheworldthegame/)

<p align="right">(<a href="#readme-top">back to top</a>)</p>


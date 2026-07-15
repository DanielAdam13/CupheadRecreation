<a name="readme-top"></a>

<!-- GENERAL GAME INFO -->
<br />
<div align="center">

  <h2 align="center">CUPHEAD</h2>
  
  <p align="center">
   Cuphead is a classic run and gun action game heavily focused on boss battles. Inspired by cartoons of the 1930s, the visuals and audio are painstakingly created with the same techniques of the era.
    <br />
    <strong>Original game : </strong>
    <a href="https://cuphead.fandom.com/wiki/Cuphead_(video_game)"><strong>General info »</strong></a>
    ·
    <a href="https://www.youtube.com/watch?v=tIi-QRWjqIg"><strong>Youtube video »<strong></a>
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

This project was made in the first year of my University Game Development studies. This means that it is not perfected but it is still worth showing because of all the techniques I used 
and the challenges I encountered.

![Game image](/images_&_spritesheets/Run'n'Gun_Screenshot.png?raw=true "Run'n'Gun level")


Here's why:
* Cuphead is a game with great art style and capturing atmosphere.
* I figured Cuphead would be a great game to recreate since it uses camera, has a cool character controller and has a lot of potential with enemy and projectile classes, all having different interactions.

<p align="right">(<a href="#readme-top">back to top</a>)</p>


## My version

### The minimum I will most certainly develop:
* I will make a "Run'N'Gun" level that has correct collision and showcases its resepctful enemies with all their in-game behaviour and sounds.
* I will implement animations, collisions, spawning projectiles, death handling and apply polymorphism within a manager class for every enemy.
* I will animate, apply a correct character controller with collisions and keyboard states, implement parrying and logic to create projectiles to Cuphead.
* I will develop a logic for projectiles that interacts with the rest of the game, namely - the camera, the player and the enemies, while applying polymorphism.
* UI for the player - cards, health, announcments and death screen.

### What I will probably make as well:
* Sound effects for every attack and action of every object (depends on what I can find online).

### What I plan to create if I have enough time left:
* I would start working on the Flower Boss level.
* Running flower enemy in Run'N'Gun.
* I would make a level manager that starts and ends a specific level whenever it has to.

<p align="right">(<a href="#readme-top">back to top</a>)</p>


<!-- GETTING STARTED -->
## Getting Started
Download the folder and unzip it. Inside of it there is the project folder DAE13_Adamov_Daniel. 

### Prerequisites
This is an example of how to list things you need to use the software and how to install them.
* You need Visual Studio 2022. Download through Visual Studio Installer.

### How to run the project

The first option is opening the Prog2Engine.sln file and running it in visual studio.
The second option is to go into x64/Debug and find the ForestFolliesCuphead.exe.

<p align="right">(<a href="#readme-top">back to top</a>)</p>



<!-- HOW TO PLAY -->
## How to play

Goal : Reach the end of the level without dying.
Optional : Collect coins, kill enemies and parry everything that has pink color.

### Controls
* ARROW KEYS for movement and aiming
* X for shooting normal projectiles and V for using the special attack
* Z to jump
* SHIFT to dash
* C to lock while shooting

For UI:
* ESC to pause / unpause
* Z to accept
* R to restart level
* M to mute level music (not sound effects)


<p align="right">(<a href="#readme-top">back to top</a>)</p>



<!-- CLASS STRUCTURE -->
## Class structure 

### Object composition 
All classes that use textures and sound objects have pointers to the appropriate objects. However, they are passed through their constructors and the lifetime of these members are not managed by the classes where 
they are passed to. All textures and sound effects are initalized and deleted in Game (except for Cuphead since there is always going to be only one instance).

We also have the Manager classes. They have std::vectors that manager but don't initialize their elements. They are pushed from other classes where appropriate. For example,
Cuphead pushes new PeaShooter projectiles to BulletManager but BulletManager decides when to Draw and Update them or if they are in the camera screen. Game pushes enemies into EnemyManager but EnemyManager handles 
their interaction with taking damage from the player, as well as Updating and Animating and deleting them. Same applies for VisualEffectManager and CoinManager.

There is also the Animator class (the most reused class in the project). If a current frame for an animation needs to be calculated in a certain class, an animator object is added 
on the stack and then used in Animate(or Update) and Draw.

### Inheritance 
Inheritance is used for the projectile and enemy classes. For the enemies we have an abstract class Enemy from which every enemy type is going to derive. Further yet, Mushroom and Spike are classes that derive from Enemy but they are not final. Tulip inherits Mushroom and BigChomper derives from Spike.

In the case of prohectiles, their base abstract class is called Projectile. The classes that derive from it are PeaShooter, MushroomCloud and TulipSeed.

### Other class relationships
Composition:

We use it for the Animator class. The animator class has automatic lifetime duration in every class where used.
We also have the Cuphead class with its textures and sound effects. Cuphead handles when they are initialized, drawn or played and deleted.

Aggregation:

The textures and sounds for the enemies are all initalized and deleted in Game as well as some of the manager classes that would have the same parameters between levels. 
They are passed as pointers to the constructors of the appropriate classes.
They have a USES A relationship.
The classes that apply this are: 
* Acorn, BigChomper, Mushroom, Spike, Tulip
* Effect, Coin, Explosion
* UIManager, CoinManager, EnemyManager

<p align="right">(<a href="#readme-top">back to top</a>)</p>


<!-- CHECKLIST -->
## Checklist

- [x] Accept / set up github project
- [x] week 01 topics applied
    - [x] const keyword applied proactively (variables, functions,..)
    - [x] static keyword applied proactively (class variables, static functions,..)
    - [x] object composition (optional)
- [x] week 02 topics applied
- [x] week 03 topics applied
- [x] week 04 topics applied
- [x] week 05 topics applied
- [x] week 06 topics applied
- [x] week 07 topics applied
- [x] week 08 topics applied
- [ ] week 09 topics applied (optional)
- [x] week 10 topics applied (optional)

<p align="right">(<a href="#readme-top">back to top</a>)</p>

<!-- CONTACT -->
## Contact

Adamov Daniel - daniel.adamov@student.howest.be

Project Link: [https://github.com/HowestDAE/dae13-DanielAdam13.git](https://github.com/HowestDAE/dae13-DanielAdam13.git)

<p align="right">(<a href="#readme-top">back to top</a>)</p>


<!-- ACKNOWLEDGMENTS -->
## Acknowledgments

Use this space to list resources you find helpful and would like to give credit to. 

* [Sprites source](https://www.spriters-resource.com/pc_computer/cupheaddontdealwiththedevil/)
* AssetStudio

<p align="right">(<a href="#readme-top">back to top</a>)</p>


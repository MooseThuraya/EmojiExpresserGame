# Emoji Expresser (Apr. 2022 - Jun. 2022)

Emoji Expresser is a game that allows players to interact with an emoji to express different emotions. Developed using OpenGL, Emoji Expresser is an expressive game that allows players to perform actions and see the emotions reflected on an Emoji. Players are indulged in an expressive experience involving dynamic visuals and funny sound effects.

## Video Demo (Include Audio!!!)

https://user-images.githubusercontent.com/33456577/216513007-e4abf4cc-01f0-4c11-8521-c0b4c0b0e049.mp4

## Screenshots

<h3 align="center"> Main Menu </h3>
<div align="center">
<img src="images/main.png" width=500 height=500 />
</div>

<h3 align="center"> Gameplay </h3>
<div align="center">
<img src="images/gameplay.png" width=500 height=500 />
</div>

## Features/Actions
The game allows players to interact with an emoji to express different emotions based on the actions:
- Slap
- Pat
- Poke
- Love

## Introduction
The game starts off with a start screen, which includes some sample emojis in the background and an announcer introducing the game (My-Mustafa's-voice!). Players can press the play button to be taken to another screen, including the expressive emoji and four options. The game guides players in playing the game by offering colored buttons for valid actions and greyed-out options for invalid actions. Upon pressing actions, players can see emotional expressions done to the emoji and the actions used to express it.

## Development
The project was developed with the aim of creating animated emojis. The initial sketches were hand-drawn, and the game was developed using OpenGL. The development process involved creating the basic emoji face, understanding the algorithm to create a circle, and creating an emoji face as a circle with a yellow color. We realized that the Emoji expression would look more expressive if the transition of the emojis could be shown on the same face. We also made artistic decisions on what emojis to show in the game and initially decided to have a smile, sarcastic, and Valentine emojis. The transitions were implemented using interpolation and triangulation, and the buttons were added for a better user experience. The game was further improved by adding an angry emoji and including transitions for the emoji. In the end, the game had the following transitions for actions:

- Smiley [Poke] → Sarcastic
- Smiley [Love] → Valentine
- Valentine [Poke] → Sarcastic
- Valentine [Slap] → Angry
- Smiley [Slap] → Angry
- Sarcastic [Pat] → Smiley
- Sarcastic [Love] → Valentine
- Angry [Love] → Valentine
- Angry [Pat] → Smiley
- Sarcastic [Slap] → Angry

## User Experience
The fun aspect of the project initially was animating the emojis from one face to another face. We added sound effects to make the project cool and funny. During the presentation, almost everyone was laughing at the expressions of the game. The students were interested in trying the game, especially trying to slap the emoji and immediately patting it to make it feel better. The overall experience was enhanced with the use of sound effects.

## Success and Creativity
We feel that the project was successful because we were able to make Emoji Expressor similar to what it was intended to be. We think that the project is complete and creative, with the added sound effects bringing a different flavor to the fun of the game. We made good progress considering how difficult it is to use OpenGL. We have added colored buttons for valid actions and greyed-out options for invalid actions.

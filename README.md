This is an ongoing project designed to learn SDL2. To see a future features list, check out the ROADMAP to see what will be added.

Notes:
  - This project was initially designed with the purpose of making a simple program that opened an application. Features have been added to make it a more entertaining display, which has shifted
    the goal of the project into a fun, simple game.

  - Due to the shift from simple application to more complex game, the structure of the code is less than ideal. Had this originally been a major project, the structure of the code would have
    been thought out BEFORE work had been done. I appologize if the code is jumbled. Feel free to message me with any questions regarding the code.


Known Bugs:
  - The scroll wheel will sometimes lose pixels when alternating using the scroll wheel and dragging with the mouse. This is not currently a priority as it is minimal loss.

Peculiarities:
  - The tracking of textures is odd. An Ltexture object is created with each new texture and stored in an unordered map. Objects are rendered by the key value of the Ltexture object, rather than
    the object itself. This feature was made this way in order to keep textures loaded in a designated area, even when not in use. Future updates may add a more complex system where duplicate
    textures can be created while only comitting one texture to memory. (I did not start this project believing I would be re-using textures).

  - Currently the "game engine" and the game itself are very interconnected. Had I started this project making a game I would have seperated them accordingly. Future progress may be made to
    untangle them, but I currently value a better final product over reuseability.

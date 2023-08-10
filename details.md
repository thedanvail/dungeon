Suppose your team wants to develop a text-based dungeon crawler game. The game designer came up with an idea and wants to demo it with an internal playtest next week. It’s up to you to get the code working.
These are the gameplay specifications:

 - The player configures how many levels the dungeon should have.
 - In each level, the player enters a room, beats the monsters inside (hopefully!), and then chooses between 2 doors that lead to a room in the next level.
 - Players win if they manage to progress through all the levels.
 - The battle happens automatically, going in rounds until either the player or all monsters die. In a round, everything happens at once: monsters attack the player, treasures give the player a bonus, and the player can attack only one monster.
 - The dungeon is designed as sprawling catacombs, each level going deeper. Level 0 is just the entrance with 2 doors. After that first choice, the player enters one of the 2 rooms of the first playable level. Deeper, level 2 would have 3 rooms, because the middle room can be accessed by going right from the lvl1 left room or by going left on the lvl1 right room. Level 3 would have 4 rooms and so on.
 - The number of monsters and treasures in a room follows a specially devised scheme so that there are more monsters than treasures on the right of the dungeon and vice-versa. The middle of the dungeon is far more dangerous than the sides. On top of that, each level will bring in bigger quantities of both.
 - To provide some variety, there are 2 different types of monsters, but this could be increased in the future according to player feedback.

Feel free to rename and refactor the existing code. Ease of diffing is not important.
Being an open-ended problem, there is no perfect solution and it could always be made nicer. What we will look for is correctness and best practices/direction.

If any of the improvements you consider would take a significant amount of time, mention your idea and how you would accomplish it on the notes as "future work" instead of implementing it. Please be kind to your own time.

Don’t worry too much about the design of the game itself. Imagine that, once you’re done, a game designer will take over and balance the gameplay.

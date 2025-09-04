### ⚠️ DEPRECATED ⚠️

State Alchemist has been archived and will no longer be supported in any way.

It was decided that the tool is too dangerous for the general public to use. It can easily fall into a fatally broken state if a game tries to load a mod file immediately after it changes. Such an action would cause the app to continue to crash at times until a backup is restored. It is very easy to fall into this problem by accident, so users shouldn't be expected to have to blindly guess when is a good time to change mods in a game to avoid an issue so devastating.

This critical issue cannot be fixed by any possible code change in State Alchemist as it is related to the core underlying behavior of Atmosphere's LayeredFS.

Instead, it's recommended to use [Simple Mod Alchemist](https://github.com/gtiersma/Simple_Mod_Alchemist) instead. Simple Mod Alchemist is a homebrew app instead of a Tesla overlay, so it won't have this issue. It should be easier to use too.

If you already have mods that are set up for State Alchemist, Simple Mod Alchemist will load them automatically, so there will be no need for any reorganization.

The State Alchemist release has been taken down, but the source code will be left up. Anyone that's code-savvy can still build the overlay from source if someone still really wants to use it despite its dangers.

![StateAlchemist](title.png)

Font by Woodcutter

# Overview

State Alchemist is a Nintendo Switch homebrew for managing game mods. State Alchemist runs as a Tesla-based overlay that can be shown while a game is running.

# Quick Start (tl;dr)

1. Extract the contents of the latest release to the root of your SD card.

2. In the `mod_alchemy` folder that now exists on your SD root, create a folder with the name of the game's title ID.

3. Within that folder, create folders for each group you want to categorize your mods in. Example: `mod_alchemy/<title_id>/Characters/`

4. Within those folders, create folders for each thing in the game that you plan to replace with a mod. Example: `mod_alchemy/<title_id>/Characters/Mario/`

5. Within those folders, create folders for each mod you have. Example: `mod_alchemy/<title_id>/Characters/Mario/Spongebob/`

6. Within those folders, move each mod's folder structure into its corresponding folder. The folder structure should typically begin with the `romfs` folder (which should then contain the other folders and files that comprise the mod).

7. Have fun, and **make sure to relaunch the game any time after changing mods**.

8. If this **Quick Start** guide doesn't make sense, read below for more details.

# Features

* **Speed**: When enabling mods, State Alchecmist moves the files instead of copying and pasting them, so even large files (such as music and video mods) can be toggled almost instantly* It's better for your SD card too.

* **Organization**: The overlay will only show mods for the currently-running game. Those mods are then organized into custom groups.

* **Random Selections**: The overlay can enable and disable all the mods at random. You also have complete control over the random process: You can "lock" mods (preventing a mod from changing) OR set how likely you want each mod to be enabled.

* **Memory Efficient**: A common problem with Tesla overlays are memory overflow errors (frequency dependent on the Switch game and how much memory is used at a point in time). Keeping memory usage minimal has been a top priority in this overlay's development, ensuring that this problem should be rare.

*If a mod consists of many files (such as over a hundred, regardless of their file sizes), enabling or disabling it may be very slow due to the nature of how State Alchemist manages its memory. It's recommended to avoid using mods that consist of a large number of files (such as entire mod packs) with State Alchemist.

# Requirements

State Alchemist uses the Tesla overlay system that can be found here: [Tesla Menu](https://github.com/WerWolv/Tesla-Menu/releases)

Tesla must be installed on your SD card to use State Alchemist.

# Installation

1. Install the Tesla overlay system if you haven't yet. It can be found here: [Tesla Menu](https://github.com/WerWolv/Tesla-Menu/releases)

2. Download the latest release of State Alchemist, extracting its contents to the root of your SD card.

3. There should now be a `mod_alchemy` folder in your SD card root.

4. In `mod_alchemy`, create a folder. Give that folder the name of a game's **title ID** you want to mod. You can find the title ID for the game at this website: [NSWDB](https://nswdb.com/)

5. In the folder you created in step 4, create a folder for each "group" you wish to categorize your mods in. Some typical examples of names you may want to use would be "Characters", "Levels", "Music", etc.

6. In each of those folders created in step 5, create a folder for each thing you want to mod in a game. For example, if you have a mod that would replace Mario in a game, you would want to create a folder named "Mario" in a folder like `mod_alchemy/<title_id>/Characters/`. Then create folders for the names of any other characters you have mods for in the `Characters` folder.

7. In each of those folders created in step 6, create a folder for each mod you have. For example, if you have a mod that replaces Mario with Spongebob, you would want to created a folder named "Spongebob" in a folder like `mod_alchemy/<title_id>/Characters/Mario/`. Then create folders for the names of any other mods you have that replace Mario in the `Mario` folder.

8. In each of those folders created in step 7, place the folders with the files for each mod in that folder. For example, if you have a mod that replaces Mario with Spongebob, you would want to place the folder structure for the "Spongebob" mod in a folder like `mod_alchemy/<title_id>/Characters/Mario/Spongebob/`. In most cases, a mod's file structure should start with a folder named `romfs`, so there should be a `mod_alchemy/<title_id>/Characters/Mario/Spongebob/romfs/` folder which would then contain the rest of the folders and files that make up the mod. The folder structure should match the game's filesystem.

9. Remove any files that are currently in the `/atmosphere/contents/<title_id>/` folder that may conflict with files from any mods that you set up in step 8. If you want to keep them, move them to a mod folder within `mod_alchemy` created in the manner described in steps 5-8. If you don't feel very sure of what you're doing, it's a good idea to create a backup of `/atmosphere/contents/<title_id>/`.

10. Create a backup of the `mod_alchemy/<title_id>/`. State Alchemist operates by moving files between `mod_alchemy/<title_id>/` and `/atmosphere/contents/<title_id>/`. If you ever disable all the mods in State Alchemist, it should bring all the mod files back to the folders under `mod_alchemy/<title_id>/`, but I make no guarantee that there won't be a problem that will prevent that, so I recommend backing up those folders and files **before you start using State Alchemist** just to be safe.

11. You're ready to use State Alchemist! You can access it the same as you would any other Tesla overlay. See the Tesla documentation for more details on how to do so: [Tesla GbaTemp Thread](https://gbatemp.net/threads/tesla-the-nintendo-switch-overlay-menu.557362/)

### Adding Mods

Mods can be added to State Alchemist at any time using the same instructions listed above.

### Deleting Mods

If there ever is a mod you're using with State Alchemist that you want to delete permanently, **make sure to disable that mod in State Alchemist first** if it isn't already disabled. This will make sure all files for that mod are returned to their original locations within the folders in `mod_alchemy`.

Once you're sure it's disabled, deleting the mod is as easy as deleting the `mod_alchemy/<title_id>/<group_name>/<thing_being_modded>/<mod_name>` folder belonging to it from your SD card.

Disabling the mod is necessary because enabled mods have their files mixed in with all the other enabled mod files within the `/atmosphere/contents/<title_id>/` folder. Disabling the mod will move its files back to the original folder they were in: `mod_alchemy/<title_id>/<group_name>/<thing_being_modded>/<mod_name>`.

# Using State Alchemist

After opening State Alchemist, you will be greeted with 3 menu options:

* **View Mod Groups**: Go here to select a group of mods to interact with. The groups listed will correspond to the folders you created in step 5 of the installation instructions. There are two kinds of interaction:

  * **A button**: Using the A button on a group will display all items belonging to that group. There are then two kinds of interaction that can be performed on each of those items:
  
    * **A button**: Using the A button on an item belonging to a group will list all the mods belonging to that item. You can select whatever mod you want to enable it. **Make sure to relaunch the game when you are done enabling or disabling mods**.
    
    * **X button**: Using the X button on an item belonging to a group will list all the mods belonging to that item as well, but it will instead show you sliders that you can use to change the likelihood of the random feature picking that specific mod. The further left the slider is, the less likely it will be enabled at random. The further right, the more likely.
    
    * All mods listed will correspond to the folders created in step 7 of the installation instructions.
  
  * **X button**: Using the X button on a group will display all items belonging to that group along with each mod that is currently enabled for the item. You'll be able to lock mods that are currently active to prevent the random feature from changing them to a different mod.
  
  * All items listed from each group correspond to the folders created in step 6 of the installation instructions.
  
* **Pick at Random**: Changes all mods at random. **Make sure to relaunch the game when the random feature finishes**. Also **avoid using this feature at any point when the game may be loading**.

* **Disable All Mods**: Turns off all mods that are currently enabled. **Make sure to relaunch the game when it finishes**. Also **avoid using this feature at any point when the game may be loading**.

# Help / FAQs

### State Alchemist crashes or has a problem the moment I try to open it from the Tesla menu.

It could be conflicting with another overlay (even if that overlay isn't open).

Try removing all the other `.ovl` overlay files from your SD card (except the main Tesla overlay, of course). Then try opening State Alchemist from Tesla again (this time no other overlays should be listed).

If it still has that same issue, please file a GitHub Issue and describe the problem in detail.

---

### State Alchemist claims the game folder does not exist

Make sure you named the folder you created in step 4 in the **Installation Instructions** correctly. Make sure the folder's name really is the correct title ID for that game.

---

### After enabling a mod, I don't notice it in the game.

Make sure to relaunch the game after enabling it.

If you did, disable the mod in State Alchemist. Then in a file explorer, go to that mod's folder in the `mod_alchemy/<title_id>/<group_name>/<thing_being_modded>/` folder to inspect its folders and files.

Make sure the folders and files are all named correctly. The folder directly in the `mod_alchemy/<title_id>/<group_name>/<thing_being_modded>/<mod_name>/` folder should usually be named `romfs`. `romfs` should contain more folders matching the game's file system. Make sure the mod's files are named properly and in the correct folders.

---

### After changing or disabling a mod, the game crashes the moment the mod would normally be loaded, but I know the mod should work fine.

Make sure to relaunch the game after enabling it.

---

### The game crashed while having State Alchemist pick mods as random

When using that feature, make sure to only use it at a point when the game does not load (that includes times where the game might not be showing a loading screen, but it may still be loading things to display).

If you're not sure if the game is loading or not, trying using the random feature in a different place in the game. Main menus and pause screens are safe in most cases.

If the game continues to crash when using the random feature, try locking all the mods except one. Try the random feature again. If it works, try unlocking a few more mods. See if it works again. Repeat until you can narrow down a problematic mod.

If you're sure there's no problematic mod, feel free to file a GitHub Issue. Mention the game you're using, the approximate amount of time it takes for the random feature to cause a crash, the kinds of mods you are using, and any peculiar details that can be shared.

---

### Mod is only being partially enabled. Not all files belonging to that mod were enabled.

Use a file explorer to go within `/atmosphere/contents/<title_id>/` and find the places the files should be located at (matching the locations of the files that weren't moved in `mod_alchemy/<title_id>/<group_name>/<thing_being_modded>/<mod_name>/`.

If there is already a file within `/atmosphere/contents/<title_id>/` with the same name and place as a file that's not being enabled within `mod_alchemy/<title_id>/<group_name>/<thing_being_modded>/<mod_name>/`, then that is the reason it's not enabling. There's already a file enabled in its place. There's a conflict. If you really want the mod to be fully enabled, you will need to find out where that file came from. It may have come from another mod that was enabled by State Alchemist.

Try using **Disable All Mods** in State Alchemist. Then try searching your SD card for that file's name. You should be able to see all the mods that use it. If some of those mods may have been enabled at the same time, that would've been the problem. You will need to try to sort out the conflicting files between them.

If the file is still located within `/atmosphere/contents/<title_id>/` even after disabling all mods in State Alchemist, then that file does not belong to any of the mods in State Alchemist. It was put there manually, so you will need to figure out why it's there and if you need it.

Once the conflicting files are sorted out, and the files can now be moved to their spots within `/atmosphere/contents/<title_id>/`, the mod should be enabling itself fully.

If there was no conflict to begin with, then there is a bug that can be encountered infrequently that causes specific files to not be moved.

It seems to be out of the control of State Alchemist, so there may be nothing that can be done about it at the moment. It's infrequent, so it shouldn't happen often.

But even if this bug occurs, the rest of the files should still be moving properly when enabling or disabling the mod, so there should be no need to worry about lost files.

---

### The menu options are weird or messed up. Maybe I'm seeing an option named "romfs" in the menu.

The folder structure in the `mod_alchemy` folder is probably not set up correctly.

If you tried enabling any mods in the menu (or if you're not sure if you did), first use the **Disable All Mods** option in the State Alchemist main menu to fix any folders or files that may have been improperly moved from the `mod_alchemy` folder.

Review steps 5-9 in the installation instructions.

Make sure all the mods are set up in the following manner in the following folder structure: `mod_alchemy/<title_id>/<group_name>/<thing_being_modded>/<mod_name>/romfs/<mod folders and files here>`

---

### Tesla froze for a while when I tried to enable a mod.

This can happen if a mod consists of a really large number files (even if those files are tiny).

Wait for it to finish. Interrupting it could corrupt your mods or even your SD card, and then you would need to restore it from a backup. It may take up to 10 minutes if the mod consists of over 1,000 files.

Once it finishes, either follow the instructions in the **Deleting Mods** section to remove it.

Using such large mods (such as mod packs) isn't recommended with State Alchemist. State Alchemist is built for smaller, individual mods that would be comprised of under ~100 files.

If the mod doesn't consist of that many files, you may want to double-check it. Go to the `/atmosphere/contents/<title_id>/` folder on your SD card if the mod is active (`mod_alchemy/<title_id>/<group_name>/<thing_being_modded>/<mod_name>/` if it's not active) and look through the folders and files the mod consists of. See if there are a large number of files that shouldn't be there. Follow the instructions in the **Deleting Mods** section to remove it.

---

### I was taken to an error menu while using State Alchemist

Please file a GitHub issue with the exact error you see. Also describe how you came across the error, the folders that contain the related mod (if it was related to a specific mod), and any other details you think may be helpful.

---

### I want to remove State Alchemist and all mods it is using from my SD card

In the State Alchemist menu, choose the **Disable All Mods** option before you delete anything.

After doing so, delete the `mod_alchemy` folder from the root of your SD card, along with the State Alchemist `.ovl` file from the Tesla overlays directory.

---

### I want to remove State Alchemist and all mods it is using from my SD card, but I can't access the State Alchemist overlay for some reason

You'll need to learn how to manually manage State Alchemist mods. Please see the **Advanced** section.

---

### Can files from other mods be left in the game's title ID folder under the Atmosphere folder?

Yes.

State Alchemist will only touch its own mod files (including those that it moved into the `/atmosphere/contents/<title_id>/` folder).

If there are any mod files you put directly within the `/atmosphere/contents/<title_id>/` folder, those mods will stay there and will not show up in State Alchemist.

---

### How does State Alchemist handle conflicts between files?

If State Alchemist tries to enable a mod that uses the same files as a mod that already exists within the `/atmosphere/contents/<title_id>/` folder (whether those files are managed by State Alchemist or not), those files won't be activated.

State Alchemist will only move non-conflicting files belonging to the mod being enabled. Whatever files are currently in `/atmosphere/contents/<title_id>/` will stay there.

For example, if you enable a mod with a file named `mario`, but there's already a file named `mario` in the same spot within the `/atmosphere/contents/<title_id>/` folder, the `mario` file for the mod you enabled won't be used. The other `mario` file that's already there will be used.

---

### Why do I need to relaunch the game every time after I change mods?

When the game is launched, all the mods currently enabled (aka, located in `/atmosphere/contents/<title_id>/`) have their file locations tracked in memory so that the game will load those files when it needs to.

If you change mods, those do not update.

In other words, the moment you enable a mod in State Alchemist, the game will not know a mod was enabled. It will continue to not load it.

The moment you disable a mod in State Alchemist, the game will not know that mod was disabled. It will try to find the mod's files, and when it can't, it will crash the game.

The game must be relaunched for those tracked locations to be updated.

This behavior is related to how the Layeredfs technology in Atmosphere loads mods (not State Alchemist itself), so it's not something that can be easily changed.

There is one exception that I've noticed: music

I think because music is "streamed" and not loaded all at once like other files, it actually does not seem to require the game to be relaunched for the music to be changed (at least, not in my experience). But the music can get heavily distorted if the track is changed while it is playing though.

And although music seems to work, it's not an intended feature, but a side-effect. I still would recommend relaunching the game anyway, even if you only change music.

---

### Where did the name State Alchemist come from?

The tool was originally named Mod Alchemist, a reference to its random feature and how it is capable of "mixing" different combinations of mods to create new experiences, almost as an alchemist would mix substances.

While this software was being built, another repo related to Nintendo Switch homebrew decided to also use the name "Mod Alchemist". That app managed to be released months before State Alchemist was finished.

To avoid confusion, it was decided this Tesla-based app needed a different name so that the community could easily distinguish the two. "Mod Alchemist" was renamed to "State Alchemist".

"State Alchemist" is a humorous reference to this situation, taken from the hit anime, Fullmetal Alchemist. In episodes 11 & 12 (titled "The Other Brothers"), Ed, the real Fullmetal Alchemist encounters a conman who is claiming to be the one and only Fullmetal Alchemist to a local community. Ed must then prove his legitimacy by revealing his State Alchemist watch, proving that he is the real one, and the other is a fake.

Renaming this app to "State Alchemist" is a way of claiming its authenticity as the original Mod Alchemist.

---

# Advanced: Managing State Alchemist Mods Manually

By understanding how State Alchemist interacts with files and folders on the SD card, you can change State Alchemist's mod settings from a file explorer and maybe a text editor. This also means it's also possible to create a script to automatically modify State Alchemist's settings as well.

### Enabled Mods

To see which mod is currently enabled over something, navigate to `mod_alchemy/<title_id>/<group_name>/<thing_being_modded>/`.

In that directory, you may see a `.txt` file that shares a name with one of the mod folders. That is the active mod. The `.txt` contains the paths of each file belonging to that mod that was moved to `/atmosphere/contents/<title_id>/`. Keep in mind that not all the mod's files may have been moved if there was a conflicting file in `/atmosphere/contents/<title_id>/`, but the `.txt` records only those that *were* moved.

Within the mod's folder, you'll notice that the folders remain even though the files are gone.

If there is no `.txt` file, that means no mod is enabled.

**To manually disable a mod**:

1. Move each file listed in the `.txt` file from within the `/atmosphere/contents/<title_id>/` folder to the same corresponding folder within the `mod_alchemy/<title_id>/<group_name>/<thing_being_modded>/<mod_name>/` folder.

2. Once you are sure you moved all the files listed in the `.txt` file, delete the `.txt` file.

3. The next time you open State Alchemist, it will show the mod as disabled, and the mod will be able to enable and re-disable itself properly.

Once there is no mod enabled in the `mod_alchemy/<title_id>/<group_name>/<thing_being_modded>/` folder, you can now manually enable a different mod if desired:

**To manually enable a mod**:

1. Create a `.txt` file directly in `mod_alchemy/<title_id>/<group_name>/<thing_being_modded>/`, naming it **the exact same** as the name of the folder of the mod you wish to enable.

2. Within that `.txt` file, include every file belonging to that mod (including the folder path beginning in `mod_alchemy/<title_id>/<group_name>/<thing_being_modded>/<mod_name>/` leading to each file). In most cases, the path will always begin with `romfs`. Each mod file with its folder path should be on a new line.

3. Move all folder(s) in `mod_alchemy/<title_id>/<group_name>/<thing_being_modded>/<mod_name>/` to `/atmosphere/contents/<title_id>/`.

4. The next time you open State Alchemist, it will show the mod as enabled, and the mod will be able to disable and re-enable itself properly.

### Likelihoods of mods being randomly picked

To the see what is set as the likelihood of a mod being picked, navigate to that mods folder in `mod_alchemy/<title_id>/<group_name>/<thing_being_modded>/`.

There may be `~~##` at the end of the folder name, where `##` could be two of any digit. That number is the likelihood on a scale from 0 to 100, with `00` being never.

If there is no `~~##` at the end of the folder name, that means that mod is set to the default likelihood of `100`.

The likelihood of no mod being picked can be found at the end of the folder containing the mod folders (in other words, the `mod_alchemy/<title_id>/<group_name>/<thing_being_modded>/` folder).

To change the likelihood, simply change the last two digits on the end of the folder name. But keep in mind there must be two digits after the `~~`.

* `~~100` will not be interpreted correctly. If you want to set it to `100`, remove the `~~##` from the folder name entirely.

* `~~5` will also not be interpreted correctly. If you want to set it to `5`, the folder name should end with two digits: `~~05`.

### Locked Mods

If a mod is locked, the folder containing the mod folder (aka `mod_alchemy/<title_id>/<group_name>/<thing_being_modded>/`) will have a name that **begins** with a `~`. This signifies that whatever mod is currently active within that folder will not be changed when the random feature is used.

If there is no active mod within that folder, then the random feature will leave those mods disabled if it is locked.

To manually lock or unlock a mod, rename the `mod_alchemy/<title_id>/<group_name>/<thing_being_modded>/` folder that contains it so that it begins with a `~`. Vice versa for unlocking.

# Special Thanks

* **WerWolv** for creating the Tesla overlay system

* **SciresM** and the other contributors for creating and maintaining LayeredFs and Atmosphere

* **devkitPro** for creating libnx

# Legal Disclaimers

This software's title may be inspired by characters and events that are part of the Fullmetal Alchemist television series, however the creation of this software is in no way affiliated with Aniplex of America, the license holder of Fullmetal Alchemist.

This software is built with the sole intention of running on Nintendo Switch. The Nintendo Switch is a product consisting of both hardware and software by Nintendo Co. Ltd. This software has not been licensed by Nintendo in any form. Nintendo is not affiliated with the creation of this software in any form.

The Nintendo Switch logo is a trademark of Nintendo Co. Ltd.

This software is purely a non-profit endeavor. Any usages of copyrighted material comprised within this software have not been used in a manner to gain compensation in any form.

All other portions of this software are licensed under the GPL 2.0 standard, giving the general public permission to use, modify, or distribute this software in accordance with the terms and conditions that can be viewed [here](https://github.com/gtiersma/mod-alchemist?tab=GPL-2.0-1-ov-file#GPL-2.0-1-ov-file).

The author assumes no responsibility for any problems that may occur from modified derivative works from this software.

This software is not intended to be used in any manner that involves or encourages digital piracy. The author assumes no responsibility for any crimes users may perform through the use of this software.

The author provides no guarantee that the software will operate as intended or if it will operate at all. By using this software in any form or manner, you agree that the author will not be held responsible for any damage that may occur to the device(s) it may be used with, whether the damage be physical or digital.

# README

## Description

Notesy is going to be a command line interface for keeping simple notes.
Initial plans are to keep it light, just pull from a file directory set up maybe.
Making it for Windows at the moment, writing in C++.
Eventually it'd be neat to offer some sort of synchronization.
Also thinking about versioning the notes and allowing some sort of rollbacks.
Search features are another possibility, but for now it's going to be fairly straightforward.

## TODO
* come up with all commands (see below)
* command table (think shell) with names, function pointers
* initialization strategy
* some sort of config file format, notesy directory
* decide on notes storage document format
* organize the project / solution

## Commands (to do!)
* init - initialize notesy, set up a directory for it to keep notes in
* list commmands for collections / topics, notes, subnotes?
* add commands for collections, notes, subnotes
* remove / delete commands
* config commands
* search (maybe)
* sync (eventually)

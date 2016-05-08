# ntsy

## Description

ntsy (pronounced "notesy") is a lightweight command-line interface for keeping simple notes on Windows.

## Status

Taking a break... 
Still needs init function.
Known issue: indexes >= 10 in a colleciton are not being found in interactive mode sub-commands.

## Commands

```
ntsy ls [<abbr.>]          - Lists collections, or if collection is specified, notes.
ntsy col <name> <abbr.>    - Adds a new collection.
ntsy rm <abbr.>            - Removes a collection.
ntsy jot <abbr.> <text>    - Adds a quick note to a collection.
ntsy open <abbr.>		   - Opens a collection in interactive mode.
	// sub-commands
	add 		      - Adds a new note.
	read <#>		  - Reads full note text.
	edit <#>		  - Opens note for editing.
	rm <#>			  - Deletes a note.
ntsy config <key> <value>  - Edits a configuration setting.

```

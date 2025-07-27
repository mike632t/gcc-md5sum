#
# makefile
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.
#
# Note - Shell commands must start with a tab character at the beginning
# of each line NOT spaces..!
#
#  30 Jul 23   0.1   - Initial version - MT
#   4 Aug 23         - Added backup files to tar archive - MT
#  11 Nov 24   0.2   - Project name derived from current folder - MT
#

PROJECT	= $(strip $(notdir $(abspath $(CURDIR)/.)))

SOURCE	=  $(wildcard *.c) 	# Compile all source files
INCLUDE	=  $(wildcard *.h) 	# Automatically get all include files
BACKUP	=  $(wildcard *.c.[0-9])
OBJECT	=  $(SOURCE:.c=.o)
PROGRAM	=  $(SOURCE:.c=)

FILES	=  $(SOURCE) $(BACKUP) $(INCLUDE) makefile LICENSE # README.md .gitignore .gitattributes 
LANG	=  LANG_$(shell (echo $$LANG | cut -f 1 -d '_'))
UNAME	=  $(shell uname)

#LIBS	=  -lm
FLAGS	=  -fcommon -Wall -pedantic -std=gnu99
FLAGS	+= -Wno-comment -Wno-unused-function #-Wno-deprecated-declarations -Wno-builtin-macro-redefined
FLAGS	+= -D $(LANG)

# Operating system specific settings
ifeq ($(UNAME), NetBSD) 	# NetBSD..
LIBS	+=  -lcompat
#FLAGS	+=  -I /usr/X11R7/include/ -L /usr/X11R7/lib/ -R /usr/X11R7/lib
endif

# Compiler specific settings
ifeq ($(CC), cc)
#LIBS	+=  
FLAGS	+=  -no-pie
endif

ifdef DEBUG
FLAGS	+=  -g
endif

make:$(PROGRAM) $(OBJECT)

all:clean $(PROGRAM)

%.o : %.c
ifdef DEBUG
	@echo $(CC) $(FLAGS) -c $<
endif
	@$(CC) $(FLAGS) -c $<

%: %.o
ifdef DEBUG
	@echo $(CC) $(FLAGS) -o $@ $< $(LIBS)
endif
	@$(CC) $(FLAGS) -o $@ $<  $(LIBS)
	@ls --color $@

test: $(PROGRAM)
ifdef DEBUG
	@echo "for program in $(PROGRAM); do ./$$program * | md5sum -c ; done"
endif
	@for program in $(PROGRAM); do ./$$program * | md5sum -c ; done

clean:
ifdef DEBUG
	@echo rm -f $(OBJECT) 
	@echo rm -f $(PROGRAM)
endif
	@rm -f $(OBJECT) # -v
	@rm -f $(PROGRAM) # -v

backup: clean
	@echo "$(PROJECT)-`date +'%Y%m%d%H%M'`.tar.gz"; tar -czpf ..\/$(PROJECT)-`date +'%Y%m%d%H%M'`.tar.gz $(FILES)

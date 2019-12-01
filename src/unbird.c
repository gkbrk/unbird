// unbird - Utility to turn Bird-style Literate Programming documents into code.
// Copyright (C) 2019 Gokberk Yaltirakli
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void processFile(FILE *file);

int main(int argc, char **argv) {
  FILE *file = stdin;

  if (argc > 1)
    file = fopen(argv[1], "r");

  processFile(file);
  return 0;
}

bool lineIsCode(char *line) {
  if (line[0] != '>') return false;
  if (line[1] == '\n' || line[1] == ' ') return true;
  return false;
}

bool lineEmpty(char *line) {
  return line[0] == '\n';
}

void processFile(FILE *file) {
  char *line;
  size_t len;

  bool prevCode = false;
  bool prevEmpty = false;
  while (getline(&line, &len, file) != -1) {
    bool isCode = lineIsCode(line);
    bool empty = lineEmpty(line);

    bool prevBroken = !prevEmpty && !prevCode && isCode;
    bool nextBroken = !prevEmpty && !empty && prevCode && !isCode;
    if (prevBroken || nextBroken) {
      fprintf(stderr, "Code sections should have empty lines around them\n");
      exit(-1);
    }

    if (isCode) {
      if (line[1] == '\n')
        putc('\n', stdout);
      else
        fputs(line + 2, stdout);
    }

    if (!isCode && prevCode) {
      putc('\n', stdout);
    }

    prevCode = isCode;
    prevEmpty = empty;
  }
}

